#ifndef REVERSE_HEADER
#define REVERSE_HEADER

namespace rcl{

inline double endian(const double& k){
  struct {unsigned char a,b,c,d,e,f,g,h;} eight;
  eight.a = reinterpret_cast<decltype(eight) const&>(k).h;
  eight.b = reinterpret_cast<decltype(eight) const&>(k).g;
  eight.c = reinterpret_cast<decltype(eight) const&>(k).f;
  eight.d = reinterpret_cast<decltype(eight) const&>(k).e;
  eight.e = reinterpret_cast<decltype(eight) const&>(k).d;
  eight.f = reinterpret_cast<decltype(eight) const&>(k).c;
  eight.g = reinterpret_cast<decltype(eight) const&>(k).b;
  eight.h = reinterpret_cast<decltype(eight) const&>(k).a;
  return reinterpret_cast<double&>(eight);
}
inline float endian(const float& k){
  struct {unsigned char a,b,c,d;} four;
  four.a = reinterpret_cast<decltype(four) const&>(k).d;
  four.b = reinterpret_cast<decltype(four) const&>(k).c;
  four.c = reinterpret_cast<decltype(four) const&>(k).b;
  four.d = reinterpret_cast<decltype(four) const&>(k).a;
  return reinterpret_cast<float&>(four);
}

inline int endian(const int& k){
  struct {unsigned char a,b,c,d;} four;
  four.a = reinterpret_cast<decltype(four) const&>(k).d;
  four.b = reinterpret_cast<decltype(four) const&>(k).c;
  four.c = reinterpret_cast<decltype(four) const&>(k).b;
  four.d = reinterpret_cast<decltype(four) const&>(k).a;
  return reinterpret_cast<int&>(four);
}
inline unsigned int endian(const unsigned int& k){
  struct {unsigned char a,b,c,d;} four;
  four.a = reinterpret_cast<decltype(four) const&>(k).d;
  four.b = reinterpret_cast<decltype(four) const&>(k).c;
  four.c = reinterpret_cast<decltype(four) const&>(k).b;
  four.d = reinterpret_cast<decltype(four) const&>(k).a;
  return reinterpret_cast<unsigned int&>(four);
}

inline long long int endian(const long long int& k){
  struct {unsigned char a,b,c,d,e,f,g,h;} eight;
  eight.a = reinterpret_cast<decltype(eight) const&>(k).h;
  eight.b = reinterpret_cast<decltype(eight) const&>(k).g;
  eight.c = reinterpret_cast<decltype(eight) const&>(k).f;
  eight.d = reinterpret_cast<decltype(eight) const&>(k).e;
  eight.e = reinterpret_cast<decltype(eight) const&>(k).d;
  eight.f = reinterpret_cast<decltype(eight) const&>(k).c;
  eight.g = reinterpret_cast<decltype(eight) const&>(k).b;
  eight.h = reinterpret_cast<decltype(eight) const&>(k).a;
  return reinterpret_cast<long long int&>(eight);
}
inline unsigned long long int endian(const unsigned long long int& k){
  struct {unsigned char a,b,c,d,e,f,g,h;} eight;
  eight.a = reinterpret_cast<decltype(eight) const&>(k).h;
  eight.b = reinterpret_cast<decltype(eight) const&>(k).g;
  eight.c = reinterpret_cast<decltype(eight) const&>(k).f;
  eight.d = reinterpret_cast<decltype(eight) const&>(k).e;
  eight.e = reinterpret_cast<decltype(eight) const&>(k).d;
  eight.f = reinterpret_cast<decltype(eight) const&>(k).c;
  eight.g = reinterpret_cast<decltype(eight) const&>(k).b;
  eight.h = reinterpret_cast<decltype(eight) const&>(k).a;
  return reinterpret_cast<unsigned long long int&>(eight);
}

template<typename T> class reverse{
private:
  T x;
public:
  reverse(){return;}
  reverse(const T& k){x=endian(k);}
  operator T(){return endian(x);}
};

typedef reverse<float> rfloat;
typedef reverse<double> rdouble;

}



#endif
