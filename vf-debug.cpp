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

int vf_debug_main(int argc,char **argv){
  if (argc==1){
    std::cout << "give vf file" << std::endl;
    return 1;
    }
  std::vector<std::string> args(argv,argv+argc);
  for (int idx=1;idx<argc;idx++) {
    rcl::stringmap dst;
    std::cout << "Loading file " << args[idx] << std::endl;
    rcl::load_vf_meta(dst,args[idx]);
    for (auto itr=dst.begin();itr!=dst.end();itr++){
      std::cout << "# " << itr->first << ": " << itr->second << std::endl; 
      }
    std::cout << " ------------------------------------------------------- " << std::endl;
  }
  return 0;
}
static rcl::reg_app reg("vf_debug",vf_debug_main);
