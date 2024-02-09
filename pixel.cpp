#include "pixel.hpp"

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

Pixel::Pixel(unsigned int intensity) : Pixel(intensity, intensity, intensity) {}

Pixel::Pixel(unsigned int r, unsigned int g, unsigned int b){
    red   = r;
    green = g;
    blue  = b;
}

Pixel::~Pixel() {
    red = green = blue = 0; // Not really necessary
}

unsigned int& Pixel::operator[](const char* color) {
    if     (strcmp(color, "red")   == 0) return red;
    else if(strcmp(color, "green") == 0) return green;
    else if(strcmp(color, "blue")  == 0) return blue;
    else throw InputOutOfBoundsException("Bad colour index", color);
}

const unsigned int& Pixel::operator[](const char* color) const{
    if     (strcmp(color, "red")   == 0) return red;
    else if(strcmp(color, "green") == 0) return green;
    else if(strcmp(color, "blue")  == 0) return blue;
    else throw InputOutOfBoundsException("Bad colour index", color);
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

// Shorten a long name
using IOExcept = Pixel::InputOutOfBoundsException;
IOExcept::InputOutOfBoundsException(const char* errorMsg, const char* offendingIdx){
    errorMessage = errorMsg;
    offendingIndex = offendingIdx;
}
const char* IOExcept::returnError() const{
    return errorMessage;
}
const char* IOExcept::returnOffendingIndex() const {
    return offendingIndex;
}

