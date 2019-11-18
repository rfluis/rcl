#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include <vector>
#include "tpoint.hpp"
#include "triad.hpp"
#include "numerical.hpp"

namespace rcl{
class pixel32{
private:
  unsigned char rr,gg,bb,aa;
public:
  pixel32() {reinterpret_cast<unsigned int&>(*this)=0; return;}
  pixel32(const unsigned int& p) {reinterpret_cast<unsigned int&>(*this)=p;}
  pixel32(const unsigned char& rr,const unsigned char& gg,const unsigned char& bb) { set(rr,gg,bb); return;}
  pixel32(const unsigned char& rr,const unsigned char& gg,const unsigned char& bb,const unsigned char& aa) { set(rr,gg,bb,aa); return;}

  unsigned char& r() { return rr;}
  const unsigned char& r() const { return rr;}
  unsigned char& g() { return gg;}
  const unsigned char& g() const { return gg;}
  unsigned char& b() { return bb;}
  const unsigned char& b() const { return bb;}
  unsigned char& a() { return aa;}
  const unsigned char& a() const { return aa;}
  unsigned int& P()  { return reinterpret_cast<unsigned int&>(*this);}
  const unsigned int& P() const { return reinterpret_cast<const unsigned int&>(*this);}
  void set(const unsigned int& x) {reinterpret_cast<unsigned int&>(*this)=x; return;}
  void set(const unsigned char& r,const unsigned char& g,const unsigned char& b){set(r,g,b,255);return;}
  void set(const unsigned char& r,const unsigned char& g,const unsigned char& b,const unsigned char& a){
    this->rr=r;    this->gg=g;    this->bb=b;    this->aa=a;    return;    }
  void blend(const pixel32& fg){
    unsigned int d,t;
    d = 255*fg.aa + aa*(255-fg.aa);
    if (d==0) return;
    t = 255*fg.rr*fg.aa + rr*aa*(255-fg.aa);
    rr = static_cast<unsigned char>(t/d);
    t = 255*fg.gg*fg.aa + gg*aa*(255-fg.aa);
    gg = static_cast<unsigned char>(t/d);
    t = 255*fg.bb*fg.aa + bb*aa*(255-fg.aa);
    bb = static_cast<unsigned char>(t/d);
    aa = static_cast<unsigned char>(d/255);
    return;
  }
  unsigned int* getiptr() { return reinterpret_cast<unsigned int*>(this);}
  const unsigned int* getiptr() const { return reinterpret_cast<const unsigned int*>(this);}
  unsigned char* getbptr() { return reinterpret_cast<unsigned char*>(this);}
  const unsigned char* getbptr() const { return reinterpret_cast<const unsigned char*>(this);}
};

inline bool operator ==(const rcl::pixel32& a,const rcl::pixel32& b){return (a.P()==b.P());}
inline bool operator !=(const rcl::pixel32& a,const rcl::pixel32& b){return (a.P()!=b.P());}
inline bool operator  >(const rcl::pixel32& a,const rcl::pixel32& b){return (a.P()>b.P());}
inline bool operator  <(const rcl::pixel32& a,const rcl::pixel32& b){return (a.P()<b.P());}
inline bool operator >=(const rcl::pixel32& a,const rcl::pixel32& b){return (a.P()>=b.P());}
inline bool operator <=(const rcl::pixel32& a,const rcl::pixel32& b){return (a.P()<=b.P());}


template<typename T>
class tbitmap{
private:
  union{
    rcl::upoint sz;
    unsigned int xx,yy;
    };
  std::vector<T> d;
public:
  tbitmap() {return;}
  tbitmap(const unsigned int& x) {fit(x,x); return;}
  tbitmap(const unsigned int& x,const unsigned int& y) { fit(x,y); return;}
  tbitmap(const upoint& p){ fit(p); return;}
  tbitmap(const tbitmap& src){this->sz=src.sz;this->d=src.d;return;}
  void fit (const unsigned int& x,const unsigned int& y) {this->sz.x()=x;this->sz.y()=y;d.resize(sz.x()*sz.y());return;}
  void fit (const upoint& p){fit(p.x(),p.y()); return;} 
  
  T& operator[](const unsigned int& x) { return d[x];}
  const T& operator[](const unsigned int& x)const { return d[x];}
  
  T& operator[](const rcl::upoint& p) { return d[p.x()+p.y()*xx];}
  const T& operator[](const rcl::upoint& p)const { return d[p.x()+p.y()*xx];}
  
  T& operator()(const unsigned int& x) { return d[x];}
  const T& operator()(const unsigned int& x)const { return d[x];}
  
  T& operator()(const unsigned int& x,const unsigned int& y) {return d[x+(y*xx)];}
  const T& operator()(const unsigned int& x,const unsigned int& y)const {return d[x+(y*xx)];}
  
  T& operator()(const rcl::upoint& p) { return d[p.x()+p.y()*xx];}
  const T& operator()(const rcl::upoint& p)const { return d[p.x()+p.y()*xx];}
  
  void pset(const unsigned int& x,const unsigned int& y,const T& c){ d[x+(y*xx)]=c; return;}
  const T& pget(const unsigned int& x,const unsigned int& y)const {return d[x+(y*xx)];}
  unsigned int size () const {return d.size();}
  unsigned int GetX() const {return sz.x();}
  unsigned int GetY() const {return sz.y();}
  rcl::upoint GetXY() const {return sz;}
  unsigned char* getptr () { return reinterpret_cast<unsigned char*>(&d[0]); }
  const unsigned char* getptr () const {return reinterpret_cast<const unsigned char*>(&d[0]); }
};

class image32{
private:
  unsigned int xx,yy;
  std::vector<pixel32> d;
public:
  image32(){fit(1,1);return;}
  image32(const unsigned int& x) {fit(x,x); return;}
  image32(const unsigned int& x,const unsigned int& y) { fit(x,y); return;}
  image32(const upoint& p){ fit(p); return;}
  image32(const image32& src){this->xx=src.xx;this->yy=src.yy;this->d=src.d;return;}
  void fit (const unsigned int& x,const unsigned int& y) {this->xx=x;this->yy=y;d.resize(xx*yy);return;}
  void fit (const upoint& p){fit(p.x(),p.y()); return;} 
  
  pixel32& operator[](const unsigned int& x) { return d[x];}
  const pixel32& operator[](const unsigned int& x)const { return d[x];}
  
  pixel32& operator[](const rcl::upoint& p) { return d[p.x()+p.y()*xx];}
  const pixel32& operator[](const rcl::upoint& p)const { return d[p.x()+p.y()*xx];}
  
  pixel32& operator()(const unsigned int& x) { return d[x];}
  const pixel32& operator()(const unsigned int& x)const { return d[x];}
  
  pixel32& operator()(const unsigned int& x,const unsigned int& y) {return d[x+(y*xx)];}
  const pixel32& operator()(const unsigned int& x,const unsigned int& y)const {return d[x+(y*xx)];}
  
  pixel32& operator()(const rcl::upoint& p) { return d[p.x()+p.y()*xx];}
  const pixel32& operator()(const rcl::upoint& p)const { return d[p.x()+p.y()*xx];}
  
  void pset(const unsigned int& x,const unsigned int& y,const rcl::pixel32& c){ d[x+(y*xx)]=c; return;}
  const pixel32& pget(const unsigned int& x,const unsigned int& y)const {return d[x+(y*xx)];}
  unsigned int size () const {return d.size();}
  unsigned int GetX() const {return xx;}
  unsigned int GetY() const {return yy;}
  rcl::upoint GetXY() const {return rcl::upoint(xx,yy);}
  rcl::upair GetSize() const {return rcl::upair(xx,yy);}
  unsigned char* getptr () { return reinterpret_cast<unsigned char*>(&d[0]); }
  const unsigned char* getptr () const {return reinterpret_cast<const unsigned char*>(&d[0]); }
  
  image32 crop (const upoint& src,const upoint& csize) const ;
  image32 flip () const ;   // up - down
  image32 mirror () const ; // left - right
  image32 resize_nn(const unsigned int& x,const unsigned int& y) const;
  void put(const image32& sprite,const upoint& pos);
  void blend(const image32& sprite,const upoint& pos);
  
  void hline(const unsigned int& x1,const unsigned int& x2, const unsigned int& y,const rcl::pixel32& c);
  void vline(const unsigned int& x,const unsigned int& y1, const unsigned int& y2,const rcl::pixel32& c);
  void line(const unsigned int& x1,const unsigned int& y1,const unsigned int& x2,const unsigned int& y2, const rcl::pixel32& color);
  void circle(const unsigned int& xc,const unsigned int& yc,const unsigned int& radius,const rcl::pixel32& color);
  void disk(const unsigned int& xc,const unsigned int& yc,const unsigned int& radius,const rcl::pixel32& color);
  void rect(const unsigned int& x1,const unsigned int& y1,const unsigned int& x2,const unsigned int& y2,const rcl::pixel32& color);
 };

pixel32 HSLtoRGB(const ftriad& color);
ftriad RGBtoHSL(const pixel32& color); 
inline pixel32 HSLtoRGB(const float& h,const float& s,const float& l){ return HSLtoRGB(ftriad(h,s,l));}
pixel32 France(const float& x);
pixel32 GrayScale(const float& x);
pixel32 UnitGrayScale(const float& x);





}//end namespace rcl

#endif
