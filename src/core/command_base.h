#pragma once

#include <functional>

#include "mavlink_include.h"

namespace mavsdk {

class CommandBase {
public:
    /**
     * @brief Default Constructor.
     */
    CommandBase() = default;

    /**
     * @brief Default Destructor.
     */
    virtual ~CommandBase() = default;

    enum class Result {
        Success,
        ConnectionError,
        Busy,
        Timeout,
        Cancelled,
        InProgress
    };

    friend std::ostream& operator<<(std::ostream& str, Result const& value);

    struct CommandAck {
        uint16_t command{0};
        uint8_t result{0};
        uint8_t progress{0};
        int32_t result_param2{0};

        CommandAck() {}

        CommandAck(const mavlink_command_ack_t& command_ack)
        {
            command = command_ack.command;
            result = command_ack.result;
            progress = command_ack.progress;
            result_param2 = command_ack.result_param2;
        }
    };

    friend bool operator==(const CommandAck& lhs, const CommandAck& rhs);

    friend std::ostream& operator<<(std::ostream& str, CommandAck const& value);

    struct CommandLong {
        uint16_t command{0};
        uint8_t confirmation = 0;
        struct Params {
            float param1 = NAN;
            float param2 = NAN;
            float param3 = NAN;
            float param4 = NAN;
            float param5 = NAN;
            float param6 = NAN;
            float param7 = NAN;
        } params{};

        CommandLong() {}

        CommandLong(const mavlink_command_long_t& command_long)
        {
            command = command_long.command;
            confirmation = command_long.confirmation;
            params.param1 = command_long.param1;
            params.param2 = command_long.param2;
            params.param3 = command_long.param3;
            params.param4 = command_long.param4;
            params.param5 = command_long.param5;
            params.param6 = command_long.param6;
            params.param7 = command_long.param7;
        }

        // TODO: rename to set_all
        static void set_as_reserved(Params& params, float reserved_value = NAN)
        {
            params.param1 = reserved_value;
            params.param2 = reserved_value;
            params.param3 = reserved_value;
            params.param4 = reserved_value;
            params.param5 = reserved_value;
            params.param6 = reserved_value;
            params.param7 = reserved_value;
        }
    };

    friend bool operator==(const CommandLong& lhs, const CommandLong& rhs);

    friend std::ostream& operator<<(std::ostream& str, CommandLong const& value);

    using ResultAndAckCallback = std::function<void(Result, CommandAck)>;
    using CommandCallback = std::function<void(CommandLong)>;

    /**
     * @brief Copy constructor (object is not copyable).
     */
    CommandBase(const CommandBase&) = delete;

    /**
     * @brief Assign operator (object is not copyable).
     */
    const CommandBase& operator=(const CommandBase&) = delete;
};

} // namespace mavsdk
