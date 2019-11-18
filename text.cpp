#include "text.hpp"

namespace rcl{
/*Assembly order:
 * void dump( DESTINATION, SOURCE )*/

void    dump(std::string& dst, const bytevector& src){
  unsigned int size;
  if (dst.size()<4) return;
  size = reinterpret_cast<const unsigned int&>(src[0]); 
  if (dst.size()>=4+size){
    dst.insert(dst.end(),src.begin()+4,src.begin()+4+size);
    return;
    }
  else{
    dst.insert(dst.end(),src.begin()+4,src.end());
    }
  return;
  }
  
bv_citr dump(std::string& dst, const bv_citr& first, const bv_citr& last){
  bv_citr cursor;
  unsigned int size;
  
  if (std::distance(first,last)<4) return last;
  size = reinterpret_cast<const unsigned int&>(*first); 
  if (std::distance(first,last)>=4+size) cursor = first+4+size; 
  else cursor = last;
  dst.insert(dst.end(),first+4,cursor);
  return cursor;
  }

bytep   dump(std::string& dst, const bytep first, const bytep last){
  bytep cursor;
  unsigned int size;
  
  if (std::distance(first,last)<4) return last;
  size = reinterpret_cast<const unsigned int&>(*first); 
  if (std::distance(first,last)>=4+size) cursor = first+4+size; 
  else cursor = last;
  dst.insert(dst.end(),first+4,cursor);
  return cursor;
  }
  
void    dump(bytevector&  dst, const std::string& src){
  unsigned int size = src.length();
  dst.insert(dst.end(),reinterpret_cast<const unsigned char*>(&size),reinterpret_cast<const unsigned char*>(&size)+4);
  dst.insert(dst.end(),src.begin(),src.end());
  return;
  }
  
void    dump(bytevector&  dst, const std::string::const_iterator& first, const std::string::const_iterator& last){
  unsigned int size = std::distance(first,last);
  dst.insert(dst.end(),reinterpret_cast<const unsigned char*>(&size),reinterpret_cast<const unsigned char*>(&size)+4);
  dst.insert(dst.end(),first,last);
  return;
  }
  
void    dump(bytevector&  dst, const bytep first, const bytep last){
  unsigned int size = std::distance(first,last);
  dst.insert(dst.end(),reinterpret_cast<const unsigned char*>(&size),reinterpret_cast<const unsigned char*>(&size)+4);
  dst.insert(dst.end(),first,last);
  return;
  }
  
void    dump(stringmap&   dst, const bytevector& src){
  bytevector::const_iterator itr = src.begin();
  while (itr!=src.end()){
    std::string key,val;
    itr = dump(key,itr,src.end());
    if (itr==src.end()) return;
    itr = dump(val,itr,src.end());
    dst[key]=val;
    }
    return;
  }
  
void    dump(stringmap&   dst, const stringvector& src){
  stringvector::const_iterator end = src.end();
  if (src.size()&1) end--;
  for (stringvector::const_iterator itr=src.begin();itr!=end;) dst[*itr++]=*itr++;
  return;
  }
  
void    dump(stringvector& dst, const bytevector& src){
  bytevector::const_iterator itr = src.begin();
  while (itr!=src.end()){
    std::string str;
    itr = dump(str,itr,src.end());
    dst.push_back(str);
    }
    return;
  }
  
void    dump(stringvector& dst, const stringmap& src){
  for (stringmap::const_iterator itr=src.begin();itr!=src.end();itr++){ 
    dst.push_back(itr->first); 
    dst.push_back(itr->second); }
  return;
  }
void    dump(bytevector& dst , const stringvector& src){
  for (stringvector::const_iterator itr = src.begin(); itr!=src.end();itr++) dump(dst,*itr);
  return;
  }
void    dump(bytevector& dst , const stringmap& src){
  for (stringmap::const_iterator itr = src.begin(); itr!=src.end() ; itr++) {dump(dst,itr->first); dump(dst,itr->second);}
  return;
  }

void parsetextfile(stringvector& dst,const bytevector& src){
  std::string line;
  for (unsigned int idx=0;idx<src.size();idx++){
    if (src[idx]=='\n') {dst.push_back(line); line=std::string();}
    else line.push_back(src[idx]);
    }
  return;
  }


}


