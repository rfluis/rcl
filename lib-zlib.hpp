#ifndef LIB_ZLIB_HEADER
#define LIB_ZLIB_HEADER

#include "vstream.hpp"
#include <zlib.h>

#define CHUNK 16384

namespace rcl{
  
  class deflate_stream{};
  class inflate_stream{};
  
  class zlib_cfg{
  private:
    int level,strategy;
    bool gzip,deflate;
  public:
    zlib_cfg(){level=9;strategy=0;gzip=0;deflate=0; return;}
    zlib_cfg(int level,int strategy,bool gzip,bool deflate){
      if (level<1 || level>9) level=9;
      if (strategy<0 || strategy>4 ) strategy=0;
      if (gzip && deflate) gzip=deflate=0;
      this->level=level;
      this->strategy=strategy;
      this->gzip=gzip;
      this->deflate=deflate;
      return;
    }
  int& getlevel() {return level;}
  const int& getlevel() const {return level;}
  int& getstrategy() {return strategy;}
  const int& getstrategy() const {return strategy;}
  int getzs() const {
    if (strategy==0) return Z_DEFAULT_STRATEGY;
    if (strategy==1) return Z_FILTERED;
    if (strategy==2) return Z_HUFFMAN_ONLY;
    if (strategy==3) return Z_RLE;
    if (strategy==4) return Z_DEFAULT_STRATEGY|Z_FIXED;
    return Z_DEFAULT_STRATEGY;
  }
  bool& getgz() {return gzip;}
  const bool& getgz() const {return gzip;}
  bool& getdf() {return deflate;}
  const bool& getdf() const {return deflate;}
  bool iszl() const { if (gzip==false||deflate==false) return true; else return false;}
  };
  bool is_GZIP(const std::vector<unsigned char>& file);
  void ungzip(std::vector<unsigned char>& file);
  
  int deflate(std::vector<unsigned char>& dst,const std::vector<unsigned char>& src,const rcl::zlib_cfg& cfg);
  int deflate(std::vector<unsigned char>& dst,const std::vector<unsigned char>& src,bool gzip);
  int inflate(std::vector<unsigned char>& dst,const std::vector<unsigned char>& src,bool gzip);
  
}

#endif
