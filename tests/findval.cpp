#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include "tpoint.hpp"
#include "triad.hpp"
#include "numerical.hpp"

long double f(const long double& x){ return std::exp(-std::pow(x-2,2))-std::exp(-std::pow(x+3,2));}
long double f2(const long double& x,const long double& y){ return std::exp(-std::pow(x-2,2)-std::pow(y+3,2))-std::exp(-std::pow(x+3,2)-std::pow(y-4,2));}

template <typename T,typename FN>
class proxy_function{
public:
  proxy_function(const FN& fn):func(fn){return;}
private:
  const FN& func;
  std::map<T,T> cache;
public:
  void eval(const T& x){ cache[x]=func(x); return;}
  const T& operator()(const T& x){
    typename std::map<T,T>::const_iterator found = cache.find(x);
    if(found==cache.end()) eval(x);
    return cache[x];     
   }
};

/*
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
rcl::pair<rcl::pair<T,T>,rcl::pair<T,T> > findval(const rcl::pair<T,T>& xp,const rcl::pair<T,T>& yp,
                                                  const rcl::pair<unsigned int,unsigned int>& samples,
                                                  const FN& f,const CMP& cmp){
  const T& xi = xp.first;
  const T& xf = xp.second;
  const T& yi = yp.first;
  const T& yf = yp.second;
  
  rcl::pair<T,T> delta((xf-xi)/static_cast<T>(samples.x()-1),(yf-yi)/static_cast<T>(samples.y()-1));
  rcl::pair<rcl::pair<T,T>,T> max(rcl::pair<T,T>(xi,yi),f(xi,yi));

  for (unsigned int idx=0;idx<samples.x();idx++) for (unsigned int idy=0;idy<samples.y();idy++){
    rcl::pair<rcl::pair<T,T>,T> current;
    current.first.x() = xi+idx*delta.x();
    current.first.y() = yi+idy*delta.y();
    current.y() = f(current.first.x(),current.first.y());
    if (cmp(current.y(),max.y())) max = current;}
    
  return rcl::pair<rcl::pair<T,T>,rcl::pair<T,T> > (rcl::pair<T,T>(max.x().x()-delta.x(),max.x().x()+delta.x()),rcl::pair<T,T>(max.x().y()-delta.y(),max.x().y()+delta.y())); 
}
*/

int main(){
  std::cout << sizeof(long double) << " : " << sizeof(long double)*8 << std::endl;
  rcl::pair<long double,long double> init(-5,5),next;
  next = findval(init,50,f,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next.x() << " , " << next.y() << std::endl;
  next = findval(next,50,f,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next.x() << " , " << next.y() << std::endl;
  next = findval(next,50,f,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next.x() << " , " << next.y() << std::endl;
  next = findval(next,50,f,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next.x() << " , " << next.y() << std::endl;
  next = findval(next,16,f,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next.x() << " , " << next.y() << std::endl;
  
  std::cout << " =================================================" << std::endl;
  
  next = findval(init,50,f,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next.x() << " , " << next.y() << std::endl;                   
  next = findval(next,50,f,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next.x() << " , " << next.y() << std::endl;                   
  next = findval(next,50,f,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next.x() << " , " << next.y() << std::endl;                   
  next = findval(next,50,f,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next.x() << " , " << next.y() << std::endl;                   
  next = findval(next,16,f,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next.x() << " , " << next.y() << std::endl;
  
  std::cout << " =================================================" << std::endl;
  
  rcl::pair<rcl::pair<long double,long double>,rcl::pair<long double,long double> > init2(init,init),next2;
  next2=findval(init,init,rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  next2=findval(next2.x(),next2.y(),rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  next2=findval(next2.x(),next2.y(),rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  next2=findval(next2.x(),next2.y(),rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  next2=findval(next2.x(),next2.y(),rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a<b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  
  std::cout << " =================================================" << std::endl;
  
  next2=findval(init,init,rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  next2=findval(next2.x(),next2.y(),rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  next2=findval(next2.x(),next2.y(),rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  next2=findval(next2.x(),next2.y(),rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  next2=findval(next2.x(),next2.y(),rcl::pair<unsigned int,unsigned int>(50,50),f2,[](const long double& a,const long double& b)->bool{return a>b;});
  std::cout << next2.first.x() << " , " << next2.first.y() << " - " << next2.second.x() << " , " << next2.second.y()<< std::endl;
  
  return 0;
}
