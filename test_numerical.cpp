#include "busybox.hpp"
#include <cmath>
#include <iostream>
#include <iomanip>
#include "tpoint.hpp"


namespace rcl{
  
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

template<typename T> T f(const T& x,const T& y){
  //std::cout << "f("<<x<<","<<y<<")"<< std::endl;
  T rmin = std::pow(x-static_cast<T>(2),static_cast<T>(2))+std::pow(y-static_cast<T>(3),static_cast<T>(2));
  T rmax = std::pow(x+static_cast<T>(3),static_cast<T>(2))+std::pow(y+static_cast<T>(4),static_cast<T>(2));
  //std::cout << "f("<<x<<","<<y<<") = "<< std::exp(-rmin/10)-std::exp(-rmax/10) << std::endl;
  return std::exp(-rmin/10)-std::exp(-rmax/10);
  }

int numerical_test_main(int argc,char **argv){
  
  
  std::cout << "Calculando el mínimo de ..." << std::endl;
  const double& epsilon = 1e-3;
  rcl::tpoint<double> minp=rcl::findmin2d(-5.,5.,-5.,5.,epsilon,epsilon,10,10,f<double>);
  std::cout << "Minimo localizado en :" << minp.x() << " , " << minp.y() << std::endl;
                     minp=rcl::findmin2d(minp.x()-100.*epsilon,minp.x()+100.*epsilon,minp.y()-100.*epsilon,minp.y()+100.*epsilon,epsilon,epsilon,8,8,f<double>);
  std::cout << "Minimo localizado en :" << minp.x() << " , " << minp.y() << std::endl;
                     minp=rcl::findmin2d(minp.x()-100.*epsilon,minp.x()+100.*epsilon,minp.y()-100.*epsilon,minp.y()+100.*epsilon,epsilon,epsilon,8,8,f<double>);
  std::cout << "Minimo localizado en :" << minp.x() << " , " << minp.y() << std::endl;
                     minp=rcl::findmin2d(minp.x()-100.*epsilon,minp.x()+100.*epsilon,minp.y()-100.*epsilon,minp.y()+100.*epsilon,epsilon,epsilon,8,8,f<double>);
  std::cout << "Minimo localizado en :" << minp.x() << " , " << minp.y() << std::endl;
                    
  std::cout << "Calculando el maximo de ..." << std::endl;
  rcl::tpoint<double> maxp=rcl::findmax2d(-5.,5.,-5.,5.,1e-4,1e-4,10,10,f<double>);
  std::cout << "Maximo localizado en :" << maxp.x() << " , " << maxp.y() << std::endl;
  
  
  std::cout << "Minimo localizado en :" << minp.x() << " , " << minp.y() << std::endl;
  std::cout << "Maximo localizado en :" << maxp.x() << " , " << maxp.y() << std::endl;
  return 0;
}

static rcl::reg_app reg("numerical_test",numerical_test_main);
