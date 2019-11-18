#include "busybox.hpp"
#include "vf.hpp"
#include <iostream>
#include "files.hpp"

int vf_check_main(int argc,char **argv){
  std::string filename(argv[1]);
  std::vector<std::string> lista;
  rcl::load_file(lista,filename);
  for (int idx=0;idx<lista.size();idx++){
    if (rcl::is_vf_signed(lista[idx])) std::cout << lista[idx] << std::endl;
    }
  return 0;
}
static rcl::reg_app reg("vf_check_tool",vf_check_main);

