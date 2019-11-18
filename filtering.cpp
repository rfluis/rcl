#include "filtering-template.hpp"


template class std::vector<rcl::fpoint>;

namespace rcl{
template std::vector<rcl::fpoint> seqfilter(const std::vector<fpoint>& data,const float& threshold);
template std::vector<rcl::dpoint> seqfilter(const std::vector<dpoint>& data,const double& threshold);

template std::vector<float> fix_toroidal_movement(const std::vector<float>& src,const float& limit);
template std::vector<double> fix_toroidal_movement(const std::vector<double>& src,const double& limit);
}
