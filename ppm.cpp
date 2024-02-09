#include "ppm.hpp"

#include <fstream> // Cannot up-cast without this include

PPM::PPM() {
    pixels = {};
    magic = "";
    width = height = maxColor = 0;
}

PPM::PPM(const PPM& other) {
    pixels = other.pixels;
    magic = other.magic;
    width  = other.width;
    height = other.height;
    maxColor = other.maxColor;
}

PPM::PPM(PPM&& other) { 
    pixels = std::move(other.pixels);
    magic = std::move(other.magic);
    width  = other.width;
    height = other.height;
    maxColor = other.maxColor;
}

PPM::PPM(std::ifstream& in) : PPM(){
    in >> *this;
    if(in.fail()) 
        throw std::runtime_error("Failed to parse PPM file");
}

//Not needed but helps to have blank data in case of an error
PPM::~PPM() { 
    pixels.clear(); 
    magic = "";
    width = height = maxColor = 0;
}

const PPM& PPM::operator=(const PPM& other){
    pixels = other.pixels;
    magic = other.magic;
    width  = other.width;
    height = other.height;
    maxColor = other.maxColor;
    return *this;
}

const PPM& PPM::operator=(PPM&& other){
    if(&other == this) return *this;
    pixels = std::move(other.pixels);
    magic = std::move(other.magic);
    width  = other.width;
    height = other.height;
    maxColor = other.maxColor;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const PPM& image){
    //Header
    out << image.magic << '\n' 
        << image.width << " " << image.height << '\n' 
        << image.maxColor << '\n';

    for(unsigned int i = 0; i < image.width * image.height; i++){
        out << image.pixels[i] 
            <<  ((i+1) % image.width == 0 ? "\n" : " ");
    }
    return out;
}

constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

std::istream& operator>>(std::istream& in, PPM& image){
    in >> image.magic >> std::ws;
    while(in.peek() == '#') 
        in.ignore(max_size, '\n');
    in >> image.width >> image.height;
    in >> image.maxColor;

    if(in.fail())
        return in;
    
    size_t imageSize = image.width * image.height;
    image.pixels.reserve(imageSize);
    image.pixels.assign(std::istream_iterator<Pixel>(in), {});

    if(image.pixels.size() == imageSize)                    //Clear the failbit on a successful parse
        in.clear(in.rdstate() & ~std::ios_base::failbit);   //This is set because the iterator tries to parse past the EOF

    return in;
}

std::vector<Pixel>::iterator PPM::begin()  { return pixels.begin(); }
std::vector<Pixel>::iterator PPM::end()    { return pixels.end(); }
std::vector<Pixel>::const_iterator PPM::cbegin() { return pixels.cbegin(); }
std::vector<Pixel>::const_iterator PPM::cend()   { return pixels.cend(); }
