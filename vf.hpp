#ifndef VF_HEADER
#define VF_HEADER

#include "polar.hpp"
#include "text.hpp"
#include "lib-zlib.hpp"

namespace rcl{

class vf_crush_cfg{
private:
  bool echo;
  bool quick;
  bool force;
  bool nosaf;
  std::string label;
public:
  vf_crush_cfg(){quick=false;force=false;echo=true;nosaf=false; return;}
  vf_crush_cfg(const bool& e){quick=false;force=false;echo=e;nosaf=false; return;}
  vf_crush_cfg(const bool& e,const bool& q){quick=q;force=false;echo=e;nosaf=false; return;}
  vf_crush_cfg(const bool& e,const bool& q,const bool& naf){quick=q;force=false;echo=e;nosaf=naf; return;}
  vf_crush_cfg setforce(){force=true; return *this;}
  vf_crush_cfg setlabel(const std::string& label){this->label=label; return *this;}
  bool get_echo() const{ return echo;}
  bool get_quick()const{ return quick;}
  bool get_nosaf()const{ return nosaf;}
  bool get_force()const{ return force;}
  const std::string& get_label()const{return label;}
};

void crush_vf(std::vector<unsigned char>& dst,const rcl::ffield& src,const vf_crush_cfg& cfg);
void encode_vf(std::vector<unsigned char>& dst,const rcl::ffield& src);
void decode_vf(rcl::ffield& dst,const std::vector<unsigned char>& src);
void decode_vf_meta(rcl::stringmap& dst,const std::vector<unsigned char>& src);
void load_vf_meta(rcl::stringmap& dst,const std::string& filename);
bool is_vf_crush(const std::string& filename);

bool is_vf_signed(const std::string& filename);
}
#endif
