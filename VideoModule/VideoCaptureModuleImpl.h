#pragma once
#include <memory>
#include "VideoCaptureModule.h"

class VideoCaptureModuleImpl :public VideoCaptureModule
{
public:
  static std::shared_ptr<VideoCaptureModule> Create(const char* deviceIdUtf8);

  virtual void RegisterCaptureDataCallback(
    VideoSinkInterface* dataCallback) override;

  //  Remove capture data callback
  void DeRegisterCaptureDataCallback() override;

  // Start capture device
  int32_t StartCapture(const VideoCaptureCapability& capability) override;

  int32_t StopCapture() override;

  // Returns the name of the device used by this module.
  const char* CurrentDeviceName() const override;

  // Returns true if the capture device is running
  bool CaptureStarted() override;

  // Gets the current configuration.
  int32_t CaptureSettings(VideoCaptureCapability& settings) override;

  // Set the rotation of the captured frames.
  // If the rotation is set to the same as returned by
  // DeviceInfo::GetOrientation the captured frames are
  // displayed correctly if rendered.
  int32_t SetCaptureRotation(VideoRotation rotation) override;

  // Tells the capture module whether to apply the pending rotation. By default,
  // the rotation is applied and the generated frame is up right. When set to
  // false, generated frames will carry the rotation information from
  // SetCaptureRotation. Return value indicates whether this operation succeeds.
  bool SetApplyRotation(bool enable) override;

  // Return whether the rotation is applied or left pending.
  bool GetApplyRotation() override;

  int32_t IncomingFrame(uint8_t* videoFrame,
    size_t videoFrameLength,
    const VideoCaptureCapability& frameInfo,
    int64_t captureTime = 0);
protected:
  VideoCaptureModuleImpl();
  ~VideoCaptureModuleImpl();
};

