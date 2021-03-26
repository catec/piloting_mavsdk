#include "checklist_robotic_vehicle_impl.h"
#include "system.h"
#include "global_include.h"
#include "mavlink_checklist_transfer_robotic_vehicle.h"

namespace mavsdk {

using namespace std::placeholders; // for `_1`

ChecklistRoboticVehicleImpl::ChecklistRoboticVehicleImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

ChecklistRoboticVehicleImpl::ChecklistRoboticVehicleImpl(std::shared_ptr<System> system) :
    PluginImplBase(system)
{
    _parent->register_plugin(this);
}

ChecklistRoboticVehicleImpl::~ChecklistRoboticVehicleImpl()
{
    _parent->unregister_plugin(this);
}

void ChecklistRoboticVehicleImpl::init()
{
    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_CHECK_LIST_REQUEST,
        [this](const mavlink_message_t&) {
            LogDebug() << "Checklist request list message received!";
            upload_checklist();
        },
        this);
}

void ChecklistRoboticVehicleImpl::deinit()
{
    _parent->unregister_all_mavlink_message_handlers(this);
}

void ChecklistRoboticVehicleImpl::enable() {}

void ChecklistRoboticVehicleImpl::disable() {}

void ChecklistRoboticVehicleImpl::set_upload_checklist(const ChecklistBase::Checklist& checklist)
{
    std::lock_guard<std::recursive_mutex> lock(_checklist_data.mutex);
    _checklist_data.checklist = checklist;
}

void ChecklistRoboticVehicleImpl::upload_checklist_async(
    const ChecklistBase::Checklist& checklist, const ChecklistBase::ResultAckCallback& callback)
{
    std::lock_guard<std::recursive_mutex> lock(_checklist_data.mutex);
    _checklist_data.checklist = checklist;
    _checklist_data.upload_callback = callback;
}

void ChecklistRoboticVehicleImpl::upload_checklist()
{
    std::lock_guard<std::recursive_mutex> lock(_checklist_data.mutex);

    if (!_checklist_data.upload_callback) {
        LogErr() << "Not upload callback set";
        return;
    }

    if (_checklist_data.last_upload.lock()) {
        LogWarn() << "Upload checklist is busy, skipping...";
        auto temp_callback = _checklist_data.upload_callback;
        _parent->call_user_callback([temp_callback]() {
            temp_callback(ChecklistBase::Result::Busy, ChecklistBase::Ack::Unknown);
        });
        return;
    }

    auto checklist_transfer = std::dynamic_pointer_cast<MAVLinkChecklistTransferRoboticVehicle>(
        _parent->checklist_transfer());
    if (!checklist_transfer) {
        LogWarn() << "Checklist Transfer is NULL";
        return;
    }

    auto checklist = convert_to_int_items(_checklist_data.checklist);

    _checklist_data.last_upload = checklist_transfer->upload_items_async(
        checklist,
        [this](MAVLinkChecklistTransfer::Result result, MAVLinkChecklistTransfer::Ack ack) {
            auto result_and_ack = convert_to_result_and_ack(result, ack);
            std::lock_guard<std::recursive_mutex> lock_cb(_checklist_data.mutex);
            if (_checklist_data.upload_callback) {
                auto temp_callback = _checklist_data.upload_callback;
                _parent->call_user_callback([temp_callback, result_and_ack]() {
                    temp_callback(result_and_ack.first, result_and_ack.second);
                });
            }
        }

    );
}

ChecklistBase::Result ChecklistRoboticVehicleImpl::cancel_checklist_upload()
{
    auto ptr = _checklist_data.last_upload.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No checklist upload to cancel... ignoring";
    }

    return ChecklistBase::Result::Success;
}

MAVLinkChecklistTransfer::Checklist
ChecklistRoboticVehicleImpl::convert_to_int_items(const ChecklistBase::Checklist& checklist_base)
{
    MAVLinkChecklistTransfer::Checklist checklist;

    for (const auto& item : checklist_base.items) {
        MAVLinkChecklistTransfer::ChecklistItem next_item{item.index, item.name, item.description};

        checklist.items.push_back(next_item);
    }

    return checklist;
}

std::pair<ChecklistBase::Result, ChecklistBase::Ack>
ChecklistRoboticVehicleImpl::convert_to_result_and_ack(
    MAVLinkChecklistTransfer::Result result, MAVLinkChecklistTransfer::Ack ack)
{
    std::pair<ChecklistBase::Result, ChecklistBase::Ack> result_pair;
    result_pair.first = convert_result(result);
    result_pair.second = convert_ack(ack);
    return result_pair;
}

ChecklistBase::Ack ChecklistRoboticVehicleImpl::convert_ack(MAVLinkChecklistTransfer::Ack ack)
{
    switch (ack) {
        case MAVLinkChecklistTransfer::Ack::Accepted:
            return ChecklistBase::Ack::Accepted;
        case MAVLinkChecklistTransfer::Ack::Error:
            return ChecklistBase::Ack::Error;
        case MAVLinkChecklistTransfer::Ack::NoSpace:
            return ChecklistBase::Ack::NoSpace;
        case MAVLinkChecklistTransfer::Ack::InvalidSequence:
            return ChecklistBase::Ack::InvalidSequence;
        case MAVLinkChecklistTransfer::Ack::Cancelled:
            return ChecklistBase::Ack::Cancelled;
        case MAVLinkChecklistTransfer::Ack::Unknown:
        default:
            return ChecklistBase::Ack::Unknown;
    }
}

ChecklistBase::Result
ChecklistRoboticVehicleImpl::convert_result(MAVLinkChecklistTransfer::Result result)
{
    switch (result) {
        case MAVLinkChecklistTransfer::Result::Success:
            return ChecklistBase::Result::Success;
        case MAVLinkChecklistTransfer::Result::ConnectionError:
            return ChecklistBase::Result::ConnectionError;
        case MAVLinkChecklistTransfer::Result::Timeout:
            return ChecklistBase::Result::Timeout;
        case MAVLinkChecklistTransfer::Result::Cancelled:
            return ChecklistBase::Result::TransferCancelled;
        case MAVLinkChecklistTransfer::Result::InvalidSequence:
            return ChecklistBase::Result::InvalidArgument;
        case MAVLinkChecklistTransfer::Result::ProtocolError:
            return ChecklistBase::Result::ProtocolError;
        default:
            return ChecklistBase::Result::Unknown;
    }
}

} // namespace mavsdk
