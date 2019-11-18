#include "vf.hpp"
#include <iomanip>
#include "files.hpp"
#include <algorithm>

namespace rcl{


void crush_vf(std::vector<unsigned char>& dst,const rcl::ffield& src,const vf_crush_cfg& cfg){
  const unsigned char vfsig[40]={
  0x46,0x27,0x1a,0xad,0xda,0x3e,0x16,0xb6,0xa7,0x72,0x2b,0x1c,0xa3,0xe2,0xee,0x8a,
  0xcf,0x8c,0x6a,0xbb,0x62,0x9d,0xec,0xff,0x0a,0x89,0xa9,0xad,0xeb,0x1e,0x77,0xe5,
  0x5e,0x72,0xda,0x2b,0xf8,0x58,0xa5,0x7b};
  bool echo = cfg.get_echo();
  bool quick = cfg.get_quick();
  bool nosaf = cfg.get_nosaf();
  unsigned int size;
  rcl::vstream out(dst);
  rcl::stringmap meta = src.getmeta();
  rcl::utriad nodes = src.getnodes();
  std::vector<rcl::rpolar> master;
  rcl::VectoLpol(master,src);
  rcl::chop(master);
  std::vector<unsigned char> bzdata;  
  int bsize=2147483647,bzl=9,bzs=0,bsaf=0,bsbit=1,bperm=0;
  for (int zl=1;zl<=9;zl++) 
  for (int zs=0;zs<4;zs++) 
  for (int perm=1;perm<12;perm++)
  for (int saf=0;saf<(1<<nodes.z());saf++)
  for (int sbit=0;sbit<2;sbit++){
    bool skip=false;
    if (quick){
      if (zl!=9) skip=true;
      if (sbit==0) skip=true;
      if (zs!=0) skip=true;      }
    if (nosaf){ 
      if (saf!=0) skip=true;
      //if (perm==1) skip=true;
      //if (perm==2) skip=true;
      //if (perm==3) skip=true;
      //if (perm==4) skip=true;
      //if (perm==5) skip=true;
      //if (perm==7) skip=true;
      //if (perm==8) skip=true;
      //if (perm==9) skip=true;
    }
    if (nodes.z()==1) {
      if (perm==1) skip=true;
      if (perm==3) skip=true;
      if (perm==4) skip=true;
      if (perm==5) skip=true;
      if (perm>8)  skip=true;    }
    if (!skip){
      std::vector<unsigned char> data,zdata;
      std::vector<rcl::rpolar> v = master;
      rcl::SAF_Filter(v,saf,nodes);
      rcl::Permute(v,nodes,perm);
      if (sbit==0) rcl::LpoltoRBin(data,v);
      else         rcl::LpoltoRSBin(data,v);
      rcl::deflate(zdata,data,rcl::zlib_cfg(zl,zs,0,0));
      if (echo) std::cout << cfg.get_label() << " : ";
      if (echo) std::cout << "l"<< zl << ":s" << zs << ":p" << std::hex << perm << std::dec << ":n" << std::hex << saf << std::dec << ":b" << sbit << " - ";
      if (echo) std::cout << std::setw(8) << zdata.size() << " - " << std::setw(8) << bsize;
      if (zdata.size()<bsize){ bsize=zdata.size(); bzl=zl; bzs=zs; bsaf=saf; bsbit=sbit; bperm=perm; std::swap(bzdata,zdata); if (echo) std::cout << " * ";}
      if (echo) std::cout << std::endl;
    }
  }
  meta["vf_crush"]="true";
  meta["vf_v2"]="true";
  meta["vf_zl"]=rcl::to_str(bzl);
  meta["vf_zs"]=rcl::to_str(bzs);
  meta["vf_saf"]=rcl::to_str(bsaf);
  meta["vf_perm"]=rcl::to_str(bperm);
  if (bsbit==1) meta["vf_sbit"]="true";
  else          meta["vf_sbit"]="false";
  
  std::vector<unsigned char> dic,zdic;
  rcl::dump(dic,meta);
  rcl::deflate(zdic,dic,false);
  
  out.write(vfsig,sizeof(vfsig));
  
  size = zdic.size();
  out.write(reinterpret_cast<const unsigned char*>(&size),4);
  out.write(&zdic[0],size);
  
  size = bzdata.size();
  out.write(reinterpret_cast<const unsigned char*>(&size),4);
  out.write(&bzdata[0],size);
}

void encode_vf(std::vector<unsigned char>& dst,const rcl::ffield& srcc){
  const unsigned char vfsig[40]={
  0x46,0x27,0x1a,0xad,0xda,0x3e,0x16,0xb6,0xa7,0x72,0x2b,0x1c,0xa3,0xe2,0xee,0x8a,
  0xcf,0x8c,0x6a,0xbb,0x62,0x9d,0xec,0xff,0x0a,0x89,0xa9,0xad,0xeb,0x1e,0x77,0xe5,
  0x5e,0x72,0xda,0x2b,0xf8,0x58,0xa5,0x7b};
  unsigned int size;
  std::vector<unsigned char> dic,zdic,bdata,zdata;
  rcl::vstream out(dst);
  rcl::ffield src(srcc);
  
  if (!src.is_meta("vf_v2"))   src.reg_meta("vf_v2","true");
  if (!src.is_meta("vf_saf"))  src.reg_meta("vf_saf","0");
  if (!src.is_meta("vf_perm")) src.reg_meta("vf_perm","0");
  if (!src.is_meta("vf_sbit")) src.reg_meta("vf_sbit","true");
  if (!src.is_meta("vf_zl"))   src.reg_meta("vf_zl","9");
  if (!src.is_meta("vf_zs"))   src.reg_meta("vf_zs","0");
  
  rcl::dump(dic,src.getmeta());
  rcl::deflate(zdic,dic,false);
  
  out.write(vfsig,sizeof(vfsig));
  
  size = zdic.size();
  out.write(reinterpret_cast<const unsigned char*>(&size),4);
  out.write(&zdic[0],size);
  
  if (src.is_meta("vf_v2") && src.get_meta("vf_v2")=="false"){
    std::vector<rcl::lpolar> pdata;
    rcl::VectoLpol(pdata,src);
    rcl::LpoltoSBin(bdata,pdata);
    rcl::deflate(zdata,bdata,false);
    }
  else{
    std::vector<rcl::rpolar> pdata;
    if (src.is_meta("vf_saf")){
    if (src.get_val<int>("vf_saf")!=0){
      rcl::ffield w = src;
      SAF_Filter(w,w.get_val<unsigned int>("vf_saf"));
      rcl::VectoLpol(pdata,w);
      }
    else {rcl::VectoLpol(pdata,src);} 
    }
    else {rcl::VectoLpol(pdata,src);} 
    
    rcl::chop(pdata);
    if (src.is_meta("vf_perm")) rcl::Permute(pdata,src.getnodes(),src.get_val<int>("vf_perm"));
    if (src.get_meta("vf_sbit")=="false") rcl::LpoltoRBin(bdata,pdata);
    else                                  rcl::LpoltoRSBin(bdata,pdata);
    int zl=9,zs=0;
    if (src.is_meta("vf_zl")) zl = src.get_val<int>("vf_zl");
    if (src.is_meta("vf_zs")) zs = src.get_val<int>("vf_zs");
    rcl::deflate(zdata,bdata,rcl::zlib_cfg(zl,zs,0,0));
    //std::cout << "zdata=" << zdata.size() ;
    //std::cout << "([zl]" <<zl << ",[zs]" << zs << ",[span]" << src.get_meta("vf_sbit") ;
    //std::cout << ",[P]" << src.get_meta("vf_perm") << ",[saf]" << src.get_meta("vf_saf") << ")" << std::endl;
    }
  
  size = zdata.size();
  out.write(reinterpret_cast<const unsigned char*>(&size),4);
  out.write(&zdata[0],size);
  
  return;
}

void decode_vf(rcl::ffield& dst,const std::vector<unsigned char>& src){
  const unsigned char vfsig[40]={
  0x46,0x27,0x1a,0xad,0xda,0x3e,0x16,0xb6,0xa7,0x72,0x2b,0x1c,0xa3,0xe2,0xee,0x8a,
  0xcf,0x8c,0x6a,0xbb,0x62,0x9d,0xec,0xff,0x0a,0x89,0xa9,0xad,0xeb,0x1e,0x77,0xe5,
  0x5e,0x72,0xda,0x2b,0xf8,0x58,0xa5,0x7b};
  unsigned int size;
  std::vector<unsigned char> dic,zdic,bdata,zdata;
  
  rcl::cvstream in(src);
  
  std::vector<unsigned char>::const_iterator cursor;
  cursor = std::search(src.begin(),src.end(),vfsig,vfsig+sizeof(vfsig));
  if (cursor==src.begin()) in.advance(sizeof(vfsig));
    
  in.sread(reinterpret_cast<unsigned char*>(&size),4);
  zdic.resize(size);
  in.sread(&zdic[0],size);
  rcl::inflate(dic,zdic,false);
  rcl::dump(dst.getmeta(),dic);
  dst.parse_meta();
  
  in.sread(reinterpret_cast<unsigned char*>(&size),4);
  zdata.resize(size);
  in.sread(&zdata[0],size);
  rcl::inflate(bdata,zdata,false);
  
  if (dst.is_meta("vf_v2")==false || dst.get_meta("vf_v2")!="true"){
    std::vector<rcl::lpolar> pdata;
    rcl::SBintoLpol(pdata,bdata);
    rcl::LpoltoVec(dst,pdata);
    return;
  }
  else{
    std::vector<rcl::rpolar> rdata;
    if (dst.get_meta("vf_sbit")=="false") rcl::RBintoLpol(rdata,bdata);
    if (dst.get_meta("vf_sbit")=="true") rcl::RSBintoLpol(rdata,bdata);
    if (dst.is_meta("vf_perm")) 
      rcl::Permute(rdata,dst.getnodes(),InversePerm(dst.get_val<int>("vf_perm")));
    rcl::LpoltoVec(dst,rdata);
    if (dst.is_meta("vf_saf")) rcl::SAF_Filter(dst,dst.get_val<unsigned int>("vf_saf"));
  }
  return;
}

void decode_vf_meta(rcl::stringmap& dst,const std::vector<unsigned char>& src){
  const unsigned char vfsig[40]={
  0x46,0x27,0x1a,0xad,0xda,0x3e,0x16,0xb6,0xa7,0x72,0x2b,0x1c,0xa3,0xe2,0xee,0x8a,
  0xcf,0x8c,0x6a,0xbb,0x62,0x9d,0xec,0xff,0x0a,0x89,0xa9,0xad,0xeb,0x1e,0x77,0xe5,
  0x5e,0x72,0xda,0x2b,0xf8,0x58,0xa5,0x7b};
  unsigned int size;
  std::vector<unsigned char> dic,zdic;
  rcl::cvstream in(src);
  
  std::vector<unsigned char>::const_iterator cursor;
  cursor = std::search(src.begin(),src.end(),vfsig,vfsig+sizeof(vfsig));
  if (cursor==src.begin()) in.advance(sizeof(vfsig));
  
  //if (cursor==src.begin()) std::cout << "File has signature" << std::endl;
  //else                     std::cout << "File doesn't have signature" << std::endl;    
  
  in.sread(reinterpret_cast<unsigned char*>(&size),4);
  zdic.resize(size);
  in.sread(&zdic[0],size);
  rcl::inflate(dic,zdic,false);
  rcl::dump(dst,dic);
}

void load_vf_meta(rcl::stringmap& dst,const std::string& filename){
  const unsigned char vfsig[40]={
  0x46,0x27,0x1a,0xad,0xda,0x3e,0x16,0xb6,0xa7,0x72,0x2b,0x1c,0xa3,0xe2,0xee,0x8a,
  0xcf,0x8c,0x6a,0xbb,0x62,0x9d,0xec,0xff,0x0a,0x89,0xa9,0xad,0xeb,0x1e,0x77,0xe5,
  0x5e,0x72,0xda,0x2b,0xf8,0x58,0xa5,0x7b};
  std::vector<unsigned char> buffer;
  std::fstream file(filename,std::fstream::in|std::fstream::binary);
  if (!file.good()) return;
  file.seekg(0,std::fstream::end);
  size_t size = file.tellg();
  if (size<44) return;
  buffer.resize(44);
  file.seekg(0);
  file.read(reinterpret_cast<char*>(&buffer[0]),44);
  
  unsigned int hsize;
  std::vector<unsigned char>::const_iterator cursor;
  cursor = std::search(buffer.begin(),buffer.end(),vfsig,vfsig+sizeof(vfsig));
  
  if (cursor==buffer.begin()) {
    //std::cout << "File has signature" << std::endl;
    hsize = reinterpret_cast<unsigned int&>(buffer[sizeof(vfsig)]);
    buffer.resize(buffer.size()+hsize);
    file.read(reinterpret_cast<char*>(&buffer[44]),hsize);
    }
  else{
    //std::cout << "File doesn't have signature" << std::endl;
    hsize = reinterpret_cast<unsigned int&>(buffer[0]);
    buffer.resize(4+hsize);
    //std::cout << "Header is " << hsize << " bytes long" << std::endl;
    if (hsize>44) file.read(reinterpret_cast<char*>(&buffer[44]),hsize-40);
    //std::cout << "File stopped at " << file.tellg() << std::endl;
    //std::cout << "Buffer is " << buffer.size() << " bytes long" << std::endl;
    }
  decode_vf_meta(dst,buffer);
  return;
  }

bool is_vf_crush(const std::string& filename){
  rcl::stringmap dst;
  rcl::load_vf_meta(dst,filename);
  if (dst["vf_crush"]=="true") return true;
  return false;
  }

bool is_vf_signed(const std::string& filename){
  const unsigned char vfsig[40]={
  0x46,0x27,0x1a,0xad,0xda,0x3e,0x16,0xb6,0xa7,0x72,0x2b,0x1c,0xa3,0xe2,0xee,0x8a,
  0xcf,0x8c,0x6a,0xbb,0x62,0x9d,0xec,0xff,0x0a,0x89,0xa9,0xad,0xeb,0x1e,0x77,0xe5,
  0x5e,0x72,0xda,0x2b,0xf8,0x58,0xa5,0x7b};
  
  unsigned char sigread[40];
  std::ifstream file(filename,std::fstream::in|std::fstream::binary);
  if (!file.good()) return false;
  file.read(reinterpret_cast<char*>(sigread),40);
  if (!file.good()) return false;
  for (unsigned int idx=0;idx<40;idx++) if (vfsig[idx]!=sigread[idx]) return false;
  return true;  
  }


}
