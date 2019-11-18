#ifndef TSCALAR_HEADER
#define TSCALAR_HEADER

#include "numerical.hpp"
namespace rcl{

template<typename T> class tscalar{
private:
  T p;
public:
  tscalar():p(0){return;}
  tscalar(const T& x):p(x){return;}
  template <typename Q> tscalar(const tscalar<Q>& x):p(static_cast<T>(x)){return;}
  typedef T basetype;
  basetype& x(){return p;}
  basetype& y(){return p;}
  basetype& z(){return p;}
  const basetype& x()const{return p;}
  const basetype& y()const{return p;}
  const basetype& z()const{return p;}
  operator T&() {return p;}
  T norm2()const{return p*p;}
  T norm() const {return rcl::abs(p);}
  operator const T&()const {return p;}
  void normalize(){return;}
};


typedef rcl::tscalar<float>  fscalar;
typedef rcl::tscalar<double> dscalar;
}

#endif
