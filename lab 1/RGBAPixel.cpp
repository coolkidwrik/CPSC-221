#include <iostream>
#include <string>
#include <algorithm>
#include "lodepng/lodepng.h"
#include "PNG.h"
#include "RGBAPixel.h"

namespace cs221util {

    RGBAPixel::RGBAPixel() : r(255), g(255), b(255), a(1.0) {}

    RGBAPixel::RGBAPixel(int r, int g, int b) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 1;
    }

    RGBAPixel::RGBAPixel(int r, int g, int b, double a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
}