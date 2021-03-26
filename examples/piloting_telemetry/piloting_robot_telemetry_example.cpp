#include <chrono>
#include <iostream>
#include <thread>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry_robotic_vehicle/telemetry_robotic_vehicle.h>

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
    std::vector<uint8_t> sensor_component_ids{35, 45};
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

    sleep_for(seconds(3));

    ///////////////////

    auto telemetry_rv = TelemetryRoboticVehicle{target_system};

    TelemetryBase::PositionVelocityNed vehicle_position;
    vehicle_position.system_id = local_system_id;
    vehicle_position.component_id = local_component_id;
    vehicle_position.position.north_m = 1.1f;
    vehicle_position.position.east_m = 2.2f;
    vehicle_position.position.down_m = 3.3f;
    vehicle_position.velocity.north_m_s = 4.4f;
    vehicle_position.velocity.east_m_s = 5.5f;
    vehicle_position.velocity.down_m_s = 6.6f;

    std::vector<TelemetryBase::PositionVelocityNed> sensor_positions;
    for (unsigned i = 0; i < sensor_component_ids.size(); ++i) {
        TelemetryBase::PositionVelocityNed data;
        data.system_id = local_system_id;
        data.component_id = sensor_component_ids[i];
        data.position.north_m = 1.1f;
        data.position.east_m = 2.2f;
        data.position.down_m = 3.3f;
        data.velocity.north_m_s = 4.4f;
        data.velocity.east_m_s = 5.5f;
        data.velocity.down_m_s = 6.6f;
        sensor_positions.push_back(data);
    }

    TelemetryBase::Attitude vehicle_attitude;
    vehicle_attitude.system_id = local_system_id;
    vehicle_attitude.component_id = local_component_id;
    vehicle_attitude.quaternion_angle.x = 7.7f;
    vehicle_attitude.quaternion_angle.y = 8.8f;
    vehicle_attitude.quaternion_angle.z = 9.9f;
    vehicle_attitude.quaternion_angle.w = 10.10f;
    vehicle_attitude.angular_velocity.roll_rad_s = 11.11f;
    vehicle_attitude.angular_velocity.pitch_rad_s = 12.12f;
    vehicle_attitude.angular_velocity.yaw_rad_s = 13.13f;

    std::vector<TelemetryBase::Attitude> sensor_attitudes;
    for (unsigned i = 0; i < sensor_component_ids.size(); ++i) {
        TelemetryBase::Attitude data;
        data.system_id = local_system_id;
        data.component_id = sensor_component_ids[i];
        data.quaternion_angle.x = 7.7f;
        data.quaternion_angle.y = 8.8f;
        data.quaternion_angle.z = 9.9f;
        data.quaternion_angle.w = 10.10f;
        data.angular_velocity.roll_rad_s = 11.11f;
        data.angular_velocity.pitch_rad_s = 12.12f;
        data.angular_velocity.yaw_rad_s = 13.13f;
        sensor_attitudes.push_back(data);
    }

    TelemetryBase::TextStatus text_status;
    text_status.type = TelemetryBase::TextStatusType::Info;
    text_status.text = "text status example";

    unsigned text_counter = 0;

    for (;;) {
        std::cout << "Sending vehicle position" << std::endl;
        auto now = system_clock::now().time_since_epoch();
        vehicle_position.stamp_ms = duration_cast<milliseconds>(now).count();
        telemetry_rv.send_local_position_ned(vehicle_position);

        for (unsigned i = 0; i < sensor_positions.size(); ++i) {
            auto& sensor_position = sensor_positions[i];
            std::cout << "Sending sensor (" << unsigned(sensor_position.component_id)
                      << ") position" << std::endl;
            auto now = system_clock::now().time_since_epoch();
            sensor_position.stamp_ms = duration_cast<milliseconds>(now).count();
            telemetry_rv.send_local_position_ned(sensor_position);
        }

        std::cout << "Sending vehicle attitude" << std::endl;
        now = system_clock::now().time_since_epoch();
        vehicle_attitude.stamp_ms = duration_cast<milliseconds>(now).count();
        telemetry_rv.send_attitude(vehicle_attitude);

        for (unsigned i = 0; i < sensor_attitudes.size(); ++i) {
            auto& sensor_attitude = sensor_attitudes[i];
            std::cout << "Sending sensor (" << unsigned(sensor_attitude.component_id)
                      << ") attitude" << std::endl;
            auto now = system_clock::now().time_since_epoch();
            sensor_attitude.stamp_ms = duration_cast<milliseconds>(now).count();
            telemetry_rv.send_attitude(sensor_attitude);
        }

        std::cout << "Sending text status" << std::endl;
        if (text_counter == 1) {
            text_status.type = TelemetryBase::TextStatusType::Warning;
        } else if (text_counter == 2) {
            text_status.type = TelemetryBase::TextStatusType::Error;
        } else {
            text_status.type = TelemetryBase::TextStatusType::Info;
        }
        telemetry_rv.send_text_status(text_status);

        text_counter = ++text_counter % 3;

        sleep_for(milliseconds(1000));
    }

    std::cout << "Finished..." << std::endl;

    return EXIT_SUCCESS;
}
