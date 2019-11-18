#include "tfield_io.hpp"
#include "fitting.hpp"
#include "newdisplay.hpp"
#include "files.hpp"
#include "lib-png.hpp"
#include <iomanip>

int thiele_tool_adj_main(int argc,char **argv){
  rcl::ffield mag;
  
  //std::vector<std::string> infiles;
  
  const char *files[] = {"m000000","m000001","m000002","m000003","m000004","m000005","m000006","m000007",
                   "m000008","m000009","m000010","m000011","m000012","m000013","m000014","m000015",
                   "m000016","m000017","m000018","m000019","m000020","m000021","m000022","m000023",
                   "m000024","m000025","m000026","m000027","m000028","m000029","m000030","m000031",
                   "m000032","m000033","m000034","m000035","m000036","m000037","m000038","m000039",
                   "m000040","m000041","m000042","m000043","m000044","m000045","m000046","m000047",
                   "m000048","m000049","m000050","m000051","m000052","m000053","m000054","m000055",
                   "m000056","m000057","m000058","m000059","m000060","m000061","m000062","m000063",
                   "m000064","m000065","m000066","m000067","m000068","m000069","m000070","m000071",
                   "m000072","m000073","m000074","m000075","m000076","m000077","m000078","m000079"};
  std::vector<std::string> infiles(files,files+sizeof(files)/sizeof(char*));
  
  for (unsigned int id_file=0;id_file<infiles.size();id_file++)
  {
    std::string infile = "/volatil/calibracion/" + infiles[id_file] + ".vf";
    std::string outfile = "/volatil/calibracion/" + infiles[id_file] + ".p.png";
    std::string fitfile = "/volatil/calibracion/" + infiles[id_file] + ".f.png";
    std::string mixfile = "/volatil/calibracion/" + infiles[id_file] + ".h.png";
    
    std::cout << "Leyendo fichero:" << infile << std::endl;
    rcl::load_vecfile(mag,infile);
    std::cout << "Dimensiones : " << mag.getnodes().x() << " × " << mag.getnodes().y() << " × " << mag.getnodes().z() << std::endl;
        
    rcl::ftriad pos = locate_skyrmion(mag);
    std::cout << "Las coordenadas del skyrmion son: " << pos.x() << " , " << pos.y() << " - con r=" << pos.z() << std::endl; 
    
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
  /*
  rcl::pair<rcl::fpoint,rcl::fpoint> res = rcl::fit_skyrmion(mag);
  std::cout << "Coordenadas :" << res.first.x() << " , " << res.first.y() << std::endl;
  std::cout << "Las medidas son radio=" << res.second.x() << " y ancho de pared=" << res.second.y() << std::endl;
  */
  return 0;
}
