#include "busybox.hpp"
#include "tfield_io.hpp"
#include "fitting.hpp"
#include "filtering.hpp"
#include "newdisplay.hpp"
#include "files.hpp"
#include "lib-png.hpp"
#include "filepath.hpp"
#include <iomanip>

namespace rcl{
  bool check_ffield(const rcl::ffield& mag){
    if (mag.getnodes().volume()!=mag.getdata().size()) return false;
    return true;
    }
  void check_ffield_data(const rcl::ffield& mag){
    int size = mag.size();
    rcl::fvector ans;
    for (unsigned int idx=0;idx<size;idx++) ans+=mag[idx];
    std::cout << "Memcheck for ffield" << ans.x() << "," << ans.y() << "," << ans.z() << std::endl;
    }
}

int get_sky_sizes_main(int argc,char **argv){
  
  if (argc<2) {
    std::cout << "USO:" << std::endl;
    std::cout << "programa fichero.txt carpeta.out <carpeta2.out> ... carpeta_n.out" << std::endl;
    return 1;
    }
  
  std::vector<std::string> paths(argv+1,argv+argc);
  std::string outfilepath = paths[0];
  std::fstream ofs(outfilepath,std::fstream::out|std::fstream::binary);
  
  
  for(unsigned int idx=1;idx<paths.size();idx++)  {
    std::vector<std::string> files;
    for(unsigned int counter=0;counter<1000000;counter++){
      std::stringstream ss;
      ss << paths[idx] << "/" << "m" << std::setw(6) << std::setfill('0') << counter << ".vf";
      if (!rcl::check_file(ss.str())) break;
      files.push_back(ss.str());
      }
    std::vector<float> numdata;
    std::vector<float> pbcx,pbcy,conx,cony,drad,ddel;;
    numdata.resize(18*files.size());
    pbcx.resize(files.size());
    pbcy.resize(files.size());
    drad.resize(files.size());
    ddel.resize(files.size());
    float lx,ly,cx,cy;
    std::cout << "Numero de archivos en " << paths[idx] << " = " << files.size() << std::endl;
    for (unsigned int counter=0;counter<files.size();counter++){
      double ms,aex,ku,dmi,lz,bz,by,alpha,j,t,dt;
      int index;
      std::vector<std::string>::const_iterator itr=files.begin()+counter;
      std::cout << "Loading file:" << files[counter] << std::endl;
      std::stringstream ss(std::string(itr->begin()+13,itr->end()));
      std::string data;
      
      std::getline(ss,data,'_');      std::stringstream(data) >> ms;      data.clear();      
      std::getline(ss,data,'_');      std::stringstream(data) >> aex;      data.clear();
      std::getline(ss,data,'_');      std::stringstream(data) >> ku;      data.clear();
      std::getline(ss,data,'_');      std::stringstream(data) >> dmi;      data.clear();
      std::getline(ss,data,'_');      std::stringstream(data) >> lz;      data.clear();
      std::getline(ss,data,'_');      std::stringstream(data) >> bz;      data.clear();
      std::getline(ss,data,'_');      std::stringstream(data) >> by;      data.clear();
      std::getline(ss,data,'_');      std::stringstream(data) >> alpha;      data.clear();
      std::getline(ss,data,'m');      std::stringstream(data) >> j;      data.clear();
      std::getline(ss,data,'.');      std::stringstream(data) >> index;      data.clear();
      
      dt = 15./j;
      t = dt*index;
      
      std::string infile(*itr);
      rcl::ffield mag;
      rcl::load_vecfile(mag,infile); 
      rcl::ftriad pos = rcl::locate_skyrmion_PBC(mag);
      unsigned int shx,shy;
      shx = (mag.getnodes().x()*3)/2 - static_cast<unsigned int>(pos.x());
      shy = (mag.getnodes().y()*3)/2 - static_cast<unsigned int>(pos.y());
      
      rcl::ffield centered;
      centered = rcl::shift_y(rcl::shift_x(mag,shx),shy);
      
      rcl::pair<rcl::fpoint,rcl::fpoint> res = rcl::get_skyrmion_sizes(centered);
      
      centered.clear();
      numdata[18*counter+0]=ms;
      numdata[18*counter+1]=aex;
      numdata[18*counter+2]=ku;
      numdata[18*counter+3]=dmi;
      numdata[18*counter+4]=lz;
      numdata[18*counter+5]=bz;
      numdata[18*counter+6]=by;
      numdata[18*counter+7]=alpha;
      numdata[18*counter+8]=j;
      numdata[18*counter+9]=t;
      numdata[18*counter+12]=res.second.x();
      numdata[18*counter+13]=res.second.y();
      drad[counter]=res.second.x();
      ddel[counter]=res.second.y();
      pbcx[counter]=std::floor(pos.x())+res.first.x();
      pbcy[counter]=std::floor(pos.y())+res.first.y();  
      pbcx[counter]-=pbcx[0];
      pbcy[counter]-=pbcy[0];
      lx = static_cast<float>(mag.getnodes().x());
      ly = static_cast<float>(mag.getnodes().y());
      cx = mag.getsize().x();
      cy = mag.getsize().y();
    }// fin bucle files
    conx = rcl::fix_toroidal_movement(pbcx,lx);
    cony = rcl::fix_toroidal_movement(pbcy,ly);
    for (unsigned int counter=0;counter<files.size();counter++){
      numdata[18*counter+10]=conx[counter];
      numdata[18*counter+11]=cony[counter];
    }
    for (unsigned int counter=1;counter<files.size();counter++){
      float dt = 15./numdata[8];
      numdata[18*counter+14]=(conx[counter]-conx[counter-1])/dt;
      numdata[18*counter+15]=(cony[counter]-cony[counter-1])/dt;
      numdata[18*counter+16]=(drad[counter]-drad[counter-1])/dt;
      numdata[18*counter+17]=(ddel[counter]-ddel[counter-1])/dt;
    }
    
    for (unsigned int counter=0;counter<files.size();counter++){
      numdata[18*counter+10]*=cx;
      numdata[18*counter+11]*=cy;
      numdata[18*counter+12]*=std::sqrt(cx*cx+cy*cy);
      numdata[18*counter+13]*=std::sqrt(cx*cx+cy*cy);
      numdata[18*counter+14]*=cx;
      numdata[18*counter+15]*=cy;
      numdata[18*counter+16]*=std::sqrt(cx*cx+cy*cy);
      numdata[18*counter+17]*=std::sqrt(cx*cx+cy*cy);
    }
    
    /* Saltarse los 2 primeros estados porque tiene que relajarse el Skyrmion                         */
    /* En 0 no hay velocidad, porque no se puede evaluar la derivada                                  */
    /* En 1 la velocidad es incorrecta porque se compara un skyrmion no relajado con otro relajado    */
    /* En 2 se compara el estado 1 con el 2. Ambos relajados.                                         */
    /*                        ↓ Aquí                                                                  */
    for (unsigned int counter=2;counter<files.size();counter++){ 
      for (unsigned int item=0;item<18;item++) ofs << numdata[18*counter+item] << " ";
      for (unsigned int item=0;item<18;item++) std::cout << numdata[18*counter+item] << " ";
      std::cout << std::endl;
      ofs << std::endl;
    }
    
  }// fin bucle paths
  
  
  
  return 0;
}


static rcl::reg_app reg("get_sky_sizes_tool",get_sky_sizes_main);
/*
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
*/

/*
 #include <sstream>
#include <fstream>
#include <iostream>

struct material{
  double ms,aex,ku,d,lz,bz,by,alpha,j;
  material(){
    ms=1e6;
    aex=20e-12;
    ku=800e3;
    d=1.8e-3;
    lz=1e-9;
    bz=0;
    by=0;
    alpha=0.3;
    j=30e9;
    return;
    }
material& set_ms(const double& x){ms=x;return *this;}
material& set_aex(const double& x){aex=x;return *this;}
material& set_ku(const double& x){ku=x;return *this;}
material& set_d(const double& x){d=x;return *this;}
material& set_lz(const double& x){lz=x;return *this;}
material& set_bz(const double& x){bz=x;return *this;}
material& set_by(const double& x){by=x;return *this;}
material& set_alpha(const double& x){alpha=x;return *this;}
material& set_j(const double& x){j=x;return *this;}
double get_autosave()const {return 15./j;}
double get_runtime()const{return 750./j;}
std::string make_name()const{
  std::stringstream name;
  name << "dyn_skyrmion_" ;
  name << ms << "_" ;
  name << aex << "_" ;
  name << ku << "_" ;
  name << d << "_" ;
  name << lz << "_" ;
  name << bz << "_" ;
  name << by << "_" ;
  name << alpha << "_" ;
  name << j ;
  return name.str();
  }
std::string make_sname()const{
  std::stringstream name;
  name << "ini_skyrmion_" ;
  name << ms << "_" ;
  name << aex << "_" ;
  name << ku << "_" ;
  name << d << "_" ;
  name << lz << "_" ;
  name << bz << "_" ;
  name << by << "_" ;
  name << alpha ;
  return name.str();
  }
};

void make_scripts(const material& m);
void make_stat(const material& m);
void make_dyn(const material& m);

int main(){
  std::cout << "Barrer ms" << std::endl;
  for (double ms=920e3;ms<=1000e3;ms+=10e3)  make_scripts(material().set_ms(ms));
  
  std::cout << "Barrer aex" << std::endl;
  for (double aex=18e-12;aex<=22e-12;aex+=5e-13)  make_scripts(material().set_aex(aex));
  
  std::cout << "Barrer ku" << std::endl;
  for (double ku=780e3;ku<=850e3;ku+=10e3)  make_scripts(material().set_ku(ku));
  
  std::cout << "Barrer d" << std::endl;
  for (double d=1.6e-3;d<=2.1e-3;d+=0.1e-3)  make_scripts(material().set_d(d));
  
  std::cout << "Barrer lz" << std::endl;
  for (double lz=0.5e-9;lz<=1.5e-9;lz+=1e-10)  make_scripts(material().set_lz(lz));
  
  //std::cout << "Barrer bz" << std::endl;
  //for (int bz=-250;bz<=250;bz+=25)     make_scripts(material().set_bz(bz*1e-3));
  
  std::cout << "Barrer by" << std::endl;
  for (int by=-100;by<=100;by+=10)  make_scripts(material().set_by(by*1e-3));
  
  std::cout << "Barrer alpha" << std::endl;
  for (double alpha=0.05;alpha<=0.5;alpha+=0.05)  make_scripts(material().set_alpha(alpha));
  return 0;
}

void make_scripts(const material& mm){
  material m(mm);
  make_stat(m);
  for (double j=5e9;j<=60e9;j+=5e9) make_dyn(m.set_j(j));
  return;
  } 

void make_stat(const material& m){
  std::fstream fs(m.make_sname()+".mx3",std::fstream::out|std::fstream::binary);
  fs << "SetMesh(256,256,1,1e-9,1e-9,"<< m.lz<<",16,16,0)" << std::endl;
  fs << "Msat = "<< m.ms << std::endl;
  fs << "Aex = "<< m.aex << std::endl;
  fs << "alpha = "<< m.alpha << std::endl;
  fs << "dind = "<< m.d << std::endl;
  fs << "ku1 = "<< m.ku << std::endl;
  fs << "B_ext = vector (0,"<< m.by <<","<< m.bz <<")"<< std::endl;
  fs << "anisu = vector (0,0,1)"<< std::endl;
  fs << "m=neelskyrmion(1,-1)"<< std::endl;
  fs << "minimize()"<< std::endl;
  fs << "saveas(m,\"../"<< m.make_sname() <<".ovf\")"<< std::endl;
  return;
}

void make_dyn(const material& m){
  std::fstream fs(m.make_name()+".mx3",std::fstream::out|std::fstream::binary);
  fs << "SetMesh(256,256,1,1e-9,1e-9,"<< m.lz<<",16,16,0)" << std::endl;
  fs << "Msat = "<< m.ms << std::endl;
  fs << "Aex = "<< m.aex << std::endl;
  fs << "alpha = "<< m.alpha << std::endl;
  fs << "dind = "<< m.d << std::endl;
  fs << "ku1 = "<< m.ku << std::endl;
  fs << "B_ext = vector (0,"<< m.by <<","<< m.bz <<")"<< std::endl;
  fs << "anisu = vector (0,0,1)"<< std::endl;
  fs << "m.loadfile(\""<< m.make_sname() <<".ovf\")"<< std::endl;
  fs << "lambda=1"<< std::endl;
  fs << "pol=-0.33"<< std::endl;
  fs << "epsilonprime=0"<< std::endl;
  fs << "fixedlayer=vector(0,1,0)"<< std::endl;
  fs << "J=vector(0,0,"<<m.j<<")"<< std::endl;
  fs << "autosave(m,"<< m.get_autosave() <<")"<< std::endl;
  fs << "run(" << m.get_runtime() << ")"<< std::endl;
  return;
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
