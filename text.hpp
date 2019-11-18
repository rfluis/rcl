#ifndef TEXT_HEADER
#define TEXT_HEADER

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>

namespace rcl{
typedef std::vector<std::string> stringvector;
typedef std::map<std::string,std::string> stringmap;
typedef std::map<std::string,stringvector> stringbook;
typedef std::map<std::string,bool> stringcheck; 
typedef std::vector<unsigned char> bytevector;
typedef bytevector::const_iterator bv_citr;
typedef unsigned char* bytep;

class sorted_list{
public:
  typedef std::string key_type;
  typedef std::vector<key_type> storage_type;
  typedef typename storage_type::size_type size_type;
private:
  storage_type data;
  bool is_sorted = false;
public:
  sorted_list(){ is_sorted=false; return;}
  sorted_list(const storage_type& orig){data=orig; sort();return;}
  void sort(){if (is_sorted) return; std::sort(data.begin(),data.end()); is_sorted=true; return;}
  void add(const key_type& key){if (!exists(key)) push(key); return;}
  bool exists(const key_type& key){sort(); return std::binary_search(data.begin(),data.end(),key); }
  void push(const key_type& key) {data.push_back(key);}
};
  
void    dump(std::string& dst, const bytevector& src);  
bv_citr dump(std::string& dst, const bv_citr& first, const bv_citr& last);
bytep   dump(std::string& dst, const bytep first, const bytep last);
void    dump(bytevector&  dst, const std::string& src);
void    dump(bytevector&  dst, const std::string::const_iterator& first, const std::string::const_iterator& last);
void    dump(bytevector&  dst, const bytep first, const bytep last);
void    dump(stringmap&   dst, const bytevector& src);
void    dump(stringmap&   dst, const stringvector& src);
void    dump(stringvector& dst,const bytevector& src);
void    dump(stringvector& dst,const stringmap& src);
void    dump(bytevector&  dst, const stringvector& src);
void    dump(bytevector&  dst, const stringmap& src);

void parsetextfile(stringvector& dst,const bytevector& src);

template <typename T> 
inline std::string to_str(const T& v){std::stringstream ss; ss << v; return ss.str();}

}  
  

#endif
