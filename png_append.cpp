#include "busybox.hpp"
#include "graphics.hpp"
#include "lib-png.hpp"
#include "files.hpp"
#include "filepath.hpp"
#include <iostream>

int png_append_h_main(int argc,char **argv){
  std::vector<rcl::image32> images_in;
  rcl::image32 image_out;
  for (int idx=1;idx<argc-1;idx++) {
    if (rcl::check_file(argv[idx])){
      std::vector<unsigned char> buffer = rcl::load_file(argv[idx]);
      rcl::image32 img;
       rcl::decode_png(img,buffer);
      std::cout << "Loaded : " << argv[idx] << " - " << img.GetX() << "×" << img.GetY() << std::endl;
      images_in.push_back(img);
      }
      else{
        std::cout << "Error loading " << argv[idx] << std::endl;
        }
    }
  int x=0,y=1,c=0;
  for (int idx=1;idx<argc-1;idx++) if (images_in[idx-1].GetY()>y) y=images_in[idx-1].GetY();
  for (int idx=1;idx<argc-1;idx++) x+=images_in[idx-1].GetX();
  image_out.fit(x,y);
  std::cout << " Result : " << x << "×" << y << " - " << std::flush;
  for (int idx=1;idx<argc-1;idx++) {image_out.put(images_in[idx-1],rcl::upoint(c,0)); c+=images_in[idx-1].GetX();}
  std::vector<unsigned char> outbuffer;
  rcl::encode_png(outbuffer,image_out);
  rcl::save_file(argv[argc-1],outbuffer);
  std::cout << "Saved : " << argv[argc-1] << std::endl;
  return 0;
}

static rcl::reg_app reg1("png_append_h",png_append_h_main);


int png_append_v_main(int argc,char **argv){
  std::vector<rcl::image32> images_in;
  rcl::image32 image_out;
  for (int idx=1;idx<argc-1;idx++) {
    if (rcl::check_file(argv[idx])){
      std::vector<unsigned char> buffer;
      rcl::load_file(buffer,argv[idx]);
      rcl::image32 img;
      rcl::decode_png(img,buffer);
      std::cout << "Loaded : " << argv[idx] << " - " << img.GetX() << "×" << img.GetY() << std::endl;
      images_in.push_back(img);
      }
      else{
        std::cout << "Error loading " << argv[idx] << std::endl;
        }
    }
  int x=1,y=0,c=0;
  for (int idx=1;idx<argc-1;idx++) if (images_in[idx-1].GetX()>x) x=images_in[idx-1].GetX();
  for (int idx=1;idx<argc-1;idx++) y+=images_in[idx-1].GetY();
  image_out.fit(x,y);
  std::cout << " Result : " << x << "×" << y << " - " << std::flush;
  for (int idx=1;idx<argc-1;idx++) {image_out.put(images_in[idx-1],rcl::upoint(0,c)); c+=images_in[idx-1].GetY();}
  std::vector<unsigned char> outbuffer;
  rcl::encode_png(outbuffer,image_out);
  rcl::save_file(argv[argc-1],outbuffer);
  std::cout << "Saved : " << argv[argc-1] << std::endl;
  return 0;
}

static rcl::reg_app reg2("png_append_v",png_append_v_main);
