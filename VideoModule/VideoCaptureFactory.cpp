#include "VideoCaptureFactory.h"
#include "VideoCaptureModuleImpl.h"
#ifdef WIN32

#elif MAC_OS

#endif // WIN32

#include "windows/DeviceInfoDS.h"


std::shared_ptr<VideoCaptureModule> VideoCaptureFactory::Create(const char* deviceIdUTF8)
{
  return VideoCaptureModuleImpl::Create(deviceIdUTF8);
}

VideoCaptureModule::DeviceInfo* VideoCaptureFactory::CreateDeviceInfo()
{
  return VideoCaptureModuleImpl::CreateDeviceInfo();
}
