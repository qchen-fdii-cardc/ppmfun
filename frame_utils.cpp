#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#ifdef __cplusplus
extern "C"
{
#endif
#define MSF_GIF_IMPL
#include "msf_gif.h"
#ifdef __cplusplus
}
#endif

#include "frame_utils.h"

bool frame_to_png(const Frame &frame, const char *filename)
{
    if (frame.width() == 0 || frame.height() == 0)
        return false;
    unsigned char *data = new unsigned char[frame.width() * frame.height() * 3];
    for (int j = 0; j < frame.height(); j++)
    {
        for (int i = 0; i < frame.width(); i++)
        {
            data[(j * frame.width() + i) * 3 + 0] = frame.r(i, j);
            data[(j * frame.width() + i) * 3 + 1] = frame.g(i, j);
            data[(j * frame.width() + i) * 3 + 2] = frame.b(i, j);
        }
    }
    int result = stbi_write_png(filename, frame.width(), frame.height(), 3, data, frame.width() * 3);
    delete[] data;
    return result != 0;
}

bool frames_to_gif(const char *filename, const std::vector<Frame> &frames, int delay_cs)
{
    if (frames.empty())
        return false;
    int width = frames[0].width();
    int height = frames[0].height();

    MsfGifState gifState = {};
    if (!msf_gif_begin(&gifState, width, height))
    {
        return false;
    }

    std::vector<uint8_t> pixels;
    for (const auto &frame : frames)
    {
        if (frame.width() != width || frame.height() != height)
            continue;

        pixels.clear();
        pixels.reserve(width * height * 4);
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                pixels.push_back(frame.at(i, j).r);
                pixels.push_back(frame.at(i, j).g);
                pixels.push_back(frame.at(i, j).b);
                pixels.push_back(255); // Alpha
            }
        }
        if (!msf_gif_frame(&gifState, pixels.data(), delay_cs, 16, width * 4))
        {
            msf_gif_end(&gifState); // clean up
            return false;
        }
    }

    MsfGifResult result = msf_gif_end(&gifState);
    bool success = false;
    if (result.data)
    {
        FILE *fp = nullptr;
        if (fopen_s(&fp, filename, "wb") == 0 && fp)
        {
            fwrite(result.data, 1, result.dataSize, fp);
            fclose(fp);
            success = true;
        }
    }
    msf_gif_free(result);

    return success;
}
