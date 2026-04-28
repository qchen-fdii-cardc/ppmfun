#ifndef FRAME_UTILS_H
#define FRAME_UTILS_H

#include "frame.h"
#include <vector>

// Export a single frame to a PNG file
bool frame_to_png(const Frame &frame, const char *filename);

// Export a series of frames to a GIF file
bool frames_to_gif(const char *filename, const std::vector<Frame> &frames, int delay_cs);

#endif // FRAME_UTILS_H
