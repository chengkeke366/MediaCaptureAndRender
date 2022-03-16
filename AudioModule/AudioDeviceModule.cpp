#include "AudioDeviceModule.h"

AudioDeviceModule::AudioDeviceModule()
{

}

AudioDeviceModule::~AudioDeviceModule()
{

}

int16_t AudioDeviceModule::PlayoutDevices()
{
  return 0;
}

int16_t AudioDeviceModule::RecordingDevices()
{
  return 0;
}

int32_t AudioDeviceModule::PlayoutDeviceName(uint16_t index, char name[kAdmMaxDeviceNameSize], char guid[kAdmMaxGuidSize])
{
  return 0;
}

int32_t AudioDeviceModule::RecordingDeviceName(uint16_t index, char name[kAdmMaxDeviceNameSize], char guid[kAdmMaxGuidSize])
{
  return 0;
}

int32_t AudioDeviceModule::SetPlayoutDevice(uint16_t index)
{
  return 0;
}

int32_t AudioDeviceModule::SetPlayoutDevice(WindowsDeviceType device)
{
  return 0;
}

int32_t AudioDeviceModule::SetRecordingDevice(uint16_t index)
{
  return 0;
}

int32_t AudioDeviceModule::SetRecordingDevice(WindowsDeviceType device)
{
  return 0;
}

int32_t AudioDeviceModule::InitRecording(uint8_t channel, uint32_t SampleRate, uint8_t BytesPerSample)
{
  return 0;
}

int32_t AudioDeviceModule::InitPlayout(uint8_t channel, uint32_t SampleRate, uint8_t BytesPerSample)
{
  return 0;
}

void AudioDeviceModule::StartRecorde(const char* output_file)
{
  
}

int32_t AudioDeviceModule::StartPlayout(const char* intput_file)
{
  return 0;
}

int32_t AudioDeviceModule::SpeakerVolume(uint32_t& volume)
{
  return 0;
}

int32_t AudioDeviceModule::SetSpeakerVolume(uint32_t volume)
{
  return 0;
}

int32_t AudioDeviceModule::MinSpeakerVolume(uint32_t& minVolume)
{
  return 0;
}

int32_t AudioDeviceModule::MaxSpeakerVolume(uint32_t& maxVolume)
{
  return 0;
}

int32_t AudioDeviceModule::SetMicrophoneVolume(uint32_t volume)
{
  return 0;
}

int32_t AudioDeviceModule::MicrophoneVolume(uint32_t& volume)
{
  return 0;
}

int32_t AudioDeviceModule::MaxMicrophoneVolume(uint32_t& maxVolume)
{
  return 0;
}

int32_t AudioDeviceModule::MinMicrophoneVolume(uint32_t& minVolume)
{
  return 0;
}

int32_t AudioDeviceModule::SetSpeakerMute(bool enable)
{
  return 0;
}

int32_t AudioDeviceModule::SpeakerMute(bool& enabled)
{
  return 0;
}

int32_t AudioDeviceModule::SetMicrophoneMute(bool enable)
{
  return 0;
}

int32_t AudioDeviceModule::MicrophoneMute(bool& enabled)
{
  return 0;
}
