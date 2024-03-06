#include "graphics.hpp"
#include <algorithm>
#include <cmath> //for sin() and cos() calculations

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
    /*
     matrix used for rotation
     cos(x) -sin(x)
     sin(x)  cos(x)
     */
    //mathematical variables required for angle calculation
    double radians = angle * M_PI / 180;
    double sinAngle = std::sin(radians);
    double cosAngle = std::cos(radians);

    PPM newImage = image;
    
    //get center points
    int centerX = image.GetWidth() / 2;
    int centerY = image.GetHeight() / 2;

    for (unsigned int y = 0; y < newImage.GetHeight(); ++y){
        for (unsigned int x = 0; x < newImage.GetWidth(); ++x){
            //translate point to origin for rotation calculation
            int translateX = x - centerX;
            int translateY = y - centerY;

            //apply rotation matrix
            int originalX = std::round(centerX + (translateX * cosAngle - translateY * sinAngle));
            int originalY = std::round(centerY + (translateX * sinAngle + translateY * cosAngle));

            //check whether original position is within bounds (else add blackspace)
            if (originalX >= 0 && originalX < static_cast<int>(image.GetWidth()) && originalY >= 0 && originalY < static_cast<int>(image.GetHeight())){
                    newImage[y * image.GetWidth() + x] = image[originalY * image.GetWidth() + originalX];
            }else{
                Pixel& newPixel = newImage[y * image.GetWidth() + x];
                newPixel["red"] = 0;
                newPixel["green"] = 0;
                newPixel["blue"] = 0;
            }
        }
    }

    image = newImage;
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
