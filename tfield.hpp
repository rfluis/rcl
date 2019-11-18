#ifndef TFIELD_HEADER
#define TFIELD_HEADER

#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
//#include "triad.hpp"
#include "tvector.hpp"
#include "tscalar.hpp"
#include "polar64.hpp"
namespace rcl{
  
template <typename T> class tfield{
private:
  std::vector<T> data;
  utriad nodes;
  ftriad cellsz;
  std::map<std::string,std::string> meta;
public:
  tfield(){nodes=1;cellsz=1e-9; fit(); return;}
  tfield(const utriad& n,const ftriad& c):nodes(n),cellsz(c){fit();return;}
  tfield(const utriad& n):nodes(n){fit();return;}
  tfield(const tfield& orig){meta = orig.getmeta();parse_meta();data = orig.getdata();return;}
  ~tfield(){clear(); return;}
  void fit(){data.resize(nodes.volume());return;}
  void fit(const utriad& n){nodes=n; fit(); return;}
  void fit(const ftriad& c){cellsz=c; return;}
  void fit(const utriad& n,const ftriad& c){fit(c);fit(n); return;}
  void clear(){nodes=utriad(0,0,0);data.clear(); meta.clear(); return;}
  const utriad& getnodes()const {return nodes;}
  const ftriad& getsize() const {return cellsz;}
  unsigned int size()const{ return nodes.volume();}
  unsigned int minsize(const tfield& rhs)const{ if(size()<rhs.size()) return size(); else return rhs.size();}
  T* begin(){ return &(*data.begin());}
  const T* begin()const{return &(*data.begin());}
  T* end(){ return &(*data.end());}
  const T* end()const{return &(*data.end());}
  std::vector<T>& getdata() {return data;}
  const std::vector<T>& getdata()const {return data;}
  tfield get_slice(const unsigned int& layer)const{
    if (layer>nodes.z()-1) return tfield();
    rcl::utriad nnodes(nodes.x(),nodes.y(),1);
    tfield ans(nnodes,cellsz);
    for (unsigned int idy=0;idy<nodes.y();idy++)
      for (unsigned int idx=0;idx<nodes.x();idx++)
        ans[rcl::utriad(idx,idy,0)]=(*this)[rcl::utriad(idx,idy,layer)];
    return ans;
    }
  T& operator[](const unsigned int& idx)      {return data[idx];}
  const T& operator[](const unsigned int& idx)const{return data[idx];}
  T& operator[](const utriad& idx)      {return data[nodes.addr(idx)];}
  const T& operator[](const utriad& idx)const{return data[nodes.addr(idx)];}
  T operator()(const float& x,const float& y,const unsigned int& layer)const;
        std::map<std::string,std::string>& getmeta()     {return meta;}
  const std::map<std::string,std::string>& getmeta()const{return meta;}
  void reg_meta(const std::string& key,const std::string& val);
  bool is_meta(const std::string& key)const;
  std::string get_meta(const std::string& key)const ;
  template <typename Q> Q get_val(const std::string& key)const;
  void parse_meta();
  tfield& operator+=(const tfield& rhs);
  tfield& operator-=(const tfield& rhs);
  tfield& operator^=(const tfield& rhs);
  tfield& operator^=(const T& rhs);
  tfield& operator&=(const tfield& rhs);
  tfield& operator*=(const typename T::basetype& rhs);
  tfield& operator/=(const typename T::basetype& rhs);
};  

template <typename T> inline tfield<T>operator+(tfield<T> lhs,const tfield<T>& rhs) { lhs+=rhs; return lhs;}
template <typename T> inline tfield<T>operator-(tfield<T> lhs,const tfield<T>& rhs) { lhs-=rhs; return lhs;}
template <typename T> inline tfield<T>operator^(tfield<T> lhs,const tfield<T>& rhs) { lhs^=rhs; return lhs;}
template <typename T> inline tfield<T>operator^(tfield<T> lhs,const T& rhs)         { lhs^=rhs; return lhs;}
template <typename T> inline tfield<T>operator&(tfield<T> lhs,const tfield<T>& rhs) { lhs&=rhs; return lhs;}

template <typename T> inline tfield<T>operator*(tfield<T> lhs,const typename T::basetype& rhs) { lhs*=rhs; return lhs;}
template <typename T> inline tfield<T>operator/(tfield<T> lhs,const typename T::basetype& rhs) { lhs/=rhs; return lhs;}
template <typename T> inline tfield<T>operator*(const typename T::basetype& rhs,tfield<T> lhs) { lhs*=rhs; return lhs;}
template <typename T> inline tfield<T>operator/(const typename T::basetype& rhs,tfield<T> lhs) { lhs/=rhs; return lhs;}

template <typename T> tfield<T> shift_x(const tfield<T>& f,const unsigned int& shift){
  tfield<T> ans(f.getnodes(),f.getsize());
  utriad nodes = f.getnodes();
  unsigned int pshift=shift,offset;
  while (pshift>nodes.x()) pshift-=nodes.x();
  offset = nodes.x() - pshift;
  for(unsigned int idz=0;idz<nodes.z();idz++)
    for(unsigned int idy=0;idy<nodes.y();idy++){
      for(unsigned int idx=0;idx<offset;idx++)  
        ans[rcl::utriad(idx+pshift,idy,idz)] = f[rcl::utriad(idx,idy,idz)];
      for(unsigned int idx=offset;idx<nodes.x();idx++)  
        ans[rcl::utriad(idx-offset,idy,idz)] = f[rcl::utriad(idx,idy,idz)];
    }
  return ans;
  }

template <typename T> tfield<T> shift_y(const tfield<T>& f,const unsigned int& shift){
  tfield<T> ans(f.getnodes(),f.getsize());
  utriad nodes = f.getnodes();
  unsigned int pshift=shift,offset;
  while (pshift>nodes.y()) pshift-=nodes.y();
  offset = nodes.y() - pshift;
  for(unsigned int idz=0;idz<nodes.z();idz++){
    for(unsigned int idy=0;idy<offset;idy++)
      for(unsigned int idx=0;idx<nodes.x();idx++) 
        ans[rcl::utriad(idx,idy+pshift,idz)] = f[rcl::utriad(idx,idy,idz)];
    for(unsigned int idy=offset;idy<nodes.y();idy++)  
      for(unsigned int idx=0;idx<nodes.x();idx++) 
        ans[rcl::utriad(idx,idy-offset,idz)] = f[rcl::utriad(idx,idy,idz)];
  }
  return ans;
  }

template <typename T> tfield<T> shift_z(const tfield<T>& f,const unsigned int& shift){
  tfield<T> ans(f.getnodes(),f.getsize());
  utriad nodes = f.getnodes();
  unsigned int pshift=shift,offset;
  while (pshift>nodes.z()) pshift-=nodes.z();
  offset = nodes.z() - pshift;
  for(unsigned int idz=0;idz<offset;idz++)
    for(unsigned int idy=0;idy<nodes.y();idy++)
      for(unsigned int idx=0;idx<nodes.x();idx++)
        ans[rcl::utriad(idx,idy,idz+pshift)] = f[rcl::utriad(idx,idy,idz)];
  for(unsigned int idz=offset;idz<nodes.z();idz++)
    for(unsigned int idy=0;idy<nodes.y();idy++)
      for(unsigned int idx=0;idx<nodes.x();idx++)
        ans[rcl::utriad(idx,idy,idz-offset)] = f[rcl::utriad(idx,idy,idz)];
  
  return ans;
  }

template <typename T> tfield<T> diffx(const tfield<T>& f){
  tfield<T> ans(f.getnodes(),f.getsize());
  float delta = f.getsize().x();
  utriad nodes = f.getnodes();
  /* Interior */
  for(unsigned int idz=0;idz<nodes.z();idz++)
    for(unsigned int idy=0;idy<nodes.y();idy++)
      for(unsigned int idx=1;idx<nodes.x()-1;idx++){
          ans[utriad(idx,idy,idz)]=f[utriad(idx+1,idy,idz)]-f[utriad(idx-1,idy,idz)];
          ans[utriad(idx,idy,idz)]/=(2.*delta);
        }
  /* Bordes */
  for (unsigned int idz=0;idz<nodes.z();idz++)
    for(unsigned int idy=0;idy<nodes.y();idy++){
      ans[utriad(0,idy,idz)]=f[utriad(1,idy,idz)]-f[utriad(0,idy,idz)];      
      ans[utriad(0,idy,idz)]/=delta;
    }
  for (unsigned int idz=0;idz<nodes.z();idz++)
    for(unsigned int idy=0;idy<nodes.y();idy++){
      ans[utriad(nodes.x()-1,idy,idz)]=f[utriad(nodes.x()-1,idy,idz)]-f[utriad(nodes.x()-2,idy,idz)];      
      ans[utriad(nodes.x()-1,idy,idz)]/=delta;
    }  
  return ans;  
  }
template <typename T> tfield<T> diffy(const tfield<T>& f){
  tfield<T> ans(f.getnodes(),f.getsize());
  float delta = f.getsize().x();
  utriad nodes = f.getnodes();
  /* Interior */
  for (unsigned int idz=0;idz<nodes.z();idz++)
    for(unsigned int idy=1;idy<nodes.y()-1;idy++)
      for (unsigned int idx=0;idx<nodes.x();idx++){
          ans[utriad(idx,idy,idz)]=f[utriad(idx,idy+1,idz)]-f[utriad(idx,idy-1,idz)];
          ans[utriad(idx,idy,idz)]/=(2.*delta);
        }
  /* Bordes */
  for (unsigned int idz=0;idz<nodes.z();idz++)
    for(unsigned int idx=0;idx<nodes.x();idx++){
      ans[utriad(idx,0,idz)]=f[utriad(idx,1,idz)]-f[utriad(idx,0,idz)];      
      ans[utriad(idx,0,idz)]/=delta;
    }
  for (unsigned int idz=0;idz<nodes.z();idz++)
    for(unsigned int idx=0;idx<nodes.x();idx++){
      ans[utriad(idx,nodes.y()-1,idz)]=f[utriad(idx,nodes.y()-1,idz)]-f[utriad(idx,nodes.y()-2,idz)];      
      ans[utriad(idx,nodes.y()-1,idz)]/=delta;
    }
  return ans;  
  }

template <typename T> void normalize(tfield<T>& f){
  for (unsigned int idx=0;idx<f.size();idx++) if (f[idx].norm2()!=0) f[idx].normalize();
  return;  
}

template <typename T> typename T::basetype get_normpeak(tfield<T>& f){
  typename T::basetype peak=0;
  for (unsigned int idx=0;idx<f.size();idx++) if (f[idx].norm2()>peak) peak=f[idx].norm2();
  return std::sqrt(peak);  
}
template <typename T> void normpeak(tfield<T>& f){
  f/=get_normpeak(f);
  return;  
}



template <typename T> T tfield<T>::operator()(const float& x,const float& y,const unsigned int& layer)const{
    const typename T::basetype& unit=1.;
    T ans;
    float px = x - std::floor(x);
    float py = y - std::floor(y);
    unsigned int fx = (unsigned int) std::floor(x);
    unsigned int fy = (unsigned int) std::floor(y);
    unsigned int cx = (unsigned int) std::ceil(x);
    unsigned int cy = (unsigned int) std::ceil(y);
    T p00 = (*this)[utriad(fx,fy,layer)];
    T p10 = (*this)[utriad(cx,fy,layer)];
    T p01 = (*this)[utriad(fx,cy,layer)];
    T p11 = (*this)[utriad(cx,cy,layer)];
    ans = p00*((unit-px)*(unit-py)) + p01*((unit-px)*py) + p10*(px*(unit-py)) + p11*(px*py);
    ans.normalize();
    return ans;
    }  


template <typename T>  void tfield<T>::reg_meta(const std::string& key,const std::string& val){ meta[key]=val;}
template <typename T>  bool tfield<T>::is_meta(const std::string& key)const{if (meta.find(key)!=meta.end()) return true; return false;}
template <typename T>  std::string tfield<T>::get_meta(const std::string& key)const {
    if (is_meta(key)) return meta.find(key)->second; 
      return std::string();}
template <typename T> template <typename Q> Q tfield<T>::get_val(const std::string& key)const{
    Q ans;
    std::stringstream(get_meta(key)) >> ans;
    return ans;
    }
template <typename T>  void tfield<T>::parse_meta(){
    utriad n;
    ftriad c;
    std::stringstream(get_meta("xnodes")) >> n.x();
    std::stringstream(get_meta("ynodes")) >> n.y();
    std::stringstream(get_meta("znodes")) >> n.z();
    std::stringstream(get_meta("xstepsize")) >> c.x();
    std::stringstream(get_meta("ystepsize")) >> c.y();
    std::stringstream(get_meta("zstepsize")) >> c.z();
    fit(n,c);
    return;
  }
template <typename T>  tfield<T>& tfield<T>::operator+=(const tfield& rhs){
    unsigned int limit = minsize(rhs);
    for (unsigned int idx=0;idx<limit;idx++) data[idx]+=rhs[idx];
    return *this;
    }
template <typename T>  tfield<T>& tfield<T>::operator-=(const tfield& rhs){
    unsigned int limit = minsize(rhs);
    for (unsigned int idx=0;idx<limit;idx++) data[idx]-=rhs[idx];
    return *this;
    }
template <typename T>  tfield<T>& tfield<T>::operator^=(const tfield& rhs){
    unsigned int limit = minsize(rhs);
    for (unsigned int idx=0;idx<limit;idx++) data[idx]^=rhs[idx];
    return *this;
    }
template <typename T>  tfield<T>& tfield<T>::operator^=(const T& rhs){
    unsigned int limit = size();
    for (unsigned int idx=0;idx<limit;idx++) data[idx]^=rhs;
    return *this;
  }
template <typename T>  tfield<T>& tfield<T>::operator&=(const tfield& rhs){
    unsigned int limit = minsize(rhs);
    for (unsigned int idx=0;idx<limit;idx++) data[idx]&=rhs[idx];
    return *this;
    }
template <typename T>  tfield<T>& tfield<T>::operator*=(const typename T::basetype& rhs){
    unsigned int limit = size();
    for (unsigned int idx=0;idx<limit;idx++) data[idx]*=rhs;
    return *this;
    }
template <typename T>  tfield<T>& tfield<T>::operator/=(const typename T::basetype& rhs){
    unsigned int limit = size();
    for (unsigned int idx=0;idx<limit;idx++) data[idx]/=rhs;
    return *this;
    }
template <typename T>  tfield<rcl::tscalar<T> > operator*(const tfield<rcl::tvector<T> >& lhs,const tfield<rcl::tvector<T> >& rhs){
  unsigned int limit = lhs.minsize(rhs);
  tfield<rcl::tscalar<T> > ans(lhs.getnodes(),lhs.getsize());
  for (unsigned int idx=0;idx<limit;idx++) ans[idx]=lhs[idx]*rhs[idx];
  return ans;
  }

typedef tfield<rcl::fvector> ffield;
typedef tfield<rcl::fscalar> fsfield;



}
#endif

