#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include "VideoCaptureModule.h"
#include "VideoCaptureCapability.h"

/*封装的意义：不同平台可以提供共用这一套API 进行设备信息访问*/

class DeviceInfoImpl : public VideoCaptureModule::DeviceInfo
{
public:
  DeviceInfoImpl();
  ~DeviceInfoImpl(void) override;
  int32_t NumberOfCapabilities(const char* deviceUniqueIdUTF8) override;
  int32_t GetCapability(const char* deviceUniqueIdUTF8,
    const uint32_t deviceCapabilityNumber,
    VideoCaptureCapability& capability) override;

  int32_t GetBestMatchedCapability(const char* deviceUniqueIdUTF8,
    const VideoCaptureCapability& requested,
    VideoCaptureCapability& resulting) override;
  int32_t GetOrientation(const char* deviceUniqueIdUTF8,
    VideoRotation& orientation) override;

protected:
  /* Initialize this object*/

  virtual int32_t Init() = 0;
  /*
   * Fills the member variable _captureCapabilities with capabilities for the
   * given device name.
   */
  virtual int32_t CreateCapabilityMap(const char* deviceUniqueIdUTF8)=0;

protected:
  // Data members
  typedef std::vector<VideoCaptureCapability> VideoCaptureCapabilities;
  VideoCaptureCapabilities _captureCapabilities;
  std::mutex _apiLock;
  char* _lastUsedDeviceName;
  uint32_t _lastUsedDeviceNameLength;

};

