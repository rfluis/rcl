#include "cacheromfs.hpp"

namespace rcl{
  
//const unsigned char cacheromfs_signature[42]={0x46,0x27,0x1A,0xAD,0xDA,0x05,0xAD,0xA9,0xDC,0x8A,0xC7,0x28,0x2E,0xE8,0xAC,0x31,0xAA,0xED,0x8A,0x77,0xB3,0xF8,0x59,0x5A,0xB5,0x17,0x9A,0x74,0xE9,0xE5,0xC8,0x58,0xA5,0x79,0x2C,0xAC,0xB5,0xE9,0x82,0x69,0xC8,0x5E};  
// void    dump(std::string& dst, const bytevector& src);
// void    dump(bytevector&  dst, const std::string& src);

void dump(bytevector& dst,const std::map<std::string,upair>& src){
  for (std::map<std::string,rcl::upair>::const_iterator itr=src.begin();itr!=src.end();itr++){
    rcl::dump(dst,itr->first);
    dst.insert(dst.end(),reinterpret_cast<const char*>(&itr->second),reinterpret_cast<const char*>(&itr->second)+8);
    }
  return;
  }
void dump(std::map<std::string,upair>& dst,const bytevector& src){
  rcl::bytevector::const_iterator itr = src.begin();
  while (itr!=src.end()){
    std::string str;
    rcl::upair up;
    itr = dump(str,itr,src.end());
    up = reinterpret_cast<const rcl::upair&>(*itr);
    itr+=8;
    dst.insert(std::pair<std::string,rcl::upair>(str,up));
    }
  return;
}

bool is_crfs(const std::string& filename){
  const unsigned char cacheromfs_signature[42]={0x46,0x27,0x1A,0xAD,0xDA,0x05,0xAD,0xA9,0xDC,0x8A,0xC7,0x28,0x2E,0xE8,0xAC,0x31,0xAA,0xED,0x8A,0x77,0xB3,0xF8,0x59,0x5A,0xB5,0x17,0x9A,0x74,0xE9,0xE5,0xC8,0x58,0xA5,0x79,0x2C,0xAC,0xB5,0xE9,0x82,0x69,0xC8,0x5E};
  unsigned char sigread[42];
  std::ifstream file(filename,std::fstream::in|std::fstream::binary);
  if (!file.good()) return false;
  file.read(reinterpret_cast<char*>(sigread),42);
  if (!file.good()) return false;
  for (unsigned int idx=0;idx<42;idx++) 
    if (sigread[idx]!=cacheromfs_signature[idx]) 
      return false;
  return true; 
}

cacheromfs_table::cacheromfs_table(const std::map<std::string,rcl::upair>& orig){ table=orig; return;}
bool cacheromfs_table::check_key(const std::string& key)const{
    if (table.find(key)==table.end()) return false;
    else return true;
    }
rcl::upair cacheromfs_table::get_pair(const std::string& key)const{
    rcl::upair ans(0,0);
    std::map<std::string,rcl::upair>::const_iterator itr;
    itr = table.find(key);
    if (itr==table.end()) return ans;
    ans = itr->second;
    return ans;
    }
void cacheromfs_table::increment_offset(const unsigned int& bias){
  for (std::map<std::string,rcl::upair>::iterator itr=table.begin();itr!=table.end();itr++)
    itr->second.first+=bias;
  return;
  }
void cacheromfs_table::decrement_offset(const unsigned int& bias){
  for (std::map<std::string,rcl::upair>::iterator itr=table.begin();itr!=table.end();itr++)
    itr->second.first-=bias;
  return;
  }
rcl::stringvector cacheromfs_table::get_filenames()const{
  rcl::stringvector list;
  for (auto itr=table.begin();itr!=table.end();itr++) list.push_back(itr->first);
  return list;
  }
  
void icacheromfs::open_file(const std::string& path){
  std::map<std::string,rcl::upair> filetable;
  rcl::bytevector tabledata,zlibdata;
  unsigned int tablesize;
  const unsigned char cacheromfs_signature[42]={0x46,0x27,0x1A,0xAD,0xDA,0x05,0xAD,0xA9,0xDC,0x8A,0xC7,0x28,0x2E,0xE8,0xAC,0x31,0xAA,0xED,0x8A,0x77,0xB3,0xF8,0x59,0x5A,0xB5,0x17,0x9A,0x74,0xE9,0xE5,0xC8,0x58,0xA5,0x79,0x2C,0xAC,0xB5,0xE9,0x82,0x69,0xC8,0x5E};  
  unsigned char sigread[42];
  std::ifstream file(path,std::fstream::in|std::fstream::binary);
  //std::cout << "icacheromfs::open_file >> " << "Abrir archivo " << path << std::endl;
  if (!file.good()) return;
  file.read(reinterpret_cast<char*>(sigread),42);
  //std::cout << "icacheromfs::open_file >> " << "Leer firma "<< std::endl;
  if (!file.good()) return;
  for (unsigned int idx=0;idx<42;idx++) 
    if (sigread[idx]!=cacheromfs_signature[idx]) return;
  //std::cout << "icacheromfs::open_file >> " << "Comprobar firma "<< std::endl;
  file.read(reinterpret_cast<char*>(&tablesize),4);
  //std::cout << "icacheromfs::open_file >> " << "Leer tamaño de tabla = " << tablesize << std::endl;
  if (!file.good()) return;
  zlibdata.resize(tablesize);
  file.read(reinterpret_cast<char*>(&zlibdata[0]),tablesize);
  //std::cout << "icacheromfs::open_file >> " << "Leer tabla "<< std::endl;
  if (!file.good()) return;
  rcl::inflate(tabledata,zlibdata,0);
  //std::cout << "icacheromfs::open_file >> " << "Inflar tabla en " << tabledata.size() << std::endl;
  zlibdata.clear();
  rcl::dump(filetable,tabledata);
  //std::cout << "icacheromfs::open_file >> " << "Filetable size = " << filetable.size() << std::endl;
  table = filetable;
  filename = path;
  table.increment_offset(46+tablesize);
  rcl::stringvector list = get_filenames();
  //for (int idx=0;idx<list.size();idx++){
    //std::cout << "icacheromfs::open_file >> " << "File " << idx << ":" << list[idx] << std::endl;
    //}
  return;
  }

void icacheromfs::read(rcl::bytevector& dst,const std::string& key)const{
  rcl::upair address = table.get_pair(key);
  if (address==rcl::upair(0,0)) return;
  std::ifstream file(filename,std::fstream::in|std::fstream::binary);
  if (!file.good()) return;
  dst.resize(address.second);
  file.seekg(address.first);
  if (!file.good()) return;
  file.read(reinterpret_cast<char*>(&dst[0]),address.second);
  return;
  }

bool ocacheromfs_mem::check_key(const std::string& key)const{
  if (table.find(key)==table.end()) return false;
  else return true;
  }
void ocacheromfs_mem::populate_from_file(const std::string& filename){
  rcl::icacheromfs source(filename);
  rcl::stringvector files = source.get_filenames();
  for (unsigned int idx=0;idx<files.size();idx++)
    table[files[idx]] = source.read(files[idx]);
  return;
  }
void ocacheromfs_mem::insert_data(const std::string& key,const rcl::bytevector& data){
  table[key]=data;
  return;}
void ocacheromfs_mem::insert_file(const std::string& key,const std::string& filepath){
  rcl::bytevector data;
  rcl::load_file(data,filepath);
  insert_data(key,data);
  return;}
void ocacheromfs_mem::write_to_file(const std::string& filename) const {
  const unsigned char cacheromfs_signature[42]={0x46,0x27,0x1A,0xAD,0xDA,0x05,0xAD,0xA9,0xDC,0x8A,0xC7,0x28,0x2E,0xE8,0xAC,0x31,0xAA,0xED,0x8A,0x77,0xB3,0xF8,0x59,0x5A,0xB5,0x17,0x9A,0x74,0xE9,0xE5,0xC8,0x58,0xA5,0x79,0x2C,0xAC,0xB5,0xE9,0x82,0x69,0xC8,0x5E};  
  std::ofstream file(filename,std::fstream::out|std::fstream::binary);
  file.write(reinterpret_cast<const char*>(cacheromfs_signature),42);
  std::map<std::string,rcl::upair> ftable;
  rcl::bytevector tabledata,zlibdata;
  unsigned int offset = 0;
  for (auto itr = table.begin();itr!=table.end();itr++){
    ftable[itr->first] = rcl::upair(offset,itr->second.size());
    offset+=itr->second.size();
    }
  rcl::dump(tabledata,ftable);
  rcl::deflate(zlibdata,tabledata,rcl::zlib_cfg());
  unsigned int zlibsize = zlibdata.size();
  file.write(reinterpret_cast<const char*>(&zlibsize),4);
  file.write(reinterpret_cast<const char*>(&zlibdata[0]),zlibsize);
  for (auto itr = table.begin();itr!=table.end();itr++){
    file.write(reinterpret_cast<const char*>(&(itr->second[0])),itr->second.size());
    }
  return;}

}
