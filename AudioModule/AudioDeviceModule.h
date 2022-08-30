#pragma once

#include <cstdint>
#include <thread>
#include <mmdeviceapi.h>// MMDevice
#include <Audioclient.h>// WASAPI
#include <Audiopolicy.h>
#include <endpointvolume.h>
#include "global.h"
#include <mutex>

static const int kAdmMaxDeviceNameSize = 128;
static const int kAdmMaxFileNameSize = 512;
static const int kAdmMaxGuidSize = 128;

static const int kAdmMinPlayoutBufferSizeMs = 10;
static const int kAdmMaxPlayoutBufferSizeMs = 250;


class AudioDeviceModule {
public:
    enum DeviceType {
        CAPTURE,
        RENDER
    };

    AudioDeviceModule();

    ~AudioDeviceModule();

    // Device enumeration
    uint32_t PlayoutDevicesCount();//获取播放器数量
    uint32_t RecordingDevicesCount();
    int32_t GetPlayoutDeviceName(uint16_t index,
                                 char name[kAdmMaxDeviceNameSize],
                                 char guid[kAdmMaxGuidSize]);    //获取指定下标设备名称与uuid
    int32_t GetRecordingDeviceName(uint16_t index,
                                   char name[kAdmMaxDeviceNameSize],
                                   char guid[kAdmMaxGuidSize]);
    int32_t GetDefaultDevice(const DeviceType type, uint8_t &index);

    // Device selection
    int32_t SetPlayoutDevice(uint16_t index);
    int32_t SetPlayoutDevice(WindowsDeviceType device);
    int32_t SetRecordingDevice(uint16_t index);
    int32_t SetRecordingDevice(WindowsDeviceType device);

    //Device Init
    int32_t InitRecording(uint8_t channel, uint32_t SampleRate, uint8_t BytesPerSample);
    int32_t InitPlayout(uint8_t channel, uint32_t SampleRate, uint8_t BytesPerSample);

    // Audio capture
    int32_t StartRecorde(const char *output_file);

    //Audio render
    int32_t StartPlayout(const char *intput_file);

    //volume
    int32_t SpeakerVolume(uint32_t &volume);

    int32_t SetSpeakerVolume(uint32_t volume);

    int32_t MinSpeakerVolume(uint32_t &minVolume);

    int32_t MaxSpeakerVolume(uint32_t &maxVolume);

    int32_t SetMicrophoneVolume(uint32_t volume);

    int32_t MicrophoneVolume(uint32_t &volume);

    int32_t MaxMicrophoneVolume(uint32_t &maxVolume);

    int32_t MinMicrophoneVolume(uint32_t &minVolume);

    //mute
    int32_t SetSpeakerMute(bool enable);

    int32_t SpeakerMute(bool &enabled);

    int32_t SetMicrophoneMute(bool enable);

    int32_t MicrophoneMute(bool &enabled);

private:
    int32_t _GetDefaultMicDevice(IMMDevice **ppDevice);
    int32_t _GetDefaultSpeakerDevice(IMMDevice **ppDevice);

    int32_t GetDeviceName(uint16_t index, DeviceType type,
                          char name[kAdmMaxDeviceNameSize],
                          char guid[kAdmMaxGuidSize]);

    int32_t DoCaptureThread();
private:
    IMMDeviceEnumerator *m_device_enumer{nullptr};
    IMMDeviceCollection *m_capture_collection{nullptr};
    IMMDeviceCollection *m_render_collection{nullptr};
    IMMDevice *m_current_capture_device{nullptr};
    IMMDevice *m_current_render_device{nullptr};

    IAudioClient *m_in_client{nullptr};
    IAudioClient *m_out_client{nullptr};
    IAudioCaptureClient *m_auido_capture_client{nullptr};
    IAudioRenderClient *m_auido_render_client{nullptr};
    IAudioEndpointVolume *m_capture_volume{nullptr};
    ISimpleAudioVolume *m_render_volume{nullptr};

    HRESULT _co_hr;
    HANDLE _hRenderSamplesReadyEvent;
    HANDLE _hPlayThread;
    HANDLE _hRenderStartedEvent;
    HANDLE _hShutdownRenderEvent;

    HANDLE _hCaptureSamplesReadyEvent;
    HANDLE _hRecThread;
    HANDLE _hCaptureStartedEvent;
    HANDLE _hShutdownCaptureEvent;

    /*
    _recChannelsPrioList[0] = 2;  // stereo is prio 1
    _recChannelsPrioList[1] = 1;  // mono is prio 2
    _recChannelsPrioList[2] = 4;  // quad is prio 3
    _recChannelsPrioList[3] = 6;  // DB Phone vc200.
     */
    uint16_t _recChannelsPrioList[4]{2,1,4,6};

    /*
    _playChannelsPrioList[0] = 2;  // stereo is prio 1
    _playChannelsPrioList[1] = 1;  // mono is prio 2
    _playChannelsPrioList[2] = 4;  // quad is prio 3
     * */
    uint16_t _playChannelsPrioList[3] {2,1,4};
    UINT _recAudioFrameSize;
    uint32_t _recSampleRate;
    uint32_t _recBlockSize;
    uint32_t _recChannels;

    std::mutex m_device_mutex;
    bool m_recorde_initialized = false; //录音设备是否已经初始化
    bool m_recording = false;//是否正在录音
    bool m_playout_initialized = false;//播放设备是否已经初始化
    bool m_playing = false;//是否正在播放

    std::shared_ptr<std::thread> m_capture_thread;
};

