#include "busybox.hpp"
#include "tfield_io.hpp"
#include "fitting.hpp"
#include "newdisplay.hpp"
#include "files.hpp"
#include "lib-png.hpp"
#include <iomanip>

int thiele_tool_adj_main(int argc,char **argv){
  std::vector<std::string> files(argv+1,argv+argc);
  for (unsigned int index=0;index<files.size();index++){
    std::string infile = files[index];
    rcl::FilePath infile_fp(infile);
    std::string outfile = infile_fp.GetRoot()+".png";
    rcl::ffield mag;
    rcl::load_vecfile(mag,infile);
    std::cout << "Loaded file: " << infile << " as " << mag.getnodes().x() << "×" << mag.getnodes().y() << "×" << mag.getnodes().z() << " vector field." << std::endl; 
    rcl::ftriad pos = rcl::locate_skyrmion_PBC(mag);
    std::cout << "Skyrmion coordinates are " << pos.x() << "," << pos.y() << " and radius=" << pos.z() << std::endl;
    unsigned int shx,shy;
    shx = (mag.getnodes().x()*3)/2 - static_cast<unsigned int>(pos.x());
    shy = (mag.getnodes().y()*3)/2 - static_cast<unsigned int>(pos.y());
    if (shx>mag.getnodes().x()) shx-=mag.getnodes().x();
    if (shy>mag.getnodes().y()) shy-=mag.getnodes().y();
    std::cout << "Skyrmion shift is x=" << shx << ", y=" << shy << std::endl;
    std::cout << "New coordinates are" << shx+pos.x() << "," << shy+pos.y() << std::endl;
    rcl::ffield centered = rcl::shift_x(rcl::shift_y(mag,shy),shx);
    rcl::ftriad cpos = rcl::locate_skyrmion_PBC(centered);
    std::cout << "New Centered Skyrmion coordinates are " << cpos.x() << "," << cpos.y() << " and radius=" << cpos.z() << std::endl;
    rcl::image32 left = rcl::display_field(mag,0,"nrainbow");
    rcl::image32 right = rcl::display_field(centered,0,"nrainbow");
    rcl::image32 mix(left.GetX()*2,left.GetY());
    mix.put(left,rcl::upoint());
    mix.put(right,rcl::upoint(left.GetX(),0));
    std::vector<unsigned char> mix_data;
    rcl::encode_png(mix_data,mix);
    rcl::save_file(outfile,mix_data); 
    rcl::pair<rcl::fpoint,rcl::fpoint> res = rcl::get_skyrmion_sizes(centered);
    std::cout << "Coordenadas :" << res.first.x() << " , " << res.first.y() << std::endl;
    std::cout << "Las medidas son radio=" << res.second.x() << " y ancho de pared=" << res.second.y() << std::endl;
    std::cout << "Saved file: " << outfile << std::endl;
    std::cout << "===================================================================" << std::endl;
    }  

  return 0;
}
static rcl::reg_app reg("thiele_tool_adj",thiele_tool_adj_main);

/*
int thiele_tool_adj_main(int argc,char **argv){
  std::vector<std::string> files(argv+1,argv+argc);
  for (unsigned int index=0;index<files.size();index++){
    std::string infile = files[index];
    rcl::FilePath infile_fp(infile);
    std::string outfile = infile_fp.GetRoot()+".png";
    rcl::ffield mag;
    rcl::load_vecfile(mag,infile);
    std::cout << "Loaded file: " << infile << " as " << mag.getnodes().x() << "×" << mag.getnodes().y() << "×" << mag.getnodes().z() << " vector field." << std::endl; 
    rcl::ftriad pos = rcl::locate_skyrmion_PBC(mag);
    std::cout << "Skyrmion coordinates are " << pos.x() << "," << pos.y() << " and radius=" << pos.z() << std::endl;
    unsigned int shx,shy;
    shx = (mag.getnodes().x()*3)/2 - static_cast<unsigned int>(pos.x());
    shy = (mag.getnodes().y()*3)/2 - static_cast<unsigned int>(pos.y());
    if (shx>mag.getnodes().x()) shx-=mag.getnodes().x();
    if (shy>mag.getnodes().y()) shy-=mag.getnodes().y();
    std::cout << "Skyrmion shift is x=" << shx << ", y=" << shy << std::endl;
    std::cout << "New coordinates are" << shx+pos.x() << "," << shy+pos.y() << std::endl;
    rcl::ffield centered = rcl::shift_x(rcl::shift_y(mag,shy),shx);
    rcl::ftriad cpos = rcl::locate_skyrmion_PBC(centered);
    std::cout << "New Centered Skyrmion coordinates are " << cpos.x() << "," << cpos.y() << " and radius=" << cpos.z() << std::endl;
    rcl::image32 left = rcl::display_field(mag,0,"nrainbow");
    rcl::image32 right = rcl::display_field(centered,0,"nrainbow");
    rcl::image32 mix(left.GetX()*2,left.GetY());
    mix.put(left,rcl::upoint());
    mix.put(right,rcl::upoint(left.GetX(),0));
    std::vector<unsigned char> mix_data;
    rcl::encode_png(mix_data,mix);
    rcl::save_file(outfile,mix_data); 
    rcl::pair<rcl::fpoint,rcl::fpoint> res = rcl::fit_skyrmion(centered);
    std::cout << "Coordenadas :" << res.first.x() << " , " << res.first.y() << std::endl;
    std::cout << "Las medidas son radio=" << res.second.x() << " y ancho de pared=" << res.second.y() << std::endl;
    std::cout << "Saved file: " << outfile << std::endl;
    std::cout << "===================================================================" << std::endl;
    }  

  return 0;
}
*/

  /*
  rcl::ffield mag;
  rcl::load_vecfile(mag,infile);
      
  rcl::ftriad pos = locate_skyrmion(mag);
  
  rcl::pair<rcl::fpoint,rcl::fpoint> res = get_skyrmion_sizes(mag);
  std::cout << "Coordenadas :" << res.first.x() << " , " << res.first.y() << std::endl;
  std::cout << "Las medidas son radio=" << res.second.x() << " y ancho de pared=" << res.second.y() << " en celdas del fichero origen." << std::endl;
  
  rcl::Flat_Neel_Skyrmion<float> fit(res.first,res.second);
  fit.getnodes() = mag.getnodes();
  
  std::vector<unsigned char> infile_plot,fit_plot,mix_data;
  
  rcl::image32 left = rcl::display_field(mag,0,"nrainbow");
  rcl::image32 right = rcl::display_field(fit.render_field(),0,"nrainbow");
  rcl::image32 mix(left.GetX()*2,left.GetY());
  
  mix.put(left,rcl::upoint());
  mix.put(right,rcl::upoint(left.GetX(),0));
  
  rcl::encode_png(infile_plot,left);
  rcl::encode_png(fit_plot,right);
  rcl::encode_png(mix_data,mix);
  
  rcl::save_file(outfile,infile_plot);
  rcl::save_file(fitfile,fit_plot);
  rcl::save_file(mixfile,mix_data); 
  }
  * */
  /*
  rcl::pair<rcl::fpoint,rcl::fpoint> res = rcl::fit_skyrmion(mag);
  std::cout << "Coordenadas :" << res.first.x() << " , " << res.first.y() << std::endl;
  std::cout << "Las medidas son radio=" << res.second.x() << " y ancho de pared=" << res.second.y() << std::endl;
  */
