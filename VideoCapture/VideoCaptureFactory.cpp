#include "VideoCaptureFactory.h"

std::shared_ptr<VideoCaptureModule> VideoCaptureFactory::Create(
	const char* deviceUniqueIdUTF8) {
	return VideoCaptureImpl::Create(deviceUniqueIdUTF8);
}

VideoCaptureModule::DeviceInfo* VideoCaptureFactory::CreateDeviceInfo() {

	return VideoCaptureImpl::CreateDeviceInfo();
}