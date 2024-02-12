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

const PPM& Graphics::ScaleImage(PPM& image, double factor){
    PPM resized;
    int imageWidth = image.GetWidth(),
        imageHeight = image.GetHeight(),
        centerCol = imageWidth / 2,
        centerRow = imageHeight / 2;

    //Manually set the metadata to avoid a full array copy.
    resized.SetMetaData(image);
    resized.Reserve(image.GetSize());

    auto pivotedScale = [&factor](int center, int value){
        return center + static_cast<int>((value - center) / factor);
    };

    for(int row = 0; row < imageHeight; row++){
        for(int col = 0; col < imageWidth;  col++){
            int dest = row * imageWidth + col;
            int src = pivotedScale(centerRow, row) * imageWidth +
                      pivotedScale(centerCol, col);
            resized[dest] = image[src];
        }
    }

    image = std::move(resized);
    return image;
}
