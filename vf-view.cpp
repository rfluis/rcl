#include "busybox.hpp"
#include <iostream>
#include <iomanip>
#include "oommf.hpp"
#include "files.hpp"
#include "lib-zlib.hpp"
#include "text.hpp"
#include "vf.hpp"
#include "filepath.hpp"
#include "lib-png.hpp"
#include "newdisplay.hpp"
#include "tfield_io.hpp"
#include "sdlwin.hpp"

class viewer_state{
private:
  std::vector<std::string> list;
  unsigned int idx;
  unsigned int layer;
  unsigned int valuedim;
  bool osd=true;
  bool finish=false;
  bool cacheok=false;
  bool plotok=false;
  bool flag_normpeak=true;
  std::string mode;
  std::string type;
  rcl::ffield cache;
  rcl::image32 plot;
  rcl::sdl_window surface;
public:
  viewer_state(){mode="rainbow"; idx=0; valuedim = 3; return;}
  void add_file(const std::string& name){list.push_back(name); return;}
  const std::string& getcurrent() const {return list[idx];}
  void inc()        {if (idx<list.size()-1){idx++; cacheok=false; plotok=false;}  return;}
  void dec()        {if (idx>0)            {idx--; cacheok=false; plotok=false;}  return;}
  void layer_inc()  { if (layer<cache.getnodes().z()-1) {layer++; plotok=false;} return; }
  void layer_dec()  { if (layer>0)                      {layer--; plotok=false;} return; }
  void layer_check(){ if (layer>cache.getnodes().z()-1) {layer=cache.getnodes().z()-1; plotok=false;} return;}
  void setmode(const std::string& m){mode=m; if (flag_normpeak) if (m=="nrainbow") mode="rainbow"; plotok=false; return;}
  void reset(){std::cout << "Reloading data:" << std::endl; flag_normpeak=false; cacheok=false;plotok=false;redraw(); return;}
  const unsigned int& get_valuedim()const {return valuedim;}
  bool ch_normpeak()const{return flag_normpeak;}
  void normpeak(){
    flag_normpeak = true;
    float peak = get_normpeak(cache);
    std::cout << " -- Applied Normpeak ("<< peak <<") -> ";
    cache/=peak;
    peak = get_normpeak(cache);
    std::cout << "["<< peak <<"]";
    plotok=false;
    }
  rcl::ffield& getcache() {return cache;}
  rcl::image32& getimage() {return plot;}
  rcl::pixel32& getimage(const unsigned int& x,const unsigned int& y) {return plot(x,y);}
  void update(){
    //std::cout << "Called update" << std::endl;
    if (cacheok==false) {
      //std::cout << "loading "<< getcurrent() << std::endl; 
      rcl::load_vecfile(cache,getcurrent()); 
      cacheok=true; 
      std::cout << "loaded "<< getcurrent() << " with size:";
      valuedim = cache.get_val<int>("valuedim");
      std::cout << cache.getnodes().x() << "×";
      std::cout << cache.getnodes().y() << "×";
      std::cout << cache.getnodes().z() << " with " << valuedim << " components ";
      if (flag_normpeak) normpeak();
      std::cout << std::endl; 
      } 
    return;}
  void redraw(){
    //std::cout << "Called redraw" << std::endl;
    update(); 
    layer_check();
    update_win(); 
    if (plotok==false) {plot = rcl::display_field(cache,layer,mode); plotok=true; surface.copy_window(plot);}
    return;}
  void update_win(){
    surface.init(getx(),gety());
    surface.update_caption(gen_caption());
    surface.update_win();
    return;}
  rcl::sdl_window& get_sdl(){return surface;}
  unsigned int getx(){return cache.getnodes().x();}
  unsigned int gety(){return cache.getnodes().y();}
  bool finished(){ return finish;}
  void mfinish(){ finish=true; return;}
  std::string gen_caption(){
    std::stringstream ss;
    ss << list[idx] << " - " << cache.getnodes().x() << "x" << cache.getnodes().y() << "x" << cache.getnodes().z() << " - ";
    ss << "layer:" << layer << " - " << "mode:" << mode;
    return ss.str();
    }
};

void show(viewer_state& st);

void show(viewer_state& st){
  st.redraw();
  st.update_win();
  SDL_Event event;
  bool done = 0;
  while(done == 0)  {
    while(SDL_PollEvent(&event))    {
      if(event.type == SDL_QUIT) {st.mfinish(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_ESCAPE]) {st.mfinish(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_q]) {st.mfinish(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_DOWN]) {st.dec(); st.dec(); st.dec(); st.dec(); st.dec(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_UP]) {st.inc(); st.inc(); st.inc(); st.inc(); st.inc(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_LEFT]) {st.dec(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_RIGHT]) {st.inc(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_r]) {st.reset(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_e]) {st.normpeak(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_PLUS]) {st.layer_inc(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_MINUS]){st.layer_dec(); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_1]){ st.setmode("rainbow");done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_2]){ st.setmode("prainbow");done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_3]){ st.setmode("nrainbow"); done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_4]){ st.setmode("urainbow");done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_5]){ if (st.get_valuedim()==1) st.setmode("zmono"); else st.setmode("xfrance");done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_6]){ if (st.get_valuedim()==1) st.setmode("zmono"); else st.setmode("yfrance");done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_7]){ st.setmode("zfrance");done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_8]){ if (st.get_valuedim()==1) st.setmode("zgray"); else st.setmode("xgray");done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_9]){ if (st.get_valuedim()==1) st.setmode("zgray"); else st.setmode("ygray");done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_0]){ st.setmode("zgray");done = 1;}
      else if(SDL_GetKeyState(NULL)[SDLK_o]){ done = 1;}
    }
    st.update_win();
    SDL_Delay(50); //pause 50 ms so it consumes less processing power
  }
  return;
}

int vf_view_main(int argc,char **argv){
  if (argc==1){
    std::cout << argv[0] << " file.ovf    para visualizar" << std::endl;
    std::cout << argv[0] << " file.ovf.gz para visualizar" << std::endl;
    std::cout << argv[0] << " file.vf     para visualizar" << std::endl;
    return 1;
  }
  viewer_state st;
  for (unsigned int idx=1;idx<argc;idx++) st.add_file(argv[idx]);
  //for (unsigned int idx=1;idx<argc;idx++) std::cout << "Got " << argv[idx] << std::endl;
  while (!st.finished()) show(st);

  return 0;
}
static rcl::reg_app reg("vf_view",vf_view_main);



/*avoid too large window size by downscaling large image
  unsigned jump = 1;
  if(w / 1024 >= jump) jump = w / 1024 + 1;
  if(h / 1024 >= jump) jump = h / 1024 + 1;
  SDL_Surface* scr = SDL_SetVideoMode(w / jump, h / jump, 32, SDL_HWSURFACE);
  plot the pixels of the PNG file
  for(unsigned y = 0; y + jump - 1 < h; y += jump)
  for(unsigned x = 0; x + jump - 1 < w; x += jump)
  bufp = (Uint32 *)scr->pixels + (y * scr->pitch / 4) / jump + (x / jump);
  init SDL */
   
   //set window caption
