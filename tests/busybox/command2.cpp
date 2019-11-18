#include "busybox.hpp"

int command2(int argc,char **argv){
  for (unsigned int idx=0;idx<argc;idx++)
  std::cout << "COMMAND2 - Argument " << idx << " is " << argv[idx] << std::endl;
  return 33;
  }
  
reg_app cmd2("command2",command2);
