#ifndef STANDALONE_TOOL
#include "busybox.hpp"
#endif

#include <iostream>
#include <iomanip>
#include "filepath.hpp"
#include "lib-png.hpp"
#include "newdisplay.hpp"
#include "tfield_io.hpp"

#ifdef STANDALONE_TOOL
int main(int argc,char **argv){
#endif

#ifndef STANDALONE_TOOL
int any2asciiovf_main(int argc,char **argv){
#endif

if (argc==1){
  std::cout << "Uso de la herramienta:" << std::endl;
  std::cout << " any2asciiovf fichero1.vf <fichero2.vf> <fichero3.vf> <fichero4.vf> ..." << std::endl;
  std::cout << " Toma el fichero VF y lo guarda como OVF Ver 1.0 en modo texto/ascii" << std::endl;
  return 1;
  }
  
  for (int idx=1;idx<argc;idx++){
    rcl::ffield f;
    rcl::load_vecfile(f,argv[idx]);
    rcl::FilePath fp(argv[idx]);
    std::cout << "Convirtiendo :" << fp.GetFullPath() << " -> " << fp.GetRoot() << ".ovf  " << std::flush;
    std::string outfilename = fp.GetRoot()+".ovf";
    std::vector<unsigned char> outfile;
    rcl::saveOVF1Tvm(outfile,f);
    rcl::save_file(outfilename,outfile);
    std::cout << " [HECHO]" << std::endl;
    }
  


return 0;
}


#ifndef STANDALONE_TOOL
static rcl::reg_app reg("any2asciiovf",any2asciiovf_main);
#endif

