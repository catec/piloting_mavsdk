#pragma once

#include "plugins/checklist/checklist.h"
#include "mavlink_include.h"
#include "plugin_impl_base.h"
#include "system.h"
#include "mavlink_checklist_transfer.h"

namespace mavsdk {

class ChecklistImpl : public PluginImplBase {
public:
    explicit ChecklistImpl(System& system);
    explicit ChecklistImpl(std::shared_ptr<System> system);
    ~ChecklistImpl();

    void init() override;
    void deinit() override;

    void enable() override;
    void disable() override;

    std::pair<ChecklistBase::Result, ChecklistBase::Checklist> download_checklist();
    void download_checklist_async(const ChecklistBase::DownloadChecklistCallback& callback);
    ChecklistBase::Result cancel_checklist_download();

    // Non-copyable
    ChecklistImpl(const ChecklistImpl&) = delete;
    const ChecklistImpl& operator=(const ChecklistImpl&) = delete;

private:
    // FIXME: make static
    std::pair<ChecklistBase::Result, ChecklistBase::Checklist> convert_to_result_and_checklist(
        MAVLinkChecklistTransfer::Result result, MAVLinkChecklistTransfer::Checklist& checklist);

    static ChecklistBase::Result convert_result(MAVLinkChecklistTransfer::Result result);

    struct ChecklistData {
        mutable std::recursive_mutex mutex{};
        std::weak_ptr<MAVLinkChecklistTransfer::WorkItem> last_download{};
    } _checklist_data{};

    void* _timeout_cookie{nullptr};
};

} // namespace mavsdk
