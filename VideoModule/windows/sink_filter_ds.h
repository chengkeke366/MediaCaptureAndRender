/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_VIDEO_CAPTURE_MAIN_SOURCE_WINDOWS_SINK_FILTER_DS_H_
#define MODULES_VIDEO_CAPTURE_MAIN_SOURCE_WINDOWS_SINK_FILTER_DS_H_

#include <dshow.h>

#include <atomic>
#include <memory>
#include <vector>

#include "VideoModule/VideoCaptureCapability.h"
#include "VideoModule/VideoCaptureModuleImpl.h"

template <class T>
class ComRefCount : public T {
public:
  ComRefCount() {}
  ~ComRefCount() {}

  template <class P0>
  explicit ComRefCount(P0&& p0) : T(std::forward<P0>(p0)) {}

  STDMETHOD_(ULONG, AddRef)() override {
    // Relaxed memory order: The current thread is allowed to act on the
  // resource protected by the reference counter both before and after the
  // atomic op, so this function doesn't prevent memory access reordering.
    ref_count_.fetch_add(1, std::memory_order_relaxed);
    return 1;
  }

  STDMETHOD_(ULONG, Release)() override {
    int ref_count_after_subtract =
      ref_count_.fetch_sub(1, std::memory_order_acq_rel) - 1;
    if (ref_count_after_subtract == 0) {
      delete this;
      return 0;
    }
    return 1;
  }

protected:


private:
  std::atomic<int> ref_count_{ 0 };
};


// forward declarations
class CaptureSinkFilter;

// Input pin for camera input
// Implements IMemInputPin, IPin.
class CaptureInputPin : public IMemInputPin, public IPin {
 public:
  CaptureInputPin(CaptureSinkFilter* filter);

  HRESULT SetRequestedCapability(const VideoCaptureCapability& capability);

  // Notifications from the filter.
  void OnFilterActivated();
  void OnFilterDeactivated();

 protected:
  virtual ~CaptureInputPin();

 private:
  CaptureSinkFilter* Filter() const;

  HRESULT AttemptConnection(IPin* receive_pin, const AM_MEDIA_TYPE* media_type);
  std::vector<AM_MEDIA_TYPE*> DetermineCandidateFormats(
      IPin* receive_pin,
      const AM_MEDIA_TYPE* media_type);
  void ClearAllocator(bool decommit);
  HRESULT CheckDirection(IPin* pin) const;

  // IUnknown
  STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override;

  // clang-format off
  // clang isn't sure what to do with the longer STDMETHOD() function
  // declarations.

  // IPin
  STDMETHOD(Connect)(IPin* receive_pin,
                     const AM_MEDIA_TYPE* media_type) override;
  STDMETHOD(ReceiveConnection)(IPin* connector,
                               const AM_MEDIA_TYPE* media_type) override;
  STDMETHOD(Disconnect)() override;
  STDMETHOD(ConnectedTo)(IPin** pin) override;
  STDMETHOD(ConnectionMediaType)(AM_MEDIA_TYPE* media_type) override;
  STDMETHOD(QueryPinInfo)(PIN_INFO* info) override;
  STDMETHOD(QueryDirection)(PIN_DIRECTION* pin_dir) override;
  STDMETHOD(QueryId)(LPWSTR* id) override;
  STDMETHOD(QueryAccept)(const AM_MEDIA_TYPE* media_type) override;
  STDMETHOD(EnumMediaTypes)(IEnumMediaTypes** types) override;
  STDMETHOD(QueryInternalConnections)(IPin** pins, ULONG* count) override;
  STDMETHOD(EndOfStream)() override;
  STDMETHOD(BeginFlush)() override;
  STDMETHOD(EndFlush)() override;
  STDMETHOD(NewSegment)(REFERENCE_TIME start, REFERENCE_TIME stop,
                        double rate) override;

  // IMemInputPin
  STDMETHOD(GetAllocator)(IMemAllocator** allocator) override;
  STDMETHOD(NotifyAllocator)(IMemAllocator* allocator, BOOL read_only) override;
  STDMETHOD(GetAllocatorRequirements)(ALLOCATOR_PROPERTIES* props) override;
  STDMETHOD(Receive)(IMediaSample* sample) override;
  STDMETHOD(ReceiveMultiple)(IMediaSample** samples, long count,
                             long* processed) override;
  STDMETHOD(ReceiveCanBlock)() override;
  // clang-format on


  VideoCaptureCapability requested_capability_ ;
  // Accessed on the main thread when Filter()->IsStopped() (capture thread not
  // running), otherwise accessed on the capture thread.
  VideoCaptureCapability resulting_capability_;
  DWORD capture_thread_id_ = 0;
  IMemAllocator *allocator_ ;
  std::shared_ptr<IPin> receive_pin_;
  std::atomic_bool flushing_{false};
  std::atomic_bool runtime_error_{false};
  // Holds a referenceless pointer to the owning filter, the name and
  // direction of the pin. The filter pointer can be considered const.
  PIN_INFO info_ = {};
  AM_MEDIA_TYPE media_type_{};
};

// Implement IBaseFilter (including IPersist and IMediaFilter).
class CaptureSinkFilter : public IBaseFilter {
 public:
  CaptureSinkFilter(VideoCaptureModuleImpl* capture_observer);

  HRESULT SetRequestedCapability(const VideoCaptureCapability& capability);

  // Called on the capture thread.
  void ProcessCapturedFrame(unsigned char* buffer,
                            size_t length,
                            const VideoCaptureCapability& frame_info);

  void NotifyEvent(long code, LONG_PTR param1, LONG_PTR param2);
  bool IsStopped() const;

  //  IUnknown
  STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override;

  // IPersist
  STDMETHOD(GetClassID)(CLSID* clsid) override;

  // IMediaFilter.
  STDMETHOD(GetState)(DWORD msecs, FILTER_STATE* state) override;
  STDMETHOD(SetSyncSource)(IReferenceClock* clock) override;
  STDMETHOD(GetSyncSource)(IReferenceClock** clock) override;
  STDMETHOD(Pause)() override;
  STDMETHOD(Run)(REFERENCE_TIME start) override;
  STDMETHOD(Stop)() override;

  // IBaseFilter
  STDMETHOD(EnumPins)(IEnumPins** pins) override;
  STDMETHOD(FindPin)(LPCWSTR id, IPin** pin) override;
  STDMETHOD(QueryFilterInfo)(FILTER_INFO* info) override;
  STDMETHOD(JoinFilterGraph)(IFilterGraph* graph, LPCWSTR name) override;
  STDMETHOD(QueryVendorInfo)(LPWSTR* vendor_info) override;

 protected:
  virtual ~CaptureSinkFilter();

 private:
  const std::shared_ptr< ComRefCount<CaptureInputPin>> input_pin_;
  VideoCaptureModuleImpl* const capture_observer_;
  FILTER_INFO info_ = {};
  // Set/cleared in JoinFilterGraph. The filter must be stopped (no capture)
  // at that time, so no lock is required. While the state is not stopped,
  // the sink will be used from the capture thread.
  IMediaEventSink* sink_ = nullptr;
  FILTER_STATE state_  = State_Stopped;
};
#endif  // MODULES_VIDEO_CAPTURE_MAIN_SOURCE_WINDOWS_SINK_FILTER_DS_H_
