
#ifndef TINPUT_HEADER
#define TINPUT_HEADER

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace rcl{
template <typename T> inline void readfrombuffer(const std::vector<unsigned char>& buffer,std::string& name,T& value){
  std::vector<unsigned char>::const_iterator cursor; 
  cursor = search (buffer.begin(),buffer.end(),name.begin(),name.end());
  cursor = cursor + name.length();
  std::string data;
  while(*cursor!='\n') { if (*cursor!=':') data.push_back(*cursor); cursor++; }
  std::stringstream mystream(data);
  mystream >> value;
  return;
}
template <typename T> inline void readfrombuffer(const std::vector<unsigned char>& buffer,const char *name,T& value){
  std::string namestring(name);
  readfrombuffer(buffer,namestring,value);
  return;
}
template <typename T> inline void stringtovalue(T& value,const std::string& str){
  std::stringstream(str) >> value; return;
}


  } // end namespace rcl
#endif
