#pragma once

#include <stdint.h>
#include "global.h"

enum {
	kVideoCaptureUniqueNameLength = 1024
};  // Max unique capture device name lenght
enum { kVideoCaptureDeviceNameLength = 256 };  // Max capture device name lenght
enum { kVideoCaptureProductIdLength = 128 };   // Max product id length

struct VideoCaptureCapability {
	int32_t width;
	int32_t height;
	int32_t maxFPS;
	VideoType videoType;
	bool interlaced;

	VideoCaptureCapability() {
		width = 0;
		height = 0;
		maxFPS = 0;
		videoType = VideoType::kUnknown;
		interlaced = false;
	}
	bool operator!=(const VideoCaptureCapability& other) const {
		if (width != other.width)
			return true;
		if (height != other.height)
			return true;
		if (maxFPS != other.maxFPS)
			return true;
		if (videoType != other.videoType)
			return true;
		if (interlaced != other.interlaced)
			return true;
		return false;
	}
	bool operator==(const VideoCaptureCapability& other) const {
		return !operator!=(other);
	}
};