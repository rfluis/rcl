#ifndef CACHEROMFS_HEADER
#define CACHEROMFS_HEADER

#include <fstream>
#include <iostream>
#include "text.hpp"
#include "pair.hpp"
#include "files.hpp"
#include "lib-zlib.hpp"

namespace rcl{

void dump(rcl::bytevector& dst,const std::map<std::string,rcl::upair>& src);
void dump(std::map<std::string,rcl::upair>& dst,const rcl::bytevector& src);
bool is_crfs(const std::string& filename);

class cacheromfs_table{
  private:
  std::map<std::string,rcl::upair> table; // string , offset , size
  public:
  cacheromfs_table(){return;}
  cacheromfs_table(const std::map<std::string,rcl::upair>& orig);
  bool check_key(const std::string& key)const;
  rcl::stringvector get_filenames()const;
  rcl::upair get_pair(const std::string& key)const;
  void increment_offset(const unsigned int& bias);
  void decrement_offset(const unsigned int& bias);
};

class icacheromfs{
  private:
  cacheromfs_table table;
  std::string filename;
  public:
  icacheromfs(){return;}
  icacheromfs(const std::string& path){open_file(path); return;}
  void open_file(const std::string& path);
  rcl::stringvector get_filenames()const {return table.get_filenames();}
  bool check_key(const std::string& key)const{return table.check_key(key);}
  void read(rcl::bytevector& dst,const std::string& key)const;
  rcl::bytevector read(const std::string& key)const{
    rcl::bytevector data;
    read(data,key);
    return data;
    }
};

class ocacheromfs_mem{
  private:
  std::map<std::string,rcl::bytevector> table;
  public:
  ocacheromfs_mem(){return;}
  bool check_key(const std::string& key)const;
  void populate_from_file(const std::string& filename);
  void insert_file(const std::string& key,const std::string& filepath);
  void insert_data(const std::string& key,const rcl::bytevector& data);
  void write_to_file(const std::string& filename) const;
};

class ocacheromfs_files{
  private:
  class ocrfsf_entry{
    public:
    std::string key,path;
    unsigned int size;
    };
};
  
}

#endif
