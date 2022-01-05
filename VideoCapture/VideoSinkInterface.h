#pragma once

template <typename VideoFrameT>
class VideoSinkInterface {
public:
	virtual ~VideoSinkInterface() = default;

	virtual void OnFrame(const VideoFrameT& frame) = 0;

	// Should be called by the source when it discards the frame due to rate
	// limiting.
	virtual void OnDiscardedFrame() {}
};
