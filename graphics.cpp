#include "graphics.hpp"
#include <algorithm>
#include <numeric>

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
    const std::string filterType(filter);
    struct Kernel{
        int width;
        std::vector<int> data;
    };
    const Kernel kernel = [&filterType]() -> const Kernel{
        if (filterType == "blur") {
            return { 
                3,
                { 1, 1, 1,
                  1, 1, 1,
                  1, 1, 1 }
            };
        }
        else if (filterType == "sharpen") {
            return {
                3,
                { -1, -1, -1,
                  -1,  9, -1,
                  -1, -1, -1 }
            };
        }
        else if (filterType == "emboss") {
            return {
                3,
                { -2, -1, 0,
                  -1,  1, 1,
                   0,  1, 2 }
            };
        }
        else{
            throw std::invalid_argument("Unknown filter type " + filterType);
        }
    }();
    
    const PPM oldImage(image); // Make a copy to avoid modifying the original pixels during calculations
    const int kernelWeight = std::accumulate(kernel.data.cbegin(), kernel.data.cend(), 0),
              kernelCenter = kernel.data.size() / 2,
              kernelRadius = kernel.width / 2;

    //The narrowing conversion becomes problematic for very large images
    const int imageWidth = image.GetWidth(),
              imageHeight = image.GetHeight(),
              maxColor = image.GetMaxColor();

    for (int imageRow = kernelRadius; imageRow < imageHeight - kernelRadius; ++imageRow) {
        for (int imageColumn = kernelRadius; imageColumn < imageWidth - kernelRadius; ++imageColumn) {

            const int pixelIndex = imageRow * imageWidth + imageColumn;

            int redSum = 0, greenSum = 0, blueSum = 0;
            for (int kernelRow = -kernelRadius; kernelRow <= kernelRadius; ++kernelRow) {
                for (int kernelColumn = -kernelRadius; kernelColumn <= kernelRadius; ++kernelColumn) {
                    const int offsetPixelIndex = pixelIndex + (kernelRow * imageWidth + kernelColumn);
                    const int kernelIndex = kernelRow * kernel.width + kernelColumn + kernelCenter;

                    const Pixel& p = oldImage [offsetPixelIndex];
                    const int weight = kernel.data[kernelIndex];
                    redSum   += weight * p["red"];
                    greenSum += weight * p["green"];
                    blueSum  += weight * p["blue"];
                }
            }

            //Idk how to nicely format this...
            image[pixelIndex] = Pixel(
                std::clamp(redSum   / kernelWeight, 0, maxColor),
                std::clamp(greenSum / kernelWeight, 0, maxColor),
                std::clamp(blueSum  / kernelWeight, 0, maxColor)
            );
        }
    }
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
    //mathematical variables required for angle calculation
    double radians = angle * M_PI / 180;
    double sinAngle = std::sin(radians);
    double cosAngle = std::cos(radians);

    //get center points
    int centerX = image.GetWidth() / 2;
    int centerY = image.GetHeight() / 2;

    auto transformer = [centerX, centerY, sinAngle, cosAngle](int x, int y){
        int newX = centerX + (x - centerX) * cosAngle + (y - centerY) * sinAngle;
        int newY = centerY - (x - centerX) * sinAngle + (y - centerY) * cosAngle;
        return std::make_pair(newX, newY);
    };
    return indexTransform(image, transformer);
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
