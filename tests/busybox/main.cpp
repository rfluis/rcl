#include "busybox.hpp"

int main(int argc,char **argv){
  std::cout << " --- Main Function: --- " << std::endl;
  std::vector<std::string> list = regApps.app_list();
  for (unsigned int idx=0;idx<list.size();idx++)
    std::cout << idx << " : " << list[idx] << std::endl;
  return 0;
}
