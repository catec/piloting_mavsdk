#include "hl_action_impl.h"
#include "system.h"
#include "global_include.h"
#include "mavlink_hl_action_transfer_ground_station.h"

namespace mavsdk {

using namespace std::placeholders; // for `_1`

HLActionImpl::HLActionImpl(System& system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

HLActionImpl::HLActionImpl(std::shared_ptr<System> system) : PluginImplBase(system)
{
    _parent->register_plugin(this);
}

HLActionImpl::~HLActionImpl()
{
    _parent->unregister_plugin(this);
}

void HLActionImpl::init() {}

void HLActionImpl::enable() {}

void HLActionImpl::disable() {}

void HLActionImpl::deinit()
{
    _parent->unregister_timeout_handler(_timeout_cookie);
    _parent->unregister_all_mavlink_message_handlers(this);
}

std::pair<HLActionBase::Result, HLActionBase::HLActionList> HLActionImpl::download_hl_action()
{
    auto prom = std::promise<std::pair<HLActionBase::Result, HLActionBase::HLActionList>>();
    auto fut = prom.get_future();

    download_hl_action_async(
        [&prom](HLActionBase::Result result, HLActionBase::HLActionList hl_action_list) {
            prom.set_value(std::make_pair<>(result, hl_action_list));
        });
    return fut.get();
}

void HLActionImpl::download_hl_action_async(
    const HLActionBase::DownloadHLActionListCallback& callback)
{
    if (_hl_action_data.last_download.lock()) {
        _parent->call_user_callback([callback]() {
            if (callback) {
                HLActionBase::HLActionList hl_action_list{};
                callback(HLActionBase::Result::Busy, hl_action_list);
            }
        });
        return;
    }

    auto hl_action_transfer = std::dynamic_pointer_cast<MAVLinkHLActionTransferGroundStation>(
        _parent->hl_action_transfer());
    if (!hl_action_transfer) {
        LogWarn() << "HLAction Transfer is NULL";
        return;
    }

    _hl_action_data.last_download = hl_action_transfer->download_items_async(
        [this, callback](
            MAVLinkHLActionTransfer::Result result,
            MAVLinkHLActionTransfer::HLActionList hl_action_list) {
            auto result_and_hl_action = convert_to_result_and_hl_action(result, hl_action_list);
            _parent->call_user_callback([callback, result_and_hl_action]() {
                if (callback) {
                    callback(result_and_hl_action.first, result_and_hl_action.second);
                }
            });
        });
}

HLActionBase::Result HLActionImpl::cancel_hl_action_download()
{
    auto ptr = _hl_action_data.last_download.lock();
    if (ptr) {
        ptr->cancel();
    } else {
        LogWarn() << "No hl_action download to cancel... ignoring";
    }

    return HLActionBase::Result::Success;
}

std::pair<HLActionBase::Result, HLActionBase::HLActionList>
HLActionImpl::convert_to_result_and_hl_action(
    MAVLinkHLActionTransfer::Result result, MAVLinkHLActionTransfer::HLActionList& hl_action_list)
{
    std::pair<HLActionBase::Result, HLActionBase::HLActionList> result_pair;

    result_pair.first = convert_result(result);
    if (result_pair.first != HLActionBase::Result::Success) {
        return result_pair;
    }

    HLActionBase::HLActionItem new_hl_action_item{};
    for (const auto& int_item : hl_action_list.items) {
        LogDebug() << "Assembling Message: " << int(int_item.index);

        new_hl_action_item.index = int_item.index;
        new_hl_action_item.command = int_item.command;
        new_hl_action_item.name = int_item.name;
        new_hl_action_item.description = int_item.description;

        result_pair.second.items.push_back(new_hl_action_item);
    }

    return result_pair;
}

HLActionBase::Result HLActionImpl::convert_result(MAVLinkHLActionTransfer::Result result)
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
