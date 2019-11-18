#ifndef LIB_PNG_HEADER
#define LIB_PNG_HEADER

#include "vstream.hpp"
#include "graphics.hpp"
#include <zlib.h>
#include <png.h>

namespace rcl{
int decode_png(rcl::image32& dst,const std::vector<unsigned char>& src);
int encode_png(std::vector<unsigned char>& dst,const rcl::image32& src);

std::vector<unsigned char> encode_png(const rcl::image32& src);
rcl::image32 decode_png(const std::vector<unsigned char>& src);
}

#endif
