#include "graphics.hpp"
#include <algorithm>

template<typename T>
inline bool between(T value, T lower, T upper){
    return (value >= lower && value < upper);
}

// Sets every pixel at index (x,y) to the value at indexTransformer(x,y) (or default if not in range)
PPM& indexTransform(PPM& image, std::function<std::pair<int, int>(int, int)> indexTransformer){
    const PPM imageCopy(image);
    const int width = image.GetWidth(),
              height = image.GetHeight();
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            auto [sampleX, sampleY] = indexTransformer(x, y);
            if(between(sampleX, 0, width) && between(sampleY, 0, height))
                image[x + y * width] = imageCopy[sampleX + sampleY * width];
            else 
                image[x + y * width] = {}; // Zero initialize
        }
    }
    return image;
}

const PPM& Graphics::ApplyFilter(PPM& image, const char* filter){
    //TODO
    return image;
}

const PPM& Graphics::MakeGreyScale(PPM& image){
    std::transform(image.begin(), image.end(), image.begin(), [](Pixel p){
        int avg = (p["red"] + p["green"] + p["blue"]) / 3;
        return Pixel(avg);
    });
    return image;
}

const PPM& Graphics::RotateImage(PPM& image, double angle){
    //TODO
    return image;
}

const PPM& Graphics::ScaleImage(PPM& image, double factor){
    int centerX = image.GetWidth() / 2,
        centerY = image.GetHeight() / 2;
    auto transformer = [factor, centerX, centerY](int x, int y){ 
        int newX = centerX + (x - centerX) / factor; 
        int newY = centerY + (y - centerY) / factor;
        return std::make_pair(newX, newY);
    };
    return indexTransform(image, transformer);
}

const PPM& Graphics::TranslateImage(PPM& image, int x, int y){
    auto transformer = [offsetX = x, offsetY = y](int x, int y){
        int newX = x - offsetX;
        int newY = y - offsetY;
        return std::make_pair(newX, newY);
    };
    return indexTransform(image, transformer);
}
