#include <iostream>
#include "busybox.hpp"
#include "oommf.hpp"
#include "files.hpp"
#include "lib-zlib.hpp"
#include "text.hpp"
#include "vf.hpp"
#include "filepath.hpp"
#include "lib-png.hpp"
#include "newdisplay.hpp"
#include "comsol.hpp"
#include "tfield_io.hpp"


int ovf_diff_tool_main(int argc,char **argv){
std::vector<std::string> args(argv,argv+argc);
  if (argc==4){
    rcl::ffield ovf1,ovf2;
    rcl::load_vecfile(ovf1,args[1]);
    rcl::load_vecfile(ovf2,args[2]);
    std::vector<unsigned char> ovfb;
    rcl::saveOVF2vm(ovfb,ovf1-ovf2);
    rcl::save_file(args[3],ovfb);
  }
  else{
    std::cout << argv[0] << "fichero1.ovf fichero2.ovf ficheros.ovf"<< std::endl;
    return 1;
  }
  return 0;
}
static rcl::reg_app reg("ovf_diff_tool",ovf_diff_tool_main);
