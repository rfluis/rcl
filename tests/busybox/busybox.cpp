#define REGAPPS_DECLARE
#include "busybox.hpp"

app_container regApps;

int app_container::callback(int argc,char** argv){
  std::cout << "app container asked for a non-registered function" << std::endl;
  return 0;
  }
