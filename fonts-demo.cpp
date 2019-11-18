#include <iostream>
#include "busybox.hpp"
#include "fonts.hpp"
#include "lib-png.hpp"
#include "files.hpp"

int fonts_demo_main(int argc,char **argv){
  std::string outfile = "fontdemo.png";
  std::string text = "Ricardo Francisco Luis Martinez";
  if (argc>1) text = argv[1];
  if (argc>2) outfile = argv[2];
  rcl::font fnt = rcl::loadfont(0);
  rcl::image32 imagen = fnt.print(text,rcl::pixel32(255,0,0));
  std::vector<unsigned char> png;
  rcl::encode_png(png,imagen);
  rcl::save_file(outfile,png);

  return 0;
  }

static rcl::reg_app reg("fonts_demo",fonts_demo_main);
