#ifndef POLAR64_HEADER
#define POLAR64_HEADER
#include "tvector.hpp"
#include "vfloat.hpp"
#include "constants.hpp"
#include "tscalar.hpp"

namespace rcl{
  
class lpolar{
private:
  unsigned long long int e:8;
  unsigned long long int m:23;
  long long int t:16;
  long long int f:17;
public:
  lpolar(){e=0;m=0;t=0;f=0;return;}
  lpolar(const dvector& v){
    vfloat rad = v.norm();
    float theta=v.theta()*pfactor<float>;
    float fi=v.phi()*pfactor<float>;
    e = rad.e();
    m = rad.m();
    t = theta;
    f = fi;
    return;
  }
  lpolar(const fvector& v){
    vfloat rad = v.norm();
    float theta=v.theta()*pfactor<float>;
    float fi=v.phi()*pfactor<float>;
    e = rad.e();
    m = rad.m();
    t = theta;
    f = fi;
    return;
  }
  const unsigned long long int& getl()const{ return reinterpret_cast<const unsigned long long int&>(*this);}
  void setr(const float& r){
    const vfloat& rad = reinterpret_cast<const vfloat&>(r);
    e = rad.e();
    m = rad.m();
    return;
  }
  fvector getv()const{ 
    vfloat rad;
    rad.e() = e;
    rad.m() = m;
    rad.s() = 0;
    float theta=t*ipfactor<float>;
    float fi=f*ipfactor<float>;
    return polar<float>(rad,theta,fi);
    }
  lpolar& operator+=(const lpolar& rhs){
    e+=rhs.e;
    m+=rhs.m;
    t+=rhs.t;
    f+=rhs.f;
    return *this;  }
  lpolar& operator-=(const lpolar& rhs){
    e-=rhs.e;
    m-=rhs.m;
    t-=rhs.t;
    f-=rhs.f;
    return *this;  }
  long long int gett() const {return t;}
  long long int getf() const {return f;}
};
inline lpolar operator+(lpolar lhs,const lpolar& rhs){ lhs+=rhs; return lhs;}
inline lpolar operator-(lpolar lhs,const lpolar& rhs){ lhs-=rhs; return lhs;}
inline lpolar operator^(const lpolar& lhs,const lpolar& rhs){
  unsigned long long int x;
  x = reinterpret_cast<const unsigned long long int&>(lhs)^reinterpret_cast<const unsigned long long int&>(rhs);
  return reinterpret_cast<lpolar&>(x);
}

class rpolar{
private:
  unsigned long long int e:8;
  unsigned long long int m:23;
  unsigned long long int t:16;
  unsigned long long int f:17;
public:
  rpolar(){e=0;m=0;t=0;f=0;return;}
  rpolar(const dvector& v){
    rcl::vfloat rad = v.norm();
    float theta=v.theta()*rfactor<float>;
    float fi=v.phi()*rfactor<float>;
    if (fi<0) fi+=131072.f;
    e = rad.e();
    m = rad.m();
    t = theta;
    f = fi;
    return;
  }
  rpolar(const fvector& v){
    rcl::vfloat rad = v.norm();
    float theta=v.theta()*rfactor<float>;
    float fi=v.phi()*rfactor<float>;
    //if (fi<0) fi+=65535.;
    if (fi<0) fi+=131072.f;
    e = rad.e();
    m = rad.m();
    t = static_cast<unsigned long long> (theta);
    f = static_cast<unsigned long long> (fi);
    return;
  }
  const unsigned long long int& getl()const{ return reinterpret_cast<const unsigned long long int&>(*this);}
  void setr(const float& r){
    const rcl::vfloat& rad = reinterpret_cast<const vfloat&>(r);
    e = rad.e();
    m = rad.m();
    return;
  }
  float getr() const{
    rcl::vfloat rad;
    rad.e() = e;
    rad.m() = m;
    rad.s() = 0;
    return rad;
    }
  float get_scalar() const{
    rcl::vfloat rad;
    rad.e() = e;
    rad.m() = m;
    if (t==0) rad.s() = 0;
    else      rad.s() = 1;
    return rad;
    }
  fvector getv()const{ 
    rcl::vfloat rad;
    rad.e() = e;
    rad.m() = m;
    rad.s() = 0;
    float theta=t*irfactor<float>;
    float fi=f*irfactor<float>;
    return polar<float>(rad,theta,fi);
    }
  rpolar& operator+=(const rpolar& rhs){
    e+=rhs.e;
    m+=rhs.m;
    t+=rhs.t;
    f+=rhs.f;
    return *this;  }
  rpolar& operator-=(const rpolar& rhs){
    e-=rhs.e;
    m-=rhs.m;
    t-=rhs.t;
    f-=rhs.f;
    return *this;  }
  unsigned int gett() const {return t;}
  unsigned int getf() const {return f;}
  void negative(){
    t=65535-t;
    if (f<65535) f+=65536;
    else         f-=65536;
    return;}
};


inline rpolar operator+(rpolar lhs,const rpolar& rhs){ lhs+=rhs; return lhs;}
inline rpolar operator-(rpolar lhs,const rpolar& rhs){ lhs-=rhs; return lhs;}
inline rpolar operator^(const rpolar& lhs,const rpolar& rhs){
  unsigned long long int x;
  x = reinterpret_cast<const unsigned long long int&>(lhs)^reinterpret_cast<const unsigned long long int&>(rhs);
  return reinterpret_cast<rpolar&>(x);
}


template <> inline void convert(rcl::lpolar& dst,const rcl::fvector& src){  dst=rcl::lpolar(src); return;}
template <> inline void convert(rcl::rpolar& dst,const rcl::fvector& src){  dst=rcl::rpolar(src); return;}
template <> inline void convert(rcl::lpolar& dst,const rcl::dvector& src){  dst=rcl::lpolar(src); return;}
template <> inline void convert(rcl::rpolar& dst,const rcl::dvector& src){  dst=rcl::rpolar(src); return;}
template <> inline void convert(rcl::fvector& dst,const rcl::lpolar& src){  dst=src.getv(); return;}
template <> inline void convert(rcl::fvector& dst,const rcl::rpolar& src){  dst=src.getv(); return;}

}
#endif


