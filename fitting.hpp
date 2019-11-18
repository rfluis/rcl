#ifndef RCL_FITTING
#define RCL_FITTING

#include "numerical.hpp"
#include "tpoint.hpp"
#include "tvector.hpp"
#include "tfield.hpp"
#include "skyrmion_models.hpp"
#include <iostream>

namespace rcl{

// Forward Declarations
template <typename T> T get_majoritarty_mz(const rcl::tfield<rcl::tvector<T> >& mag,unsigned int& layer);
template <typename T> T get_topologycal_charge(const rcl::tfield<rcl::tvector<T> >& mag,unsigned int& layer);
template <typename T> rcl::tfield<rcl::tscalar<T> > get_topologycal_charge_density(const rcl::tfield<rcl::tvector<T> >& mag,unsigned int& layer);
template <typename T> rcl::triad<T> estimate_skyrmion(const rcl::tfield<rcl::tvector<T> >& mag);
template <typename T> rcl::triad<T> locate_skyrmion(const rcl::tfield<rcl::tvector<T> >& mag);
template <typename T> rcl::triad<T> locate_skyrmion_PBC(const rcl::tfield<rcl::tvector<T> >& mag);
template <typename T> rcl::pair<rcl::tpoint<T>,rcl::tpoint<T> > get_skyrmion_sizes(const rcl::tfield<rcl::tvector<T> >& mag);
template <typename T> std::vector<T> make_trapezoids (const std::vector<rcl::tpoint<T> >& a, const std::vector<rcl::tpoint<T> >& b);
}

#endif

