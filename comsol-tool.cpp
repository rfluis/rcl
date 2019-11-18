#include <iostream>
#include "busybox.hpp"
#include "oommf.hpp"
#include "files.hpp"
#include "lib-zlib.hpp"
#include "text.hpp"
#include "vf.hpp"
#include "filepath.hpp"
#include "lib-png.hpp"
#include "newdisplay.hpp"
#include "comsol.hpp"
#include "tfield_io.hpp"

void comsol2mumax(const std::string& filename);

int comsol_tool_main(int argc,char **argv){
  std::vector<std::string> args(argv,argv+argc);
  if (argc==1){
    std::cout << argv[0] << "<fichero.txt> <fichero2.txt.gz> ..."<< std::endl;
    return 1;
  }
  for (int idx=1;idx<args.size();idx++) comsol2mumax(args[idx]);
  return 0;
}
static rcl::reg_app reg("comsol_tool",comsol_tool_main);

void comsol2mumax(const std::string& filename){
  rcl::FilePath fpath(filename);
  rcl::ffield current,currentz;
  rcl::fsfield regions;
  rcl::image32 mask;
  
  rcl::load_vecfile(current,filename);
  currentz.fit(current.getnodes());
  regions.fit(current.getnodes());
  mask.fit(current.getnodes().x(),current.getnodes().y());
  
  for (unsigned int idy=0;idy<current.getnodes().y();idy++)
  for (unsigned int idx=0;idx<current.getnodes().x();idx++){
  rcl::utriad p(idx,idy,0);
  if (current[p]!=rcl::fvector()){
    currentz[p]=rcl::fvector(0,0,current[p].norm());
    regions[p]=std::floor(std::atan2(current[p].y(),current[p].x())/rcl::TPI<float>*255.);
    if (regions[p]<0) regions[p]+=255.;
    mask(p.x(),p.y())=rcl::pixel32(0xff000000);
  }
  else
  {
    currentz[p]=rcl::fvector(0,0,0);
    mask(p.x(),p.y())=rcl::pixel32(0xffffffff);
  }
}
     
  {
    std::string pngp = fpath.GetRoot() + ".png";
    std::vector<unsigned char> pngb;
    rcl::encode_png(pngb,mask.flip());
    rcl::save_file(pngp,pngb);
  }
  
  {
    std::string ovfp = fpath.GetRoot() + "_xy.ovf";
    std::vector<unsigned char> ovfb;
    rcl::saveOVF2vm(ovfb,current);
    rcl::save_file(ovfp,ovfb);
  }
  
  {
    std::string ovfp = fpath.GetRoot() + "_z.ovf";
    std::vector<unsigned char> ovfb;
    rcl::saveOVF2vm(ovfb,currentz);
    rcl::save_file(ovfp,ovfb);
  }
  
    {
    std::string ovfp = fpath.GetRoot() + "_r.ovf";
    std::vector<unsigned char> ovfb;
    rcl::saveOVF2vm(ovfb,regions);
    rcl::save_file(ovfp,ovfb);
  }
  
  return;
}

/*
void comsol2mumax(const std::string& filename){
  rcl::FilePath fpath(filename);
  std::vector<unsigned char> infile;
  rcl::load_file(infile,filename);
  if (fpath.GetLext()==".gz"){
    std::vector<unsigned char> temp;
    rcl::inflate(temp,infile,true);
    std::swap(infile,temp);
    }
  std::vector<std::string> lines;
  rcl::parsetextfile(lines,infile);
  infile.clear();
  
  std::map<float,unsigned int> px,py;
  for (int idx=0;idx<lines.size();idx++){
    if (lines[idx][0]!='%'){
      float lx,ly;
      std::stringstream(lines[idx]) >> lx >> ly;
      if (px.find(lx)==px.end()) px[lx]=0;
      if (py.find(ly)==py.end()) py[ly]=0;
    }
  }
  
  {unsigned int idx=0;for (auto itr=px.begin();itr!=px.end();itr++) itr->second=idx++;}
  {unsigned int idx=0;for (auto itr=py.begin();itr!=py.end();itr++) itr->second=idx++;}
  
  std::cout << "Guessed geometry:" << px.size() << " , " << py.size() << std::endl;  
  
  rcl::utriad size(px.size(),py.size(),1);
  
  rcl::ffield current(size),currentz(size);
  rcl::fsfield regions(size);
  rcl::image32 mask(size.x(),size.y());
  
  for (int idx=0;idx<lines.size();idx++){
    if (lines[idx][0]!='%'){
      std::string sjx,sjy;
      float lx,ly,jx,jy;
      std::stringstream(lines[idx]) >> lx >> ly >> sjx >> sjy;
      rcl::utriad p(px[lx],py[ly],0);
      if (sjx!="NaN") {
        std::stringstream(sjx) >> jx;
        std::stringstream(sjy) >> jy;
        current[p]=rcl::fvector(jx,jy,0);
        currentz[p]=rcl::fvector(0,0,current[p].norm());
        regions[p]=std::floor(std::atan2(jy,jx)/rcl::PI<float>*248.);
        if (regions[p]<0) regions[p]+=248.;
        mask(p.x(),p.y())=rcl::pixel32(0xff000000);
      }
      else{
        current[p]=rcl::fvector(0,0,0);
        currentz[p]=rcl::fvector(0,0,0);
        mask(p.x(),p.y())=rcl::pixel32(0xffffffff);
      }
    }
  }
  
  {
    std::string pngp = fpath.GetRoot() + ".png";
    std::vector<unsigned char> pngb;
    rcl::encode_png(pngb,mask);
    rcl::save_file(pngp,pngb);
  }
  
  {
    std::string ovfp = fpath.GetRoot() + "_xy.ovf";
    std::vector<unsigned char> ovfb;
    rcl::saveOVF2vm(ovfb,current);
    rcl::save_file(ovfp,ovfb);
  }
  
  {
    std::string ovfp = fpath.GetRoot() + "_z.ovf";
    std::vector<unsigned char> ovfb;
    rcl::saveOVF2vm(ovfb,currentz);
    rcl::save_file(ovfp,ovfb);
  }
  
    {
    std::string ovfp = fpath.GetRoot() + "_r.ovf";
    std::vector<unsigned char> ovfb;
    rcl::saveOVF2vm(ovfb,regions);
    rcl::save_file(ovfp,ovfb);
  }
  
  return;
}

*/
