#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    Color(unsigned char _r = 0, unsigned char _g = 0, unsigned char _b = 0) : r(_r), g(_g), b(_b) {}
};

template <typename T>
const inline unsigned char clamp(const T &x)
{
    return static_cast<unsigned char>(x);
}

class Frame
{
public:
    Frame(const int w, const int h, const int mc = 255) : _width(w), _height(h), _max_color(mc)
    {
        _image.resize(w);
        for (auto i = 0; i < w; i++)
        {
            _image[i].resize(h);
        }
    }

    Frame(const char *filename)
    {
        _width = 0, _height = 0, _max_color = 0;
        std::ifstream file(filename);

        if (!file.is_open())
            return;

        std::string head;
        file >> head;
        if (head != "P3")
            return;
        char c;
        while (true)
        {
            file >> c;
            if (c == '#')
            {
                std::string line;
                std::getline(file, line);
            }
            else
            {
                file.unget();
                break;
            }
        }
        file >> _width >> _height >> _max_color;
        _image.resize(_width);
        for (auto i = 0; i < _width; i++)
        {
            _image[i].resize(_height);
        }

        for (auto j = 0; j < _height; j++)
        {
            for (auto i = 0; i < _width; i++)
            {
                int r = 0, g = 0, b = 0;
                file >> r >> g >> b;
                _image[i][j] = Color(r, g, b);
            }
        }
    }
    unsigned char &r(int i, int j) { return _image[i][j].r; }
    const unsigned char &r(int i, int j) const { return _image[i][j].r; }
    unsigned char &g(int i, int j) { return _image[i][j].g; }
    const unsigned char &g(int i, int j) const { return _image[i][j].g; }
    unsigned char &b(int i, int j) { return _image[i][j].b; }
    const unsigned char &b(int i, int j) const { return _image[i][j].b; }
    Color &at(int i, int j) { return _image[i][j]; }
    const Color &at(int i, int j) const { return _image[i][j]; }
    int width() const { return _width; }
    int height() const { return _height; }
    int max_color() const { return _max_color; }
    // export
    std::string dump() const
    {
        std::ostringstream buff;
        buff << "P3\n";
        buff << width() << " " << height() << "\n";
        buff << max_color() << "\n";
        for (auto i = 0; i < width(); i++)
        {
            for (auto j = 0; j < height(); j++)
            {
                buff << r(i, j) % max_color() << " "
                     << g(i, j) % max_color() << " "
                     << b(i, j) % max_color() << "\n";
            }
        }

        return buff.str();
    }

    size_t dump(const char *fn) const
    {
        std::ofstream file(fn);
        if (!file.is_open())
            return -1;
        auto s = dump();
        file << s;
        return s.length();
    }

private:
    int _width, _height, _max_color;
    std::vector<std::vector<Color>> _image;
};
#endif // FRAME_H