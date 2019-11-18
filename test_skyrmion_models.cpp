#include "busybox.hpp"
#include "skyrmion_models.hpp"
#include "newdisplay.hpp"
#include "lib-png.hpp"
#include "files.hpp"

int test_skyrmion_models_main(int argc,char **argv){
  rcl::Flat_Neel_Skyrmion<float> skyrmion;
  std::vector<unsigned char> dst;
  skyrmion.getpos() = rcl::fpoint(256e-9,256e-9);
  skyrmion.getrdw() = rcl::fpoint(100e-9,50e-9);
  skyrmion.getnodes() = rcl::utriad(512,512,1);
  skyrmion.getcells() = rcl::ftriad(1e-9);
  rcl::ffield data = skyrmion.render_field();
  rcl::image32 img = rcl::display_field(data,0,"urainbow");
  rcl::encode_png(dst,img);
  rcl::save_file("skyrmion.png",dst);
  
  return 0;
}

static rcl::reg_app reg("test_skyrmion_models",test_skyrmion_models_main);
