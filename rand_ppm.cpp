#include "frame.h"
#include <cstdlib>
#include <cmath>
#include "frame_utils.h"

int main()
{
    Frame f(480, 480, 255);

    for (auto i = 0; i < f.width(); i++)
    {
        for (auto j = 0; j < f.height(); j++)
        {
            Color c(rand() % f.max_color(),
                    rand() % f.max_color(),
                    rand() % f.max_color());
            f.at(i, j) = c;
        }
    }

    f.dump("rand.ppm");

    frame_to_png(f, "rand.png");
    return 0;
}