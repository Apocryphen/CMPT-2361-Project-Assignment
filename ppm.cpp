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

/***** PPM Image *****/

PPMImage::PPMImage() {
    pixels = {};
    format = "";
    width = height = channelDepth = 0;
}

//To avoid all this duplication I'm using copy swap after this
PPMImage::PPMImage(const PPMImage& other) {
    pixels = other.pixels;
    format = other.format;
    width  = other.width;
    height = other.height;
    channelDepth = other.channelDepth;
}

PPMImage::PPMImage(PPMImage&& other) : PPMImage() { swap(*this, other); }

//Not needed but helps to have blank data in case of an error
PPMImage::~PPMImage() { 
    pixels.clear(); 
    format = "";
    width = height = channelDepth = 0;
}

//Pass by value to take advabtage of copy elison.
PPMImage& PPMImage::operator=(PPMImage other){
    swap(*this, other);
    return *this;
}

PPMImage& PPMImage::operator=(PPMImage&& other){
    swap(*this, other);
    return *this;
}

// For the copy swap idiom
void swap(PPMImage& a, PPMImage& b){
    using std::swap;
    swap(a.pixels, b.pixels);
    swap(a.format, b.format);
    swap(a.width, b.width);
    swap(a.height, b.height);
    swap(a.channelDepth, b.channelDepth);
}

std::ostream& operator<<(std::ostream& out, const PPMImage& image){
    //Header
    out << image.format << '\n' 
        << image.width << " " << image.height << '\n' 
        << image.channelDepth << '\n';

    for(int i = 0; i < image.width * image.height; i++){
        out << image.pixels[i] << " ";
        if((i+1) % image.width == 0) out << '\n';
    }
    return out;
}

constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

std::istream& operator>>(std::istream& in, PPMImage& image){
    in >> image.format >> std::ws;
    while(in.peek() == '#') 
        in.ignore(max_size, '\n');
    in >> image.width >> image.height;
    in >> image.channelDepth;

    std::cout << "Got: Format = " << image.format
              << " width, height = (" << image.width << ", " << image.height
              << ") channel depth = " << image.channelDepth << std::endl;

    image.pixels = std::vector(std::istream_iterator<Pixel>(in),
            std::istream_iterator<Pixel>());

    return in;
}
