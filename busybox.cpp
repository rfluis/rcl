#define REGAPPS_DECLARE
#include "busybox.hpp"


namespace rcl{
rcl::app_container regApps;
}

int rcl::app_container::callback(int argc,char** argv){
  std::cout << "app container asked for a non-registered function" << std::endl;
  return 0;
  }



#ifndef LAUNCHER
int main(int argc,char **argv)
#else
extern "C" int busybox_launcher_main(int argc,char **argv)
#endif
{
  rcl::FilePath executable(argv[0]);
  if (executable.GetFile()=="busybox") if (argc==1) {
    std::cout << " wrong use" << std::endl;
    return 1;
    };
  
  std::string option;  int bargc;  char** bargv;
  if (executable.GetFile()=="busybox") {option = argv[1]; bargc=argc-1; bargv=argv+1;}
  else {bargc=argc; bargv=argv; option = argv[0];}
  if (option=="help" || option =="-h" || option=="--help" ) {
    std::vector<std::string> list = rcl::regApps.app_list();
    for (unsigned int idx=0;idx<list.size();idx++)
      std::cout << idx << " : " << list[idx] << std::endl;
    return 0;
    }
  return rcl::regApps[option](bargc,bargv);
}


/*
int vf_tool_main(int argc,char **argv);
int thiele_tool_adj_main(int argc,char **argv);
int thiele_tool_main(int argc,char **argv);
int embed_tool_main(int argc,char **argv);
int oommf_test_main(int argc,char **argv);
int fonts_demo_main(int argc,char **argv);
int comsol_tool_main(int argc,char **argv);
int gen_main(int argc,char **argv);
int vf_debug_main(int argc,char **argv);
int vf_crush_main(int argc,char **argv);
int vf_plot_main(int argc,char **argv);
int vf_view_main(int argc,char **argv);
int vf_check_main(int argc,char **argv);
int get_sky_sizes_main(int argc,char **argv);
int ovf_diff_tool_main(int argc,char **argv);
int skyfit_tool_main(int argc,char **argv);
int skyfit_zoom_tool_main(int argc,char **argv);
int unit_calculator_main(int argc,char **argv);
int vf_render_main(int argc,char **argv);
int populate_cache_main(int argc,char **argv);
int extract_cache_main(int argc,char **argv);
int any2asciiovf_main(int argc,char **argv);
*/

/*
{
  std::vector<rcl::busybox_app> app;
  app.push_back(rcl::busybox_app("get_sky_sizes_tool",get_sky_sizes_main));
  app.push_back(rcl::busybox_app("vf_tool",vf_tool_main));
  app.push_back(rcl::busybox_app("vf_crush",vf_crush_main));
  app.push_back(rcl::busybox_app("vf_plot",vf_plot_main));
  app.push_back(rcl::busybox_app("vf_view",vf_view_main));
  app.push_back(rcl::busybox_app("vf_debug",vf_debug_main));
  app.push_back(rcl::busybox_app("vf_check",vf_check_main));
  app.push_back(rcl::busybox_app("thiele_tool_adj",thiele_tool_adj_main));
  app.push_back(rcl::busybox_app("embed_tool",embed_tool_main));
  app.push_back(rcl::busybox_app("embed",embed_tool_main));
  app.push_back(rcl::busybox_app("oommf_test",oommf_test_main));
  app.push_back(rcl::busybox_app("fonts_demo",fonts_demo_main));
  app.push_back(rcl::busybox_app("comsol_tool",comsol_tool_main));
  app.push_back(rcl::busybox_app("gen",gen_main));
  app.push_back(rcl::busybox_app("ovf_diff",ovf_diff_tool_main));
  app.push_back(rcl::busybox_app("sky_fit",skyfit_tool_main));
  app.push_back(rcl::busybox_app("sky_fit_zoom",skyfit_zoom_tool_main));
  app.push_back(rcl::busybox_app("unit_calculator",unit_calculator_main));
  app.push_back(rcl::busybox_app("vfrender",vf_render_main));
  app.push_back(rcl::busybox_app("populate_cache",populate_cache_main));
  app.push_back(rcl::busybox_app("extract_cache",extract_cache_main));
  app.push_back(rcl::busybox_app("any2asciiovf",any2asciiovf_main));
  
  
  rcl::FilePath executable(argv[0]);
  if (executable.GetFile()=="busybox") if (argc==1) {
    std::cout << " wrong use" << std::endl;
    return 1;
    };
  std::string option;
  int bargc;
  char** bargv;
  if (executable.GetFile()=="busybox") {option = argv[1]; bargc=argc-1; bargv=argv+1;}
  else {bargc=argc; bargv=argv; option = argv[0];}
  if (option=="help" || option =="-h" || option=="--help" ) {
    for (int idx=0;idx<app.size();idx++) 
      std::cout << static_cast<const std::string&>(app[idx]) << std::endl;
    return 0;
    }
  for (int idx=0;idx<app.size();idx++) if(option==static_cast<const std::string&>(app[idx])) return app[idx](bargc,bargv);
  std::cout << "not found" << std::endl;
  
  return 1;
}
*/
