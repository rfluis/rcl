#include "busybox.hpp"
#include <iostream>


static int app_main(int argc,char **argv){
  std::cout << "Hello World !!" << std::endl;
  return 0;
  }
static rcl::reg_app reg1("app_hello_world",app_main);
