#ifndef POLAR_FUNCTIONS_HEADER
#define POLAR_FUNCTIONS_HEADER

#include "polar64.hpp"
#include "bitstream.hpp"
#include "tfield.hpp"
#include "numerical.hpp"
#include <algorithm>
namespace rcl{
  
int InversePerm(const int& perm);
void SAF_Filter(rcl::ffield& f,const unsigned int& p);
void SAF_Filter(std::vector<rcl::rpolar>& f,const unsigned int& p,const rcl::utriad& n);
void diffx_filter(std::vector<rcl::rpolar>& f,const rcl::utriad& n);
void diffy_filter(std::vector<rcl::rpolar>& f,const rcl::utriad& n);
void diffz_filter(std::vector<rcl::rpolar>& f,const rcl::utriad& n);
void difft_filter(std::vector<rcl::rpolar>& f,const std::vector<rcl::rpolar>& pf);
void chop(std::vector<rcl::rpolar>& v);

void VectoLpol(std::vector<rcl::lpolar>& v,const rcl::ffield& f);
void LpoltoVec(rcl::ffield& f,const std::vector<rcl::lpolar>& v);
void LpoltoSBin(std::vector<unsigned char>& s,const std::vector<rcl::lpolar>& v);
void SBintoLpol(std::vector<rcl::lpolar>& v,const std::vector<unsigned char>& s);
void LpoltoBin(std::vector<unsigned char>& s,const std::vector<rcl::lpolar>& v);
void BintoLpol(std::vector<rcl::lpolar>& v,const std::vector<unsigned char>& s);
void Permute(std::vector<rcl::lpolar>& v,const rcl::utriad& nodes,const int& perm);

void VectoLpol(std::vector<rcl::rpolar>& v,const rcl::ffield& f);
void LpoltoVec(rcl::ffield& f,const std::vector<rcl::rpolar>& v);
void LpoltoSBin(std::vector<unsigned char>& s,const std::vector<rcl::rpolar>& v);
void LpoltoRSBin(std::vector<unsigned char>& s,const std::vector<rcl::rpolar>& v);
void SBintoLpol(std::vector<rcl::rpolar>& v,const std::vector<unsigned char>& s);
void RSBintoLpol(std::vector<rcl::rpolar>& v,const std::vector<unsigned char>& s);
void LpoltoBin(std::vector<unsigned char>& s,const std::vector<rcl::rpolar>& v);
void LpoltoRBin(std::vector<unsigned char>& s,const std::vector<rcl::rpolar>& v);
void BintoLpol(std::vector<rcl::rpolar>& v,const std::vector<unsigned char>& s);
void RBintoLpol(std::vector<rcl::rpolar>& v,const std::vector<unsigned char>& s);
void Permute(std::vector<rcl::rpolar>& v,const rcl::utriad& nodes,const int& perm);
}
#endif
