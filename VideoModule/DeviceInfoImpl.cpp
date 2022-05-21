#include "DeviceInfoImpl.h"
#include "VideoModule/windows/DeviceInfoDS.h"

DeviceInfoImpl::DeviceInfoImpl() 
  : _lastUsedDeviceName(NULL), _lastUsedDeviceNameLength(0) 
{

}

DeviceInfoImpl::~DeviceInfoImpl(void)
{
  std::lock_guard locker(_apiLock);
  free(_lastUsedDeviceName);

}

int32_t DeviceInfoImpl::NumberOfCapabilities(const char* deviceUniqueIdUTF8)
{
  return 0;
}

int32_t DeviceInfoImpl::GetCapability(const char* deviceUniqueIdUTF8, const uint32_t deviceCapabilityNumber, VideoCaptureCapability& capability)
{
  return 0;
}

int32_t DeviceInfoImpl::GetBestMatchedCapability(const char* deviceUniqueIdUTF8, const VideoCaptureCapability& requested, VideoCaptureCapability& resulting)
{
  return 0;
}

int32_t DeviceInfoImpl::GetOrientation(const char* deviceUniqueIdUTF8, VideoRotation& orientation)
{
  return 0;
}
