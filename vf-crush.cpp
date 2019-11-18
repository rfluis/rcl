#include "busybox.hpp"
#include <iostream>
#include <iomanip>
#include "oommf.hpp"
#include "filepath.hpp"
#include "tfield_io.hpp"

int vf_crush_main(int argc,char **argv){
  if (argc==1){
  std::cout << argv[0] << " file.ovf    para convertir a file.vf" << std::endl;
  std::cout << argv[0] << " file.ovf.gz para convertir a file.vf" << std::endl;
  std::cout << argv[0] << " file.vf     para convertir a file.vf" << std::endl;
  return 1;
  }
  std::vector<std::string> args;
  std::string first(argv[1]);
  
  if (first=="quick") args = std::vector<std::string>(argv+2,argv+argc);
  else if (first=="quickpy") args = std::vector<std::string>(argv+2,argv+argc);
  else args = std::vector<std::string>(argv+1,argv+argc);
  
  for (int idx=0;idx<args.size();idx++){
    rcl::FilePath file(args[idx]);
    std::cout << "* Input file:" << args[idx] <<"  -> Output file :" << file.GetRoot() << ".vf" << std::endl;
    if (rcl::check_file(args[idx]) && !rcl::is_vf_crush(file.GetRoot()+".vf") && !rcl::is_lock(file.GetRoot()+".vf"))
    {
      //rcl::save_lock(file.GetRoot()+".vf");
      std::vector<unsigned char> iofile;
      rcl::ffield vdata;
      rcl::load_vecfile(vdata,args[idx]);
      if (first=="quickpy") std::cout << "Quick Permalloy Compression" << std::endl;
      if (first=="quickpy")    rcl::crush_vf(iofile,vdata,rcl::vf_crush_cfg(1,1,1).setlabel(file.GetRoot()+".vf"));
      else if (first=="quick") rcl::crush_vf(iofile,vdata,rcl::vf_crush_cfg(1,1,0));
      else                     rcl::crush_vf(iofile,vdata,rcl::vf_crush_cfg(1,0,0));
      
      rcl::save_file(file.GetRoot()+".vf",iofile);
      std::cout << "* Wrote " << file.GetRoot()+".vf" << std::endl;
      }
    else {
      std::cout << "* Skipping " << args[idx] << " because of ";
      if (!rcl::check_file(args[idx])) std::cout << "source file doesn't exists ";
      else if (rcl::is_vf_crush(file.GetRoot()+".vf")) std::cout << "target file exists and it's already crushed ";
      else if (rcl::is_lock(file.GetRoot()+".vf")) std::cout << "target file is locked"; 
      std::cout << std::endl;
    }
  }
  return 0;
}
static rcl::reg_app reg("vf_crush",vf_crush_main);
