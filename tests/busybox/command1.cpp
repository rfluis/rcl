#include "busybox.hpp"

int command1(int argc,char **argv){
  // Such main-like function doesn't make any sense
  // Just to know if previous infrastructure works
  return argc*argc;
  }
  
reg_app cmd1("command1",command1);
