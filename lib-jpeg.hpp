#ifndef LIB_JPEG_HEADER
#define LIB_JPEG_HEADER

#include "vstream.hpp"
#include "graphics.hpp"
#include <jpeglib.h>

namespace rcl{
  int decode_jpg(rcl::image32& dst,const std::vector<unsigned char>& src);
  int encode_jpg(std::vector<unsigned char>& dst,const rcl::image32& src);
  
}

#endif
