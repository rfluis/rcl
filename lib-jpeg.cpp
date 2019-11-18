#include "lib-jpeg.hpp"

namespace rcl{
  static void JpgWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
    rcl::vstream *destp = reinterpret_cast<rcl::vstream*>(png_get_io_ptr(png_ptr));
    destp->swrite(data,length);
    return;
}
static void JpgReadCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
    rcl::cvstream *srcp = reinterpret_cast<rcl::cvstream*>(png_get_io_ptr(png_ptr));
    srcp->sread(data,length);
    return;
}

int decode_jpg(rcl::image32& dst,const std::vector<unsigned char>& src){
  int rc, i, j;
  
	unsigned long jpg_size;
	unsigned char *jpg_buffer;
  unsigned long bmp_size;
	unsigned char *bmp_buffer;
	int row_stride, width, height, pixel_size;
  struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
  
  cinfo.err = jpeg_std_error(&jerr);	
	jpeg_create_decompress(&cinfo);
  jpeg_mem_src(&cinfo, jpg_buffer, jpg_size);
  rc = jpeg_read_header(&cinfo, TRUE);
  if (rc != 1) return 1;
  jpeg_start_decompress(&cinfo);
  width = cinfo.output_width;
	height = cinfo.output_height;
	pixel_size = cinfo.output_components;
  bmp_size = width * height * pixel_size;
	bmp_buffer = (unsigned char*) malloc(bmp_size);
  while (cinfo.output_scanline < cinfo.output_height) {
		unsigned char *buffer_array[1];
		buffer_array[0] = bmp_buffer + (cinfo.output_scanline) * row_stride;
		jpeg_read_scanlines(&cinfo, buffer_array, 1);
	}
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  
  return 0;
}
int encode_jpg(std::vector<unsigned char>& dst,const rcl::image32& src){
  
  }
}
