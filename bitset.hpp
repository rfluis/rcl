#ifndef BITSET_HEADER
#define BITSET_HEADER
#include "binpots.hpp"

namespace rcl{
template <unsigned int N> class bitset{
private:
  unsigned int x;
  class ref{
    public:
    ref(bitset& ob,const unsigned int& om): master(ob),mask(om){/*std::cout << "{ref = "<< ob << ":" << master<< "," << om << ":" << mask<<"}";*/return;}
    private:
    const unsigned int mask;  bitset& master;
    public:
    ref& operator=(const bool& rhs){ 
	    if (rhs) master.apply(mask);
	    else     master.erase(mask);
	    return *this;}
    operator const bool () const {/*std::cout << "["<<master<<" & "<<mask<<"]";*/return (master.check(mask));}
    };
public:
  bitset():x(0){return;}
  bitset(const unsigned int& k):x(k) { /* x=k&msk<N>;*/ }
  const unsigned int& data()const { return x;}
  template <unsigned int y> bitset(const bitset<y>& k):x(k){/*x=k.x&minmsk<N,y>;*/ return;}
  bool check(const unsigned int& mask) const { if (x&mask) return true; else return false;}
  bool get(const unsigned int& k) const {return x&(msk<N-1> >> k);}
  bool first() { return (x&pot<N>);}
  operator const unsigned int& () const { return x;}
  void invert() { x = ~x;}
  void apply(const unsigned int& mask) {x=x|mask; return;}
  void erase(const unsigned int& mask) {x=x&(~mask); return;}
  template <unsigned int X> void write() {x|=pot<X>; return;}
  template <unsigned int X> void erase() {x&=npot<X>; return;}
  //bool operator [](const unsigned int& k) const { return x&(msk<N-1> >> k);}
  ref operator[](const unsigned int& k) {return ref(*this,pot<N-1> >> k);}
};


}
#endif
