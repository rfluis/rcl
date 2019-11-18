#ifndef NUMERICAL_TEMPLATES_HEADER
#define NUMERICAL_TEMPLATES_HEADER

#include "numerical.hpp"

namespace rcl{
  
template<typename T> void sumplex(std::vector<T>& data){
  std::size_t size = data.size();
  std::size_t ppot = 1;
  unsigned int llog = log2floor(size);
  for (unsigned int idx=0;idx<llog;idx++) ppot*=2;
  if (size!=ppot) for (unsigned int idx=ppot;idx<size;idx++) data[idx-ppot]+=data[idx];
  while (ppot!=1){
    ppot/=2; 
    #pragma omp parallel for
    for (unsigned int idx=0;idx<ppot;idx++) data[idx]+=data[idx+ppot];
  }
  return;
}

} //end namespace rcl


#endif
