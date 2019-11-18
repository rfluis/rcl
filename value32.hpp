#ifndef VALUE32_HEADER
#define VALUE32_HEADER
#include <string>
#include <algorithm>
#include "vfloat.hpp"

namespace rcl{
class value32{
private:
  union {int sv;unsigned int iv; vfloat fv; unsigned char b[4];};
public:
  value32() {iv=0; return;}
  value32(const float& v){ fv=v; return;}
  value32(const unsigned int& v){iv=v; return;}
  value32(const int& v){sv=v; return;}
  value32(const char* s) {std::copy(&s[0],&s[4],b); return;}
  value32(const std::string& s) {std::copy(&s[0],&s[4],b); return;}
  const unsigned int&  getv() const {return iv;}
  const int&           get()  const {return sv;}
  const float&         getf() const {return fv;}
  const unsigned char& getc0() const {return b[0];}
  const unsigned char& getc1() const {return b[1];}
  const unsigned char& getc2() const {return b[2];}
  const unsigned char& getc3() const {return b[3];}
  std::string gets() const { return std::string(&b[0],&b[4]);}
};
inline bool operator < (const value32& a, const value32& b) { return a.getv()<b.getv(); }
}
  
#endif
