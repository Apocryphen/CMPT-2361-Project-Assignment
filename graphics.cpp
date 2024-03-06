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
    std::string filterType(filter);

    // Define the filter kernels for blur, sharpen, and emboss
    std::vector<std::vector<int>> kernel; // trying to do array but showing error all the times

    // Using an enum to switch between filters
    enum Filter { BLUR, SHARPEN, EMBOSS, UNKNOWN };
    Filter filterEnum = UNKNOWN;

    if (filterType == "blur") {
        filterEnum = BLUR;
        kernel = {
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, 1}
        };
    }
    else if (filterType == "sharpen") {
        filterEnum = SHARPEN;
        kernel = {
            {-1, -1, -1},
            {-1, 9, -1},
            {-1, -1, -1}
        };
    }
    else if (filterType == "emboss") {
        filterEnum = EMBOSS;
        kernel = {
            {-2, -1, 0},
            {-1, 1, 1},
            {0, 1, 2}
        };
    }

    // Apply the chosen filter based on the filterEnum
    switch (filterEnum)
    {
    case BLUR:
    case SHARPEN:
    case EMBOSS: {
        PPM newImage = image; // Make a copy to avoid modifying the original pixels during calculations
        int kernelSize = kernel.size();
        int kernelRadius = kernelSize / 2;

        for (unsigned y = 0; y < image.GetHeight(); ++y) {
            for (unsigned x = 0; x < image.GetWidth(); ++x) {
                int redSum = 0, greenSum = 0, blueSum = 0, weightSum = 0;
                for (int dy = -kernelRadius; dy <= kernelRadius; ++dy) {
                    for (int dx = -kernelRadius; dx <= kernelRadius; ++dx) {

                        int px = std::max(0, std::min(static_cast<int>(x) + dx, static_cast<int>(image.GetWidth()) - 1));
                        int py = std::max(0, std::min(static_cast<int>(y) + dy, static_cast<int>(image.GetHeight()) - 1));

                            Pixel& p = image[py * image.GetWidth() + px];
                            int weight = kernel[dy + kernelRadius][dx + kernelRadius];
                            redSum += weight * p["red"];
                            greenSum += weight * p["green"];
                            blueSum += weight * p["blue"];
                            weightSum += weight;
                        
                    }
                }
                if (filterEnum == BLUR) {
                    weightSum = 9; // For the blur filter, to normalize the kernel sum to 1
                }

                if (weightSum == 0) weightSum = 1; // Avoid division by zero
                Pixel& newPixel = newImage[y * image.GetWidth() + x];
                newPixel["red"] = std::min(std::max(redSum / weightSum, 0), 255);
                newPixel["green"] = std::min(std::max(greenSum / weightSum, 0), 255);
                newPixel["blue"] = std::min(std::max(blueSum / weightSum, 0), 255);
            }
        }
        image = newImage; // Copy the modified image back to the original image
        break;
    }
    default:
        throw std::invalid_argument("Unknown filter type");
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
