#pragma once

#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#include <cstdint>
#include "connection.h"

namespace mavsdk {

class UdpConnection : public Connection {
public:
    explicit UdpConnection(
        Connection::receiver_callback_t receiver_callback,
        const std::string& local_ip,
        int local_port);
    ~UdpConnection();

    ConnectionResult start() override;
    ConnectionResult stop() override;

    bool send_message(const mavlink_message_t& message) override;

    void add_remote(const std::string& remote_ip, const int remote_port);

    // Non-copyable
    UdpConnection(const UdpConnection&) = delete;
    const UdpConnection& operator=(const UdpConnection&) = delete;

private:
    ConnectionResult setup_port();
    void start_recv_thread();

    void receive();

    std::string _local_ip;
    int _local_port_number;

    std::mutex _remote_mutex{};
    struct Remote {
        std::string ip{};
        int port_number{0};

        bool operator==(const UdpConnection::Remote& other)
        {
            return ip == other.ip && port_number == other.port_number;
        }
    };
    std::vector<Remote> _remotes{};

    int _socket_fd{-1};
    std::thread* _recv_thread{nullptr};
    std::atomic_bool _should_exit{false};
};

} // namespace mavsdk
