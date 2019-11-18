#ifndef UTIL_HEADER
#define UTIL_HEADER

#include <vector>

namespace rcl{

template <typename T> 
class inventory{
private:
  std::vector<T> data;
public:
  inventory(){return;}
  inventory(const std::vector<T>& idata):data(idata){return;}
  T& operator[](const unsigned int& x){return data[x];}
  const T& operator[](const unsigned int& x)const{return data[x];}
  unsigned int size()const{return size;}
  void push_back(const T& x){data.push_back(x);}
  bool is_inside(const T& x)const{for (unsigned int idx=0;idx<data.size();idx++) if (data[idx]==x) return true; return false;}
  void add(const T& x){if (!is_inside(x)) push_back(x); return;}
};  

}

#endif
