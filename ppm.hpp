#ifndef PPM_HPP
#define PPM_HPP

#include <iostream> 
#include <vector>
#include <string>

#include "pixel.hpp"

class PPM{
    public:
        PPM();
        PPM(const PPM& other);
        PPM(PPM&& other);
        PPM(std::ifstream& in);
        ~PPM();

        std::string getComment() const;
        std::string getMagic() const;
        unsigned int GetHeight() const;
        unsigned int GetWidth() const;
        unsigned int GetSize() const;
        unsigned int GetMaxColor() const;

        void SetMetaData(const PPM& other);
        
        void Resize(unsigned int factor);
        void SaveImageToFile(std::string fileName) const;

        void SetComment(std::string newComment);
        void SetMagic(std::string newMagic);
        void SetHeight(unsigned int newHeight);
        void SetWidth(unsigned int newWidth);
        void SetMaxColor(unsigned int newMaxColor);

        const PPM& operator=(const PPM& other);
        const PPM& operator=(PPM&& other);

        Pixel& operator[](unsigned int index);
        const Pixel& operator[](unsigned int index) const;

        friend std::ostream& operator<<(std::ostream& out, const PPM& pixel);
        friend std::istream& operator>>(std::istream& in, PPM& pixel);

        std::vector<Pixel>::iterator begin();
        std::vector<Pixel>::iterator end();
        std::vector<Pixel>::const_iterator cbegin();
        std::vector<Pixel>::const_iterator cend();

    private:
        std::vector<Pixel> pixels;
        std::string magic;
        std::string comment;
        unsigned int width, height;
        unsigned int maxColor;
};

#endif
