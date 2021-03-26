#include <chrono>
#include <iostream>
#include <thread>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/command/command.h>

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

int main(int, char**)
{
    const uint8_t local_system_id = 2;
    const uint8_t local_component_id = 26;
    const std::string local_ip = "127.0.0.1";
    const std::string target_ip = "127.0.0.1";
    const int local_port = 14540;
    const int target_port = 14541;
    const int target_system_id = 1;

    Mavsdk mavsdk;
    Mavsdk::Configuration config(
        local_system_id, local_component_id, true, Mavsdk::Configuration::UsageType::GroundStation);
    mavsdk.set_configuration(config);

    sleep_for(seconds(1));

    const auto connection_result =
        mavsdk.setup_udp_connection(local_ip, local_port, target_ip, target_port);
    if (connection_result != ConnectionResult::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Connection failed: " << connection_result
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Connection established correctly!" << std::endl;

    std::shared_ptr<System> target_system;
    while (!(target_system = mavsdk.system(target_system_id))) {
        std::cout << ERROR_CONSOLE_TEXT << "No system (" << target_system_id << ") alive"
                  << NORMAL_CONSOLE_TEXT << std::endl;
        sleep_for(seconds(1));
    }
    std::cout << "Target system (" << unsigned(target_system->get_system_id()) << ") connected!"
              << std::endl;

    ///////////////////

    auto command = Command{target_system};

    {
        CommandBase::CommandLong command_data;
        command_data.command = MAV_CMD_DO_SET_HOME_QUATERNION;
        command_data.params.param1 = 1.1f;
        command_data.params.param2 = 2.2f;
        command_data.params.param3 = 3.3f;
        command_data.params.param4 = 4.4f;
        command_data.params.param5 = 5.5f;
        command_data.params.param6 = 6.6f;
        command_data.params.param7 = 7.7f;

        auto result = command.send_command(command_data);

        std::cout << TELEMETRY_CONSOLE_TEXT // set to blue
                  << "CommandAck received:\n"
                  << "   Result: " << result.first << "\n"
                  << "   Ack: " << result.second
                  << NORMAL_CONSOLE_TEXT << std::endl;
    }

    sleep_for(seconds(1));

    {
        CommandBase::CommandLong command_data;
        command_data.command = MAV_CMD_NAV_WAYPOINT_QUATERNION;
        command_data.params.param1 = 1.1f;
        command_data.params.param2 = 2.2f;
        command_data.params.param3 = 3.3f;
        command_data.params.param4 = 4.4f;
        command_data.params.param5 = 5.5f;
        command_data.params.param6 = 6.6f;
        command_data.params.param7 = 7.7f;

        auto result = command.send_command(command_data);

        std::cout << TELEMETRY_CONSOLE_TEXT // set to blue
                  << "CommandAck received:\n"
                  << "   Result: " << result.first << "\n"
                  << "   Ack: " << result.second
                  << NORMAL_CONSOLE_TEXT << std::endl;
    }

    for (;;)
        sleep_for(seconds(3));

    std::cout << "Finished..." << std::endl;

    return 0;
}
