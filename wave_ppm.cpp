#include "frame.h"
#include <cstdlib>
#include <cmath>
#include <sstream>

#include "frame_utils.h"

int main()
{
    Frame f(480, 480, 255);

    for (auto i = 0; i < f.width(); i++)
    {
        for (auto j = 0; j < f.height(); j++)
        {
            double u = 2.0 * i / f.width() - 1.0;
            double v = 2.0 * j / f.height() - 1.0;
            double r = sqrt(u * u + v * v);
            double s = sin(12.0 * r - 2.5 * r * r) * exp(-2.0 * r);
            Color c((unsigned char)(80 + 40 * s),
                    (unsigned char)(150 + 60 * s),
                    (unsigned char)(220 + 35 * s));

            f.at(i, j) = c;
        }
    }
    auto fn = std::string(__FILE__);
    fn = fn.substr(0, fn.find_last_of('.')) + ".ppm";

    f.dump(fn.c_str());

    frame_to_png(f, "wave.png");
    return 0;
}