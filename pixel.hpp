#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <iostream> // For friend decls
                    
class Pixel {
    public:
        Pixel();
        Pixel(const Pixel& other);
        Pixel(Pixel&& other);
        Pixel(unsigned int r, unsigned int g, unsigned int b);
        ~Pixel();

        unsigned int& operator[](const char* color);
        const unsigned int& operator[](int index) const;
        const unsigned int& operator[](const char* color) const;

        Pixel& operator=(const Pixel& other);
        Pixel& operator=(Pixel&& other);

        friend std::ostream& operator<<(std::ostream& out, const Pixel& pixel);
        friend std::istream& operator>>(std::istream& in, Pixel& pixel);

    private:
        unsigned int red, green, blue;
        class InputOutOfBoundsException{
            public:
                InputOutOfBoundsException(const char* errorMsg, const char* offendingIdx);
                const char* returnError() const;
                const char* returnOffendingIndex() const;
            private:
                const char* errorMessage;
                const char* offendingIndex;
        };
};

#endif
