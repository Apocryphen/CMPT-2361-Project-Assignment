#include <iostream>
#include <fstream>
#include <string>
#include "ppm.hpp"

using std::string;

int main(int argc, char** argv){
    string fileName = "ex.ppm"; // Default filename
    if (argc == 2) fileName = argv[1];

    std::ifstream infile(fileName);

    if(!infile.is_open()) {
        std::cerr << "Failed to open file " << fileName << std::endl;
        return 1;
    }

    infile.close();
    return 0;
}
