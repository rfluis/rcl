#include "fitting_templates.hpp"

namespace rcl{

template float get_majoritarty_mz(const rcl::tfield<rcl::tvector<float> >& mag,unsigned int& layer);
template rcl::tfield<rcl::tscalar<float> > get_topologycal_charge_density(const rcl::tfield<rcl::tvector<float> >& mag,unsigned int& layer);
template rcl::triad<float> estimate_skyrmion(const rcl::tfield<rcl::tvector<float> >& mag);
template rcl::triad<float> locate_skyrmion(const rcl::tfield<rcl::tvector<float> >& mag);
template rcl::triad<float> locate_skyrmion_PBC(const rcl::tfield<rcl::tvector<float> >& mag);
template rcl::pair<rcl::tpoint<float>,rcl::tpoint<float> > get_skyrmion_sizes(const rcl::tfield<rcl::tvector<float> >& mag);
template std::vector<float> make_trapezoids (const std::vector<rcl::tpoint<float> >& a, const std::vector<rcl::tpoint<float> >& b);
  
  
}
