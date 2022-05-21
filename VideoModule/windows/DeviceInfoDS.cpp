#include "VideoCaptureDS.h"

DeviceInfoDS* DeviceInfoDS::Create()
{
  DeviceInfoDS* dsInfo = new DeviceInfoDS();
  if (!dsInfo || dsInfo->Init() != 0) {
    delete dsInfo;
    dsInfo = NULL;
  }
  return dsInfo;
}

DeviceInfoDS::DeviceInfoDS()
{

}

DeviceInfoDS::~DeviceInfoDS()
{

}

int32_t DeviceInfoDS::Init()
{
  return 0;
}

int32_t DeviceInfoDS::CreateCapabilityMap(const char* deviceUniqueIdUTF8)
{
  return 0;
}

uint32_t DeviceInfoDS::NumberOfDevices()
{
  return 0;
}

int32_t DeviceInfoDS::GetDeviceName(uint32_t deviceNumber, char* deviceNameUTF8, uint32_t deviceNameLength, char* deviceUniqueIdUTF8, uint32_t deviceUniqueIdUTF8Length, char* productUniqueIdUTF8 /*= 0*/, uint32_t productUniqueIdUTF8Length /*= 0*/)
{
  return 0;
}

int32_t DeviceInfoDS::NumberOfCapabilities(const char* deviceUniqueIdUTF8)
{
  return 0;
}

int32_t DeviceInfoDS::GetCapability(const char* deviceUniqueIdUTF8, const uint32_t deviceCapabilityNumber, VideoCaptureCapability& capability)
{
  return 0;
}

int32_t DeviceInfoDS::GetOrientation(const char* deviceUniqueIdUTF8, VideoRotation& orientation)
{
  return 0;
}

int32_t DeviceInfoDS::GetBestMatchedCapability(const char* deviceUniqueIdUTF8, const VideoCaptureCapability& requested, VideoCaptureCapability& resulting)
{
  return 0;
}

int32_t DeviceInfoDS::DisplayCaptureSettingsDialogBox(const char* deviceUniqueIdUTF8, const char* dialogTitleUTF8, void* parentWindow, uint32_t positionX, uint32_t positionY)
{
  return 0;
}
