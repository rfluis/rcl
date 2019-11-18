#include "skyrmion-models.hpp"
#include "newdisplay.hpp"

int main(){
  rcl::Flat_Neel_Skyrmion<float> skyrmion(rcl::fpoint(256e-9,256e-9),rcl::fpoint(100e-9,50e-9));
  std::vector<unsigned char> dst;
  skyrmion.getnodes() = rcl::utriad(512,512,1);
  skyrmion.getcells() = 1e-9;
  rcl::ffield data = skyrmion.render_field();
  rcl::image32 img = rcl::display_field(data,0,"urainbow");
  rcl::encode_png(dst,img);
  rcl::save_file("skyrmion.png",dst);
  
  return 0;
}
