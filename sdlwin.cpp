#include "sdlwin.hpp"

namespace rcl{
void sdl_window::copy_window(const rcl::image32& img){
    if (img.GetX()!=w) return;
    if (img.GetY()!=h) return;
    for(unsigned y=0;y<h;y++) for(unsigned x=0;x<w;x++){
    unsigned int r = static_cast<unsigned int>(img(x,y).r()); //red
    unsigned int g = static_cast<unsigned int>(img(x,y).g()); //green
    unsigned int b = static_cast<unsigned int>(img(x,y).b()); //blue
    unsigned int a = static_cast<unsigned int>(img(x,y).a()); //alpha

    unsigned int checkerColor = 191 + 64 * (((x / 16) % 2) == ((y / 16) % 2));
    r = (a * r + (255 - a) * checkerColor) / 255;
    g = (a * g + (255 - a) * checkerColor) / 255;
    b = (a * b + (255 - a) * checkerColor) / 255;

    Uint32* bufp;
    bufp = (Uint32 *)scrp->pixels + (y * scrp->pitch / 4) + x;
    *bufp = 65536 * r + 256 * g + b;
    }
    refresh=false;
    update_win();
    /*std::cout << "Copied a bitmap of : " << img.GetX() << "x" << img.GetY() << std::endl;*/
  }
}
