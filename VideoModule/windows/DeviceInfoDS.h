#pragma once

#include <memory>
#include "VideoModule/DeviceInfoImpl.h"
class DeviceInfoDS : public DeviceInfoImpl {

public:
  // Factory function.
  static DeviceInfoDS* Create();
  DeviceInfoDS();
  ~DeviceInfoDS() override;

  int32_t Init() override;
  int32_t CreateCapabilityMap(const char* deviceUniqueIdUTF8) override;
  virtual uint32_t NumberOfDevices() override;

  // Returns the available capture devices.
  // deviceNumber   - Index of capture device.
  // deviceNameUTF8 - Friendly name of the capture device.
  // deviceUniqueIdUTF8 - Unique name of the capture device if it exist.
  //                      Otherwise same as deviceNameUTF8.
  // productUniqueIdUTF8 - Unique product id if it exist.
  //                       Null terminated otherwise.
  int32_t GetDeviceName(uint32_t deviceNumber,
    char* deviceNameUTF8,
    uint32_t deviceNameLength,
    char* deviceUniqueIdUTF8,
    uint32_t deviceUniqueIdUTF8Length,
    char* productUniqueIdUTF8 = 0,
    uint32_t productUniqueIdUTF8Length = 0) override;

  // Returns the number of capabilities this device.
  int32_t NumberOfCapabilities(const char* deviceUniqueIdUTF8) override;

  // Gets the capabilities of the named device.
  int32_t GetCapability(const char* deviceUniqueIdUTF8,
    const uint32_t deviceCapabilityNumber,
    VideoCaptureCapability& capability) override;


  // Gets clockwise angle the captured frames should be rotated in order
  // to be displayed correctly on a normally rotated display.
  int32_t GetOrientation(const char* deviceUniqueIdUTF8,
    VideoRotation& orientation) override;

  // Gets the capability that best matches the requested width, height and
  // frame rate.
  // Returns the deviceCapabilityNumber on success.
  int32_t GetBestMatchedCapability(
    const char* deviceUniqueIdUTF8,
    const VideoCaptureCapability& requested,
    VideoCaptureCapability& resulting) override;

  // Display OS /capture device specific settings dialog
  int32_t DisplayCaptureSettingsDialogBox(
    const char* deviceUniqueIdUTF8,
    const char* dialogTitleUTF8,
    void* parentWindow,
    uint32_t positionX,
    uint32_t positionY) override;
};
