#include "VideoCaptureModuleImpl.h"
#include "VideoModule/windows/VideoCaptureDS.h"

std::shared_ptr<VideoCaptureModule> VideoCaptureModuleImpl::Create(const char* deviceIdUtf8)
{
  if (deviceIdUtf8 == nullptr)
  {
    return nullptr;
  }
  auto impl = std::make_shared<VideoCaptureDS>();
  impl->Init(deviceIdUtf8);
  return impl;
}

void VideoCaptureModuleImpl::RegisterCaptureDataCallback(VideoSinkInterface* dataCallback)
{

}

void VideoCaptureModuleImpl::DeRegisterCaptureDataCallback()
{

}

int32_t VideoCaptureModuleImpl::StartCapture(const VideoCaptureCapability& capability)
{

}

int32_t VideoCaptureModuleImpl::StopCapture()
{

}

const char* VideoCaptureModuleImpl::CurrentDeviceName() const
{

}

bool VideoCaptureModuleImpl::CaptureStarted()
{

}

int32_t VideoCaptureModuleImpl::CaptureSettings(VideoCaptureCapability& settings)
{

}

int32_t VideoCaptureModuleImpl::SetCaptureRotation(VideoRotation rotation)
{

}

bool VideoCaptureModuleImpl::SetApplyRotation(bool enable)
{

}

bool VideoCaptureModuleImpl::GetApplyRotation()
{

}

int32_t VideoCaptureModuleImpl::IncomingFrame(uint8_t* videoFrame, size_t videoFrameLength, const VideoCaptureCapability& frameInfo, int64_t captureTime /*= 0*/)
{

}

VideoCaptureModuleImpl::VideoCaptureModuleImpl()
{

}

VideoCaptureModuleImpl::~VideoCaptureModuleImpl()
{

}
