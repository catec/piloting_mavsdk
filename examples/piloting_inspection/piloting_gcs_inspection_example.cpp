#include <chrono>
#include <iostream>
#include <thread>
#include <future>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/inspection/inspection.h>
#include <mavsdk/mavlink_include.h>

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define RESULT_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

static InspectionBase::WaypointItem make_waypoint_item(
    std::string task_uuid,
    uint16_t command,
    uint8_t autocontinue,
    float param1,
    float param2,
    float param3,
    float param4,
    float x,
    float y,
    float z);

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

    auto inspection = Inspection{target_system};

    {
        std::cout << "Creating inspection to be sent..." << std::endl;
        std::vector<InspectionBase::WaypointItem> waypoint_items;
        waypoint_items.push_back(make_waypoint_item(
            "204d19ef-7e77-4804-a1ff-5602bbac60e5", MAV_CMD_NAV_WAYPOINT, 1, 1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f));
        waypoint_items.push_back(make_waypoint_item(
            "895e355b-99c4-4389-9273-d20eff6a6cc9", MAV_CMD_NAV_WAYPOINT, 1, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f));
        waypoint_items.push_back(make_waypoint_item(
            "98a1161c-5f77-426a-b1f4-dfefee1d2257", MAV_CMD_NAV_WAYPOINT, 1, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f));
        InspectionBase::WaypointList waypoint_list{};
        waypoint_list.plan_uuid = "79043d91-080a-4807-8073-d28f6b869b8a";
        waypoint_list.sync_id = 45131267;
        waypoint_list.items = waypoint_items;
        std::cout << RESULT_CONSOLE_TEXT << waypoint_list << NORMAL_CONSOLE_TEXT << std::endl;

        std::cout << "Uploading inspection..." << std::endl;
        auto prom = std::make_shared<
            std::promise<std::pair<InspectionBase::Result, InspectionBase::Ack>>>();
        auto future_result = prom->get_future();
        inspection.upload_inspection_async(
            waypoint_list, [prom](InspectionBase::Result result, InspectionBase::Ack ack) {
                prom->set_value(std::make_pair<>(result, ack));
            });

        // Blocked until sending the inspection
        const auto upload_result = future_result.get();
        const auto result = upload_result.first;
        const auto ack = upload_result.second;
        std::cout << RESULT_CONSOLE_TEXT << "Inspection upload result/ack is: " << result << " / "
                  << ack << NORMAL_CONSOLE_TEXT << std::endl;
        if (result != InspectionBase::Result::Success) {
            std::cout << ERROR_CONSOLE_TEXT << "Inspection upload failed, exiting."
                      << NORMAL_CONSOLE_TEXT << std::endl;
            return EXIT_FAILURE;
        }
    }

    sleep_for(seconds(4));

    {
        std::cout << "Downloading inspection..." << std::endl;
        const auto download_result = inspection.download_inspection();
        std::cout << RESULT_CONSOLE_TEXT
                  << "Inspection download result is: " << download_result.first
                  << NORMAL_CONSOLE_TEXT << std::endl;
        std::cout << RESULT_CONSOLE_TEXT << download_result.second << NORMAL_CONSOLE_TEXT
                  << std::endl;
    }

    inspection.subscribe_inspection_progress([&](InspectionBase::InspectionProgress progress) {
        std::cout << RESULT_CONSOLE_TEXT << progress << NORMAL_CONSOLE_TEXT << std::endl;
        if (progress.current == 2 && progress.reached == 2) {
            std::cout << "Setting current inspection item to 0" << std::endl;
            inspection.set_current_inspection_item(0);
        }
    });

    for (;;) {
        sleep_for(seconds(1));
    }

    std::cout << "Finished!" << std::endl;

    return EXIT_SUCCESS;
}

InspectionBase::WaypointItem make_waypoint_item(
    std::string task_uuid,
    uint16_t command,
    uint8_t autocontinue,
    float param1,
    float param2,
    float param3,
    float param4,
    float x,
    float y,
    float z)
{
    InspectionBase::WaypointItem new_item{};
    new_item.task_uuid = task_uuid;
    new_item.command = command;
    new_item.autocontinue = autocontinue;
    new_item.param1 = param1;
    new_item.param2 = param2;
    new_item.param3 = param3;
    new_item.param4 = param4;
    new_item.x = x;
    new_item.y = y;
    new_item.z = z;
    return new_item;
}
