#include "udp_connection.h"
#include "global_include.h"
#include "log.h"

#ifdef WINDOWS
#include <winsock2.h>
#include <Ws2tcpip.h> // For InetPton
#undef SOCKET_ERROR // conflicts with ConnectionResult::SocketError
#ifndef MINGW
#pragma comment(lib, "Ws2_32.lib") // Without this, Ws2_32.lib is not included in static library.
#endif
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h> // for close()
#endif

#include <cassert>
#include <algorithm>

#ifdef WINDOWS
#define GET_ERROR(_x) WSAGetLastError()
#else
#define GET_ERROR(_x) strerror(_x)
#endif

namespace mavsdk {

UdpConnection::UdpConnection(
    Connection::receiver_callback_t receiver_callback,
    const std::string& local_ip,
    int local_port_number) :
    Connection(receiver_callback),
    _local_ip(local_ip),
    _local_port_number(local_port_number)
{}

UdpConnection::~UdpConnection()
{
    // If no one explicitly called stop before, we should at least do it.
    stop();
}

ConnectionResult UdpConnection::start()
{
    if (!start_mavlink_receiver()) {
        return ConnectionResult::ConnectionsExhausted;
    }

    ConnectionResult ret = setup_port();
    if (ret != ConnectionResult::Success) {
        return ret;
    }

    start_recv_thread();

    return ConnectionResult::Success;
}

ConnectionResult UdpConnection::setup_port()
{
#ifdef WINDOWS
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        LogErr() << "Error: Winsock failed, error: %d", WSAGetLastError();
        return ConnectionResult::SocketError;
    }
#endif

    _socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (_socket_fd < 0) {
        LogErr() << "socket error" << GET_ERROR(errno);
        return ConnectionResult::SocketError;
    }

    struct sockaddr_in addr {};
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, _local_ip.c_str(), &(addr.sin_addr));
    addr.sin_port = htons(_local_port_number);

    if (bind(_socket_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
        LogErr() << "bind error: " << GET_ERROR(errno);
        return ConnectionResult::BindError;
    }

    return ConnectionResult::Success;
}

void UdpConnection::start_recv_thread()
{
    _recv_thread = new std::thread(&UdpConnection::receive, this);
}

ConnectionResult UdpConnection::stop()
{
    _should_exit = true;

#ifndef WINDOWS
    // This should interrupt a recv/recvfrom call.
    shutdown(_socket_fd, SHUT_RDWR);

    // But on Mac, closing is also needed to stop blocking recv/recvfrom.
    close(_socket_fd);
#else
    shutdown(_socket_fd, SD_BOTH);

    closesocket(_socket_fd);

    WSACleanup();
#endif

    if (_recv_thread) {
        _recv_thread->join();
        delete _recv_thread;
        _recv_thread = nullptr;
    }

    // We need to stop this after stopping the receive thread, otherwise
    // it can happen that we interfere with the parsing of a message.
    stop_mavlink_receiver();

    return ConnectionResult::Success;
}

bool UdpConnection::send_message(const mavlink_message_t& message)
{
    std::lock_guard<std::mutex> lock(_remote_mutex);

    if (_remotes.size() == 0) {
        LogErr() << "No known remotes";
        return false;
    }

    // Send the message to all the remotes. A remote is a UDP endpoint
    // identified by its <ip, port>. This means that if we have two
    // systems on two different endpoints, then messages directed towards
    // only one system will be sent to both remotes. The systems are
    // then expected to ignore messages that are not directed to them.
    bool send_successful = true;
    for (auto& remote : _remotes) {
        struct sockaddr_in dest_addr {};
        dest_addr.sin_family = AF_INET;

        inet_pton(AF_INET, remote.ip.c_str(), &dest_addr.sin_addr.s_addr);
        dest_addr.sin_port = htons(remote.port_number);

        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t buffer_len = mavlink_msg_to_send_buffer(buffer, &message);

        const auto send_len = sendto(
            _socket_fd,
            reinterpret_cast<char*>(buffer),
            buffer_len,
            0,
            reinterpret_cast<const sockaddr*>(&dest_addr),
            sizeof(dest_addr));

        if (send_len != buffer_len) {
            LogErr() << "sendto failure: " << GET_ERROR(errno);
            send_successful = false;
            continue;
        }
    }

    return send_successful;
}

void UdpConnection::add_remote(
    const std::string& remote_ip, const int remote_port)
{
    std::lock_guard<std::mutex> lock(_remote_mutex);
    Remote new_remote;
    new_remote.ip = remote_ip;
    new_remote.port_number = remote_port;

    auto existing_remote =
        std::find_if(_remotes.begin(), _remotes.end(), [&new_remote](Remote& remote) {
            return remote == new_remote;
        });

    if (existing_remote == _remotes.end()) {
        LogInfo() << "Added new remote system on: " << new_remote.ip << ":" << new_remote.port_number;
        _remotes.push_back(new_remote);
    }
}

void UdpConnection::receive()
{
    // Enough for MTU 1500 bytes.
    char buffer[2048];

    while (!_should_exit) {
        struct sockaddr_in src_addr = {};
        socklen_t src_addr_len = sizeof(src_addr);
        const auto recv_len = recvfrom(
            _socket_fd,
            buffer,
            sizeof(buffer),
            0,
            reinterpret_cast<struct sockaddr*>(&src_addr),
            &src_addr_len);

        if (recv_len == 0) {
            // This can happen when shutdown is called on the socket,
            // therefore we check _should_exit again.
            continue;
        }

        if (recv_len < 0) {
            // This happens on destruction when close(_socket_fd) is called,
            // therefore be quiet.
            // LogErr() << "recvfrom error: " << GET_ERROR(errno);
            continue;
        }

        _mavlink_receiver->set_new_datagram(buffer, static_cast<int>(recv_len));

        bool saved_remote = false;

        // Parse all mavlink messages in one datagram. Once exhausted, we'll exit while.
        while (_mavlink_receiver->parse_message()) {
            const uint8_t sysid = _mavlink_receiver->get_last_message().sysid;

            if (!saved_remote && sysid != 0) {
                saved_remote = true;
                add_remote(inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port));
            }

            receive_message(_mavlink_receiver->get_last_message());
        }
    }
}

} // namespace mavsdk
