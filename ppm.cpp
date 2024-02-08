#include "ppm.hpp"

Pixel::Pixel(){
    red = green = blue = 0;
}

Pixel::Pixel(const Pixel& other){
    red = other.red;
    green = other.green;
    blue = other.blue;
}

Pixel::Pixel(Pixel&& other){ 
    red = other.red;
    green = other.green;
    blue = other.blue;
}

Pixel::Pixel(unsigned int r, unsigned int g, unsigned int b){
    red   = r;
    green = g;
    blue  = b;
}

Pixel::~Pixel() {
    red = green = blue = 0; // Not really necessary
}

const unsigned int& Pixel::operator[](const char* colour) const{
    if     (strcmp(colour, "red")   == 0) return red;
    else if(strcmp(colour, "green") == 0) return green;
    else if(strcmp(colour, "blue")  == 0) return blue;
    else throw "Bad Colour Index";
}

Pixel& Pixel::operator=(const Pixel& other){
    red = other.red;
    green = other.green;
    blue = other.blue;
    return *this;
}

Pixel& Pixel::operator=(Pixel&& other){
    red = other.red;
    green = other.green;
    blue = other.blue;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Pixel& pixel){
    out << pixel.red   << " "
        << pixel.green << " "
        << pixel.blue;
    return out;
}

std::istream& operator>>(std::istream& in, Pixel& pixel){
    in >> pixel.red 
       >> pixel.green 
       >> pixel.blue;
    return in;
}

