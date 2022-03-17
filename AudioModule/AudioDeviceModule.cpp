#include "AudioDeviceModule.h"
#include <functiondiscoverykeys_devpkey.h>
#include <iostream>

AudioDeviceModule::AudioDeviceModule()
{
  CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
    __uuidof(IMMDeviceEnumerator),
    reinterpret_cast<void**>(&m_device_enumer));
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

int32_t AudioDeviceModule::GetDefaultMicDevice(IMMDevice** ppDevice)
{
  int32_t hr(S_OK);
  hr = m_device_enumer->GetDefaultAudioEndpoint(eCapture, eCommunications, ppDevice);
  return hr;
}


int32_t AudioDeviceModule::GetDefaultSpeakerDevice(IMMDevice** ppDevice)
{
  int32_t hr(S_OK);
  hr = m_device_enumer->GetDefaultAudioEndpoint(eRender, eCommunications, ppDevice);
  return hr;
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
  GetDefaultMicDevice(&m_capture_device);
  HRESULT hr = E_FAIL;
  IPropertyStore* pProps = NULL;
  hr = m_capture_device->OpenPropertyStore(STGM_READ, &pProps);
  
  PROPVARIANT varName;
  // Initialize container for property value.
  PropVariantInit(&varName);

  if (SUCCEEDED(hr)) {
    // Get the endpoint device's friendly-name property.
    hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
    if (FAILED(hr)) {
     std::cout<< "IPropertyStore::GetValue failed, hr = 0x"
        << std::endl;
    }
  }
  if ((SUCCEEDED(hr)) && (VT_EMPTY == varName.vt)) {
    hr = E_FAIL;
    std::cout << "IPropertyStore::GetValue returned no value,"
      " hr = 0x" << hr << std::endl;
  }

  if ((SUCCEEDED(hr)) && (VT_LPWSTR != varName.vt)) {
    // The returned value is not a wide null terminated string.
    hr = E_UNEXPECTED;
    std::cout << "IPropertyStore::GetValue returned unexpected"
      " type, hr = 0x"
      << hr <<std::endl;
  }
  WCHAR pszBuffer[MAX_PATH];
  const int bufferLen = sizeof(pszBuffer) / sizeof(pszBuffer)[0];
  if (SUCCEEDED(hr) && (varName.pwszVal != NULL)) {
    // Copy the valid device name to the provided ouput buffer.
    wcsncpy_s(pszBuffer, bufferLen, varName.pwszVal, _TRUNCATE);
  }

  // Create COM object with IAudioClient interface.
  hr = m_capture_device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL,
    (void**)&m_in_client);

  WAVEFORMATEX* pWfxIn = NULL;
  hr = m_in_client->GetMixFormat(&pWfxIn);

  WAVEFORMATEXTENSIBLE Wfx = WAVEFORMATEXTENSIBLE();

  Wfx.Format.nChannels = channel;
  Wfx.Format.wBitsPerSample = BytesPerSample * 8;
  Wfx.Format.nSamplesPerSec = SampleRate;
  Wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
  Wfx.Format.cbSize = 22;
  Wfx.dwChannelMask = 0;
  Wfx.Format.nBlockAlign = 4;
  Wfx.Format.nAvgBytesPerSec =
    Wfx.Format.nSamplesPerSec * Wfx.Format.nBlockAlign;
  Wfx.Samples.wValidBitsPerSample = Wfx.Format.wBitsPerSample;
  Wfx.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;

  WAVEFORMATEX* pWfxClosestMatch = NULL;
  hr = m_in_client->IsFormatSupported(
    AUDCLNT_SHAREMODE_SHARED, (WAVEFORMATEX*)&Wfx, &pWfxClosestMatch);
  if (hr == S_OK) {
    // Create a capturing stream.
    hr = m_in_client->Initialize(AUDCLNT_SHAREMODE_SHARED,  // share Audio Engine with other applications
      AUDCLNT_STREAMFLAGS_EVENTCALLBACK |  // processing of the audio buffer by
                                           // the client will be event driven
      AUDCLNT_STREAMFLAGS_NOPERSIST,   // volume and mute settings for an
                                       // audio session will not persist
                                       // across system restarts
      0,                    // required for event-driven shared mode
      0,                    // periodicity
      (WAVEFORMATEX*)&Wfx,  // selected wave format
      NULL);                // session GUID)

    if (hr != S_OK) {
      std::cout << "IAudioClient::Initialize() failed:" << std::endl;
    }
    else {
     hr = m_in_client->SetEventHandle(_hRenderSamplesReadyEvent);
     if (SUCCEEDED(hr)) {
       hr = m_in_client->GetService(__uuidof(IAudioCaptureClient),
         (void**)&m_auido_capture_client);
     }

    }
  }
  else {
    std::cout << "Is not Supported the format" << std::endl;
  }
  CoTaskMemFree(pWfxClosestMatch);
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
