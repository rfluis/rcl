#ifndef NEWDISPLAY_HEADER
#define NEWDISPLAY_HEADER

#include <vector>
#include <string>
#include <algorithm>
#include "graphics.hpp"
#include "fonts.hpp"
#include "tfield.hpp"
#include "constants.hpp"

namespace rcl{
  
rcl::image32 display_field(const rcl::ffield& data,const unsigned int& layer,std::string format);

image32 basic_plot(const rcl::upoint& size,const std::vector<rcl::fpoint>& list,const rcl::pixel32& color);
}
#endif
