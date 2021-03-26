#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/inspection_robotic_vehicle/inspection_robotic_vehicle.h>

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

    sleep_for(seconds(1));

    ///////////////////

    auto inspection_rv = InspectionRoboticVehicle{target_system};

    std::mutex local_inspection_mutex;
    InspectionBase::InspectionPlan local_inspection_plan;

    inspection_rv.download_inspection_async(
        [&](InspectionBase::Result result, InspectionBase::InspectionPlan inspection_plan) {
            std::cout << RESULT_CONSOLE_TEXT << "Inspection download result is: " << result
                      << NORMAL_CONSOLE_TEXT << std::endl;
            std::cout << RESULT_CONSOLE_TEXT << inspection_plan << NORMAL_CONSOLE_TEXT << std::endl;

            if (result == InspectionBase::Result::Success) {
                std::lock_guard<std::mutex> lock(local_inspection_mutex);
                local_inspection_plan = inspection_plan;
            }
        });

    bool inspection_upload{false};

    inspection_rv.upload_inspection_async(
        [&](InspectionBase::Result result, InspectionBase::Ack ack) {
            std::cout << RESULT_CONSOLE_TEXT << "Inspection upload result/ack is: " << result
                      << " / " << ack << NORMAL_CONSOLE_TEXT << std::endl;
            std::lock_guard<std::mutex> lock(local_inspection_mutex);
            inspection_upload = true;
        });

    std::cout << "Waiting for inspection download..." << std::endl;
    for (;;) {
        {
            std::lock_guard<std::mutex> lock(local_inspection_mutex);
            if (local_inspection_plan.inspection_items.size() != 0) {
                std::cout << "Inspection download!" << std::endl;
                break;
            }
        }
        sleep_for(seconds(1));
    }

    {
        std::cout << "Modifying local inspection and setting it..." << std::endl;
        std::lock_guard<std::mutex> lock(local_inspection_mutex);
        local_inspection_plan.inspection_items[0].x = 10.10f;
        local_inspection_plan.inspection_items[0].y = 11.11f;
        local_inspection_plan.inspection_items[0].z = 12.12f;
        inspection_rv.set_upload_inspection(local_inspection_plan);
        std::cout << RESULT_CONSOLE_TEXT << local_inspection_plan << NORMAL_CONSOLE_TEXT
                  << std::endl;
    }

    std::cout << "Waiting for inspection upload..." << std::endl;
    for (;;) {
        {
            std::lock_guard<std::mutex> lock(local_inspection_mutex);
            if (inspection_upload) {
                std::cout << "Inspection upload!" << std::endl;
                break;
            }
        }
        sleep_for(seconds(1));
    }

    uint16_t inspection_current{0};

    inspection_rv.subscribe_inspection_set_current([&](uint16_t seq) {
        std::cout << RESULT_CONSOLE_TEXT << "Inspection set current received: " << seq
                  << NORMAL_CONSOLE_TEXT << std::endl;
        std::lock_guard<std::mutex> lock(local_inspection_mutex);
        inspection_current = seq;
        if (inspection_current < local_inspection_plan.inspection_items.size())
            inspection_rv.update_current_inspection_item(inspection_current);
    });

    {
        std::lock_guard<std::mutex> lock(local_inspection_mutex);
        if (inspection_current < local_inspection_plan.inspection_items.size())
            inspection_rv.update_current_inspection_item(inspection_current);
    }

    // Simulate inspection progress
    for (;;) {
        sleep_for(seconds(2));

        std::lock_guard<std::mutex> lock(local_inspection_mutex);
        if (inspection_current < local_inspection_plan.inspection_items.size())
            inspection_rv.update_reached_inspection_item(inspection_current++);
        if (inspection_current < local_inspection_plan.inspection_items.size())
            inspection_rv.update_current_inspection_item(inspection_current);
    }

    std::cout << "Finished!" << std::endl;

    return EXIT_SUCCESS;
}
