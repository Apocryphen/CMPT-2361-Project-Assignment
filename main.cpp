#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "ppm.hpp"
#include "graphics.hpp"

void translateDemo(PPM baseImage){
    std::string demoName = "translateDemo.ppm";
    std::ofstream outfile(demoName);
    if(!outfile.is_open()){
        std::cerr << "Failed to open file " << demoName << std::endl;
        exit(EXIT_FAILURE);
    }

    Graphics::TranslateImage(baseImage, 50, 100);

    outfile << baseImage;
    outfile.close();
}

void scaleDemo(PPM baseImage){
    std::string demoName = "scaleDemo.ppm";
    std::ofstream outfile(demoName);
    if(!outfile.is_open()){
        std::cerr << "Failed to open file " << demoName << std::endl;
        exit(EXIT_FAILURE);
    }

    Graphics::ScaleImage(baseImage, 0.5);

    outfile << baseImage;
    outfile.close();
}

void greyscaleDemo(PPM baseImage){
    std::string demoName = "greyscaleDemo.ppm";
    std::ofstream outfile(demoName);
    if(!outfile.is_open()){
        std::cerr << "Failed to open file " << demoName << std::endl;
        exit(EXIT_FAILURE);
    }

    Graphics::MakeGreyScale(baseImage);

    outfile << baseImage;
    outfile.close();
}

int main(int argc, char** argv){
    std::string fileName = "ex.ppm"; // Default filename
    if (argc >= 2) fileName = argv[1];

    std::ifstream infile(fileName);

    if(!infile.is_open()) {
        std::cerr << "Failed to open file " << fileName << std::endl;
        return EXIT_FAILURE;
    }

    PPM loadedImage;
    infile >> loadedImage;

    if(infile.fail()){
        std::cerr << "Could not parse PPM File" << std::endl;
        infile.close();
        return EXIT_FAILURE;
    }
    infile.close();
    
    greyscaleDemo(loadedImage);
    scaleDemo(loadedImage);
    translateDemo(loadedImage);

    return EXIT_SUCCESS;
}
