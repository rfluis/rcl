#include "numerical_templates.hpp"
#include "tpoint.hpp"

namespace rcl{
  template void sumplex(std::vector<float>& data);
  template void sumplex(std::vector<double>& data);
  template void sumplex(std::vector<rcl::fpoint>& data);
  template void sumplex(std::vector<rcl::dpoint>& data);
}
