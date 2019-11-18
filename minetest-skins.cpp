#include "busybox.hpp"
#include "graphics.hpp"
#include "lib-png.hpp"
#include "files.hpp"
#include "filepath.hpp"
#include <iostream>


int minetest_skin_tool_main(int argc,char **argv){
  std::string infile = argv[1];
  std::vector<unsigned char> data;
  rcl::load_file(data,infile);
  rcl::image32 spritesheet;
  rcl::decode_png(spritesheet,data);
  rcl::upoint skinsize(spritesheet.GetX(),spritesheet.GetY()); // x=32px y=64px x=8u y=16u
  rcl::upoint unitsize = skinsize/rcl::upoint(16,8);
  std::cout << "Tamaño de la skin:" << skinsize.x() <<"×"<< skinsize.y() << std::endl;
  std::cout << "Tamaño del macropixel:" << unitsize.x() <<"×"<< unitsize.y() << std::endl;
  if (skinsize!=(unitsize&rcl::upoint(16,8))){
    std::cout << "El tamaño de la skin no es multiplo de 8×16" << std::endl;
    return 1;
  }
  // crop pos,size
  rcl::image32 head_front = spritesheet.crop(rcl::upoint(2,2)&unitsize,rcl::upoint(2,2)&unitsize);
  rcl::image32 head_back = spritesheet.crop(rcl::upoint(6,2)&unitsize,rcl::upoint(2,2)&unitsize);
  rcl::image32 hat_front = spritesheet.crop(rcl::upoint(10,2)&unitsize,rcl::upoint(2,2)&unitsize);
  rcl::image32 hat_back = spritesheet.crop(rcl::upoint(14,2)&unitsize,rcl::upoint(2,2)&unitsize);
  
  //head_front.blend(hat_front,0);
  //head_back.blend(hat_back,0);
  
  rcl::image32 leg_front = spritesheet.crop(rcl::upoint(1,5)&unitsize,rcl::upoint(1,3)&unitsize);
  rcl::image32 leg_back = spritesheet.crop(rcl::upoint(3,5)&unitsize,rcl::upoint(1,3)&unitsize);
  
  rcl::image32 body_front = spritesheet.crop(rcl::upoint(5,5)&unitsize,rcl::upoint(2,3)&unitsize);
  rcl::image32 body_back = spritesheet.crop(rcl::upoint(7,5)&unitsize,rcl::upoint(2,3)&unitsize);
  
  rcl::image32 arm_front = spritesheet.crop(rcl::upoint(11,5)&unitsize,rcl::upoint(1,3)&unitsize);
  rcl::image32 arm_back = spritesheet.crop(rcl::upoint(13,5)&unitsize,rcl::upoint(1,3)&unitsize);
  
  rcl::image32 preview_front(rcl::upoint(4,8)&unitsize);
  rcl::image32 preview_back(rcl::upoint(4,8)&unitsize);
  
  preview_front.blend(head_front,rcl::upoint(1,0)&unitsize);
  preview_front.blend(hat_front,rcl::upoint(1,0)&unitsize);
  preview_front.blend(body_front,rcl::upoint(1,2)&unitsize);
  preview_front.blend(leg_front,rcl::upoint(1,5)&unitsize);
  preview_front.blend(leg_front.mirror(),rcl::upoint(2,5)&unitsize);
  preview_front.blend(arm_front.mirror(),rcl::upoint(3,2)&unitsize);
  preview_front.blend(arm_front,rcl::upoint(0,2)&unitsize);
  
  preview_back.blend(head_back,rcl::upoint(1,0)&unitsize);
  preview_back.blend(hat_back,rcl::upoint(1,0)&unitsize);
  preview_back.blend(body_back,rcl::upoint(1,2)&unitsize);
  preview_back.blend(leg_back.mirror(),rcl::upoint(1,5)&unitsize);
  preview_back.blend(leg_back,rcl::upoint(2,5)&unitsize);
  preview_back.blend(arm_back,rcl::upoint(3,2)&unitsize);
  preview_back.blend(arm_back.mirror(),rcl::upoint(0,2)&unitsize);
  
  data.clear();
  rcl::FilePath fp(infile);
  rcl::encode_png(data,preview_front);
  rcl::save_file(fp.GetRoot()+"_preview.png",data);
  data.clear();
  rcl::encode_png(data,preview_back);
  rcl::save_file(fp.GetRoot()+"_preview_back.png",data);
  return 0;
}

static rcl::reg_app reg("minetest_skin_tool",minetest_skin_tool_main);
