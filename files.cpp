#include "files.hpp"

namespace rcl{



void load_file(std::vector<std::string>& buffer,const std::string& filename){
  std::fstream file(filename,std::fstream::in);
  while (!file.eof()){
    std::string line;
    std::getline(file,line);
    if (line.length()!=0) buffer.push_back(line);
    }
  return;
}
void load_file(std::vector<unsigned char>& buffer, const std::string& filename){
  std::fstream file(filename,std::fstream::in|std::fstream::binary);
  file.seekg(0,std::fstream::end);
  size_t size = file.tellg();
  buffer.resize(size);
  file.seekg(0);
  file.read(reinterpret_cast<char*>(&buffer[0]),size);
  return;
}
void save_file(const std::string& filename,const std::vector<unsigned char>& buffer){
  std::fstream file(filename,std::fstream::out|std::fstream::binary);
  file.write(reinterpret_cast<const char*>(&buffer[0]),buffer.size());
  return;
}  

void save_file(const std::string& filename,const std::vector<std::string>& buffer){
  std::fstream file(filename,std::fstream::out|std::fstream::binary);
  for (unsigned int idx=0;idx<buffer.size();idx++)
    file << buffer[idx] << std::endl;
  return;
  }

void append_to_file(const std::string& filename,std::vector<unsigned char>& buffer){
  std::fstream file(filename,std::fstream::binary|std::fstream::app);
  file.write(reinterpret_cast<char*>(&buffer[0]),buffer.size());  
  return;
}

bool check_file(const std::string& filename){
  return std::ifstream(filename).good();
}

void save_lock(const std::string& filename){
  const unsigned char locksig[8]={0x2c,0xe0,0x8a,0x10,0x3f,0x85,0x20,0xb1};
  std::fstream file(filename,std::fstream::out|std::fstream::binary);
  file.write(reinterpret_cast<const char*>(locksig),8);
  return;
}

bool is_lock(const std::string& filename){
  const unsigned char locksig[8]={0x2c,0xe0,0x8a,0x10,0x3f,0x85,0x20,0xb1};
  char lockread[8];
  std::ifstream file(filename,std::fstream::in|std::fstream::binary);
  if (!file.good()) return false;
  file.read(lockread,8);
  if (!file.good()) return false;
  if (reinterpret_cast<const unsigned long long int&>(*lockread)!=reinterpret_cast<const unsigned long long int&>(*locksig)) return false;
  return true; 
}

std::vector<unsigned char> load_file(const std::string& filename){
  std::vector<unsigned char> ans;
  load_file(ans,filename);
  return ans;
  }

}
