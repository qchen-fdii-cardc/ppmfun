#include "frame.h"
#include <cstdlib>
#include <cmath>
#include <sstream>

int main()
{
    Frame f(480, 480, 255);
    const double Lx = -0.7071;
    const double Ly = -0.7071;
    const double Lz = 0.7071;

    for (auto i = 0; i < f.width(); i++)
    {
        for (auto j = 0; j < f.height(); j++)
        {
            f.at(i, j) = Color(10, 10, 220);
        }
    }
    auto fn = std::string(__FILE__);
    fn = fn.substr(0, fn.find_last_of('.')) + ".ppm";
    f.dump(fn.c_str());
    return 0;
}