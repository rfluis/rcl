template <typename T,unsigned int N> class array;

template <typename T,unsigned int N> class array{
private:
  T elem;
  array<T,N-1> tail;
  template <unsigned int X> class elem_ref{
  public:
    elem_ref(array<T,N>& aref): ref(aref){return;}
  private:
    array<T,N>& ref;
  public:
    elem_ref& operator=(const T&){ref.elem;}
    };
  template <>
public:
  array(){return;}
  template <unsigned int X> const T& get()const;
  template <unsigned int X> T& get();
};

template <typename T> class array<T,0>{
  private:
  T elem;
  public:
  array(){return;}
};

#include <iostream>

void list(const array<int,static_cast<unsigned int>(10)>& x){
  std::cout << 0 << " : " << x.get<0>() << std::endl;
  std::cout << 1 << " : " << x.get<1>() << std::endl;
  std::cout << 2 << " : " << x.get<2>() << std::endl;
  std::cout << 3 << " : " << x.get<3>() << std::endl;
  std::cout << 4 << " : " << x.get<4>() << std::endl;
  std::cout << 5 << " : " << x.get<5>() << std::endl;
  std::cout << 6 << " : " << x.get<6>() << std::endl;
  std::cout << 7 << " : " << x.get<7>() << std::endl;
  std::cout << 8 << " : " << x.get<8>() << std::endl;
  std::cout << 9 << " : " << x.get<9>() << std::endl;
  
  }
int main(){
  array<int,static_cast<unsigned int>(10)> j;
  std::cout << " Size of j = " << sizeof(j) << std::endl;
  j.get<0>()=2;
  j.get<1>()=3;
  j.get<2>()=5;
  j.get<3>()=7;
  j.get<4>()=11;
  j.get<5>()=13;
  j.get<6>()=17;
  j.get<7>()=19;
  j.get<8>()=23;
  j.get<9>()=29;
  list(j);
  }
