#include "busybox.hpp"
#include "graphics.hpp"
#include "lib-png.hpp"
#include "files.hpp"
#include "filepath.hpp"
#include <iostream>
//g++ png-diff.cpp files.cpp lib-png.cpp graphics.cpp -o pngdiff -O3 -lz -lpng -s

int png_sum_tool_main(int argc,char **argv)
{
  std::string iframefile = argv[1];
  std::string oframefile = argv[2];
  std::string dframefile = argv[3];
  std::cout << dframefile << " = " << oframefile << " + " << iframefile << std::endl;
  std::vector<unsigned char> data;
  rcl::image32 ifr,ofr,dfr;
  rcl::load_file(data,iframefile);
  rcl::decode_png(ifr,data);
  data.clear();
  rcl::load_file(data,oframefile);
  rcl::decode_png(ofr,data);
  dfr.fit(ifr.GetX(),ifr.GetY());
  unsigned int size = ifr.size();
  for (unsigned int idx=0;idx<size;idx++){
    dfr[idx].r()=static_cast<unsigned char>((256+ifr[idx].r()+ofr[idx].r())&0xff);
    dfr[idx].g()=static_cast<unsigned char>((256+ifr[idx].g()+ofr[idx].g())&0xff);
    dfr[idx].b()=static_cast<unsigned char>((256+ifr[idx].b()+ofr[idx].b())&0xff);
    dfr[idx].a()=0xff;
    }
  data.clear();
  rcl::encode_png(data,dfr);
  rcl::save_file(dframefile,data);
}
