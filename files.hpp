#ifndef FILES_HEADER
#define FILES_HEADER

#include <vector>
#include <string>
#include <fstream>

namespace rcl{
  void load_file(std::vector<std::string>& buffer,const std::string& filename);
  void load_file(std::vector<unsigned char>& buffer, const std::string& filename);
  void save_file(const std::string& filename,const std::vector<unsigned char>& buffer);
  void save_file(const std::string& filename,const std::vector<std::string>& buffer);
  void append_to_file(const std::string& filename,std::vector<unsigned char>& buffer);
  bool check_file(const std::string& filename);
  void save_lock(const std::string& filename);
  bool is_lock(const std::string& filename);
  
  std::vector<unsigned char> load_file(const std::string& filename);
}

#endif
