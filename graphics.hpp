#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "ppm.hpp"

class Graphics{
    static const PPM& ApplyFilter(PPM& image, const char* filter);
    static const PPM& MakeGreyScale(PPM& image);
    static const PPM& RotateImage(PPM& image);
    static const PPM& ScaleImage(PPM& image);
    static const PPM& TranslateImage(PPM& image);
};

#endif
