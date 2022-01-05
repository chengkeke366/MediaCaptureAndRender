#pragma once

enum class VideoType {
	kUnknown,
	kI420,
	kIYUV,
	kRGB24,
	kARGB,
	kRGB565,
	kYUY2,
	kYV12,
	kUYVY,
	kMJPEG,
	kBGRA,
};

// enum for clockwise rotation.
enum VideoRotation {
	kVideoRotation_0 = 0,
	kVideoRotation_90 = 90,
	kVideoRotation_180 = 180,
	kVideoRotation_270 = 270
};