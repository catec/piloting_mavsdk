#include "inspection_impl.h"
#include "plugins/inspection/inspection.h"

namespace mavsdk {

Inspection::Inspection(System& system) : PluginBase(), _impl{new InspectionImpl(system)} {}

Inspection::Inspection(std::shared_ptr<System> system) :
    PluginBase(),
    _impl{new InspectionImpl(system)}
{}

Inspection::~Inspection() {}

void Inspection::upload_inspection_async(
    InspectionBase::InspectionPlan inspection_plan, const InspectionBase::ResultAckCallback callback)
{
    _impl->upload_inspection_async(inspection_plan, callback);
}

std::pair<InspectionBase::Result, InspectionBase::Ack>
Inspection::upload_inspection(InspectionBase::InspectionPlan inspection_plan) const
{
    return _impl->upload_inspection(inspection_plan);
}

InspectionBase::Result Inspection::cancel_inspection_upload() const
{
    return _impl->cancel_inspection_upload();
}

void Inspection::download_inspection_async(
    const InspectionBase::DownloadInspectionCallback callback)
{
    _impl->download_inspection_async(callback);
}

std::pair<InspectionBase::Result, InspectionBase::InspectionPlan>
Inspection::download_inspection() const
{
    return _impl->download_inspection();
}

InspectionBase::Result Inspection::cancel_inspection_download() const
{
    return _impl->cancel_inspection_download();
}

void Inspection::set_current_inspection_item(uint16_t index) const
{
    _impl->set_current_inspection_item(index);
}

void Inspection::subscribe_inspection_progress(InspectionBase::InspectionProgressCallback callback)
{
    _impl->inspection_progress_async(callback);
}

InspectionBase::InspectionProgress Inspection::inspection_progress() const
{
    return _impl->inspection_progress();
}

} // namespace mavsdk
