#include <cstdlib>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include "ppm.hpp"
#include "graphics.hpp"

const std::string outputFolder = "./output/";

void applyFilterDemo(PPM baseImage){
    //TODO
}

void rotateDemo(PPM baseImage){
    Graphics::RotateImage(baseImage, 60);
    baseImage.SaveImageToFile(outputFolder + "rotateDemo.ppm");
}

void translateDemo(PPM baseImage){
    Graphics::TranslateImage(baseImage, 50, 100);
    baseImage.SaveImageToFile(outputFolder + "translateDemo.ppm");
}

void scaleDemo(PPM baseImage){
    Graphics::ScaleImage(baseImage, 0.5);
    baseImage.SaveImageToFile(outputFolder + "scaleDemo.ppm");
}

void greyscaleDemo(PPM baseImage){
    Graphics::MakeGreyScale(baseImage);
    baseImage.SaveImageToFile(outputFolder + "greyscaleDemo.ppm");
}

PPM loadFile(std::string filename){
    std::ifstream infile(filename);

    if(!infile.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    PPM loadedImage;
    try{
        loadedImage = PPM(infile);
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
        infile.close();
        exit(EXIT_FAILURE);
    }
    infile.close();
    return loadedImage;
}

template<typename... Ts>
void timeFunction(std::string desc, void fn(Ts...), Ts... args){
    auto pre = std::chrono::high_resolution_clock::now();
    fn(args...);
    auto post = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(post - pre);
    std::cout << desc << " operation took " << diff.count() << " (ms)" << std::endl;
}

template<typename R, typename... Ts>
R timeFunction(std::string desc, R fn(Ts...), Ts... args){
    auto pre = std::chrono::high_resolution_clock::now();
    R result = fn(args...);
    auto post = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(post - pre);
    std::cout << desc << " operation took " << diff.count() << " (ms)" << std::endl;
    return result;
}


int main(int argc, char** argv){
    std::string fileName = "ex.ppm"; // Default filename
    if (argc >= 2) fileName = argv[1];

    PPM loadedImage = timeFunction("Load image", loadFile, fileName);

    try {
        timeFunction("Convert to greyscale", greyscaleDemo, loadedImage);
        timeFunction("Scale image", scaleDemo, loadedImage);
        timeFunction("Translate image", translateDemo, loadedImage);
        timeFunction("Roatate image", rotateDemo, loadedImage);
        timeFunction("Filter image", applyFilterDemo, loadedImage);
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
