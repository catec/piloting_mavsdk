#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <mutex>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/checklist_robotic_vehicle/checklist_robotic_vehicle.h>

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define RESULT_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

int main(int, char**)
{
    const uint8_t local_system_id = 1;
    const uint8_t local_component_id = 25;
    const std::string local_ip = "127.0.0.1";
    const std::string target_ip = "127.0.0.1";
    const int local_port = 14541;
    const int target_port = 14540;
    const int target_system_id = 2;

    Mavsdk mavsdk;
    Mavsdk::Configuration config(
        local_system_id,
        local_component_id,
        false,
        Mavsdk::Configuration::UsageType::RoboticVehicle);
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
        std::cout << ERROR_CONSOLE_TEXT << "No target system (" << unsigned(target_system_id)
                  << ") alive" << NORMAL_CONSOLE_TEXT << std::endl;
        sleep_for(seconds(1));
    }
    std::cout << "Target system (" << unsigned(target_system->get_system_id()) << ") connected!"
              << std::endl;

    ///////////////////

    auto checklist_rv = ChecklistRoboticVehicle{target_system};

    ChecklistBase::Checklist checklist;
    ChecklistBase::ChecklistItem item1{0, "check0", "check point 0"};
    ChecklistBase::ChecklistItem item2{1, "check1", "check point 1"};
    checklist.items.push_back(item1);
    checklist.items.push_back(item2);

    std::mutex checklist_upload_mutex;
    bool checklist_upload{false};

    checklist_rv.upload_checklist_async(
        [&](ChecklistBase::Result result, ChecklistBase::Ack ack) {
            std::cout << RESULT_CONSOLE_TEXT << "Checklist upload result/ack is: " << result
                      << " / " << ack << NORMAL_CONSOLE_TEXT << std::endl;
            std::lock_guard<std::mutex> lock(checklist_upload_mutex);
            checklist_upload = true;
        },
        checklist);

    std::cout << "Waiting for checklist upload..." << std::endl;
    for (;;) {
        {
            std::lock_guard<std::mutex> lock(checklist_upload_mutex);
            if (checklist_upload) {
                std::cout << "Checklist upload!" << std::endl;
                break;
            }
        }
        sleep_for(seconds(1));
    }

    std::cout << "Finished!" << std::endl;

    return EXIT_SUCCESS;
}
