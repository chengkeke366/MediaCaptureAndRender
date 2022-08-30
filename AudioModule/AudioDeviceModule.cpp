#include "AudioDeviceModule.h"
#include <functiondiscoverykeys_devpkey.h>
#include <memory>
#include <iostream>
#include "tools/string_utils.h"
//为什么使用Do wihle(0)结构：https://blog.csdn.net/qq_27575841/article/details/104332888
#define SAFE_RELEASE(p) \
do {                \
    if(p){          \
     p->Release();  \
     p = nullptr;   \
    }                \
} while(0)


AudioDeviceModule::AudioDeviceModule() {
    //参考：https://docs.microsoft.com/en-us/windows/win32/coreaudio/mmdevice-api
    /* UUID ： 代表COM组件
     *CLSID ： 代表COM组件中的类
     * IID ： 代表COM组件类中的导出接口
     */

    _co_hr = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
    auto hr = CoCreateInstance(
            CLSID_MMDeviceEnumerator, NULL,
            CLSCTX_ALL, IID_IMMDeviceEnumerator,
            (void**)&m_device_enumer);
    if(FAILED(hr)){
        std::cout <<"CoCreateInstance MMDeviceEnumerator fail: " << hr;
        return;
    }

    //获取Capture Collection集合
    m_device_enumer->EnumAudioEndpoints(EDataFlow::eCapture,
                                                  DEVICE_STATE_ACTIVE,
                                                  &m_capture_collection);

    uint32_t count =0;
    m_capture_collection->GetCount(&count);

    //获取Render Collection集合
   m_device_enumer->EnumAudioEndpoints(EDataFlow::eRender,
                                       DEVICE_STATE_ACTIVE,
                                       &m_render_collection);
    m_render_collection->GetCount(&count);
    _GetDefaultMicDevice(&m_current_capture_device);
    _GetDefaultSpeakerDevice(&m_current_render_device);

}

AudioDeviceModule::~AudioDeviceModule() {
    if(SUCCEEDED(_co_hr)){
        CoUninitialize();
    }
}

uint32_t AudioDeviceModule::PlayoutDevicesCount() {
    uint32_t count = 0;
    std::lock_guard<std::mutex> locker(m_device_mutex);
    SAFE_RELEASE(m_render_collection);
    auto hr = m_device_enumer->EnumAudioEndpoints(EDataFlow::eRender,
                                                  DEVICE_STATE_ACTIVE,
                                                  &m_render_collection);
    if(SUCCEEDED(hr)){
        m_render_collection->GetCount(&count);
    }
    return count;
}

uint32_t AudioDeviceModule::RecordingDevicesCount() {
    uint32_t count = 0;
    std::lock_guard<std::mutex> locker(m_device_mutex);
    SAFE_RELEASE(m_capture_collection);
    auto hr = m_device_enumer->EnumAudioEndpoints(EDataFlow::eCapture,
                                             DEVICE_STATE_ACTIVE,
                                             &m_capture_collection);
    if(SUCCEEDED(hr)){
        m_capture_collection->GetCount(&count);
    }
    return count;
}

int32_t
AudioDeviceModule::GetPlayoutDeviceName(uint16_t index, char name[kAdmMaxDeviceNameSize], char guid[kAdmMaxGuidSize]) {
    return GetDeviceName(index, DeviceType::RENDER, name, guid);
}


int32_t AudioDeviceModule::GetRecordingDeviceName(uint16_t index, char *name, char *guid) {
    return GetDeviceName(index, DeviceType::CAPTURE, name, guid);
}

int32_t AudioDeviceModule::GetDeviceName(uint16_t index, AudioDeviceModule::DeviceType type, char *name, char *guid) {

    int32_t  ret = -1;
    uint32_t  count = 0;

    IMMDeviceCollection *  device_collection  = type == DeviceType::CAPTURE? m_capture_collection:m_render_collection;
    device_collection->GetCount(&count);
    for (int i = 0; i < count; ++i) {
        if (i != index){
            continue;
        }
        IMMDevice *device = nullptr;
        HRESULT hr = device_collection->Item(i, &device);
        if(FAILED(hr)){
            SAFE_RELEASE(device);
            return -1;
        }
        //Get guid
        WCHAR *wc_uuid = nullptr;
        device->GetId(&wc_uuid);
        std::string uuid = tools::ToUtf8(wc_uuid, wcslen(wc_uuid));
        CoTaskMemFree(wc_uuid);
        strcpy(guid, uuid.c_str());

        //Get friend name
        IPropertyStore* pProps = nullptr;
        hr = device->OpenPropertyStore(STGM_READ, &pProps);
        if (FAILED(hr)) {
            std::cout <<__FUNCTION__ <<"[ERROR] "<<"IMMDevice::OpenPropertyStore failed, hr = 0x"
                      << tools::ToHex(hr) << std::endl;
            return -1;
        }

        PROPVARIANT varName;
        // Initialize container for property value.
        PropVariantInit(&varName);
        if (SUCCEEDED(hr)) {
            // Get the endpoint device's friendly-name property.
            hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
            if (FAILED(hr)) {
                std::cout <<__FUNCTION__ <<"[ERROR] "<< "IPropertyStore::GetValue failed, hr = 0x"
                          << tools::ToHex(hr);
            }
        }

        if ((SUCCEEDED(hr)) && (VT_EMPTY == varName.vt)) {
            hr = E_FAIL;
            std::cout <<__FUNCTION__ <<"[ERROR] "<< "IPropertyStore::GetValue returned no value,"
                                                    " hr = 0x"
                      << tools::ToHex(hr);
        }

        if ((SUCCEEDED(hr)) && (VT_LPWSTR != varName.vt)) {
            // The returned value is not a wide null terminated string.
            hr = E_UNEXPECTED;
            std::cout <<__FUNCTION__ <<"[ERROR] "<< "IPropertyStore::GetValue returned unexpected"
                                                    " type, hr = 0x"
                      << tools::ToHex(hr);
        }
        if (SUCCEEDED(hr) && (varName.pwszVal != NULL)) {
            // Copy the valid device name to the provided ouput buffer.
            std::string friend_name = tools::ToUtf8(varName.pwszVal, wcslen(varName.pwszVal));
            strcpy(name, friend_name.c_str());
            ret = 0;
        } else {
            // Failed to find the device name.
            strcpy(name, "<Device not available>");
        }
        PropVariantClear(&varName);
        SAFE_RELEASE(pProps);
    }
    return ret;
}




int32_t AudioDeviceModule::GetDefaultDevice(const DeviceType type, uint8_t &index) {
    IMMDevice *pDevice = nullptr;
    HRESULT hr = m_device_enumer->GetDefaultAudioEndpoint(EDataFlow::eCapture, ERole::eConsole, &pDevice);
    WCHAR *uuid = nullptr;
    pDevice->GetId(&uuid);
    std::string default_caputrue_uuid = tools::ToUtf8(uuid, wcslen(uuid));
    CoTaskMemFree(uuid);
    if (FAILED(hr)){
        SAFE_RELEASE(pDevice);
        return  -1;
    }
    uint32_t count = 0;
    IMMDeviceCollection * pCollection = nullptr;
    m_device_enumer->EnumAudioEndpoints(type == DeviceType::CAPTURE?EDataFlow::eCapture:EDataFlow::eRender, DEVICE_STATE_ACTIVE, &pCollection);
    pCollection->GetCount(&count);
    static const WCHAR szDefault[] = L"<Device not available>";
    for (int i = 0; i < count; ++i) {
        IMMDevice  *device = nullptr;
        hr = pCollection->Item(i,&device);
        if (FAILED(hr)){
            SAFE_RELEASE(device);
            return -1;
        }
        WCHAR *id = nullptr;
        hr = device->GetId(&id);
        std::string item_uuid = tools::ToUtf8(id, wcslen(id));
        CoTaskMemFree(id);
        if (hr == S_OK){
            if (item_uuid == default_caputrue_uuid){
                index = i;
                SAFE_RELEASE(device);
                break;
            }
        }else{
            SAFE_RELEASE(device);
            return -1;
        }
    }
    return 0;
}


int32_t AudioDeviceModule::_GetDefaultSpeakerDevice(IMMDevice **ppDevice) {
    int32_t hr(S_OK);
    hr = m_device_enumer->GetDefaultAudioEndpoint(eRender, eCommunications, ppDevice);
    return hr;
}

int32_t AudioDeviceModule::_GetDefaultMicDevice(IMMDevice **ppDevice) {
    int32_t hr(S_OK);
    hr = m_device_enumer->GetDefaultAudioEndpoint(eCapture, eCommunications, ppDevice);
    return hr;
}


int32_t AudioDeviceModule::SetPlayoutDevice(uint16_t index) {
    return 0;
}

int32_t AudioDeviceModule::SetPlayoutDevice(WindowsDeviceType device) {
    return 0;
}

int32_t AudioDeviceModule::SetRecordingDevice(uint16_t index) {
    return 0;
}

int32_t AudioDeviceModule::SetRecordingDevice(WindowsDeviceType device) {
    return 0;
}

int32_t AudioDeviceModule::InitRecording(uint8_t channel, uint32_t SampleRate, uint8_t BytesPerSample) {

    // Initialize the microphone (devices might have been added or removed)


    HRESULT hr = E_FAIL;
    // Create COM object with IAudioClient interface  调用 IMMDevice 的 Activate 方法创建具有指定接口的 IAudioClient
    hr = m_current_capture_device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL,
                                    (void **) &m_in_client);

    WAVEFORMATEX *pWfxIn = NULL;
    hr = m_in_client->GetMixFormat(&pWfxIn);//获取默认的格式，然后
    if (SUCCEEDED(hr)) {
        std::cout  << "Audio Engine's current capturing mix format:" << std::endl;
        // format type
        std::cout  <<  "wFormatTag     : 0x"
                      << tools::ToHex(pWfxIn->wFormatTag) << " ("
                      << pWfxIn->wFormatTag << ")"<< std::endl;
        // number of channels (i.e. mono, stereo...)
        std::cout  <<  "nChannels      : " << pWfxIn->nChannels<< std::endl;
        // sample rate
        std::cout  <<  "nSamplesPerSec : " << pWfxIn->nSamplesPerSec<< std::endl;
        // for buffer estimation
        std::cout  <<  "nAvgBytesPerSec: " << pWfxIn->nAvgBytesPerSec<< std::endl;
        // block size of data
        std::cout  <<  "nBlockAlign    : " << pWfxIn->nBlockAlign<< std::endl;
        // number of bits per sample of mono data
        std::cout  <<  "wBitsPerSample : " << pWfxIn->wBitsPerSample<< std::endl;
        std::cout  <<  "cbSize         : " << pWfxIn->cbSize<< std::endl;
    }


    WAVEFORMATEXTENSIBLE Wfx = WAVEFORMATEXTENSIBLE();
    Wfx.Format.nChannels = channel;
    Wfx.Format.wBitsPerSample = BytesPerSample * 8;
    Wfx.Format.nSamplesPerSec = SampleRate;
    Wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    Wfx.Format.cbSize = 22;
    Wfx.dwChannelMask = 0;
    Wfx.Samples.wValidBitsPerSample = Wfx.Format.wBitsPerSample;
    Wfx.Format.nBlockAlign =
            Wfx.Format.nChannels * Wfx.Format.wBitsPerSample / 8;
    Wfx.Format.nAvgBytesPerSec =
            Wfx.Format.nSamplesPerSec * Wfx.Format.nBlockAlign;
    Wfx.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;

    WAVEFORMATEX* pWfxClosestMatch = NULL;
    const int freqs[7] = {48000, 44100, 16000, 96000, 32000, 8000, 192000};
    hr = S_FALSE;

    // If the method succeeds and the audio endpoint device supports the
    // specified stream format, it returns S_OK. If the method succeeds and
    // provides a closest match to the specified format, it returns S_FALSE.
    hr = m_in_client->IsFormatSupported(
            AUDCLNT_SHAREMODE_EXCLUSIVE, (WAVEFORMATEX*)&Wfx, &pWfxClosestMatch);
    if (hr != S_OK) {
        if (pWfxClosestMatch) {
            std::cout << "nChannels=" << Wfx.Format.nChannels
                      << ", nSamplesPerSec=" << Wfx.Format.nSamplesPerSec
                      << " is not supported. Closest match: "
                         "nChannels="
                      << pWfxClosestMatch->nChannels << ", nSamplesPerSec="
                      << pWfxClosestMatch->nSamplesPerSec <<std::endl;

            Wfx.Format.nChannels =  pWfxClosestMatch->nChannels;
            Wfx.Format.wBitsPerSample = pWfxClosestMatch->wBitsPerSample;
            Wfx.Format.nSamplesPerSec = pWfxClosestMatch->nSamplesPerSec;
        }
    }else {
        std::cout << "nChannels=" << Wfx.Format.nChannels
                  << ", nSamplesPerSec=" << Wfx.Format.nSamplesPerSec
                  << " is not supported. No closest match." <<std::endl;
    }

    if (hr == S_OK) {
        _recAudioFrameSize = Wfx.Format.nBlockAlign;
        _recSampleRate = Wfx.Format.nSamplesPerSec;
        _recBlockSize = Wfx.Format.nSamplesPerSec / 100;
        _recChannels = Wfx.Format.nChannels;

        // Create a capturing stream.
        hr = m_in_client->Initialize(
                AUDCLNT_SHAREMODE_SHARED,  // share Audio Engine with other applications
                 AUDCLNT_STREAMFLAGS_EVENTCALLBACK |  // processing of the audio buffer by the client will be event driven
                AUDCLNT_STREAMFLAGS_NOPERSIST,   // volume and mute settings for an audio session will not persist across system restarts
                0,                    // required for event-driven shared mode
                0,                    // periodicity
                (WAVEFORMATEX*)&Wfx,  // selected wave format
                NULL);                // session GUID

        if (hr != S_OK) {
            std::cout << "IAudioClient::Initialize() failed:" << std::endl;
        } else {
            hr = m_in_client->SetEventHandle(_hCaptureSamplesReadyEvent);
            uint32_t size;
            m_in_client->GetBufferSize(&size);
            std::cout << "IAudioClient::GetBufferSize() :" << size <<std::endl;
            if (SUCCEEDED(hr)) {
                hr = m_in_client->GetService(__uuidof(IAudioCaptureClient),
                                             (void **) &m_auido_capture_client);
            }

        }
    } else {
        std::cout << "Is not Supported the format" << std::endl;
    }
    m_recorde_initialized = true;
    CoTaskMemFree(pWfxIn);
    CoTaskMemFree(pWfxClosestMatch);
    return 0;
}

int32_t AudioDeviceModule::InitPlayout(uint8_t channel, uint32_t SampleRate, uint8_t BytesPerSample) {
    return 0;
}

int32_t AudioDeviceModule::StartRecorde(const char *output_file) {
    if(!m_recorde_initialized)
    {
        std::cout << __FUNCTION__ <<" recorde device is not init"<<std::endl;
        return -1;
    }
    if (m_recording){
        return 0;
    }
    m_capture_thread = std::shared_ptr<std::thread>(new std::thread(&AudioDeviceModule::DoCaptureThread,this), [&](std::thread *p){
        if(p->joinable()){
            p->join();
        }
        m_recorde_initialized = false;
        m_recording = false;
    });
}

int32_t AudioDeviceModule::StartPlayout(const char *intput_file) {
    return 0;
}

int32_t AudioDeviceModule::SpeakerVolume(uint32_t &volume) {
    return 0;
}

int32_t AudioDeviceModule::SetSpeakerVolume(uint32_t volume) {
    return 0;
}

int32_t AudioDeviceModule::MinSpeakerVolume(uint32_t &minVolume) {
    return 0;
}

int32_t AudioDeviceModule::MaxSpeakerVolume(uint32_t &maxVolume) {
    return 0;
}

int32_t AudioDeviceModule::SetMicrophoneVolume(uint32_t volume) {
    return 0;
}

int32_t AudioDeviceModule::MicrophoneVolume(uint32_t &volume) {
    return 0;
}

int32_t AudioDeviceModule::MaxMicrophoneVolume(uint32_t &maxVolume) {
    return 0;
}

int32_t AudioDeviceModule::MinMicrophoneVolume(uint32_t &minVolume) {
    return 0;
}

int32_t AudioDeviceModule::SetSpeakerMute(bool enable) {
    return 0;
}

int32_t AudioDeviceModule::SpeakerMute(bool &enabled) {
    return 0;
}

int32_t AudioDeviceModule::SetMicrophoneMute(bool enable) {
    return 0;
}

int32_t AudioDeviceModule::MicrophoneMute(bool &enabled) {
    return 0;
}

int32_t AudioDeviceModule::DoCaptureThread() {

    // Get size of capturing buffer (length is expressed as the number of audio
    // frames the buffer can hold). This value is fixed during the capturing
    // session.
    //
    UINT32 bufferLength = 0;
    if (m_in_client == NULL) {
      std::cout << "input state has been modified before capture loop starts.";
        return 1;
    }
    auto hr = m_in_client->GetBufferSize(&bufferLength);

    std::cout <<  "[CAPT] size of buffer       : " << bufferLength << std::endl;

    // Allocate memory for sync buffer.
    // It is used for compensation between native 44.1 and internal 44.0 and
    // for cases when the capture buffer is larger than 10ms.
    //
    const UINT32 syncBufferSize = 2 * (bufferLength * _recAudioFrameSize);
    BYTE *syncBuffer = new BYTE[syncBufferSize];
    if (syncBuffer == NULL) {
        return (DWORD)E_POINTER;
    }
    std::cout << "[CAPT] size of sync buffer  : " << syncBufferSize
                  << " [bytes]" << std::endl;

    // Get maximum latency for the current stream (will not change for the
    // lifetime of the IAudioClient object).

    REFERENCE_TIME latency;
    m_in_client->GetStreamLatency(&latency);
    std::cout << "[CAPT] max stream latency   : " << (DWORD)latency
                  << " (" << (double)(latency / 10000.0) << " ms)" << std::endl;

    // Get the length of the periodic interval separating successive processing
    // passes by the audio engine on the data in the endpoint buffer.
    //
    REFERENCE_TIME devPeriod = 0;
    REFERENCE_TIME devPeriodMin = 0;
    //获取当前流的延迟
    m_in_client->GetDevicePeriod(&devPeriod, &devPeriodMin);
    std::cout << "[CAPT] device period        : " << (DWORD)devPeriod
                  << " (" << (double)(devPeriod / 10000.0) << " ms)" << std::endl;

    double extraDelayMS = (double)((latency + devPeriod) / 10000.0);
    std::cout << "[CAPT] extraDelayMS         : " << extraDelayMS << std::endl;

    double endpointBufferSizeMS =
            10.0 * ((double)bufferLength / (double)_recBlockSize);
    std::cout << "[CAPT] endpointBufferSizeMS : "
                  << endpointBufferSizeMS << std::endl;

    // Start up the capturing stream.
    m_in_client->Start();

    // Set event which will ensure that the calling thread modifies the recording state to true.
    SetEvent(_hCaptureStartedEvent);


}


