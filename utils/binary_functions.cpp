#include <vector>
#include <iostream>

bool sin(int freqn,int freqd,int idx)
{
  if ((idx*freqd/freqn)&1) return true;
  return false;
}

int main(){
  std::cout << "P1\n" << "3000 3000\n";
  for (unsigned int idy=1;idy<3001;idy++){
    for (unsigned int idx=0;idx<3000;idx++){
      std::cout << sin(idy,1280,idx) << " ";
      }
      std::cout << std::endl;
      std::cout << std::endl;
    }
  
}
