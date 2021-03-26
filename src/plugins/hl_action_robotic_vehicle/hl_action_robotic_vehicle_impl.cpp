#include "hl_action_robotic_vehicle_impl.h"
#include "system.h"
#include "global_include.h"
#include "mavlink_hl_action_transfer_robotic_vehicle.h"

namespace mavsdk {

using namespace std::placeholders; // for `_1`

HLActionRoboticVehicleImpl::HLActionRoboticVehicleImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

HLActionRoboticVehicleImpl::HLActionRoboticVehicleImpl(std::shared_ptr<System> system) :
    PluginImplBase(system)
{
    _parent->register_plugin(this);
}

HLActionRoboticVehicleImpl::~HLActionRoboticVehicleImpl()
{
    _parent->unregister_plugin(this);
}

void HLActionRoboticVehicleImpl::init()
{
    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_HL_ACTION_LIST_REQUEST,
        [this](const mavlink_message_t&) {
            LogDebug() << "HLAction request list message received!";
            upload_hl_action();
        },
        this);
}

void HLActionRoboticVehicleImpl::deinit()
{
    _parent->unregister_all_mavlink_message_handlers(this);
}

void HLActionRoboticVehicleImpl::enable() {}

void HLActionRoboticVehicleImpl::disable() {}

void HLActionRoboticVehicleImpl::set_upload_hl_action(
    const HLActionBase::HLActionList& hl_action_list)
{
    std::lock_guard<std::recursive_mutex> lock(_hl_action_data.mutex);
    _hl_action_data.hl_action_list = hl_action_list;
}

void HLActionRoboticVehicleImpl::upload_hl_action_async(
    const HLActionBase::HLActionList& hl_action_list,
    const HLActionBase::ResultAckCallback& callback)
{
    std::lock_guard<std::recursive_mutex> lock(_hl_action_data.mutex);
    _hl_action_data.hl_action_list = hl_action_list;
    _hl_action_data.upload_callback = callback;
}

void HLActionRoboticVehicleImpl::upload_hl_action()
{
    std::lock_guard<std::recursive_mutex> lock(_hl_action_data.mutex);

    if (!_hl_action_data.upload_callback) {
        LogErr() << "Not upload callback set";
        return;
    }

    if (_hl_action_data.last_upload.lock()) {
        LogWarn() << "Upload hl_action is busy, skipping...";
        auto temp_callback = _hl_action_data.upload_callback;
        _parent->call_user_callback([temp_callback]() {
            temp_callback(HLActionBase::Result::Busy, HLActionBase::Ack::Unknown);
        });
        return;
    }

    auto hl_action_transfer = std::dynamic_pointer_cast<MAVLinkHLActionTransferRoboticVehicle>(
        _parent->hl_action_transfer());
    if (!hl_action_transfer) {
        LogWarn() << "HLAction Transfer is NULL";
        return;
    }

    auto hl_action_list = convert_to_int_items(_hl_action_data.hl_action_list);

    _hl_action_data.last_upload = hl_action_transfer->upload_items_async(
        hl_action_list,
        [this](MAVLinkHLActionTransfer::Result result, MAVLinkHLActionTransfer::Ack ack) {
            auto result_and_ack = convert_to_result_and_ack(result, ack);
            std::lock_guard<std::recursive_mutex> lock_cb(_hl_action_data.mutex);
            if (_hl_action_data.upload_callback) {
                auto temp_callback = _hl_action_data.upload_callback;
                _parent->call_user_callback([temp_callback, result_and_ack]() {
                    temp_callback(result_and_ack.first, result_and_ack.second);
                });
            }
        });
}

HLActionBase::Result HLActionRoboticVehicleImpl::cancel_hl_action_upload()
{
    auto ptr = _hl_action_data.last_upload.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No hl_action upload to cancel... ignoring";
    }

    return HLActionBase::Result::Success;
}

MAVLinkHLActionTransfer::HLActionList HLActionRoboticVehicleImpl::convert_to_int_items(
    const HLActionBase::HLActionList& hl_actions_list_base)
{
    MAVLinkHLActionTransfer::HLActionList hl_action_list;

    for (const auto& item : hl_actions_list_base.items) {
        MAVLinkHLActionTransfer::HLActionItem next_item{
            item.index, item.command, item.name, item.description};

        hl_action_list.items.push_back(next_item);
    }

    return hl_action_list;
}

std::pair<HLActionBase::Result, HLActionBase::Ack>
HLActionRoboticVehicleImpl::convert_to_result_and_ack(
    MAVLinkHLActionTransfer::Result result, MAVLinkHLActionTransfer::Ack ack)
{
    std::pair<HLActionBase::Result, HLActionBase::Ack> result_pair;
    result_pair.first = convert_result(result);
    result_pair.second = convert_ack(ack);
    return result_pair;
}

HLActionBase::Ack HLActionRoboticVehicleImpl::convert_ack(MAVLinkHLActionTransfer::Ack ack)
{
    switch (ack) {
        case MAVLinkHLActionTransfer::Ack::Accepted:
            return HLActionBase::Ack::Accepted;
        case MAVLinkHLActionTransfer::Ack::Error:
            return HLActionBase::Ack::Error;
        case MAVLinkHLActionTransfer::Ack::NoSpace:
            return HLActionBase::Ack::NoSpace;
        case MAVLinkHLActionTransfer::Ack::InvalidSequence:
            return HLActionBase::Ack::InvalidSequence;
        case MAVLinkHLActionTransfer::Ack::Cancelled:
            return HLActionBase::Ack::Cancelled;
        case MAVLinkHLActionTransfer::Ack::Unknown:
        default:
            return HLActionBase::Ack::Unknown;
    }
}

HLActionBase::Result
HLActionRoboticVehicleImpl::convert_result(MAVLinkHLActionTransfer::Result result)
{
    switch (result) {
        case MAVLinkHLActionTransfer::Result::Success:
            return HLActionBase::Result::Success;
        case MAVLinkHLActionTransfer::Result::ConnectionError:
            return HLActionBase::Result::ConnectionError;
        case MAVLinkHLActionTransfer::Result::Timeout:
            return HLActionBase::Result::Timeout;
        case MAVLinkHLActionTransfer::Result::Cancelled:
            return HLActionBase::Result::TransferCancelled;
        case MAVLinkHLActionTransfer::Result::InvalidSequence:
            return HLActionBase::Result::InvalidArgument;
        case MAVLinkHLActionTransfer::Result::ProtocolError:
            return HLActionBase::Result::ProtocolError;
        default:
            return HLActionBase::Result::Unknown;
    }
}

} // namespace mavsdk
