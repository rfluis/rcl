#ifndef CONSTANTS_HEADER
#define CONSTANTS_HEADER

namespace rcl{
  template <typename T> const T& PI = 3.14159265358979323846264338327950288419716939937511;
  template <typename T> const T& TPI = 2.*PI<T>;
  template <typename T> const T& IPI = 1./PI<T>;
  template <typename T> const T& ITPI = 1./TPI<T>;
  template <typename T> const T& pfactor = 32767./PI<T>;
  template <typename T> const T& ipfactor = PI<T>/32767.;
  template <typename T> const T& rfactor = 65280./PI<T>;
  template <typename T> const T& irfactor = PI<T>/65280.;  
}

#endif
