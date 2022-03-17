#pragma once
#include <cstdint>

#include <mmdeviceapi.h>// MMDevice
#include <Audioclient.h>// WASAPI
#include <Audiopolicy.h>
#include <endpointvolume.h>

static const int kAdmMaxDeviceNameSize = 128;
static const int kAdmMaxFileNameSize = 512;
static const int kAdmMaxGuidSize = 128;

static const int kAdmMinPlayoutBufferSizeMs = 10;
static const int kAdmMaxPlayoutBufferSizeMs = 250;

class AudioDeviceModule
{
  enum WindowsDeviceType {
    kDefaultCommunicationDevice = -1,
    kDefaultDevice = -2
  };
public:
  AudioDeviceModule();
  ~AudioDeviceModule();

  // Device enumeration
  int16_t PlayoutDevices();
  int16_t RecordingDevices();
  int32_t PlayoutDeviceName(uint16_t index,
    char name[kAdmMaxDeviceNameSize],
    char guid[kAdmMaxGuidSize]);
  int32_t RecordingDeviceName(uint16_t index,
    char name[kAdmMaxDeviceNameSize],
    char guid[kAdmMaxGuidSize]);

  // Device selection
  int32_t SetPlayoutDevice(uint16_t index) ;
  int32_t SetPlayoutDevice(AudioDeviceModule::WindowsDeviceType device);
  int32_t SetRecordingDevice(uint16_t index) ;
  int32_t SetRecordingDevice(
      AudioDeviceModule::WindowsDeviceType device) ;

  //Device Init
  int32_t InitRecording(uint8_t channel, uint32_t SampleRate, uint8_t BytesPerSample);
  int32_t InitPlayout(uint8_t channel, uint32_t SampleRate, uint8_t BytesPerSample);

  // Audio capture
  void StartRecorde(const char* output_file);

  //Audio render
  int32_t StartPlayout(const char* intput_file);

  //volume 
  int32_t SpeakerVolume(uint32_t& volume);
  int32_t SetSpeakerVolume(uint32_t volume);
  int32_t MinSpeakerVolume(uint32_t& minVolume);
  int32_t MaxSpeakerVolume(uint32_t& maxVolume);

  int32_t SetMicrophoneVolume(uint32_t volume);
  int32_t MicrophoneVolume(uint32_t& volume);
  int32_t MaxMicrophoneVolume(uint32_t& maxVolume);
  int32_t MinMicrophoneVolume(uint32_t& minVolume);
  
  //mute
  int32_t SetSpeakerMute(bool enable);
  int32_t SpeakerMute(bool& enabled);
  int32_t SetMicrophoneMute(bool enable);
  int32_t MicrophoneMute(bool& enabled);
private:
  int32_t GetDefaultMicDevice(IMMDevice** ppDevice);
  int32_t GetDefaultSpeakerDevice(IMMDevice** ppDevice);
private:
  IMMDeviceEnumerator* m_device_enumer;
  IMMDeviceCollection* m_capture_collection;
  IMMDeviceCollection* m_render_collection;
  IMMDevice* m_capture_device;
  IMMDevice* m_render_device;

  IAudioClient* m_in_client;
  IAudioClient* m_out_client;
  IAudioCaptureClient* m_auido_capture_client;
  IAudioRenderClient* m_auido_render_client;
  IAudioEndpointVolume* m_capture_volume;
  ISimpleAudioVolume* m_render_volume;

  HANDLE _hRenderSamplesReadyEvent;
  HANDLE _hPlayThread;
  HANDLE _hRenderStartedEvent;
  HANDLE _hShutdownRenderEvent;

  HANDLE _hCaptureSamplesReadyEvent;
  HANDLE _hRecThread;
  HANDLE _hCaptureStartedEvent;
  HANDLE _hShutdownCaptureEvent;
};

