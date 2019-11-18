#include "tfield_io.hpp"
#include "fitting.hpp"
#include "newdisplay.hpp"
#include "files.hpp"
#include "lib-png.hpp"
#include "filepath.hpp"
#include <iomanip>

int main(int argc,char **argv){
  std::vector<std::string> infiles(argv+1,argv+argc);
  for (unsigned int id_file=0;id_file<infiles.size();id_file++){
    rcl::ffield mag;
    std::string infile = infiles[id_file];
    std::cout << "Leyendo fichero:" << infile << std::endl;
    rcl::FilePath fp(infile);
    rcl::load_vecfile(mag,infile);
    std::fstream side(fp.GetRoot()+"_side.txt",std::fstream::out|std::fstream::binary);
    std::fstream diag(fp.GetRoot()+"_diag.txt",std::fstream::out|std::fstream::binary);
    std::cout << "Dimensiones : " << mag.getnodes().x() << " × " << mag.getnodes().y() << " × " << mag.getnodes().z() << std::endl;
    
    for (float x=0;x<64.f;x+=0.1f) 
    {
      side << x << " " ;
      side << mag(x,static_cast<float>(mag.getnodes().y())*0.5f,0).x() << " ";
      side << mag(x,static_cast<float>(mag.getnodes().y())*0.5f,0).y() << " ";
      side << mag(x,static_cast<float>(mag.getnodes().y())*0.5f,0).z() << " ";
      side << std::sqrt(1-std::pow(mag(x,static_cast<float>(mag.getnodes().y())*0.5f,0).z(),2.f)) << " ";
      side << std::endl;
    }
    if (mag[rcl::utriad(0,0,0)]!=rcl::fvector(0,0,0))
    {
      for (float x=0;x<64.f;x+=0.1f) 
      {
        diag << x << " ";
        diag << mag(x*std::sqrt(0.5f),x*std::sqrt(0.5f),0).x() << " "; 
        diag << mag(x*std::sqrt(0.5f),x*std::sqrt(0.5f),0).y() << " ";
        diag << mag(x*std::sqrt(0.5f),x*std::sqrt(0.5f),0).z() << " ";
        diag << std::sqrt(1-std::pow(mag(x*std::sqrt(0.5f),x*std::sqrt(0.5f),0).z(),2.f));
        diag << std::endl;
      }
    }
    else
    {
      float init = static_cast<float>(mag.getnodes().y())*(std::sqrt(0.5f)-0.5f);
      for (float x=init;x<init+64.f;x+=0.1f) 
      {
        diag << x-init << " ";
        diag << mag(x*std::sqrt(0.5f),x*std::sqrt(0.5f),0).x() << " ";
        diag << mag(x*std::sqrt(0.5f),x*std::sqrt(0.5f),0).y() << " ";
        diag << mag(x*std::sqrt(0.5f),x*std::sqrt(0.5f),0).z() << " ";
        diag << std::sqrt(1-std::pow(mag(x*std::sqrt(0.5f),x*std::sqrt(0.5f),0).z(),2.f));
        diag << std::endl;
      }

    }
  }
  return 0;
}
 
