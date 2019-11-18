#include <iostream>
#include <string>
#include <vector>
#include "tpoint.hpp"

std::ostream& operator << (std::ostream& os,const rcl::spoint& p){
  os << "( " << p.x() << " , " << p.y() << " )";
  return os;
  }

int main(){
  std::vector<rcl::spoint> data;
  
  data.push_back(rcl::spoint("Leon","Salamanca"));
  data.push_back(rcl::spoint("Leon","Zamora"));
  data.push_back(rcl::spoint("Zamora","Salamanca"));
  data.push_back(rcl::spoint("Zamora","Leon"));
  data.push_back(rcl::spoint("Salamanca","Leon"));
  data.push_back(rcl::spoint("Salamanca","Zamora"));
  
  for (unsigned int idx=0;idx<6;idx++) std::cout << idx << " -- " << data[idx] << std::endl;
  
  for (unsigned int idy=0;idy<6;idy++){
    for (unsigned int idx=0;idx<6;idx++){
      if (data[idx]<data[idy])  std::cout << "[+TRUE] ";
      else                      std::cout << "[FALSE] ";
      std::cout << data[idx] << " < " << data[idy] << std::endl;
      }
    }
  
  for (unsigned int idy=0;idy<6;idy++){
    for (unsigned int idx=0;idx<6;idx++){
      if (data[idx]==data[idy]) std::cout << "[+TRUE] ";
      else                      std::cout << "[FALSE] ";
      std::cout << data[idx] << " = " << data[idy] << std::endl;
      }
    }
  
  return 0;
}
