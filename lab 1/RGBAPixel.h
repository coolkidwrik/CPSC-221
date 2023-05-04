#ifndef CS221UTIL_RGBAPIXLE_H
#define CS221UTIL_RGBAPIXLE_H

#include <string>
#include <vector>

using namespace std;

namespace cs221util {
  class RGBAPixel {
  public:
    /**
      * default constructor: creates a white pixle with full opacity
      */
     RGBAPixel();

    /**
      * Creates a pixle with specified colors and full opacity
      * @param r redness of pixle.
      * @param g greeness of pixle.
      * @param b blueness of pixle
      */
    RGBAPixel(int r, int g, int b);

     /**
      * Creates a pixle with specified colors and specified opacity
      * @param r redness of pixle.
      * @param g greeness of pixle.
      * @param b blueness of pixle
      */
    RGBAPixel(int r, int g, int b, double a);  


    unsigned char r; //red, between [0,255]
    unsigned char g; //green, between [0,255]
    unsigned char b; //blue, between [0,255]
    double a; //transparency, between [0,1]
 };
}

#endif
