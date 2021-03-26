#include "checklist_impl.h"
#include "system.h"
#include "global_include.h"
#include "mavlink_checklist_transfer_ground_station.h"

namespace mavsdk {

using namespace std::placeholders; // for `_1`

ChecklistImpl::ChecklistImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

ChecklistImpl::ChecklistImpl(std::shared_ptr<System> system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

ChecklistImpl::~ChecklistImpl()
{
    _parent->unregister_plugin(this);
}

void ChecklistImpl::init() {}

void ChecklistImpl::enable() {}

void ChecklistImpl::disable() {}

void ChecklistImpl::deinit()
{
    _parent->unregister_timeout_handler(_timeout_cookie);
    _parent->unregister_all_mavlink_message_handlers(this);
}

std::pair<ChecklistBase::Result, ChecklistBase::Checklist> ChecklistImpl::download_checklist()
{
    auto prom = std::promise<std::pair<ChecklistBase::Result, ChecklistBase::Checklist>>();
    auto fut = prom.get_future();

    download_checklist_async(
        [&prom](ChecklistBase::Result result, ChecklistBase::Checklist checklist) {
            prom.set_value(std::make_pair<>(result, checklist));
        });
    return fut.get();
}

void ChecklistImpl::download_checklist_async(
    const ChecklistBase::DownloadChecklistCallback& callback)
{
    if (_checklist_data.last_download.lock()) {
        _parent->call_user_callback([callback]() {
            if (callback) {
                ChecklistBase::Checklist checklist{};
                callback(ChecklistBase::Result::Busy, checklist);
            }
        });
        return;
    }

    auto checklist_transfer = std::dynamic_pointer_cast<MAVLinkChecklistTransferGroundStation>(
        _parent->checklist_transfer());
    if (!checklist_transfer) {
        LogWarn() << "Checklist Transfer is NULL";
        return;
    }

    _checklist_data.last_download = checklist_transfer->download_items_async(
        [this, callback](
            MAVLinkChecklistTransfer::Result result,
            MAVLinkChecklistTransfer::Checklist checklist) {
            auto result_and_checklist = convert_to_result_and_checklist(result, checklist);
            _parent->call_user_callback([callback, result_and_checklist]() {
                if (callback) {
                    callback(result_and_checklist.first, result_and_checklist.second);
                }
            });
        });
}

ChecklistBase::Result ChecklistImpl::cancel_checklist_download()
{
    auto ptr = _checklist_data.last_download.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No checklist download to cancel... ignoring";
    }

    return ChecklistBase::Result::Success;
}

std::pair<ChecklistBase::Result, ChecklistBase::Checklist>
ChecklistImpl::convert_to_result_and_checklist(
    MAVLinkChecklistTransfer::Result result, MAVLinkChecklistTransfer::Checklist& checklist)
{
    std::pair<ChecklistBase::Result, ChecklistBase::Checklist> result_pair;

    result_pair.first = convert_result(result);
    if (result_pair.first != ChecklistBase::Result::Success) {
        return result_pair;
    }

    ChecklistBase::ChecklistItem new_checklist_item{};
    for (const auto& int_item : checklist.items) {
        LogDebug() << "Assembling Message: " << int(int_item.index);

        new_checklist_item.index = int_item.index;
        new_checklist_item.name = int_item.name;
        new_checklist_item.description = int_item.description;

        result_pair.second.items.push_back(new_checklist_item);
    }

    return result_pair;
}

ChecklistBase::Result ChecklistImpl::convert_result(MAVLinkChecklistTransfer::Result result)
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
