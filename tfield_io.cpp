#include "tfield_io.hpp"

/* Cargar COMSOL , OVF , VF */

namespace rcl{

void load_vecfile (rcl::ffield& vdata,const std::string& path){
  rcl::FilePath file(path);
  std::vector<unsigned char> iofile;
  rcl::load_file(iofile,path);
  //if (rcl::is_GZIP(iofile)) std::cout << path << " is gzipped" << std::endl;
  rcl::ungzip(iofile);
  
  if (file.GetFullExt()==".ovf.gz" || file.GetFullExt()==".ovf"){
    //std::cout << path << " is oommf" << std::endl;
    rcl::loadOVF2v(vdata,iofile);
    return;
  }
  
  if (file.GetFullExt()==".vf"){
    //std::cout << path << " is vf" << std::endl;
    rcl::decode_vf(vdata,iofile); 
    return;}
  
  if (rcl::is_COMSOL(iofile)) {
    //std::cout << path << " is comsol" << std::endl;
    rcl::load_COMSOL(vdata,iofile);
  }
  return;
}

}
