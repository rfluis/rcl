#ifndef COMSOL_HEADER
#define COMSOL_HEADER
#include "tfield.hpp"
#include "files.hpp"
#include "lib-zlib.hpp"
#include "text.hpp"
#include "graphics.hpp"
#include "constants.hpp"
#include "oommf.hpp"

namespace rcl{
bool is_COMSOL(const std::vector<unsigned char>& file);
void load_COMSOL(rcl::ffield& dst,const std::vector<unsigned char>& src);
void current_field_z(rcl::ffield& dst,const rcl::ffield& src);
void current_regions(rcl::fsfield& dst,const rcl::ffield& src);
void current_mask(rcl::image32& dst,const rcl::ffield& src);
}
#endif
