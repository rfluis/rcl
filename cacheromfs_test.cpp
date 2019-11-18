#include "busybox.hpp"
#include "cacheromfs.hpp"
#include <iostream>
#include <sstream>

int cacheromfs_test_main(int argc,char **argv){
  std::map<std::string,rcl::upair> table;
  rcl::bytevector data;
  std::map<std::string,rcl::upair> rectable;
  
  for (unsigned int idx=0;idx<4;idx++)
  for (unsigned int idy=0;idy<4;idy++){
    std::stringstream ss;
    ss << "(" << idx << "," << idy << ")";
    table.insert(std::pair<std::string,rcl::upair>(ss.str(),rcl::upair(idx,idy)));
    }
  
  for (unsigned int idx=0;idx<80;idx++) std::cout << "=";
  std::cout << std::endl;
  
  for (auto itr=table.begin();itr!=table.end();itr++)
  std::cout << "> " << itr->second.first << " : " << itr->second.second << " -> " << itr->first << std::endl;
  
  for (unsigned int idx=0;idx<80;idx++) std::cout << "=";
  std::cout << std::endl;
  
  rcl::dump(data,table);
  rcl::dump(rectable,data);
  
  for (auto itr=rectable.begin();itr!=rectable.end();itr++)
  std::cout << "> " << itr->second.first << " : " << itr->second.second << " -> " << itr->first << std::endl;
  
  for (unsigned int idx=0;idx<80;idx++) std::cout << "=";
  std::cout << std::endl;
  
  return 0;
}

static rcl::reg_app reg("cacheromfs_test",cacheromfs_test_main);
