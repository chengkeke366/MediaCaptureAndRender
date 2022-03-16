#pragma once

#include <memory>
#include "VideoCaptureModule.h"

class VideoCaptureFactory
{
public:

	static std::shared_ptr<VideoCaptureModule> Create();
	static VideoCaptureModule::DeviceInfo* CreateDeviceInfo();
};

