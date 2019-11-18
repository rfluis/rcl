#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

class number{
  public:
  int x,y;
  number(){return;}
  number(const int& x,const int& y){this->x=x;this->y=y; return;}
};

inline bool operator<(const number& lhs,const number& rhs){
  if (lhs.x<rhs.x) return true;
  if (lhs.y<rhs.y) return true;
  return false;
}

inline std::ostream& operator << (std::ostream& os,const number& n){
  os << "(" << std::setw(2) << n.x << "," << std::setw(2) << n.y << ")";
  return os; 
  }

int main()
{
  std::vector<number> data;
  for (int idy=-5;idy<5;idy++) for (int idx=-5;idx<5;idx++)  data.push_back(number(idx,idy));
  std::sort(data.begin(),data.end());
  for (auto itr=data.begin();itr!=data.end();itr++) std::cout << *itr << " ";
  std::cout << std::endl;
  return 0;  
}
