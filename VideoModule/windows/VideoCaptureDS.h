#pragma once

#include <memory>
#include "VideoModule/VideoCaptureModuleImpl.h"
#include "VideoModule/windows/DeviceInfoDS.h"
#include "VideoModule/windows/sink_filter_ds.h"

#define CAPTURE_FILTER_NAME L"VideoCaptureFilter"
#define SINK_FILTER_NAME L"SinkFilter"

#include <strmif.h>//IBaseFilter  IGraphBuilder
#include <control.h>//IMediaControl

class VideoCaptureDS :public VideoCaptureModuleImpl
{
public:
  VideoCaptureDS();
  ~VideoCaptureDS();

  int32_t Init(const char* deviceUniqueIdUTF8);

  int32_t StartCapture(const VideoCaptureCapability& capability) override;
  int32_t StopCapture() override;
  bool CaptureStarted() override;
  int32_t CaptureSettings(VideoCaptureCapability& settings) override;

protected:

  // Help functions

  int32_t SetCameraOutput(const VideoCaptureCapability& requestedCapability);
  int32_t DisconnectGraph();
  HRESULT ConnectDVCamera();

  DeviceInfoDS _dsInfo;

  IBaseFilter* _captureFilter;
  IGraphBuilder* _graphBuilder;
  IMediaControl* _mediaControl;
  std::shared_ptr<CaptureSinkFilter> sink_filter_;
  IPin* _inputSendPin;
  IPin* _outputCapturePin;

  // Microsoft DV interface (external DV cameras)
  IBaseFilter* _dvFilter;
  IPin* _inputDvPin;
  IPin* _outputDvPin;
};

