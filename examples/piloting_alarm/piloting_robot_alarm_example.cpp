#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <mutex>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/alarm_robotic_vehicle/alarm_robotic_vehicle.h>

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

    auto alarm_rv = AlarmRoboticVehicle{target_system};

    AlarmBase::AlarmList alarm_list;
    AlarmBase::AlarmItem item1{0, "check0", "check point 0"};
    AlarmBase::AlarmItem item2{1, "check1", "check point 1"};
    AlarmBase::AlarmItem item3{2, "check2", "check point 2"};
    AlarmBase::AlarmItem item4{3, "check3", "check point 3"};
    AlarmBase::AlarmItem item5{4, "check4", "check point 4"};
    alarm_list.items.push_back(item1);
    alarm_list.items.push_back(item2);
    alarm_list.items.push_back(item3);
    alarm_list.items.push_back(item4);
    alarm_list.items.push_back(item5);

    std::mutex alarm_upload_mutex;
    bool alarm_upload{false};

    alarm_rv.upload_alarm_async(
        [&](AlarmBase::Result result, AlarmBase::Ack ack) {
            std::cout << RESULT_CONSOLE_TEXT << "Alarm upload result/ack is: " << result << " / "
                      << ack << NORMAL_CONSOLE_TEXT << std::endl;
            std::lock_guard<std::mutex> lock(alarm_upload_mutex);
            alarm_upload = true;
        },
        alarm_list);

    std::cout << "Waiting for alarm upload..." << std::endl;
    for (;;) {
        {
            std::lock_guard<std::mutex> lock(alarm_upload_mutex);
            if (alarm_upload) {
                std::cout << "Alarm upload!" << std::endl;
                break;
            }
        }
        sleep_for(seconds(1));
    }

    sleep_for(seconds(1));

    std::vector<AlarmBase::AlarmStatus> alarm_status_list;
    for (unsigned i = 0; i < alarm_list.items.size(); ++i) {
        AlarmBase::AlarmStatus alarm_status;
        alarm_status.index = i;
        alarm_status.status = AlarmBase::AlarmStatusType::Ok;
        alarm_status.errors_count = 0;
        alarm_status.warns_count = 0;
        alarm_status_list.push_back(alarm_status);
    }

    unsigned alarm_counter = 0;

    for (;;) {
        for (unsigned i = 0; i < alarm_status_list.size(); ++i) {
            std::cout << "Sending alarm status " << i << std::endl;
            auto& alarm_status = alarm_status_list[i];
            auto now = system_clock::now().time_since_epoch();
            alarm_status.stamp_ms = duration_cast<std::chrono::milliseconds>(now).count();
            if (alarm_counter == 1) {
                alarm_status.status = AlarmBase::AlarmStatusType::Warning;
                alarm_status.warns_count += 1;
            } else if (alarm_counter == 2) {
                alarm_status.status = AlarmBase::AlarmStatusType::Error;
                alarm_status.errors_count += 1;
            } else {
                alarm_status.status = AlarmBase::AlarmStatusType::Ok;
            }
            alarm_rv.send_alarm_status(alarm_status);

            alarm_counter = ++alarm_counter % 3;
        }

        sleep_for(milliseconds(1000));
    }

    std::cout << "Finished!" << std::endl;

    return EXIT_SUCCESS;
}
