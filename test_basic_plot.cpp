#include "busybox.hpp"
#include "newdisplay.hpp"
#include "lib-png.hpp"
#include "files.hpp"

int basic_plot_test_main(){
  std::vector<rcl::fpoint> fun;
  for (float x=10;x<20;x+=0.01) fun.push_back(rcl::fpoint(x,std::sin(3*x)*x));
  rcl::image32 img = basic_plot(rcl::upoint(800,600),fun,rcl::pixel32(0,0,0xff));
  img.line(0,0,799,599,rcl::pixel32(255,0,0));
  img.line(799,0,0,599,rcl::pixel32(255,0,0));
  std::vector<unsigned char> data;
  rcl::encode_png(data,img);
  rcl::save_file("plot.png",data);
  return 0;
}

static rcl::reg_app reg("basic_plot_test",basic_plot_test_main);
