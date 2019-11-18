#include "busybox.hpp"
#include <iostream>
#include <iomanip>
#include "tfield_io.hpp"
#include "lib-png.hpp"
#include "newdisplay.hpp"

int vf_plot_main(int argc,char **argv){
  if (argc==1){
  std::cout << argv[0] << " file.ovf    para convertir a file.png" << std::endl;
  std::cout << argv[0] << " file.ovf.gz para convertir a file.png" << std::endl;
  std::cout << argv[0] << " file.vf     para convertir a file.png" << std::endl;
  return 1;
  }
  std::vector<std::string> args(argv,argv+argc);
  for (int idx=1;idx<args.size();idx++){
    rcl::FilePath file(args[idx]);
    std::vector<unsigned char> iofile;
    rcl::ffield vdata;
    rcl::load_vecfile(vdata,args[idx]);
    iofile.clear();
    if (vdata.getnodes().z()==1){
      rcl::image32 img = rcl::display_field(vdata,0,"");
      rcl::encode_png(iofile,img);
      rcl::save_file(file.GetRoot()+".png",iofile);
      }
    else{
      for (unsigned int layer=0;layer<vdata.getnodes().z();layer++){
      rcl::image32 img = rcl::display_field(vdata,layer,"");
      rcl::encode_png(iofile,img);
      rcl::save_file(file.GetRoot()+"."+rcl::to_str(layer)+".png",iofile);
    }
    }
  }
  return 0;
}

static rcl::reg_app reg("vf_plot",vf_plot_main);
