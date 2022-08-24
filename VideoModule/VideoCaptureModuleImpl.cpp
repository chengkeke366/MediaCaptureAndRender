#include "VideoCaptureModuleImpl.h"
#include "VideoModule/windows/VideoCaptureDS.h"
#include "VideoModule/windows/DeviceInfoDS.h"

std::shared_ptr<VideoCaptureModule> VideoCaptureModuleImpl::Create(const char* deviceIdUtf8)
{
  if (deviceIdUtf8 == nullptr)
  {
    return nullptr;
  }
#ifdef WIN32
  auto impl = std::make_shared<VideoCaptureDS>();
  impl->Init(deviceIdUtf8);
#elif __linux__

#elif __APPLE__
  #include <TargetConditionals.h>
  #if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
  #elif TARGET_OS_IPHONE
    // iOS device
  #elif TARGET_OS_MAC
    //Mac OS
  #endif
#endif 

  return impl;
}

VideoCaptureModule::DeviceInfo* VideoCaptureModuleImpl::CreateDeviceInfo()
{
#ifdef WIN32
  return DeviceInfoDS::Create();
#elif __linux__

#elif __APPLE__
  #include <TargetConditionals.h>
  #if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
  #elif TARGET_OS_IPHONE
    // iOS device
  #elif TARGET_OS_MAC
    //Mac OS
  #endif
#endif 
}

void VideoCaptureModuleImpl::RegisterCaptureDataCallback(VideoSinkInterface* dataCallback)
{

}

void VideoCaptureModuleImpl::DeRegisterCaptureDataCallback()
{

}

int32_t VideoCaptureModuleImpl::StartCapture(const VideoCaptureCapability& capability)
{
  return 0;
}

int32_t VideoCaptureModuleImpl::StopCapture()
{
  return 0;
}

const char* VideoCaptureModuleImpl::CurrentDeviceName() const
{
  return 0;
}

bool VideoCaptureModuleImpl::CaptureStarted()
{
  return 0;
}

int32_t VideoCaptureModuleImpl::CaptureSettings(VideoCaptureCapability& settings)
{
  return 0;
}

int32_t VideoCaptureModuleImpl::SetCaptureRotation(VideoRotation rotation)
{
  return 0;
}

bool VideoCaptureModuleImpl::SetApplyRotation(bool enable)
{
  return 0;
}

bool VideoCaptureModuleImpl::GetApplyRotation()
{
  return 0;
}

int32_t VideoCaptureModuleImpl::IncomingFrame(uint8_t* videoFrame, size_t videoFrameLength, const VideoCaptureCapability& frameInfo, int64_t captureTime /*= 0*/)
{
  return 0;
}

VideoCaptureModuleImpl::VideoCaptureModuleImpl()
{

}

VideoCaptureModuleImpl::~VideoCaptureModuleImpl()
{

}
