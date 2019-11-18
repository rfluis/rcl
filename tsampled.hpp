#ifndef TSAMPLED_HEADER
#define TSAMPLED_HEADER

#include <vector>
#include <algorithm>
#include <iostream>
#include "tpoint.hpp"


namespace rcl{
  
template <typename T> class tsampled{
public:
  typedef rcl::tpoint<T> element;
private:
  std::vector<element> data;
public:
  tsampled(){return;}
  void sort(){
    std::sort(data.begin(),
              data.end(),
              [](const element& a, const element& b)->bool
              {return a.x() < b.x();}); 
    return;}
  rcl::upoint find (const T& needle)const {
    rcl::upoint ans;
    ans.x() = 0;
    ans.y() = data.size()-1;
    while(ans.y()-ans.x()>1){
      unsigned int middle = (ans.x() + ans.y())/2;
      if (data[middle].x()<=needle) ans.x() = middle;
      if (data[middle].x()>=needle) ans.y() = middle;
      }
    return ans;
    }
  void insert(const element& d){push_back(d); sort(); return;}
  void push_back(const element& d){data.push_back(d); return;}
  void insert(const T& vx,const T& vy){insert(element(vx,vy)); return;}
  void push_back(const T& vx,const T& vy){data.push_back(element(vx,vy)); return;}
  std::size_t size() const { return data.size(); }
  element& operator [](const std::size_t& idx) { return data[idx];}
  const element& operator [](const std::size_t& idx) const { return data[idx];}
  T operator()(const T& needle) const {
    upoint s = find(needle);
    if (s.x()==s.y()) return data[s.x()].y();
    T pf = needle - data[s.x()].x();
    T ps = data[s.y()].x() - needle;
    return ((ps*data[s.x()].y()) + (pf*data[s.y()].y()))/(pf+ps); 
    }
  };
  
  typedef tsampled<float> fsampled;
}
#endif

