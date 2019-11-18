#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>

template <typename T> class tvector{
  public:
  typedef tvector<T> self;
  T x,y,z;
  tvector():x(0),y(0),z(0){return;}
  tvector(const T& ox):x(ox),y(ox),z(ox){return;}
  tvector(const T& ox,const T& oy,const T& oz):x(ox),y(oy),z(oz){return;}
  tvector& operator+=(const tvector& rhs){ x+=rhs.x; y+=rhs.y; z+=rhs.z; return *this;}
  tvector& operator-=(const tvector& rhs){ x-=rhs.x; y-=rhs.y; z-=rhs.z; return *this;}
  tvector& operator&=(const tvector& rhs){ x*=rhs.x; y*=rhs.y; z*=rhs.z; return *this;}
  tvector& operator/=(const tvector& rhs){ x/=rhs.x; y/=rhs.y; z/=rhs.z; return *this;}
  tvector operator-(){return tvector(-x,-y,-z);}
  T norm2()const{return (x*x)+(y*y)+(z*z);}
};

template <typename T> tvector<T> operator+(tvector<T> lhs,const tvector<T>& rhs){ return lhs+=rhs;}
template <typename T> tvector<T> operator-(tvector<T> lhs,const tvector<T>& rhs){ return lhs-=rhs;}
template <typename T> tvector<T> operator&(tvector<T> lhs,const tvector<T>& rhs){ return lhs&=rhs;}
template <typename T> tvector<T> operator/(tvector<T> lhs,const tvector<T>& rhs){ return lhs/=rhs;}

template <typename T> inline tvector<T> cforce(const tvector<T>& r){
  return r/std::pow(r.norm2(),1.5);
  }

template <typename T> inline tvector<T> dforce(const tvector<T>& v){
  return -v;
  }

inline  unsigned int log2floor(const unsigned int& x){
  return static_cast<unsigned int>(std::floor(std::log2(x)));
}

template<typename T> void sumplex(std::vector<T>& data){
  std::size_t size = data.size();
  std::size_t ppot = 1;
  unsigned int llog = log2floor(size);
  for (unsigned int idx=0;idx<llog;idx++) ppot*=2;
  if (size!=ppot) for (unsigned int idx=ppot;idx<size;idx++) data[idx-ppot]+=data[idx];
  while (ppot!=1){
    ppot/=2; 
    #pragma omp parallel for
    for (unsigned int idx=0;idx<ppot;idx++) data[idx]+=data[idx+ppot];
  }
  return;
}
  
template <typename T> inline tvector<T> cforce2d(const tvector<T>& r,const int& rep){
  std::vector<typename tvector<T>::self> ans;
  for (int idy=-rep;idy<=rep;idy++)
  for (int idx=-rep;idx<=rep;idx++)
  ans.push_back(cforce(r+tvector<T>(idx,idy,0)));
  sumplex(ans);
  return ans[0];  
  }
  
template <typename T> inline tvector<T> cforce2d_self(const tvector<T>& r,const int& rep){
  std::vector<typename tvector<T>::self> ans;
  for (int idy=-rep;idy<=rep;idy++)
  for (int idx=-rep;idx<=rep;idx++)
  if (idx!=0 || idy!=0) ans.push_back(cforce(r+tvector<T>(idx,idy,0)));
  sumplex(ans);
  return ans[0];  
  }

template <typename T> class state{
  public:
  typedef state<T> self;
  typedef tvector<T> member;
  tvector<T> r,v;
};

template <typename T> class system{
  public:
  int size=0;
  std::vector<typename state<T>::self> pos,k1,k2,k3,k4;
  void push_back(const state& p){
    pos.push_back(p);
    k1.push_back(state());
    k2.push_back(state());
    k3.push_back(state());
    k4.push_back(state());
    size++;
    return;
    }
  void update(){
    return;
    }
  
  void onestep(){
    for (unsigned int particle=0;particle<size;particle++){
      tvector<T> dis = dforce(pos[particle].v;
      tvector<T> coul;
    for (unsigned int neighbor=0;neighbor<size;neighbor++){
      if (neighbor==particle) coul+=cforce2d_self(pos[particle].r-pos[neighbor].r,16);
      else                    coul+=cforce2d(pos[particle].r-pos[neighbor].r,16);
      }
    }
    }
  
}
  
int main()
{
  unsigned int size = 20;
  double ang=6.28318530717958647692528676655900576839433879875021/(size+1);
  for (unsigned int idx=0;idx<size;idx++){
    state p;
    p.r=tvector<double>(0.5,0.5,0)+0.1*tvector<double>(std::cos(ang*idx),std::sin(ang*idx),0);
    system.push_back(p);
    }
  return 0;
}
