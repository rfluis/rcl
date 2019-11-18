#ifndef RCL_FILTERING_TEMPLATES_HEADER
#define RCL_FILTERING_TEMPLATES_HEADER

#include "filtering.hpp"

namespace rcl{
  
template<typename T>
std::vector<T> fix_toroidal_movement(const std::vector<T>& src,const T& limit){
  std::vector<T> ans;
  T offset=0,threshold = limit*static_cast<T>(0.5);
  ans.push_back(src[0]);
  for (int x=1;x<src.size();x++){
    if (src[x]-src[x-1]>threshold) offset-=limit;
    if (src[x-1]-src[x]>threshold) offset+=limit;
    ans.push_back(src[x]+offset);    
    }
  return ans;
}

template<typename T>
std::vector<rcl::tpoint<T> > seqfilter(const std::vector<tpoint<T> >& data,const T& threshold){
  std::vector<tpoint<T> > ans;
  unsigned int size = data.size();
  rcl::tpoint<T> last = (data[1]-data[0]).normalized();
  ans.push_back(data[0]);
  for (unsigned int idx=1;idx<size-1;idx++){
    rcl::tpoint<T> vl = (data[idx]-data[idx-1]).normalized();
    T cp = std::fabs(vl^last);
    if (cp>threshold) {ans.push_back(data[idx]); last = vl;}
  }
  ans.push_back(data[size-1]);
  return ans;
}

}
#endif
