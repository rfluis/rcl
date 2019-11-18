// Use functor class as element reference 
// functor class allows partial especialization

template <typename T,unsigned int N> class array;

template <typename T,unsigned int N> class array{
private:
  T elem;
  array<T,N-1> tail;
public:
  array(){return;}
  template <unsigned int X> const T& get()const;
  template <unsigned int X> T& get();
};

template <typename T,unsigned int N> template <unsigned int X> const T& array<T,N>::get() const {if (X==0) return elem; else return tail.get<X-1>();}
template <typename T,unsigned int N> template <unsigned int X> T& array<T,N>::get(){if (X==0) return elem; else return tail.get<X-1>();}

template <typename T> class array<T,0>{
  private:
  T elem;
  public:
  array(){return;}
  template <unsigned int X> const T& get();
  template <unsigned int X> T& get();
};

//template <typename T> template <unsigned int X> const T& array<T,0>::get<X>()const {return elem;}
//template <typename T> template <unsigned int X> T& array<T,0>::get<X>() {return elem;}
