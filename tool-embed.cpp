#include "busybox.hpp"
#include "files.hpp"
#include <iostream>
#include <iomanip>

int embed_tool_main(int argc,char **argv){
  std::vector<unsigned char> data;
  rcl::load_file(data,argv[1]);
  unsigned int brline=1;
  std::cout << "unsigned char data[]={" << std::endl;
  for (unsigned int cursor=0;cursor<data.size();cursor++){
    std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(data[cursor]);
    if (cursor<data.size()-1) std::cout << ",";
    if (brline==16) std::cout << std::endl;
    if (brline==16) brline=0;
    brline++;
    }
  std::cout << "};" << std::endl;
  return 0;
}

static rcl::reg_app reg("embed_tool_tool",embed_tool_main);
