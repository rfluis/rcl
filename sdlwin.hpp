#ifndef SDLWIN_HEADER
#define SDLWIN_HEADER

#include "graphics.hpp"
#include <iostream>
#include <string>
#include <SDL/SDL.h>

namespace rcl{

class sdl_window{
private:
  SDL_Surface* scrp;
  unsigned int w,h;
  std::string caption;
  bool refresh;
public:
  sdl_window(){
    //std::cout << "SDL Window declared" << std::endl; 
    scrp=NULL; 
    w=0; 
    h=0; 
    refresh=false;
    return;
    }
  bool init(const unsigned int& x,const unsigned int& y){
    //std::cout << "SDL Have:"<< w << "x" << h << " called with " << x << "x" << y << std::endl;
    if (w!=x) quit();
    if (h!=y) quit();
    if (w==x) if (h==y) return true;
     w=x;
     h=y; 
    return init();
    }
  bool init(){ 
    quit();
    if(SDL_Init(SDL_INIT_VIDEO) < 0){std::cout << "error, SDL video init failed" << std::endl; return true;}
    scrp = SDL_SetVideoMode(w,h, 32, SDL_HWSURFACE);
    if(!scrp) { std::cout << "error, no SDL screen" << std::endl; return true; }
    refresh=false;
    return false;
    }
  void quit(){if (scrp!=NULL) {SDL_Quit(); scrp=NULL;} return;}
  void set_caption(const std::string& c) { caption = c; return;}
  void update_caption(const std::string& c) {caption=c; update_caption(); return;}
  void update_caption(){SDL_WM_SetCaption(caption.c_str(), NULL); return;}
  void copy_window(const rcl::image32& img);
  void update_win(){
    if (!refresh) {
      /*std::cout << "Refreshed window" << std::endl;*/ 
      SDL_UpdateRect(scrp,0,0,0,0);
      }
    refresh=true;
    return;
    }
  ~sdl_window(){quit();}
  SDL_Surface*& get_surface(){return scrp;}
  SDL_Surface *operator->() { return scrp; }
};

}
#endif
