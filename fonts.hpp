#ifndef FONTS_HEADER
#define FONTS_HEADER

#include "bitstream.hpp"
#include "graphics.hpp"
#include "lib-zlib.hpp"

namespace rcl{

class font{
private:
  unsigned int cx,cy;
  rcl::bitstream data;
public:
  font(const std::vector<unsigned char>& f,const unsigned int& w){
    data.setvec(f);
    cx=w;
    cy=data.size()/(32*cx);
    return;
  }
  rcl::image32 print (const std::string& text,const rcl::pixel32& c) const;
};

rcl::font loadfont(const unsigned int& x);
}
#endif
