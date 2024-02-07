#include <iostream>
#include <fstream>
#include <string>

using std::string;

int main(int argc, char** argv){
    string fileName = "example.ppm"; // Default filename
    if (argc == 2) fileName = argv[1];

    std::ifstream infile(fileName);

    if(!infile.is_open()) {
        std::cerr << "Failed to open file " << fileName << std::endl;
        return 1;
    }

    while (infile){
        int test;
        infile >> test;
        std::cout << "Got: " << test << std::endl;
    }

    infile.close();
    return 0;
}
