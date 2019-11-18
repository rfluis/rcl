#include "lib-png.hpp"

namespace rcl{
static void PngWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
    rcl::vstream *destp = reinterpret_cast<rcl::vstream*>(png_get_io_ptr(png_ptr));
    destp->swrite(data,length);
    return;
}
static void PngReadCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
    rcl::cvstream *srcp = reinterpret_cast<rcl::cvstream*>(png_get_io_ptr(png_ptr));
    srcp->sread(data,length);
    return;
}

int decode_png(rcl::image32& dst,const std::vector<unsigned char>& src){
  rcl::cvstream in(src);
  png_byte color_type;
  png_byte bit_depth;
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info = png_create_info_struct(png);
  setjmp(png_jmpbuf(png));
  png_set_read_fn(png, &in, PngReadCallback);
  png_read_info(png, info);
  dst.fit(png_get_image_width(png, info),png_get_image_height(png, info));
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);
  if(bit_depth == 16) png_set_strip_16(png);
  if(color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
  if(png_get_valid(png, info, PNG_INFO_tRNS))  png_set_tRNS_to_alpha(png);
  if(color_type == PNG_COLOR_TYPE_RGB ||color_type == PNG_COLOR_TYPE_GRAY ||color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
  if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png);
  png_read_update_info(png, info);
  std::vector<unsigned char*> rows(dst.GetY());
  for (size_t y = 0; y < dst.GetY(); ++y) rows[y] = dst.getptr() + y*dst.GetX()*4;
  png_read_image(png,&rows[0]);
  png_destroy_read_struct(&png, &info,(png_infopp)NULL);
  return 0;
  }
int encode_png(std::vector<unsigned char>& dst,const rcl::image32& src){
  rcl::vstream out(dst);
  if (src.GetX()==0){
    return 1;
    }
  if (src.GetY()==0){
    return 1;
    }
  png_structp p = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_infop info_ptr = png_create_info_struct(p);
  setjmp(png_jmpbuf(p));
  png_set_IHDR(p, info_ptr, src.GetX(), src.GetY(), 8,PNG_COLOR_TYPE_RGBA,
          PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
  png_set_compression_level(p, 9);
  std::vector<const unsigned char*> rows(src.GetY());
  for (size_t y = 0; y < src.GetY(); ++y) rows[y] = src.getptr() + y*src.GetX()*4;
  png_set_rows(p, info_ptr,(unsigned char**) &rows[0]);
  png_set_write_fn(p,&out, PngWriteCallback, NULL);
  png_write_png(p, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  if (p) png_destroy_write_struct(&p, NULL);
  return 0;
  }

std::vector<unsigned char> encode_png(const rcl::image32& src){
  std::vector<unsigned char> ans;
  encode_png(ans,src);
  return ans;
  }
rcl::image32 decode_png(const std::vector<unsigned char>& src){
  rcl::image32 ans;
  decode_png(ans,src);
  return ans;
  }
  
}


