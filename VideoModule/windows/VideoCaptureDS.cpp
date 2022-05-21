#include "VideoCaptureDS.h"

VideoCaptureDS::VideoCaptureDS()
{

}

VideoCaptureDS::~VideoCaptureDS()
{

}

int32_t VideoCaptureDS::Init(const char* deviceUniqueIdUTF8)
{
  return 0;
}

int32_t VideoCaptureDS::StartCapture(const VideoCaptureCapability& capability)
{
  return 0;
}

int32_t VideoCaptureDS::StopCapture()
{
  return 0;
}

bool VideoCaptureDS::CaptureStarted()
{
  bool result = false;
  return result;
}

int32_t VideoCaptureDS::CaptureSettings(VideoCaptureCapability& settings)
{
  return 0;
}

int32_t VideoCaptureDS::SetCameraOutput(const VideoCaptureCapability& requestedCapability)
{
  return 0;
}

int32_t VideoCaptureDS::DisconnectGraph()
{
  return 0;
}

HRESULT VideoCaptureDS::ConnectDVCamera()
{
  return 0;
}
