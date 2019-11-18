#include <iostream>
#include <string>
#include <vector>
#include "tpoint.hpp"

std::ostream& operator << (std::ostream& os,const rcl::spoint& p){
  os << "( " << p.x() << " , " << p.y() << " )";
  return os;
  }

std::ostream& operator << (std::ostream& os,const rcl::fpoint& p){
  os << "( " << p.x() << " , " << p.y() << " )";
  return os;
  }

int main(){
  
  for (float f=0;f<100;f++) std::cout << rcl::fpoint(f,100.-f).normalized() << std::endl;
  
  return 0;
}
