#include "hl_action_impl.h"
#include "plugins/hl_action/hl_action.h"

namespace mavsdk {

HLAction::HLAction(System& system) : PluginBase(), _impl{new HLActionImpl(system)} {}

HLAction::HLAction(std::shared_ptr<System> system) : PluginBase(), _impl{new HLActionImpl(system)}
{}

HLAction::~HLAction() {}

void HLAction::download_hl_action_async(const HLActionBase::DownloadHLActionListCallback callback)
{
    _impl->download_hl_action_async(callback);
}

std::pair<HLActionBase::Result, HLActionBase::HLActionList> HLAction::download_hl_action() const
{
    return _impl->download_hl_action();
}

HLActionBase::Result HLAction::cancel_hl_action_download() const
{
    return _impl->cancel_hl_action_download();
}

} // namespace mavsdk
