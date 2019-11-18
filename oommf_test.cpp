#include "busybox.hpp"
#include "oommf.hpp"
#include "files.hpp"
#include "lib-zlib.hpp"
#include "text.hpp"
#include "vf.hpp"

int oommf_test_main(int argc,char **argv){
  std::vector<unsigned char> buffer,data,dic,zdic,vf,outbuffer;
  std::map<std::string,std::string> meta;
  rcl::ffield field,fieldt;  
  rcl::load_file(buffer,"/home/ricardo/m_full000000.ovf.gz");
  rcl::inflate(data,buffer,true);
  rcl::loadOVF2v(field,data);
  for (auto itr=field.getmeta().begin();itr!=field.getmeta().end();itr++) 
    std::cout << "# (" << itr->first << "): [" << itr->second <<"]"<< std::endl;
  rcl::dump(dic,field.getmeta());
  rcl::dump(meta,dic);
  std::cout << "--------------------------------------------------------------------" << std::endl;
  rcl::deflate(zdic,dic,false);
  std::cout << "dic size  = " << dic.size()  << " : zdic size = " << zdic.size() << std::endl;
  std::cout << "nodes ="<< field.getnodes().x() << ","  
                        << field.getnodes().y() << "," 
                        << field.getnodes().z() << std::endl;
  std::cout << "csize ="<< field.getsize().x() << ","  
                        << field.getsize().y() << "," 
                        << field.getsize().z() << std::endl;
  std::cout << "--------------------------------------------------------------------" << std::endl;
  for (auto itr=meta.begin();itr!=meta.end();itr++) 
    std::cout << "# " << itr->first << ": " << itr->second << std::endl;
  rcl::encode_vf(vf,field);
  rcl::decode_vf(fieldt,vf);
  rcl::saveOVF2vm(outbuffer,fieldt);
  std::cout << "--------------------------------------------------------------------" << std::endl;
  std::cout << "VF size = "<< vf.size() << std::endl;
  std::cout << "OVF size = "<< buffer.size() << ":" << data.size() << " , " << outbuffer.size() << std::endl;
  std::cout << "--------------------------------------------------------------------" << std::endl;
  rcl::save_file("magnetizacion.ovf",outbuffer);
  return 0;
}

static rcl::reg_app reg("oommf_test",oommf_test_main);
