#ifndef VSTREAM_HEADER
#define VSTREAM_HEADER

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
namespace rcl{

class cvstream{
public:
  cvstream(const std::vector<unsigned char> &d):data(d){cursor=0;return;}
private:
  const std::vector<unsigned char> &data;
  std::size_t cursor;
public:
  const std::size_t& tell()const {return cursor;}
  void seek(const std::size_t& pos){
    if (pos>size()) cursor=size();
    else cursor=pos;
    return;
    }
  void advance(const std::size_t& offset){
    std::size_t pos = tell();
    pos+=offset;
    seek(pos);
    return;
    }
  void rewind(){ cursor=0; return;}
  std::size_t size() const { return data.size(); }
  std::size_t sread(unsigned char *ptr,std::size_t dsize){
    if (cursor+dsize>size()){
      std::copy(data.begin()+cursor,data.end(),ptr);
      std::size_t written = size()-cursor;
      cursor = data.size();
      return written;
      }
    else{
      std::copy(data.begin()+cursor,data.begin()+cursor+dsize,ptr);
      cursor+=dsize;
      return dsize;        
      }
    return 0;
    }
  bool eof()const{ if (cursor==size()) return true; else return false;}
};
 
class vstream{
public:
  vstream(std::vector<unsigned char> &d):data(d){cursor=0;return;}
private:
  std::vector<unsigned char> &data;
  std::size_t cursor;
public:
  class newline{};
  const std::size_t& tell()const {return cursor;}
  void seek(const std::size_t& pos){
    if (pos>size()) cursor=size();
    else cursor=pos;
    return;
    }
  void advance(const std::size_t& offset){
    std::size_t pos = tell();
    pos+=offset;
    seek(pos);
    return;
    }
  void rewind(){ cursor=0; return;}
  std::size_t size() const { return data.size(); }
  void push_back(const unsigned char& c){data.push_back(c);}
  template <typename InputIterator> 
  void push_back(const InputIterator& it1,const InputIterator& it2){data.insert(data.end(),it1,it2);}
  void push_back(const std::string& str){ push_back(str.begin(),str.end());}
  void write(const unsigned char* ptr,const std::size_t& size){data.insert(data.end(),ptr,ptr+size);}
  template<typename T> void write(const T& data){write(reinterpret_cast<const unsigned char*>(&data),sizeof(T)); return;}
  std::size_t sread(unsigned char *ptr,std::size_t dsize){
    if (cursor+dsize>size()){
      std::copy(data.begin()+cursor,data.end(),ptr);
      std::size_t written = size()-cursor;
      cursor = data.size();
      return written;
      }
    else{
      std::copy(data.begin()+cursor,data.begin()+cursor+dsize,ptr);
      cursor+=dsize;
      return dsize;        
      }
    return 0;
    }
  std::size_t swrite(unsigned char* ptr,std::size_t dsize){
    data.insert(data.begin()+cursor,ptr,ptr+dsize);
    cursor += dsize; 
    return dsize;
    }
  bool eof()const { if (cursor==size()) return true; else return false;}
  operator cvstream() const {return cvstream(data);}
};
    
template <typename T> 
inline vstream& operator <<(vstream &vs,const T& data){   std::stringstream ss;  ss << data;  vs.push_back(ss.str());  return vs; }

inline vstream& operator <<(vstream &vs,const std::string& str){ vs.push_back(str); return vs; }
inline vstream& operator <<(vstream &vs,const unsigned char& c){ vs.push_back(c);   return vs; }
inline vstream& operator <<(vstream &vs,const vstream::newline& k) {vs.push_back('\n'); return vs;}

}

#endif
