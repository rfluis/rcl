#include "busybox.hpp"
#include "tfield_io.hpp"
#include "fitting.hpp"
#include "filtering.hpp"
#include "newdisplay.hpp"
#include "files.hpp"
#include "lib-png.hpp"
#include "filepath.hpp"
#include <iomanip>

int skyfit_tool_main(int argc,char **argv){
  if (argc==1){
    std::cout << "USO:" << std::endl;
    std::cout << " programa timestep fichero.txt fichero1.vf fichero2.vf ... fichero_n.vf" << std::endl;
    return 1;
    }
  double timestep;
  const unsigned int entries = 13;// 9+4
  std::stringstream(argv[1]) >> timestep;
  std::string outfile = std::string(argv[2]); 
  std::vector<std::string> paths(argv+3,argv+argc);
  std::vector<float> data;
  data.resize(paths.size()*entries);
  
  
  // template <typename T>
  //rcl::pair<rcl::tpoint<T>,rcl::tpoint<T> > get_skyrmion_sizes(const rcl::tfield<rcl::tvector<T> >& mag)
  
  for (unsigned int idx=0;idx<paths.size();idx++){
    std::string infile = paths[idx];
    rcl::ffield mag;
    rcl::load_vecfile(mag,infile);
    rcl::pair<rcl::tpoint<float>,rcl::tpoint<float> > fit = rcl::get_skyrmion_sizes(mag);
    std::cout << infile << std::endl;
    data[entries*idx+0] = (idx-2) * timestep;
    data[entries*idx+1] = fit.x().x()*mag.getsize().x();
    data[entries*idx+2] = fit.x().y()*mag.getsize().y();
    data[entries*idx+3] = fit.y().x()*mag.getsize().x();
    data[entries*idx+4] = fit.y().y()*mag.getsize().x();  
    if (idx>1) {
      data[entries*idx+5] = (data[entries*idx+1] - data[entries*(idx-1)+1])/timestep;
      data[entries*idx+6] = (data[entries*idx+2] - data[entries*(idx-1)+2])/timestep;
      data[entries*idx+7] = (data[entries*idx+3] - data[entries*(idx-1)+3])/timestep;
      data[entries*idx+8] = (data[entries*idx+4] - data[entries*(idx-1)+4])/timestep;
      data[entries*idx+9] = std::sqrt(std::pow((data[entries*idx+1] - data[entries*(idx-1)+1]),2) + std::pow((data[entries*idx+2] - data[entries*(idx-1)+2]),2));
      data[entries*idx+10] = data[entries*(idx-1)+10] + data[entries*idx+9] ;
      data[entries*idx+11] = data[entries*idx+9]/timestep;
      data[entries*idx+12] = data[entries*idx+10]/(data[entries*idx+0]+timestep);
      }
      else{
        data[entries*idx+10] =0;
        data[entries*idx+9] = 0;
      }   
  }
  std::fstream ofs(outfile,std::fstream::out|std::fstream::binary);
  for (unsigned int idx=2;idx<paths.size();idx++){
    ofs << std::setw(12) << data[entries*idx+0] << ' ';
    ofs << std::setw(12) << data[entries*idx+1] << ' ';
    ofs << std::setw(12) << data[entries*idx+2] << ' ';
    ofs << std::setw(12) << data[entries*idx+3] << ' ';
    ofs << std::setw(12) << data[entries*idx+4] << ' ';
    ofs << std::setw(12) << data[entries*idx+5] << ' ';
    ofs << std::setw(12) << data[entries*idx+6] << ' ';
    ofs << std::setw(12) << data[entries*idx+7] << ' ';
    ofs << std::setw(12) << data[entries*idx+8] << ' ';
    ofs << std::setw(12) << data[entries*idx+9] << ' ';
    ofs << std::setw(12) << data[entries*idx+10] << ' ';
    ofs << std::setw(12) << data[entries*idx+11] << ' ';
    ofs << std::setw(12) << data[entries*idx+12] << std::endl;
    }
  return 0;
}
static rcl::reg_app reg1("skyfit_tool",skyfit_tool_main);

int skyfit_zoom_tool_main(int argc,char **argv){
  if (argc==1){
    std::cout << "USO:" << std::endl;
    std::cout << " programa timestep fichero.txt fichero1.vf fichero2.vf ... fichero_n.vf" << std::endl;
    return 1;
    }
  double timestep;
  const unsigned int entries = 13;// 9+4
  std::stringstream(argv[1]) >> timestep;
  std::string outfile = std::string(argv[2]); 
  std::vector<std::string> paths(argv+3,argv+argc);
  std::vector<float> data;
  data.resize(paths.size()*entries);
  
  
  // template <typename T>
  //rcl::pair<rcl::tpoint<T>,rcl::tpoint<T> > get_skyrmion_sizes(const rcl::tfield<rcl::tvector<T> >& mag)
  
  for (unsigned int idx=0;idx<paths.size();idx++){
    std::string infile = paths[idx];
    rcl::ffield mag;
    rcl::load_vecfile(mag,infile);
    rcl::FilePath fp(infile);
    rcl::pair<rcl::tpoint<float>,rcl::tpoint<float> > fit = rcl::get_skyrmion_sizes(mag);
    rcl::Flat_Neel_Skyrmion<float> fsky;
    fsky.getnodes() = rcl::utriad(mag.getnodes().y(),mag.getnodes().y(),1);
    fsky.getcells() = rcl::ftriad(0.5,0.5,1);
    fsky.getpos() = rcl::fpoint(0.25*mag.getnodes().y(),0.25*mag.getnodes().y());
    fsky.getrdw() = fit.y();
    rcl::ffield sfit =  fsky.render_field();
    rcl::image32 skyplot_full(mag.getnodes().x()+mag.getnodes().y(),mag.getnodes().y());
    rcl::image32 skyplot_mag = rcl::display_field(mag,0,"nrainbow");
    rcl::image32 skyplot_sky = rcl::display_field(sfit,0,"urainbow");
    skyplot_full.put(skyplot_mag,rcl::upoint(0,0));
    skyplot_full.put(skyplot_sky,rcl::upoint(mag.getnodes().x(),0));
    std::vector<unsigned char> vfdata;
    std::vector<unsigned char> pngdata;
    //rcl::crush_vf(vfdata,fsky.render_field(),rcl::vf_crush_cfg(1,1,0));
    rcl::encode_png(pngdata,skyplot_full);
    float hpeak=0,lpeak=1;
    for (unsigned int idx=0;idx<sfit.size();idx++) if (sfit[idx].norm2()>hpeak) hpeak=sfit[idx].norm2();
    for (unsigned int idx=0;idx<sfit.size();idx++) if (sfit[idx].norm2()<lpeak) lpeak=sfit[idx].norm2();
    rcl::save_file(fp.GetRoot()+".s.png",pngdata);
    //rcl::save_file(fp.GetRoot()+".s.vf",vfdata);
    std::cout << infile << "  r="<< fsky.getrdw().x() << " , d=" << fsky.getrdw().y() ;
    std::cout << " @ " <<fsky.getpos().x() << "," << fsky.getpos().y() ; 
    std::cout << " peaks=" << lpeak << "," << hpeak << "(" << hpeak-lpeak << ")" << std::endl;
    data[entries*idx+0] = (idx-2) * timestep;
    data[entries*idx+1] = fit.x().x()*mag.getsize().x();
    data[entries*idx+2] = fit.x().y()*mag.getsize().y();
    data[entries*idx+3] = fit.y().x()*mag.getsize().x();
    data[entries*idx+4] = fit.y().y()*mag.getsize().x();  
    if (idx>1) {
      data[entries*idx+5] = (data[entries*idx+1] - data[entries*(idx-1)+1])/timestep;
      data[entries*idx+6] = (data[entries*idx+2] - data[entries*(idx-1)+2])/timestep;
      data[entries*idx+7] = (data[entries*idx+3] - data[entries*(idx-1)+3])/timestep;
      data[entries*idx+8] = (data[entries*idx+4] - data[entries*(idx-1)+4])/timestep;
      data[entries*idx+9] = std::sqrt(std::pow((data[entries*idx+1] - data[entries*(idx-1)+1]),2) + std::pow((data[entries*idx+2] - data[entries*(idx-1)+2]),2));
      data[entries*idx+10] = data[entries*(idx-1)+10] + data[entries*idx+9] ;
      data[entries*idx+11] = data[entries*idx+9]/timestep;
      data[entries*idx+12] = data[entries*idx+10]/(data[entries*idx+0]+timestep);
      }
      else{
        data[entries*idx+10] =0;
        data[entries*idx+9] = 0;
      }   
  }
  std::fstream ofs(outfile,std::fstream::out|std::fstream::binary);
  for (unsigned int idx=2;idx<paths.size();idx++){
    ofs << std::setw(12) << data[entries*idx+0] << ' ';
    ofs << std::setw(12) << data[entries*idx+1] << ' ';
    ofs << std::setw(12) << data[entries*idx+2] << ' ';
    ofs << std::setw(12) << data[entries*idx+3] << ' ';
    ofs << std::setw(12) << data[entries*idx+4] << ' ';
    ofs << std::setw(12) << data[entries*idx+5] << ' ';
    ofs << std::setw(12) << data[entries*idx+6] << ' ';
    ofs << std::setw(12) << data[entries*idx+7] << ' ';
    ofs << std::setw(12) << data[entries*idx+8] << ' ';
    ofs << std::setw(12) << data[entries*idx+9] << ' ';
    ofs << std::setw(12) << data[entries*idx+10] << ' ';
    ofs << std::setw(12) << data[entries*idx+11] << ' ';
    ofs << std::setw(12) << data[entries*idx+12] << std::endl;
    }
  return 0;
}
static rcl::reg_app reg2("skyfit_tool_zoom",skyfit_zoom_tool_main);
