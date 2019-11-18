#ifndef TPOINT_HEADER
#define TPOINT_HEADER

#include <cmath>
#include <string>
#include "foperator.hpp"
#include "pair.hpp"


namespace rcl{

template <typename T> class tpoint{
private:
  rcl::pair<T,T> p;
public:
  tpoint():p(0,0){return;}
  tpoint(const T& x){p.first=x;p.second=x;return;}
  tpoint(const T& x,const T& y){p.first=x;p.second=y;return;}
  tpoint(const tpoint& ref){ this->p=ref.p; return;}
  tpoint(const rcl::pair<T,T>& ref){ this->p=ref; return;}
  tpoint(const tpoint* const& ptr){ this->p=ptr->p; return;}
  typedef T basetype;
  T& x(){ return p.first;}
  const T& x()const { return p.first;}
  T& y(){ return p.second;}
  const T& y()const { return p.second;}
  tpoint& operator+=(const tpoint& rhs){p.first+=rhs.x(); p.second+=rhs.y(); return *this;}
  tpoint& operator-=(const tpoint& rhs){p.first-=rhs.x(); p.second-=rhs.y(); return *this;}
  tpoint& operator&=(const tpoint& rhs){p.first*=rhs.x(); p.second*=rhs.y(); return *this;}
  tpoint& operator/=(const tpoint& rhs){p.first/=rhs.x(); p.second/=rhs.y(); return *this;}
  template<typename Q>
  tpoint& operator*=(const Q& rhs){p.first*=rhs; p.second*=rhs; return *this;}
  template<typename Q>
  tpoint& operator/=(const Q& rhs){p.first/=rhs; p.second/=rhs; return *this;}
  T dot (const tpoint& rhs)const{return (this->x()*rhs.x())+(this->y()*rhs.y());}
  T cross(const tpoint& rhs)const{return (this->x()*rhs.y())-(this->y()*rhs.x());}
  T sum()const { return p.first+p.second;}
  T norm2() const { return (p.first*p.first)+(p.second*p.second); }
  T norm() const { return std::sqrt(norm2());}
  void normalize() { *this/=norm(); return;}
  tpoint normalized() const { tpoint ans(this); ans.normalize(); return ans;}
  tpoint perpendicular() const { return tpoint(-p.second,p.first);}
  bool is_out(const tpoint& rhs)const{ 
    if (rhs.x()>p.first) return true; 
    if (rhs.y()>p.second) return true; 
    return false;}
};

template <typename T> inline tpoint<T> operator+(tpoint<T> lhs,const tpoint<T>& rhs){return lhs+=rhs;}
template <typename T> inline tpoint<T> operator-(tpoint<T> lhs,const tpoint<T>& rhs){return lhs-=rhs;}
template <typename T> inline tpoint<T> operator&(tpoint<T> lhs,const tpoint<T>& rhs){return lhs&=rhs;}
template <typename T> inline tpoint<T> operator/(tpoint<T> lhs,const tpoint<T>& rhs){return lhs/=rhs;}

template <typename T> inline T operator*(const tpoint<T>& lhs,const tpoint<T>& rhs){return lhs.dot(rhs);}
template <typename T> inline T operator^(const tpoint<T>& lhs,const tpoint<T>& rhs){return lhs.cross(rhs);}
template <typename T,typename Q> inline tpoint<T> operator*(tpoint<T> lhs,const Q& rhs){return lhs*=rhs;}
template <typename T,typename Q> inline tpoint<T> operator*(const Q& rhs,tpoint<T> lhs){return lhs*=rhs;}
template <typename T,typename Q> inline tpoint<T> operator/(tpoint<T> lhs,const Q& rhs){return lhs/=rhs;}
template <typename T,typename Q> inline tpoint<T> operator/(const Q& rhs,tpoint<T> lhs){return lhs/=rhs;}


template <typename T> 
inline bool operator==(const tpoint<T>& a,const tpoint<T>& b){
  if (a.x()==b.x()) if (a.y()==b.y()) return true; return false;}

template <typename T> 
inline bool operator!=(const tpoint<T>& a,const tpoint<T>& b){
  if (a.x()==b.x()) if (a.y()==b.y()) return false; return true;}

template <typename T> 
inline bool operator<(const tpoint<T>& a,const tpoint<T>& b){
  if (a.x()<b.x()) return true;
  if (b.x()<a.x()) return false;
  if (a.x()==b.x()) {
    if (a.y()<b.y()) return true;
    if (b.y()<a.y()) return false;
    }
  return false;
}

typedef tpoint<unsigned int>  upoint;  
typedef tpoint<float>         fpoint;
typedef tpoint<double>        dpoint;
typedef tpoint<std::string>   spoint;
}
#endif
