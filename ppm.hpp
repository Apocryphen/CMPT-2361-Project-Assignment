#ifndef PPM_HPP
#define PPM_HPP

#include <iostream> 
#include <vector>
#include <string>

#include "pixel.hpp"

class PPM{
    public:
        PPM() noexcept;
        PPM(const PPM& other) noexcept;
        PPM(PPM&& other) noexcept;
        PPM(std::ifstream& in); //throws a runtime error on a bad parse 
        ~PPM() noexcept;

        std::string getComment() const noexcept;
        std::string getMagic() const noexcept;
        unsigned int GetHeight() const noexcept;
        unsigned int GetWidth() const noexcept;
        unsigned int GetSize() const noexcept;
        unsigned int GetMaxColor() const noexcept;

        void SetMetaData(const PPM& other);
        
        void Resize(unsigned int factor);
        void Reserve(unsigned int factor);

        void SaveImageToFile(std::string fileName) const;

        void SetComment(std::string newComment);
        void SetMagic(std::string newMagic); 
        void SetHeight(unsigned int newHeight) noexcept;
        void SetWidth(unsigned int newWidth) noexcept;
        void SetMaxColor(unsigned int newMaxColor) noexcept;

        const PPM& operator=(const PPM& other);
        const PPM& operator=(PPM&& other) noexcept;

        Pixel& operator[](unsigned int index);
        const Pixel& operator[](unsigned int index) const;

        friend std::ostream& operator<<(std::ostream& out, const PPM& pixel);
        friend std::istream& operator>>(std::istream& in, PPM& pixel);

        std::vector<Pixel>::iterator begin() noexcept;
        std::vector<Pixel>::iterator end() noexcept;
        std::vector<Pixel>::const_iterator cbegin() const noexcept;
        std::vector<Pixel>::const_iterator cend() const noexcept;

    private:
        std::vector<Pixel> pixels;
        std::string magic;
        std::string comment;
        unsigned int width, height;
        unsigned int maxColor;
};

#endif
