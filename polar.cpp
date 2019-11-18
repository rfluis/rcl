#include "polar.hpp"

namespace rcl{

void VectoLpol(std::vector<rcl::lpolar>& v,const rcl::ffield& f){
  unsigned int nodes = f.getnodes().volume();
  if (v.size()<nodes) v.resize(nodes);
  for (unsigned int idx=0;idx<nodes;idx++) v[idx]=f[idx];
  return;
}

void LpoltoVec(rcl::ffield& f,const std::vector<rcl::lpolar>& v){
  if (v.size()<f.getnodes().volume()) return;
  unsigned int nodes = f.getnodes().volume();
  fvector* ptr;
  for (unsigned int idx=0;idx<nodes;idx++) f[idx]=v[idx].getv();
  return;
}

void LpoltoSBin(std::vector<unsigned char>& s,const std::vector<rcl::lpolar>& v){
  bitstream in(reinterpret_cast<const unsigned char*>(&v[0]),8*v.size());
  bitstream out(8*v.size());
  unsigned int len  = v.size();
  for (unsigned int b=0;b<64;b++) for (unsigned int l=0;l<len;l++) if (in.readat(b+64*l)) out.writeat(len*b+l);
  if (s.size()<8*v.size()) s.resize(8*v.size());
  unsigned int size = 8*len;
  for (unsigned int idx=0;idx<size;idx++) s[idx]=out(idx);
  return; 
}
  
void SBintoLpol(std::vector<rcl::lpolar>& v,const std::vector<unsigned char>& s){
  unsigned int size = s.size();
  unsigned int len  = size/8;
  if (v.size()<len) v.resize(len);  
  bitstream in(&(*s.begin()),size);
  bitstream out(size);
  for (unsigned int b=0;b<64;b++) for (unsigned int l=0;l<len;l++) if (in.readat(len*b+l)) out.writeat(b+64*l);
  std::copy(out.begin(),out.end(),reinterpret_cast<unsigned char*>(&v[0]));
  return; 
}

void LpoltoBin(std::vector<unsigned char>& s,const std::vector<rcl::lpolar>& v){
  unsigned int len = v.size();
  unsigned int size = len*8;
  if (s.size()<size) s.resize(size);
  rcl::lpolar *ptr;
  ptr = reinterpret_cast<rcl::lpolar*>(&s[0]);
  std::copy(v.begin(),v.end(),ptr);
  return;
}

void BintoLpol(std::vector<rcl::lpolar>& v,const std::vector<unsigned char>& s){
  unsigned int size = s.size();
  unsigned int len = size/8;
  if (v.size()<len) v.resize(len);
  const rcl::lpolar *ptr = reinterpret_cast<const rcl::lpolar*>(&s[0]);
  std::copy(ptr,ptr+len,v.begin());
}

void Permute(std::vector<rcl::lpolar>& v,const rcl::utriad& nodes,const int& perm){
  std::vector<rcl::lpolar> ans;
  if (perm<1)  return;
  if (perm>11) return;
  if (perm==6) return;
  ans.resize(nodes.volume());
  v.resize(nodes.volume());
  /*
   00 xyz->xyz x+y·nx+z·nx·ny        06 xyz->xyz x+y·nx+z·nx·ny
   01 xzy->xyz x+z·nx+y·nx·nz        07 xyz->xzy x+z·nx+y·nx·nz
   02 yxz->xyz y+x·ny+z·ny·nx        08 xyz->yxz y+x·ny+z·ny·nx
   03 yzx->xyz y+z·ny+x·ny·nz        09 xyz->yzx y+z·ny+x·ny·nz
   04 zxy->xyz z+x·nz+y·nz·nx        10 xyz->zxy z+x·nz+y·nz·nx
   05 zyx->xyz z+y·nz+x·nz·ny        11 xyz->zyx z+y·nz+x·nz·ny
   */
  if (perm==0) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==1) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[x+z*nodes.x()+y*nodes.x()*nodes.z()];
  if (perm==2) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[y+x*nodes.y()+z*nodes.y()*nodes.x()];
  if (perm==3) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[y+z*nodes.y()+x*nodes.y()*nodes.z()];
  if (perm==4) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[z+x*nodes.z()+y*nodes.z()*nodes.x()];
  if (perm==5) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[z+y*nodes.z()+x*nodes.z()*nodes.y()];
  
  if (perm==6) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==7) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+z*nodes.x()+y*nodes.x()*nodes.z()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==8) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[y+x*nodes.y()+z*nodes.y()*nodes.x()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==9) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[y+z*nodes.y()+x*nodes.y()*nodes.z()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==10) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[z+x*nodes.z()+y*nodes.z()*nodes.x()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==11) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[z+y*nodes.z()+x*nodes.z()*nodes.y()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];          
  std::swap(ans,v);
}

/* ################################################################################# */

void VectoLpol(std::vector<rcl::rpolar>& v,const rcl::ffield& f){
  unsigned int nodes = f.getnodes().volume();
  if (v.size()<nodes) v.resize(nodes);
  for (unsigned int idx=0;idx<nodes;idx++) v[idx]=f[idx];
  return;
}

void LpoltoVec(rcl::ffield& f,const std::vector<rcl::rpolar>& v){
  if (v.size()<f.getnodes().volume()) return;
  unsigned int nodes = f.getnodes().volume();
  fvector* ptr;
  for (unsigned int idx=0;idx<nodes;idx++) f[idx]=v[idx].getv();
  return;
}

void LpoltoSBin(std::vector<unsigned char>& s,const std::vector<rcl::rpolar>& v){
  bitstream in(reinterpret_cast<const unsigned char*>(&v[0]),8*v.size());
  bitstream out(8*v.size());
  unsigned int len  = v.size();
  for (unsigned int b=0;b<64;b++) for (unsigned int l=0;l<len;l++) if (in.readat(b+64*l)) out.writeat(len*b+l);
  if (s.size()<8*v.size()) s.resize(8*v.size());
  unsigned int size = 8*len;
  for (unsigned int idx=0;idx<size;idx++) s[idx]=out(idx);
  return; 
}

void LpoltoRSBin(std::vector<unsigned char>& s,const std::vector<rcl::rpolar>& v){
  bitstream64 in(reinterpret_cast<const unsigned long long int*>(&v[0]),v.size());
  bitstream out(8*v.size());
  unsigned int len  = v.size();
  for (unsigned int b=0;b<64;b++) for (unsigned int l=0;l<len;l++) if (in.readat(b+64*l)) out.writeat(len*b+l);
  if (s.size()<8*v.size()) s.resize(8*v.size());
  unsigned int size = 8*len;
  for (unsigned int idx=0;idx<size;idx++) s[idx]=out(idx);
  return; 
}
  
void SBintoLpol(std::vector<rcl::rpolar>& v,const std::vector<unsigned char>& s){
  unsigned int size = s.size();
  unsigned int len  = size/8;
  if (v.size()<len) v.resize(len);  
  bitstream in(&(*s.begin()),size);
  bitstream out(size);
  for (unsigned int b=0;b<64;b++) for (unsigned int l=0;l<len;l++) if (in.readat(len*b+l)) out.writeat(b+64*l);
  std::copy(out.begin(),out.end(),reinterpret_cast<unsigned char*>(&v[0]));
  return; 
}

void RSBintoLpol(std::vector<rcl::rpolar>& v,const std::vector<unsigned char>& s){
  unsigned int size = s.size();
  unsigned int len  = size/8;
  if (v.size()<len) v.resize(len);  
  bitstream   in(&(*s.begin()),size);
  bitstream64 out(len);
  for (unsigned int b=0;b<64;b++) for (unsigned int l=0;l<len;l++) if (in.readat(len*b+l)) out.writeat(b+64*l);
  std::copy(out.begin(),out.end(),reinterpret_cast<unsigned long long int*>(&v[0]));
  return; 
}

void LpoltoBin(std::vector<unsigned char>& s,const std::vector<rcl::rpolar>& v){
  unsigned int len = v.size();
  unsigned int size = len*8;
  if (s.size()<size) s.resize(size);
  rcl::rpolar *ptr;
  ptr = reinterpret_cast<rcl::rpolar*>(&s[0]);
  std::copy(v.begin(),v.end(),ptr);
  return;
}

void LpoltoRBin(std::vector<unsigned char>& s,const std::vector<rcl::rpolar>& v){
  unsigned int len = v.size();
  unsigned int size = len*8;
  unsigned int bits = len*64;
  if (s.size()<size) s.resize(size);
  rcl::bitstream64 in(reinterpret_cast<const unsigned long long int*>(&v[0]),len);
  rcl::bitstream  out(size);
  for (unsigned int idx=0;idx<bits;idx++) out[idx]=in[idx];
  unsigned char *ptr = reinterpret_cast<unsigned char*>(&s[0]);
  std::copy(out.begin(),out.end(),ptr);
  return;
}

void BintoLpol(std::vector<rcl::rpolar>& v,const std::vector<unsigned char>& s){
  unsigned int size = s.size();
  unsigned int len = size/8;
  if (v.size()<len) v.resize(len);
  const rcl::rpolar *ptr = reinterpret_cast<const rcl::rpolar*>(&s[0]);
  std::copy(ptr,ptr+len,v.begin());
}

void RBintoLpol(std::vector<rcl::rpolar>& v,const std::vector<unsigned char>& s){
  unsigned int size = s.size();
  unsigned int len = size/8;
  unsigned int bits = len*64;
  if (v.size()<len) v.resize(len);
  rcl::bitstream    in(s);
  rcl::bitstream64  out(len);
  for (unsigned int idx=0;idx<bits;idx++) out[idx]=in[idx];
  const rcl::rpolar *ptr = reinterpret_cast<const rcl::rpolar*>(out.begin());
  std::copy(ptr,ptr+len,v.begin());
}

void chop(std::vector<rcl::rpolar>& v){
  std::vector<float> rads;
  float diff;
  float mean;
  for (auto itr=v.begin();itr!=v.end();itr++) if (itr->getr()!=0.f) rads.push_back(itr->getr());
  std::sort(rads.begin(),rads.end());
  diff = rads[rads.size()-1]-rads[0];
  diff = diff / rads[rads.size()-1];
  if (diff<1e-4) {
    rcl::sumplex(rads);
    mean=rads[0]/static_cast<float>(rads.size());
    if (std::fabs(mean-std::floor(mean))<1e-4) mean = std::floor(mean);
    if (std::fabs(mean-std::ceil(mean))<1e-4) mean = std::ceil(mean);
    for (auto itr=v.begin();itr!=v.end();itr++) if (itr->getr()!=0.f) itr->setr(mean);
  }
  return;
  }

void Permute(std::vector<rcl::rpolar>& v,const rcl::utriad& nodes,const int& perm){
  std::vector<rcl::rpolar> ans;
  if (perm<1)  return;
  if (perm>11) return;
  if (perm==6) return;
  ans.resize(nodes.volume());
  v.resize(nodes.volume());
  /*
   00 xyz->xyz x+y·nx+z·nx·ny        06 xyz->xyz x+y·nx+z·nx·ny
   01 xzy->xyz x+z·nx+y·nx·nz        07 xyz->xzy x+z·nx+y·nx·nz
   02 yxz->xyz y+x·ny+z·ny·nx        08 xyz->yxz y+x·ny+z·ny·nx
   03 yzx->xyz y+z·ny+x·ny·nz        09 xyz->yzx y+z·ny+x·ny·nz
   04 zxy->xyz z+x·nz+y·nz·nx        10 xyz->zxy z+x·nz+y·nz·nx
   05 zyx->xyz z+y·nz+x·nz·ny        11 xyz->zyx z+y·nz+x·nz·ny
   */
  if (perm==0) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==1) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[x+z*nodes.x()+y*nodes.x()*nodes.z()];
  if (perm==2) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[y+x*nodes.y()+z*nodes.y()*nodes.x()];
  if (perm==3) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[y+z*nodes.y()+x*nodes.y()*nodes.z()];
  if (perm==4) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[z+x*nodes.z()+y*nodes.z()*nodes.x()];
  if (perm==5) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[z+y*nodes.z()+x*nodes.z()*nodes.y()];
  
  if (perm==6) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+y*nodes.x()+z*nodes.x()*nodes.y()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==7) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[x+z*nodes.x()+y*nodes.x()*nodes.z()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==8) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[y+x*nodes.y()+z*nodes.y()*nodes.x()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==9) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[y+z*nodes.y()+x*nodes.y()*nodes.z()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==10) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[z+x*nodes.z()+y*nodes.z()*nodes.x()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];
  if (perm==11) for(unsigned int z=0;z<nodes.z();z++) for(unsigned int y=0;y<nodes.y();y++) for(unsigned int x=0;x<nodes.x();x++)
          ans[z+y*nodes.z()+x*nodes.z()*nodes.y()] = v[x+y*nodes.x()+z*nodes.x()*nodes.y()];          
  std::swap(ans,v);
}

/* ################################################################################# */

int InversePerm(const int& perm){
  switch(perm){
    case 0:
      return 6;
    case 1:
      return 7;
    case 2:
      return 8;
    case 3:
      return 9;
    case 4:
      return 10;
    case 5:
      return 11;
    case 6:
      return 0;
    case 7:
      return 1;
    case 8: 
      return 2;
    case 9:
      return 3;
    case 10:
      return 4;
    case 11:
      return 5;
    default:
      return 0;
    }
  return 0;
  }

void SAF_Filter(rcl::ffield& f,const unsigned int& p){
  if (p==0) return;
  for (unsigned int z=0;z<f.getnodes().z();z++){
    if (((1<<z)&p)!=0){
      for (unsigned int y=0;y<f.getnodes().y();y++)
      for (unsigned int x=0;x<f.getnodes().x();x++) 
      f[rcl::utriad(x,y,z)].negative();
    }
  }
  return;
}

void SAF_Filter(std::vector<rcl::rpolar>& f,const unsigned int& p,const rcl::utriad& n){
  if (p==0) return;
  for (unsigned int z=0;z<n.z();z++){
    if (((1<<z)&p)!=0){
      for (unsigned int y=0;y<n.y();y++)
      for (unsigned int x=0;x<n.x();x++)
      f[x+y*n.x()+z*n.x()*n.y()].negative();
    }
  }
  return;
}

}
