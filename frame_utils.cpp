#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "frame_utils.h"

#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

namespace
{
    std::string quote_path(const std::string &path)
    {
        return "\"" + path + "\"";
    }

    std::string remove_extension(const std::string &path)
    {
        const auto dot = path.find_last_of('.');
        if (dot == std::string::npos)
            return path;
        return path.substr(0, dot);
    }
}

/**
 * Export a single frame to a PNG file
 * Call stbi_write_png() to write the image data to a PNG file.
 * The image data is stored in a 1D array of unsigned char,
 * where each pixel is represented by three consecutive bytes (R, G, B).
 * The stride (number of bytes in a row) is calculated as
 * width * 3 since each pixel has 3 bytes.
 * After writing the PNG file, the allocated memory for the image data is freed.
 * The function returns true if the PNG file was successfully written,
 * and false otherwise.
 */
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

/**
 * Export a series of frames to a GIF file
 * The function first checks if the frames vector is empty or if the delay is non-positive, returning false in those cases.
 * It then retrieves the width and height of the first frame and checks if they are valid.
 * If all checks pass, it proceeds to create temporary PPM files for each frame and uses ffmpeg to generate the GIF.
 */
bool frames_to_gif(const char *filename, const std::vector<Frame> &frames, int delay_cs)
{
    if (frames.empty())
        return false;
    if (delay_cs <= 0)
        return false;

    const int width = frames[0].width();
    const int height = frames[0].height();
    if (width <= 0 || height <= 0)
        return false;

    const std::string gif_filename(filename);
    const std::string frame_prefix = remove_extension(gif_filename) + "_ffmpeg";
    std::vector<std::string> temp_files;
    temp_files.reserve(frames.size());

    for (size_t index = 0; index < frames.size(); ++index)
    {
        const auto &frame = frames[index];
        if (frame.width() != width || frame.height() != height)
            return false;

        std::ostringstream frame_name;
        frame_name << frame_prefix << "_" << std::setw(4) << std::setfill('0') << index << ".ppm";
        const auto path = frame_name.str();
        if (frame.dump(path.c_str()) == static_cast<size_t>(-1))
        {
            for (const auto &temp_file : temp_files)
                std::remove(temp_file.c_str());
            return false;
        }
        temp_files.push_back(path);
    }

    std::ostringstream fps_stream;
    fps_stream << std::fixed << std::setprecision(3) << (100.0 / delay_cs);

    std::ostringstream cmd;
    cmd << "ffmpeg -y -framerate " << fps_stream.str()
        << " -i " << quote_path(frame_prefix + "_%04d.ppm")
        << " -vf \"scale=" << width << ":" << height
        << ":flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse\" "
        << quote_path(gif_filename);

    const int result = std::system(cmd.str().c_str());

    for (const auto &temp_file : temp_files)
        std::remove(temp_file.c_str());

    return result == 0;
}
