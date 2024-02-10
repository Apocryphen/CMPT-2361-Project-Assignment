#include "graphics.hpp"
#include <algorithm>

Pixel pixelAverage(Pixel p){
    int avg = (p["red"] + p["green"] + p["blue"]) / 3;
    return Pixel(avg);
}

const PPM& Graphics::MakeGreyScale(PPM& image){
    std::transform(image.begin(), image.end(), image.begin(), pixelAverage);
    return image;
}
