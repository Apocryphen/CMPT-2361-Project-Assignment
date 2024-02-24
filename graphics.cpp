#include "graphics.hpp"
#include <algorithm>
#include <cmath> //for sin() and cos() calculations

template<typename T>
inline bool between(T value, T lower, T upper){
    return (value >= lower && value < upper);
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

    int width = image.GetWidth();
    int height = image.GetHeight();

    //initialize empty vector
    std::vector<std::vector<Pixel>> rotatedImage;

    //get center points
    int centerX = width / 2;
    int centerY = height / 2;

    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            int prevX = cosAngle * (x - centerX) + sinAngle * (y - centerY) + centerX;
            int prevY = -sinAngle * (x - centerX) + cosAngle * (y - centerY) + centerY;

            if (prevX >= 0 && prevX < width && prevY >= 0 && prevY < height){
                rotatedImage[y][x] = image.pixels[prevY][prevX];
            }
            else{
                rotatedImage[y][x] = {0,0,0}; //set to black if not in range
            }
        }
    }

    //copy the rotatedImage back to image and return it
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            image.pixels[y][x] = rotatedImage[y][x];
        }
    }
    
    //todo : deallocate rotatedImage

    return image;
}

const PPM& Graphics::ScaleImage(PPM& image, double factor){
    PPM resized;
    unsigned int imageWidth = image.GetWidth(),
                 imageHeight = image.GetHeight(),
                 centerCol = imageWidth / 2,
                 centerRow = imageHeight / 2;

    //Manually set the metadata to avoid a full array copy.
    resized.SetMetaData(image);
    resized.Reserve(image.GetSize());

    auto pivotedScale = [&factor](int center, int value){
        return center + static_cast<int>((value - center) / factor);
    };

    for(unsigned int row = 0; row < imageHeight; row++){
        for(unsigned int col = 0; col < imageWidth;  col++){
            unsigned int dest = row * imageWidth + col;

            int srcRow = pivotedScale(centerRow, row);
            int srcCol = pivotedScale(centerCol, col);
            int src = srcRow * imageWidth + srcCol;

            //If the sample point is outside of the source image, return a black pixel
            if(between<int>(srcRow, 0, imageHeight) && between<int>(srcCol, 0, imageWidth))
                resized[dest] = image[src];
            else
                resized[dest] = Pixel();
        }
    }

    image = std::move(resized);
    return image;
}

const PPM& Graphics::TranslateImage(PPM& image, int x, int y){
    Pixel blackPixel;
    const unsigned int imageSize   = image.GetSize(),
                       imageWidth  = image.GetWidth();
    const bool shiftLeft = x <= 0,
               shiftUp   = y <= 0;

    //Shortcircut if the image is translated out of frame
    if(!between<unsigned int>(abs(x), 0, image.GetWidth()) ||
       !between<unsigned int>(abs(y), 0, image.GetHeight())){
        std::fill(image.begin(), image.end(), blackPixel);
        return image;
    }

    int xPivotIdx = (shiftLeft ? 0 : imageSize) - x; //Select the top left or bottom right of the image
    int yPivotIdx = (shiftUp   ? 0 : imageSize) - (y * imageWidth); //Select the top or bottom row of the image

    auto xPivot = image.begin() + xPivotIdx;
    auto yPivot = image.begin() + yPivotIdx;
    auto pivot  = image.begin() + (xPivotIdx + yPivotIdx) % imageSize;

    // This is for blacking out the background, if you want the image to wrap then discard these fills
    // Note that image.begin() is the bottom right pixel and image.end() is the top left after the roatate
    
    if(shiftUp) std::fill(image.begin(), yPivot, blackPixel); //Fill from the top of the frame to the top of the image
    else        std::fill(yPivot,   image.end(), blackPixel); //Fill from the bottom of the frame to the bottom of the image

    for(unsigned int row = 0; row < imageSize; row += imageWidth){
        if(shiftLeft) std::fill(image.begin() + row, xPivot + row, blackPixel); //Fill from the right of the image to the right of the frame
        else          std::fill(xPivot - row,   image.end() - row, blackPixel); //Fill from the left of the image to the left of the frame
    }

    std::rotate(image.begin(), pivot, image.end()); // Move the pivot (top or bottom corner of the image) to the bottom right of the frame,
                                                    // wrapping the rest around, the wrapping gives the same effect as moving in the opposite direction

    return image;
}
