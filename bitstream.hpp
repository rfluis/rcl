#ifndef BITSTREAM_HEADER
#define BITSTREAM_HEADER

#include <vector>
#include <iostream>
#include "binpots.hpp" // Incluida en bitset
#include "bitset.hpp"
#include <string>
namespace rcl{
  
class bitstream{
private:
  std::vector<unsigned char> data;
  unsigned char current,mask;
  class ref{
  public:
    ref(unsigned char& c,const unsigned char& m):current(c),mask(m){return;}
  private:
    unsigned char& current;
    const unsigned char mask;
  public:
    ref& operator =(const bool& bit){if (bit) current|=mask; else current&=(~mask); return *this;}
    operator bool(){ return (mask&current);}
    void flip(){current^=mask; return;}
  };
public:
  bitstream():current(0),mask(128){return;}
  bitstream(const unsigned char* begin,const unsigned int& length):current(0),mask(128),data(begin,begin+length){return;}
  bitstream(const unsigned int& size):data(size,0){return;}
  bitstream(const std::vector<unsigned char>& src):current(0),mask(128),data(src){return;}
  void push(const bool& bit){if (bit) set();  advance(); return;}
  void set(){current|=mask; return;}
  void write(){set(); advance(); return;}
  void advance(){mask>>=1; if(!mask) push_back(); return;}
  void push_back(){data.push_back(current); reset(); return;}
  void reset(){ current=0; mask=128; return;}
  template<int N> void push(const bitset<N>& k){ push<N>(k); return;}
  template<int N> void push(const unsigned int& x){ push_nth<N>(x); push<N-1>(x); return;}
  template<int N> void push_nth(const unsigned int& x){ if (x&pot<N>) write(); else advance(); return;}
  unsigned int bsize()const{ return 8*data.size();}
  unsigned int size()const {return data.size();}
  void resize(const unsigned int& x) { data.resize(x);}
  unsigned char& operator()(const unsigned int& idx){return data[idx];}
  const unsigned char& operator()(const unsigned int& idx)const{return data[idx];}
  ref operator[](const unsigned int& x){ return ref(data[x>>3],128>>(x&7));}
  void writeat(const unsigned int& x){ data[x>>3]|=(128>>(x&7));}
  bool readat(const unsigned int& x)const{return data[x>>3]&(128>>(x&7));}
  unsigned char* begin(){return &(*data.begin());}
  const unsigned char* begin()const{return &(*data.begin());}
  unsigned char* end(){return &(*data.end());}
  const unsigned char* end() const {return &(*data.end());}
  const std::vector<unsigned char>& getvec() const{return data;}
  void setvec(const std::vector<unsigned char>& d){data=d; return;}
  std::string to_binary(const unsigned int& in,const unsigned int& fi){
    std::string ans;
    for (unsigned int idx=in;idx<fi;idx++) if (data[idx>>3]&(128>>(idx&7))) ans.push_back('1'); else ans.push_back('0');
    return ans; 
    }
  std::string to_hex(const unsigned int& in,const unsigned int& fi){
    std::string ans;
    const std::string base("0123456789ABCDEF");
    for (unsigned int idx=in;idx<fi;idx++) 
    {
      ans.push_back(base[data[idx]>>4]);
      ans.push_back(base[data[idx]&15]);
    }
    return ans; 
    }
};

template<> inline void bitstream::push<0>(const unsigned int& x) { push_nth<0>(x); return;}

class bitstream64{
private:
  std::vector<unsigned long long int> data;
  unsigned long long int current,mask;
  class ref{
  public:
    ref(unsigned long long int& c,const unsigned long long int& m):current(c),mask(m){return;}
  private:
    unsigned long long int& current;
    const unsigned long long int mask;
  public:
    ref& operator =(const bool& bit){if (bit) current|=mask; else current&=(~mask); return *this;}
    operator bool(){ return (mask&current);}
    void flip(){current^=mask; return;}
  };
public:
  bitstream64():current(0),mask(9223372036854775808UL){return;}
  bitstream64(const unsigned long long int* begin,const unsigned int& length):current(0),mask(9223372036854775808UL),data(begin,begin+length){return;}
  bitstream64(const unsigned int& size):data(size,0){return;}
  bitstream64(const std::vector<unsigned long long int>& src):current(0),mask(9223372036854775808UL),data(src){return;}
  void push(const bool& bit){if (bit) set();  advance(); return;}
  void set(){current|=mask; return;}
  void write(){set(); advance(); return;}
  void advance(){mask>>=1; if(!mask) push_back(); return;}
  void push_back(){data.push_back(current); reset(); return;}
  void reset(){ current=0; mask=9223372036854775808UL; return;}
  template<int N> void push(const bitset<N>& k){ push<N>(k); return;}
  template<int N> void push(const unsigned int& x){ push_nth<N>(x); push<N-1>(x); return;}
  template<int N> void push_nth(const unsigned int& x){ if (x&pot<N>) write(); else advance(); return;}
  unsigned int bsize()const{ return 64*data.size();}
  unsigned int size()const {return data.size();}
  void resize(const unsigned int& x) { data.resize(x);}
  unsigned long long int& operator()(const unsigned int& idx){return data[idx];}
  const unsigned long long int& operator()(const unsigned int& idx)const{return data[idx];}
  ref operator[](const unsigned int& x){ return ref(data[x>>6],9223372036854775808UL>>(x&63));}
  void writeat(const unsigned int& x){ data[x>>6]|=(9223372036854775808UL>>(x&63));}
  bool readat(const unsigned int& x)const{return data[x>>6]&(9223372036854775808UL>>(x&63));}
  unsigned long long int* begin(){return &(*data.begin());}
  const unsigned long long int* begin()const{return &(*data.begin());}
  unsigned long long int* end(){return &(*data.end());}
  const unsigned long long int* end() const {return &(*data.end());}
  const std::vector<unsigned long long int>& getvec() const{return data;}
  void setvec(const std::vector<unsigned long long int>& d){data=d; return;}
  std::string to_binary(const unsigned int& in,const unsigned int& fi){
    std::string ans;
    for (unsigned int idx=in;idx<fi;idx++) if (data[idx>>6]&(9223372036854775808UL>>(idx&63))) ans.push_back('1'); else ans.push_back('0');
    return ans; 
    }
  std::string to_hex(const unsigned int& in,const unsigned int& fi){
    std::string ans;
    const std::string base("0123456789ABCDEF");
    for (unsigned int idx=in;idx<fi;idx++) 
    {
      ans.push_back(base[data[idx]>>4]);
      ans.push_back(base[data[idx]&15]);
    }
    return ans; 
    }
};

template<> inline void bitstream64::push<0>(const unsigned int& x) { push_nth<0>(x); return;}

}
#endif
