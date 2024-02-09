#include "ppm.hpp"

PPMImage::PPMImage() {
    pixels = {};
    format = "";
    width = height = channelDepth = 0;
}

PPMImage::PPMImage(const PPMImage& other) {
    pixels = other.pixels;
    format = other.format;
    width  = other.width;
    height = other.height;
    channelDepth = other.channelDepth;
}

PPMImage::PPMImage(PPMImage&& other) { 
    pixels = std::move(other.pixels);
    format = std::move(other.format);
    width  = other.width;
    height = other.height;
    channelDepth = other.channelDepth;
}

//Not needed but helps to have blank data in case of an error
PPMImage::~PPMImage() { 
    pixels.clear(); 
    format = "";
    width = height = channelDepth = 0;
}

PPMImage& PPMImage::operator=(const PPMImage& other){
    pixels = other.pixels;
    format = other.format;
    width  = other.width;
    height = other.height;
    channelDepth = other.channelDepth;
    return *this;
}

PPMImage& PPMImage::operator=(PPMImage&& other){
    if(&other == this) return *this;
    pixels = std::move(other.pixels);
    format = std::move(other.format);
    width  = other.width;
    height = other.height;
    channelDepth = other.channelDepth;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const PPMImage& image){
    //Header
    out << image.format << '\n' 
        << image.width << " " << image.height << '\n' 
        << image.channelDepth << '\n';

    for(unsigned int i = 0; i < image.width * image.height; i++){
        out << image.pixels[i] 
            <<  ((i+1) % image.width == 0 ? "\n" : " ");
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

    if(in.fail())
        return in;
    
    size_t imageSize = image.width * image.height;
    image.pixels.reserve(imageSize);
    image.pixels.assign(std::istream_iterator<Pixel>(in), {});

    if(image.pixels.size() == imageSize)                    //Clear the failbit on a successful parse
        in.clear(in.rdstate() & ~std::ios_base::failbit);   //This is set because the iterator tries to parse past the EOF

    return in;
}

std::vector<Pixel>::iterator PPMImage::begin()  { return pixels.begin(); }
std::vector<Pixel>::iterator PPMImage::end()    { return pixels.end(); }
std::vector<Pixel>::const_iterator PPMImage::cbegin() { return pixels.cbegin(); }
std::vector<Pixel>::const_iterator PPMImage::cend()   { return pixels.cend(); }
