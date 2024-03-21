#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "ppm.hpp"
#include "graphics.hpp"

const std::string outputFolder = "./output/";

void applyFilterDemo(PPM baseImage){
    const PPM original(baseImage);

    Graphics::ApplyFilter(baseImage, "emboss");
    baseImage.SaveImageToFile(outputFolder + "embossDemo.ppm");
    baseImage = original;

    Graphics::ApplyFilter(baseImage, "blur");
    baseImage.SaveImageToFile(outputFolder + "blurDemo.ppm");

    baseImage = original;
    Graphics::ApplyFilter(baseImage, "sharpen");
    baseImage.SaveImageToFile(outputFolder + "sharpenDemo.ppm");
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
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        infile.close();
        exit(EXIT_FAILURE);
    }
    infile.close();
    return loadedImage;
}

int main(int argc, char** argv){
    std::string fileName = "Shahriar.ppm"; // Default filename
    if (argc >= 2) fileName = argv[1];
    const PPM loadedImage = loadFile(fileName);
    std::cout << "Computing" << std::flush;
    try {
        greyscaleDemo(loadedImage);
        std:: cout << "." << std::flush;
        scaleDemo(loadedImage);
        std:: cout << "." << std::flush;
        translateDemo(loadedImage);
        std:: cout << "." << std::flush;
        rotateDemo(loadedImage);
        std:: cout << "." << std::flush;
        applyFilterDemo(loadedImage);
        std:: cout << ". Done!" << std::endl;
    }
    catch(const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
