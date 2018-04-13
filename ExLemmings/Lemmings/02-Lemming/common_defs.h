#ifndef _COMMON_DEFS_INCLUDE
#define _COMMON_DEFS_INCLUDE


#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 190
#define VIEWPORT_WIDTH 960
#define VIEWPORT_HEIGHT 570


enum PixelFormat {TEXTURE_PIXEL_FORMAT_RGB, TEXTURE_PIXEL_FORMAT_RGBA, TEXTURE_PIXEL_FORMAT_L};

struct CameraPos {
	unsigned int top;
	unsigned int bottom;
	unsigned int left;
	unsigned int right;
};

#endif //_COMMON_DEFS_INCLUDE


