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

Pixel::Pixel(unsigned int r, unsigned int g, unsigned int b){
    red   = r;
    green = g;
    blue  = b;
}

Pixel::~Pixel() {
    red = green = blue = 0; // Not really necessary
}

const unsigned int& Pixel::operator[](int index) const{
    switch (index){
        case 0: return red;
        case 1: return green;
        case 2: return blue;
        default: throw std::out_of_range("Bad colour index");
    }
}
const unsigned int& Pixel::operator[](const char* color) const{
    if     (strcmp(color, "red")   == 0) return red;
    else if(strcmp(color, "green") == 0) return green;
    else if(strcmp(color, "blue")  == 0) return blue;
    else throw std::out_of_range("Bad colour index");
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

