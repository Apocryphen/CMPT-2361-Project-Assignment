#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "ppm.hpp"

class Graphics{
    public:
    static const PPM& ApplyFilter(PPM& image, const char* filter);
    static const PPM& MakeGreyScale(PPM& image);
    static const PPM& RotateImage(PPM& image, double angle);
    static const PPM& ScaleImage(PPM& image, double factor);
    static const PPM& TranslateImage(PPM& image, int x, int y);
};

#endif
