#include "tfield.hpp"
#include <iostream>
#include <string>

//g++ tfield_tests.cpp -o tfield_tests -O3 -pipe

std::ostream& operator << (std::ostream& os,const rcl::utriad& t){
  os << t.x() << "," << t.y() << "," << t.z();
  return os;
  }

int main()
{
  std::string bar("==============================================================");
  rcl::utriad size(8,8,1);
  rcl::tfield<rcl::utriad> data(size);
  rcl::tfield<rcl::utriad> ans;
  
  for (unsigned int idy=0;idy<8;idy++)
    for (unsigned int idx=0;idx<8;idx++)
      data[rcl::utriad(idx,idy,0)]=rcl::utriad(idx+1,idy+1,0);
  
  std::cout << bar << std::endl;
  std::cout << " data " << std::endl;
  std::cout << bar << std::endl;
  
  for (unsigned int idy=0;idy<8;idy++)
  {
    for (unsigned int idx=0;idx<8;idx++)  
      std::cout << data[rcl::utriad(idx,idy,0)] << "\t";
    std::cout << std::endl;
  }
  
  std::cout << bar << std::endl << std::endl<< std::endl;
  
  for (unsigned int shx=0;shx<16;shx++)  {
    std::cout << bar << std::endl;
    std::cout << " shift_x("<< shx <<") gives " << std::endl;
    std::cout << bar << std::endl;
    
    ans = rcl::shift_x(data,shx);
    
    for (unsigned int idy=0;idy<8;idy++){
      for (unsigned int idx=0;idx<8;idx++)  
        std::cout << ans[rcl::utriad(idx,idy,0)] << "\t";
      std::cout << std::endl;
    }
    std::cout << bar << std::endl << std::endl<< std::endl;
  }
  
  for (unsigned int shy=0;shy<16;shy++)  {
    std::cout << bar << std::endl;
    std::cout << " shift_y("<< shy <<") gives " << std::endl;
    std::cout << bar << std::endl;
    
    ans = rcl::shift_y(data,shy);
    
    for (unsigned int idy=0;idy<8;idy++){
      for (unsigned int idx=0;idx<8;idx++)  
        std::cout << ans[rcl::utriad(idx,idy,0)] << "\t";
      std::cout << std::endl;
    }
    std::cout << bar << std::endl << std::endl<< std::endl;
  }
  
  return 0;  
}
