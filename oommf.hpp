#ifndef OOMMF_HEADER
#define OOMMF_HEADER

#include <vector>
#include "tfield.hpp"
#include <algorithm>
#include "tinput.hpp"
#include "reverse.hpp"
#include "vstream.hpp"
#include "text.hpp"
//#include <iostream>


namespace rcl{
  class OVF_format{
  private:
  bool OVF2,binary,scalar,force_scalar;
  public:
  OVF_format(){OVF2= true;binary = true;scalar = false; force_scalar=false; return;}
  OVF_format(const bool& OVF2,const bool& binary,const bool& scalar,const bool& force_scalar){this->OVF2   =OVF2;this->binary =binary;this->scalar =scalar; this->force_scalar=force_scalar; return;    }
  OVF_format(const std::string& format);
  bool get_v2()const{return OVF2;}
  bool get_bin()const{return binary;}
  bool get_force()const{return force_scalar;}
  bool get_sca()const{return scalar;}
  bool get_vec()const{return !scalar;}
  };
  
  bool is_OVF(const std::vector<unsigned char>& file);
  void saveOVF(std::vector<unsigned char>& file,const ffield& t,const OVF_format& format);
  
  void saveOVF2v(std::vector<unsigned char>& file,const ffield& t);   //DEPRECATED
  void loadOVF2v(ffield& t,const std::vector<unsigned char>& file);   //DEPRECATED
  void saveOVF2vm(std::vector<unsigned char>& file,const ffield& t);  //DEPRECATED
  void saveOVF2vm(std::vector<unsigned char>& file,const fsfield& t); //DEPRECATED
  void saveOVF1Tvm(std::vector<unsigned char>& file,const ffield& t); //DEPRECATED
  void populate_meta(rcl::ffield& m,const std::string& title,const std::string& desc,const std::string& valuelabels,const std::string& valueunits);
}

#endif
