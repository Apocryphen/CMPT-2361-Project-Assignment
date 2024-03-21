#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <iostream> // For friend decls
                    
class Pixel {
    public:
        Pixel() noexcept;
        Pixel(const Pixel& other) = default;
        Pixel(Pixel&& other)      = default;
        Pixel(unsigned int intensity) noexcept;
        Pixel(unsigned int r, unsigned int g, unsigned int b) noexcept;
        ~Pixel() noexcept;

        unsigned int& operator[](const char* color);
        const unsigned int& operator[](const char* color) const;

        Pixel& operator=(const Pixel& other) = default;
        Pixel& operator=(Pixel&& other)      = default;

        friend std::ostream& operator<<(std::ostream& out, const Pixel& pixel);
        friend std::istream& operator>>(std::istream& in, Pixel& pixel);

        class InputOutOfBoundsException{
            public:
                InputOutOfBoundsException(const char* errorMsg, const char* offendingIdx);
                const char* returnError() const;
                const char* returnOffendingIndex() const;
            private:
                const char* errorMessage;
                const char* offendingIndex;
        };
    private:
        unsigned int red, green, blue;
};

#endif
