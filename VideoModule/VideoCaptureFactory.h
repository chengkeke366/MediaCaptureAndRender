#pragma once

#include <memory>
#include "VideoCaptureModule.h"

class VideoCaptureFactory
{
public:

	static std::shared_ptr<VideoCaptureModule> Create(const char* deviceIdUTF8);
	static VideoCaptureModule::DeviceInfo* CreateDeviceInfo();
};

