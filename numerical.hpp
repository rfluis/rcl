#ifndef NUMERICAL_HEADER
#define NUMERICAL_HEADER

#include "constants.hpp"
#include "tpoint.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>

namespace rcl{
inline  unsigned int log2floor(const unsigned int& x){
  return static_cast<unsigned int>(std::floor(std::log2(x)));
}

inline unsigned int log2ceil(const unsigned int& x){
  return static_cast<unsigned int>(std::ceil(std::log2(x)));
}
template<typename T> inline T abs(const T& x){ if (x<0) return -x; else return x;}
template<typename T> inline T sqr(const T& x){ return x*x;}
template<typename T> void sumplex(std::vector<T>& data);


template <typename T,typename FN,typename CMP>
rcl::pair<T,T> findval(const rcl::pair<T,T>& xp,const unsigned int& samples,const FN& f,const CMP& cmp){
  const T& xi = xp.first;
  const T& xf = xp.second;
  
  T delta = (xf-xi)/static_cast<T>(samples-1);
  rcl::pair<T,T> max(xi,f(xi));

  for (unsigned int idx=1;idx<samples;idx++){
    rcl::pair<T,T> current;
    current.x() = xi+idx*delta;
    current.y() = f(current.x());
    if (cmp(current.y(),max.y())) max = current;}
    
  return rcl::pair<T,T> (max.x()-delta,max.x()+delta);
}

template <typename T,typename FN,typename CMP>
rcl::pair<rcl::pair<T,T>,rcl::pair<T,T> > findval(const rcl::pair<T,T>& xp,const rcl::pair<T,T>& yp,const rcl::pair<unsigned int,unsigned int>& samples,const FN& f,const CMP& cmp){
  const T& xi = xp.first;
  const T& xf = xp.second;
  const T& yi = yp.first;
  const T& yf = yp.second;
  
  rcl::pair<T,T> delta((xf-xi)/static_cast<T>(samples.x()-1),(yf-yi)/static_cast<T>(samples.y()-1));
  //std::cout << "DEBUG: findval x->("<<xi <<","<<xf <<")  y->("<< yi <<","<< yf <<") samples->["<<samples.x() <<","<<samples.y() <<"]" << std::endl;
  //std::cout << "DEBUG: findval delta.x = " << delta.x() << " delta.y = " << delta.y() << std::endl;
  rcl::pair<rcl::pair<T,T>,T> max(rcl::pair<T,T>(xi,yi),f(xi,yi));

  for (unsigned int idx=0;idx<samples.x();idx++) for (unsigned int idy=0;idy<samples.y();idy++){
    rcl::pair<rcl::pair<T,T>,T> current;
    current.first.x() = xi+idx*delta.x();
    current.first.y() = yi+idy*delta.y();
    current.y() = f(current.first.x(),current.first.y());
    //std::cout << "DEBUG: findval: TRACE:" << idx << "," << idy << " ; " << current.first.x() << "," << current.first.y() << " -> " << current.y() << std::endl;
    if (cmp(current.y(),max.y())) {max = current;}
    }
    
  return rcl::pair<rcl::pair<T,T>,rcl::pair<T,T> > (rcl::pair<T,T>(max.x().x()-delta.x(),max.x().x()+delta.x()),rcl::pair<T,T>(max.x().y()-delta.y(),max.x().y()+delta.y())); 
}

/* ************************************************************************************************** */
template <typename T,typename FN,typename CMP> 
T findval(const T& xl,const T& xr,const T& epsilon,const int& divs,const FN& f,const CMP& comp);
template <typename T,typename FN,typename CMPx,typename CMPy> 
rcl::tpoint<T> findval2d(const T& xl,const T& xr,const T& yl,const T& yr,
                 const T& epsilonx,const T& epsilony,const int& divsx,const int& divsy,
                 const FN& f,const CMPx& compx,const CMPy& compy);

template <typename T,typename FN,typename CMP> 
T findval(const T& xl,const T& xr,const T& epsilon,const int& divs,const FN& f,const CMP& comp){
  if (xl>xr) {return findval(xr,xl,epsilon,divs,f,comp);}
  T gap = xr-xl;
  if (gap<epsilon) {
    if (comp(f(xl),f(xr))) { return xl;}
    else{ return xr;}
    }
  T step = gap/divs;
  T nxl=xl;
  T nxr=xr;
  T ml = f(xl);
  T mr = f(xr);
  for(T x=xl;x<=xr;x+=step){
    T v = f(x);
    if (comp(v,ml))           { ml = v; nxl=x; }
    if (ml!=v) if(comp(v,mr)) { mr = v; nxr=x; }
    }
  if (nxl==xl && nxr==xr) {nxl = xl + epsilon; nxr = xr - epsilon;}
  else {
    if (nxl!=xl) nxl-=step*0.5;
    if (nxr!=xr) nxr+=step*0.5;
  }
  return findval(nxl,nxr,epsilon,divs,f,comp);
}

template <typename T,typename FN,typename CMPx,typename CMPy> 
rcl::tpoint<T> findval2d(const T& xl,const T& xr,const T& yl,const T& yr,
                 const T& epsilonx,const T& epsilony,const int& divsx,const int& divsy,
                 const FN& f,const CMPx& compx,const CMPy& compy){
  rcl::tpoint<T> answer;
  answer.x() = findval(xl,xr,epsilonx,divsx,
                    [&answer
                    ,&yl=static_cast<const T&>(yl)
                    ,&yr=static_cast<const T&>(yr)
                    ,&epsilony=static_cast<const T&>(epsilony)
                    ,&divsy=static_cast<const int&>(divsy)
                    ,&f=static_cast<const decltype(f)>(f)
                    ,&compy =static_cast<const decltype(compy)>(compy)](const T& x)->T{
                    answer.y() = findval(yl,yr,epsilony,divsy,
                                      [&x = static_cast<const T&>(x),&f](const T& y)->T{
                                        return f(x,y);},compy);return f(x,answer.y());},compx);
  //std::cout << "Calculada la x" << std::endl;
  answer.y() = findval(yl,yr,epsilony,divsy,[&answer,&f](const T& y)->T{return f(answer.x(),y);},compy);
  return answer;                                      
}

template <typename T,typename FN>
rcl::tpoint<T> findmin2d(const T& xl,const T& xr,const T& yl,const T& yr,
                 const T& epsilonx,const T& epsilony,const int& divsx,const int& divsy,
                 const FN& f){
                   return rcl::findval2d(xl,xr,yl,yr,epsilonx,epsilony,divsx,divsy,f,
                   [](const T& a,const T& b)->bool{return a<b;},
                   [](const T& a,const T& b)->bool{return a<b;});                   
}

template <typename T,typename FN>
rcl::tpoint<T> findmax2d(const T& xl,const T& xr,const T& yl,const T& yr,
                 const T& epsilonx,const T& epsilony,const int& divsx,const int& divsy,
                 const FN& f){
                   return rcl::findval2d(xl,xr,yl,yr,epsilonx,epsilony,divsx,divsy,f,
                   [](const T& a,const T& b)->bool{return a>b;},
                   [](const T& a,const T& b)->bool{return a>b;});                   
}
}
#endif
