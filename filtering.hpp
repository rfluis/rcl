#ifndef RCL_FILTERING_HEADER
#define RCL_FILTERING_HEADER

#include <vector>
#include "tpoint.hpp"

namespace rcl{
template<typename T> std::vector<rcl::tpoint<T> > seqfilter(const std::vector<tpoint<T> >& data,const T& threshold);
template<typename T> std::vector<T> fix_toroidal_movement(const std::vector<T>& src,const T& limit);
}
#endif
