#include "frame_utils.h"
#include <vector>
#include <cmath>

int main()
{
    const int width = 256;
    const int height = 256;
    const int num_frames = 30;
    std::vector<Frame> frames;

    for (int f = 0; f < num_frames; ++f)
    {
        Frame frame(width, height);
        float time = (float)f / num_frames;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                float x = (float)i / width - 0.5f;
                float y = (float)j / height - 0.5f;
                float radius = 0.2f + 0.1f * sin(time * 2 * 3.14159f);
                if (x * x + y * y < radius * radius)
                {
                    frame.at(i, j) = Color(255, 255, 0);
                }
                else
                {
                    frame.at(i, j) = Color(2, 2, 255);
                }
            }
        }
        frames.push_back(frame);
    }

    if (!frames.empty())
    {
        frames.front().dump("rand_animation_debug_first.ppm");
        frames[num_frames / 2].dump("rand_animation_debug_mid.ppm");
    }

    frames_to_gif("animation.gif", frames, 10);

    return 0;
}
