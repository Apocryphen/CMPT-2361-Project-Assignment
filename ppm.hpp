#ifndef PPM_HPP
#define PPM_HPP

#include <iostream> // For friend decls

class Pixel {
    public:
        Pixel();
        Pixel(const Pixel& other);
        Pixel(Pixel&& other);
        Pixel(unsigned int r, unsigned int g, unsigned int b);
        ~Pixel();

        const unsigned int& operator[](int index) const;
        const unsigned int& operator[](const char* colour) const;

        Pixel& operator=(const Pixel& other);
        Pixel& operator=(Pixel&& other);

        friend std::ostream& operator<<(std::ostream& out, const Pixel& pixel);
        friend std::istream& operator>>(std::istream& in, Pixel& pixel);

    private:
        unsigned int red, green, blue;
};

class PPMImage {
    public:
        PPMImage();
        PPMImage(const PPMImage& other);
        PPMImage(PPMImage&& other);
        ~PPMImage();

        PPMImage& operator=(const PPMImage& other);
        PPMImage& operator=(PPMImage&& other);

        friend std::ostream& operator<<(std::ostream& out, const PPMImage& pixel);
        friend std::istream& operator>>(std::istream& in, PPMImage& pixel);

        friend void swap(PPMImage& a, PPMImage& b);

        std::vector<Pixel>::iterator begin();
        std::vector<Pixel>::iterator end();
        std::vector<Pixel>::const_iterator cbegin();
        std::vector<Pixel>::const_iterator cend();

    private:
        std::vector<Pixel> pixels;
        std::string format;
        unsigned int width, height;
        unsigned int channelDepth;
};

#endif
