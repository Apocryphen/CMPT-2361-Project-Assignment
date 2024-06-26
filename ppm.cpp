#include "ppm.hpp"

#include <fstream> // Cannot up-cast without this include

PPM::PPM() noexcept {
    pixels = {};
    magic = comment = "";
    width = height = maxColor = 0;
}

PPM::PPM(const PPM& other) noexcept {
    pixels = other.pixels;
    magic = other.magic;
    comment = other.comment;
    width  = other.width;
    height = other.height;
    maxColor = other.maxColor;
}

PPM::PPM(PPM&& other) noexcept { 
    pixels = std::move(other.pixels);
    magic = std::move(other.magic);
    comment = std::move(other.comment);
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
PPM::~PPM() noexcept { 
    pixels.clear(); 
    magic = "";
    width = height = maxColor = 0;
}

void PPM::Resize(unsigned int newSize){
    pixels.resize(newSize); 
}

void PPM::Reserve(unsigned int newSize){
    pixels.reserve(newSize); // Doesn't default construct all elements
}

void PPM::SaveImageToFile(std::string fileName) const{
    std::ofstream outfile(fileName);
    if(!outfile.is_open())
        throw std::runtime_error("Failed to write to file " + fileName);

    outfile << *this;
    outfile.close();
}

std::string PPM::getComment()   const noexcept { return comment; }
std::string PPM::getMagic()     const noexcept { return magic; }
unsigned int PPM::GetHeight()   const noexcept { return height; }
unsigned int PPM::GetWidth()    const noexcept { return width; }
unsigned int PPM::GetSize()     const noexcept { return width * height; }
unsigned int PPM::GetMaxColor() const noexcept { return maxColor; }

void PPM::SetComment(std::string newComment)    { comment   = newComment; }
void PPM::SetMagic(std::string newMagic)        { magic     = newMagic; }
void PPM::SetHeight(unsigned int newHeight) noexcept     { height    = newHeight; }
void PPM::SetWidth(unsigned int newWidth) noexcept       { width     = newWidth; }
void PPM::SetMaxColor(unsigned int newMaxColor) noexcept { maxColor  = newMaxColor; }

const PPM& PPM::operator=(const PPM& other){
    pixels = other.pixels;
    magic = other.magic;
    comment = other.comment;
    width  = other.width;
    height = other.height;
    maxColor = other.maxColor;
    return *this;
}

const PPM& PPM::operator=(PPM&& other) noexcept{
    if(&other == this) return *this;
    pixels = std::move(other.pixels);
    magic = std::move(other.magic);
    comment = std::move(other.comment);
    width  = other.width;
    height = other.height;
    maxColor = other.maxColor;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const PPM& image){
    //Header
    out << image.magic << '\n' 
        << image.comment
        << image.width << " " << image.height << '\n' 
        << image.maxColor << '\n';

    for(unsigned int i = 0; i < image.width * image.height; i++){
        out << image.pixels[i] 
            <<  ((i+1) % image.width == 0 ? "\n" : " ");
    }
    return out;
}

Pixel& PPM::operator[](unsigned int index){ return pixels[index]; }
const Pixel& PPM::operator[](unsigned int index) const { return pixels[index]; }

std::istream& operator>>(std::istream& in, PPM& image){
    in >> image.magic >> std::ws;
    while(in.peek() == '#'){ 
        std::string line;
        std::getline(in, line);
        image.comment.append(line + '\n');
    }
    in >> image.width >> image.height;
    in >> image.maxColor;

    if(in.fail())
        return in;
    
    image.pixels.reserve(image.GetSize());
    image.pixels.assign(std::istream_iterator<Pixel>(in), {});

    if(image.pixels.size() == image.GetSize())                    //Clear the failbit on a successful parse
        in.clear(in.rdstate() & ~std::ios_base::failbit);   //This is set because the iterator tries to parse past the EOF

    return in;
}

std::vector<Pixel>::iterator PPM::begin() noexcept  { return pixels.begin(); }
std::vector<Pixel>::iterator PPM::end()   noexcept  { return pixels.end(); }
std::vector<Pixel>::const_iterator PPM::cbegin() const noexcept { return pixels.cbegin(); }
std::vector<Pixel>::const_iterator PPM::cend()   const noexcept { return pixels.cend(); }
