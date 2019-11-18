#ifndef TRIAD_HEADER
#define TRIAD_HEADER

#include "foperator.hpp"

namespace rcl{
template <typename T> class triad{
public:
  typedef T basetype;
private:
  T xx,yy,zz;
public:
  triad(){xx=yy=zz=0; return;}
  triad(const T& s){xx=yy=zz=s; return;}
  triad(const T& x,const T& y,const T& z){
    this->xx=x;
    this->yy=y;
    this->zz=z;
    return;
    }
  T& x() {return xx;}
  const T& x() const {return xx;}
  T& y() {return yy;}
  const T& y() const {return yy;}
  T& z() {return zz;}
  const T& z() const {return zz;}
  T volume()const {return xx*yy*zz;}
  T addr(const triad<T>& ad)const{ return ad.x()+xx*(ad.y()+yy*ad.z());} //
  T addr(const T& ym,const T& zm)const{ return xx+yy*ym+zz*zm;}
  T& operator[](const unsigned int& idx){ return reinterpret_cast<T*>(this)[idx];}
  const T& operator[](const unsigned int& idx)const{ return reinterpret_cast<const T*>(this)[idx];}
};

template <typename T> inline bool operator==(const triad<T>& lhs,const triad<T>& rhs){
  if (lhs.x()!=rhs.x()) return false;
  if (lhs.y()!=rhs.y()) return false;
  if (lhs.z()!=rhs.z()) return false;
  return true;
  }
template <typename T> inline bool operator!=(const triad<T>& lhs, const triad<T>& rhs){return !operator==(lhs,rhs);}

typedef triad<unsigned int> utriad;
typedef triad<float>        ftriad;
}

#endif
