#pragma once

#include "plugins/hl_action/hl_action.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"
#include "mavlink_hl_action_transfer.h"

namespace mavsdk {

class HLActionImpl : public PluginImplBase {
public:
    explicit HLActionImpl(System& system);
    explicit HLActionImpl(std::shared_ptr<System> system);
    ~HLActionImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    std::pair<HLActionBase::Result, HLActionBase::HLActionList> download_hl_action();
    void download_hl_action_async(const HLActionBase::DownloadHLActionListCallback& callback);
    HLActionBase::Result cancel_hl_action_download();

    // Non-copyable
    HLActionImpl(const HLActionImpl&) = delete;
    const HLActionImpl& operator=(const HLActionImpl&) = delete;

private:
    // FIXME: make static
    std::pair<HLActionBase::Result, HLActionBase::HLActionList> convert_to_result_and_hl_action(
        MAVLinkHLActionTransfer::Result result,
        MAVLinkHLActionTransfer::HLActionList& hl_action_list);

    static HLActionBase::Result convert_result(MAVLinkHLActionTransfer::Result result);

    struct HLActionData {
        mutable std::recursive_mutex mutex{};
        std::weak_ptr<MAVLinkHLActionTransfer::WorkItem> last_download{};
    } _hl_action_data{};

    void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
