#ifndef TVECTOR_HEADER
#define TVECTOR_HEADER
#include <cmath>
#include "triad.hpp"

namespace rcl{
  
template<typename T> class tvector{
private:
  triad<T> p;
public:
  typedef T basetype;
  tvector(){return;}
  tvector(const triad<T>& v):p(v){return;}
  tvector(const T& x,const T& y,const T& z){
    p.x()=x;
    p.y()=y;
    p.z()=z;
    return;
    }
  tvector(const T& z){ p.x()=0.; p.y()=0.; p.z()=z; return; }
  tvector(const tvector& v):p(v.p){return;}
  T& x(){return p.x();}
  T& y(){return p.y();}
  T& z(){return p.z();}
  const T& x()const {return p.x();}
  const T& y()const {return p.y();}
  const T& z()const {return p.z();}
  tvector& operator+=(const tvector& rhs){
    p.x()+=rhs.x();
    p.y()+=rhs.y();
    p.z()+=rhs.z();
    return *this;
    }
  tvector& operator-=(const tvector& rhs){
    p.x()-=rhs.x();
    p.y()-=rhs.y();
    p.z()-=rhs.z();
    return *this;
    }
  tvector& operator&=(const tvector& rhs){
    p.x()*=rhs.x();
    p.y()*=rhs.y();
    p.z()*=rhs.z();
    return *this;
    }
  tvector& operator^=(const tvector& rhs){
    tvector lhs(*this);
    p.x()=lhs.y()*rhs.z()-lhs.z()*rhs.y();
    p.y()=lhs.z()*rhs.x()-lhs.x()*rhs.z();
    p.z()=lhs.x()*rhs.y()-lhs.y()*rhs.x();
    return *this;
    }
  tvector& operator*=(const T& rhs){
    p.x()*=rhs;
    p.y()*=rhs;
    p.z()*=rhs;
    return *this;
    }
  tvector& operator/=(const T& rhs){
    p.x()/=rhs;
    p.y()/=rhs;
    p.z()/=rhs;
    return *this;
    }
  tvector square()const{ return tvector(p.x()*p.x(),p.y()*p.y(),p.z()*p.z()); }
  T sum()  const{return p.x()+p.y()+p.z();}
  T norm2()const{return (*this).square().sum();}
  T norm() const{return std::sqrt(norm2());}
  T rho()  const{return std::sqrt((p.x()*p.x())+(p.y()*p.y()));}
  T theta()const{return std::atan2(rho(),p.z());}
  T phi()  const{return std::atan2(p.y(),p.x());}
  void negative(){p.x()=-p.x(); p.y()=-p.y(); p.z()=-p.z(); return;}
  void normalize(){(*this)/=norm(); return;}
  operator triad<T>& (){ return p;}
};
template <typename T> inline tvector<T> operator+(tvector<T> lhs,const tvector<T>& rhs) { return lhs+=rhs;}
template <typename T> inline tvector<T> operator-(tvector<T> lhs,const tvector<T>& rhs) { return lhs-=rhs;}
template <typename T> inline tvector<T> operator&(tvector<T> lhs,const tvector<T>& rhs) { return lhs&=rhs;}
template <typename T> inline tvector<T> operator^(tvector<T> lhs,const tvector<T>& rhs) { return lhs^=rhs;}
template <typename T> inline tvector<T> operator*(tvector<T> lhs,const T& rhs) {return lhs*=rhs;}
template <typename T> inline tvector<T> operator/(tvector<T> lhs,const T& rhs) {return lhs/=rhs;}
template <typename T> inline tvector<T> operator*(const T& rhs,tvector<T> lhs) {return lhs*=rhs;}
template <typename T> inline tvector<T> operator/(const T& rhs,tvector<T> lhs) {return lhs/=rhs;}
template <typename T> inline         T  operator*(const tvector<T>& lhs,const tvector<T>& rhs){
  return (lhs.x()*rhs.x())+(lhs.y()*rhs.y())+(lhs.z()*rhs.z());}
template <typename T> inline tvector<T> polar(const T& r,const T& t,const T& f){
  return tvector<T>(r*std::sin(t)*std::cos(f),r*std::sin(t)*std::sin(f),r*std::cos(t));
  }
typedef tvector<float>  fvector;
typedef tvector<double> dvector;

template <typename T> inline bool operator==(const tvector<T>& lhs,const tvector<T>& rhs){
  if (lhs.x()!=rhs.x()) return false;
  if (lhs.y()!=rhs.y()) return false;
  if (lhs.z()!=rhs.z()) return false;
  return true;
  }
template <typename T> inline bool operator!=(const tvector<T>& lhs, const tvector<T>& rhs){return !operator==(lhs,rhs);}

template <typename T,typename S> inline void convert(T& dst,const S& src); 
}
#endif
