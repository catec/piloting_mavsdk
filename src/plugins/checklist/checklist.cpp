#include "checklist_impl.h"
#include "plugins/checklist/checklist.h"

namespace mavsdk {

Checklist::Checklist(System& system) : PluginBase(), _impl{new ChecklistImpl(system)} {}

Checklist::Checklist(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{new ChecklistImpl(system)}
{}

Checklist::~Checklist() {}

void Checklist::download_checklist_async(const ChecklistBase::DownloadChecklistCallback callback)
{
    _impl->download_checklist_async(callback);
}

std::pair<ChecklistBase::Result, ChecklistBase::Checklist> Checklist::download_checklist() const
{
    return _impl->download_checklist();
}

ChecklistBase::Result Checklist::cancel_checklist_download() const
{
    return _impl->cancel_checklist_download();
}

} // namespace mavsdk
