#include "frame.h"
#include "frame_utils.h"
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <vector>

int main()
{
    const int width = 480;
    const int height = 480;
    const int num_frames = 60;
    const double radius = 0.5; // Radius of the sphere (from 0.0 to 1.0)

    std::vector<Frame> frames;

    for (int k = 0; k < num_frames; ++k)
    {
        Frame f(width, height, 255);
        double angle = 2.0 * 3.1415926535 * k / num_frames;
        double Lx = std::cos(angle);
        double Ly = std::sin(angle);
        double Lz = 0.7071;

        for (auto i = 0; i < f.width(); i++)
        {
            for (auto j = 0; j < f.height(); j++)
            {
                double u = 2.0 * i / f.width() - 1.0;
                double v = 2.0 * j / f.height() - 1.0;
                double r2 = u * u + v * v;
                if (r2 <= radius * radius)
                {
                    double z = std::sqrt(radius * radius - r2);
                    // Normalize u, v, z to get the normal vector
                    double normal_u = u / radius;
                    double normal_v = v / radius;
                    double normal_z = z / radius;
                    double dot = Lx * normal_u + Ly * normal_v + Lz * normal_z;
                    double diff = std::max(0.0, dot);
                    unsigned char x = clamp(10.0 + 195.0 * diff);
                    Color c(x,
                            x,
                            0);
                    f.at(i, j) = c;
                }
                else
                {
                    Color c(10, 10, 221);
                    f.at(i, j) = c;
                }
            }
        }
        frames.push_back(f);
    }

    auto fn = std::string(__FILE__);
    fn = fn.substr(0, fn.find_last_of('.'));
    frames_to_gif(fn.append(".gif").c_str(), frames, 10);

    frames[0].dump("sphere_0.ppm");
    frames[1].dump("sphere_1.ppm");

    return 0;
}