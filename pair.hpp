#ifndef PAIR_HEADER
#define PAIR_HEADER

namespace rcl{
  
template <typename T,typename Q> class pair{
  public:
  T first;
  Q second;
  pair(){return;}
  pair(const T& x,const Q& y){first=x; second=y; return;}
  T& x() {return first;}
  Q& y() {return second;}
  const T& x()const {return first;}
  const Q& y()const {return second;}
  bool operator==(const pair& rhs){
    if (this->first==rhs.first) if (this->second==rhs.second) return true;
    return false;
    }
};

typedef rcl::pair<unsigned int,unsigned int> upair;

}


#endif
