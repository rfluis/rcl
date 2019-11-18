#ifndef RCL_TABLES_HEADER
#define RCL_TABLES_HEADER

#include <vector>
#include <string>
#include <sstream>
#include "tpoint.hpp"


namespace rcl{


  
template <typename T>
class table{
private:
  rcl::upoint size;
  std::vector<T> data;
  class sizeref{
  public:
    sizeref(table& orig):master(orig){return;}
  private:
    table& master;
  public:
    table& operator=(const rcl::upoint& ns){master.fit(ns);}
    operator const rcl::upoint&(){return master.getsize();}
  };
public:
  table(){return;}
  sizeref getsize(){return sizeref(*this);}
  const rcl::upoint& getsize()const{return size;}
  void fit(const rcl::upoint& ns){size = ns; fit(); return;}
  void fit(){data.resize(size.x()*size.y()); return;}
  
};
  
}

#endif
