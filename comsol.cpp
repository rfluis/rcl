#include "comsol.hpp"
//#include <iostream>
namespace rcl{

bool is_COMSOL(const std::vector<unsigned char>& file){
  const std::string comsol1("% Version:");
  const std::string comsol2("COMSOL");
  std::vector<unsigned char>::const_iterator cursor;
  cursor = search(file.begin(),file.end(),comsol1.begin(),comsol1.end());
  if (cursor==file.end()) return false;
  cursor = search(file.begin(),file.end(),comsol2.begin(),comsol2.end());
  if (cursor==file.end()) return false;
  return true;
  }

void load_COMSOL(rcl::ffield& dst,const std::vector<unsigned char>& src){
  std::vector<std::string> lines;
  rcl::parsetextfile(lines,src);
  int dimension,expressions;
  
  /*  % Dimension:          3
      % Nodes:              262144
      % Expressions:        3       */
  const std::string& dimension_str("Dimension:");
  const std::string& expressions_str("Expressions:");
  
  for (int idx=0;idx<lines.size();idx++) if (lines[idx][0]=='%'){
    if (std::search(lines[idx].begin(),lines[idx].end(),dimension_str.begin(),dimension_str.end())!=lines[idx].end()) {
      std::string::const_iterator cursor = std::search(lines[idx].begin(),lines[idx].end(),dimension_str.begin(),dimension_str.end());
      cursor+=dimension_str.length();
      std::string str(cursor,lines[idx].cend());
      std::stringstream(str) >> dimension;
      break;
      }
    }
  std::cout << dimension_str << dimension << std::endl;
  
  for (int idx=0;idx<lines.size();idx++) if (lines[idx][0]=='%'){
    if (std::search(lines[idx].begin(),lines[idx].end(),expressions_str.begin(),expressions_str.end())!=lines[idx].end()) {
      std::string::const_iterator cursor = std::search(lines[idx].begin(),lines[idx].end(),expressions_str.begin(),expressions_str.end());
      cursor+=expressions_str.length();
      std::string str(cursor,lines[idx].cend());
      std::stringstream(str) >> expressions;
      break;
      }
    }
  std::cout << expressions_str << expressions << std::endl;    
  
  
  std::map<float,unsigned int> px,py,pz;
  if (dimension==2){
    for (int idx=0;idx<lines.size();idx++){
      if (lines[idx][0]!='%'){
        float lx,ly,lz;
        std::stringstream(lines[idx]) >> lx >> ly;
        if (px.find(lx)==px.end()) px[lx]=0;
        if (py.find(ly)==py.end()) py[ly]=0;
      }
    }
  }
  
  if (dimension==3){
    for (int idx=0;idx<lines.size();idx++){
      if (lines[idx][0]!='%'){
        float lx,ly,lz;
        std::stringstream(lines[idx]) >> lx >> ly >> lz;
        if (px.find(lx)==px.end()) px[lx]=0;
        if (py.find(ly)==py.end()) py[ly]=0;
        if (pz.find(lz)==pz.end()) pz[lz]=0;
      }
    }
  }
  //if (dimension==2) std::cout << px.size() << " × " << py.size() << std::endl;
  //if (dimension==3) std::cout << px.size() << " × " << py.size() << " × " << pz.size() << std::endl;
  
  if (dimension>0){unsigned int idx=0;for (auto itr=px.begin();itr!=px.end();itr++) itr->second=idx++;}
  if (dimension>1){unsigned int idx=0;for (auto itr=py.begin();itr!=py.end();itr++) itr->second=idx++;}
  if (dimension>2){unsigned int idx=0;for (auto itr=pz.begin();itr!=pz.end();itr++) itr->second=idx++;}
  //std::cout << "COMSOL size is " << px.size() << "x" << py.size() << std::endl;
  rcl::utriad size;
  if (dimension==1) size=rcl::utriad(px.size(),1,1);
  if (dimension==2) size=rcl::utriad(px.size(),py.size(),1);
  if (dimension==3) size=rcl::utriad(px.size(),py.size(),pz.size());
  dst.fit(size);
  
  for (int idx=0;idx<lines.size();idx++){
    //std::cout << "Line " << idx << " :" << lines[idx] << std::endl;
    if (lines[idx][0]!='%'){
      std::string sjx,sjy,sjz;
      float lx,ly,lz,jx=0,jy=0,jz=0;
      jx=jy=jz=0.f;
      std::stringstream ss(lines[idx]);
      rcl::utriad p;
      if (dimension>0)    { ss >> lx;  }
      if (dimension>1)    { ss >> ly;  }
      if (dimension>2)    { ss >> lz;  }
      if (expressions>0)  { ss >> sjx; }
      if (expressions>1)  { ss >> sjy; }
      if (expressions>2)  { ss >> sjz; }
      if (dimension==1)  p=rcl::utriad(px[lx],0,0);
      if (dimension==2)  p=rcl::utriad(px[lx],py[ly],0);
      if (dimension==3)  p=rcl::utriad(px[lx],py[ly],pz[lz]);
      if (sjx!="NaN") {
        std::stringstream(sjx) >> jx;
        std::stringstream(sjy) >> jy;
        std::stringstream(sjz) >> jz;
        dst[p]=rcl::fvector(jx,jy,jz);
      }
      else dst[p]=rcl::fvector(0,0,0);
    }
  }
  
  //void populate_meta(rcl::ffield& m,
  //const std::string& title,const std::string& desc,
  //const std::string& valuelabels,const std::string& valueunits);
  rcl::populate_meta(dst,"Electric Current","Comsol Solution in regular grid","J_x J_y J_z","A_m-2 A_m-2 A_m-2");
  return;
}

void current_field_z(rcl::ffield& dst,const rcl::ffield& src){
  rcl::utriad size=src.getnodes();
  unsigned int isize=size.volume();
  dst.fit(size);
  for (unsigned int idx=0;idx<isize;idx++) dst[idx]=rcl::fvector(0.f,0.f,src[idx].norm());
  return;
  }
  
void current_regions(rcl::fsfield& dst,const rcl::ffield& src){
  rcl::utriad size=src.getnodes();
  unsigned int isize=size.volume();
  dst.fit(size);
  for (unsigned int idx=0;idx<isize;idx++) dst[idx]=std::floor(src[idx].phi()/rcl::PI<float>*248.f);
  for (unsigned int idx=0;idx<isize;idx++) if (dst[idx]<0.f) dst[idx]+=248.f;
  return;
  }
  
void current_mask(rcl::image32& dst,const rcl::ffield& src){
  rcl::utriad size=src.getnodes();
  unsigned int isize=size.x()*size.y();
  dst.fit(size.x(),size.y());
  for (unsigned int idx=0;idx<isize;idx++) {if (src[idx].norm2()==0.f) dst[idx]=rcl::pixel32(0xff000000); else dst[idx]=rcl::pixel32(0xffffffff);}
}



}
