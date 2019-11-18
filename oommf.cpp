#include "oommf.hpp"

namespace rcl{

OVF_format::OVF_format(const std::string& format){
    OVF2= true;binary = true;scalar = false;
    const std::string& sigovf2="v2";
    const std::string& sigovf1="v1";
    const std::string& sigbin="bin";
    const std::string& sigasc="ascii";
    const std::string& sigtxt="text";
    const std::string& sigsca="scalar";
    const std::string& sigvec="vector";

    if (std::search(format.begin(),format.end(),sigovf1.begin(),sigovf1.end())!=format.end()) OVF2 = false;
    if (std::search(format.begin(),format.end(),sigovf2.begin(),sigovf2.end())!=format.end()) OVF2 = true;
    if (std::search(format.begin(),format.end(),sigbin.begin(),sigbin.end())!=format.end()) binary = true;
    if (std::search(format.begin(),format.end(),sigasc.begin(),sigasc.end())!=format.end()) binary = false;
    if (std::search(format.begin(),format.end(),sigtxt.begin(),sigtxt.end())!=format.end()) binary = false;
    if (std::search(format.begin(),format.end(),sigsca.begin(),sigsca.end())!=format.end()) scalar = true;
    if (std::search(format.begin(),format.end(),sigvec.begin(),sigvec.end())!=format.end()) scalar = false;
    return;
    }

bool is_OVF(const std::vector<unsigned char>& file){
  const std::string ovf1("# OOMMF:");
  const std::string ovf2("# OOMMF OVF 2.0");
  std::vector<unsigned char>::const_iterator cursor;
  cursor = std::search(file.begin(),file.end(),ovf2.begin(),ovf2.end());
  if (cursor!=file.end()) return true;
  cursor = std::search(file.begin(),file.end(),ovf1.begin(),ovf1.end());
  if (cursor!=file.end()) return true;
  return false;
}

void saveOVF(std::vector<unsigned char>& file,const ffield& t,const OVF_format& format){
  rcl::vstream vs(file);
   
  const unsigned char signle4[]={0x38,0xB4,0x96,0x49};
  const unsigned char signbe4[]={0x49,0x96,0xB4,0x38};
  if (format.get_v2())  vs << "# OOMMF OVF 2.0" << vstream::newline();
  else                  vs << "# OOMMF: rectangular mesh v1.0" << vstream::newline();
    
  vs << "# Segment count: 1" << vstream::newline();
  vs << "# Begin: Segment" << vstream::newline();
  vs << "# Begin: Header" << vstream::newline();
  
  if (t.is_meta("Title"))  vs << "# Title: "<< t.get_meta("Title") << vstream::newline();
  else                    vs << "# Title: m" << vstream::newline();
  
  if (t.is_meta("meshtype"))  vs << "# meshtype: "<< t.get_meta("meshtype") << vstream::newline();
  else    vs << "# meshtype: rectangular" << vstream::newline();
  
  if (t.is_meta("meshunit"))  vs << "# meshunit: "<< t.get_meta("meshunit") << vstream::newline();
  else vs << "# meshunit: m" << vstream::newline();
  
  if (t.is_meta("xmin"))  vs << "# xmin: "<< t.get_meta("xmin") << vstream::newline();
  else vs << "# xmin: 0" << vstream::newline();
  
  if (t.is_meta("ymin"))  vs << "# ymin: "<< t.get_meta("ymin") << vstream::newline();
  else vs << "# ymin: 0" << vstream::newline();
  
  if (t.is_meta("zmin"))  vs << "# zmin: "<< t.get_meta("zmin") << vstream::newline();
  else vs << "# zmin: 0" << vstream::newline();
  
  if (t.is_meta("xmax"))  vs << "# xmax: "<< t.get_meta("xmax") << vstream::newline();
  else vs << "# xmax: "<< t.getnodes().x()*t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ymax"))  vs << "# ymax: "<< t.get_meta("ymax") << vstream::newline();
  else vs << "# ymax: "<< t.getnodes().y()*t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zmax"))  vs << "# zmax: "<< t.get_meta("zmax") << vstream::newline();
  else vs << "# zmax: "<< t.getnodes().z()*t.getsize().z() << vstream::newline();
  
  /** */
  if (t.is_meta("valuedim"))  vs << "# valuedim: "<< t.get_meta("valuedim") << vstream::newline();
  else vs << "# valuedim: 3" << vstream::newline();
  
  if (t.is_meta("valuelabels"))  vs << "# valuelabels: "<< t.get_meta("valuelabels") << vstream::newline();
  else vs << "# valuelabels: m_x m_y m_z" << vstream::newline();
  
  if (t.is_meta("valueunits"))  vs << "# valueunits: "<< t.get_meta("valueunits") << vstream::newline();
  else vs << "# valueunits: 1 1 1" << vstream::newline();
  
  if (t.is_meta("Desc"))  vs << "# Desc: "<< t.get_meta("Desc") << vstream::newline();
  else vs << "# Desc: " << vstream::newline();
  
  if (t.is_meta("xbase"))  vs << "# xbase: "<< t.get_meta("xbase") << vstream::newline();
  else vs << "# xbase: " << 0.5*t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ybase"))  vs << "# ybase: "<< t.get_meta("ybase") << vstream::newline();
  else vs << "# ybase: " << 0.5*t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zbase"))  vs << "# zbase: "<< t.get_meta("zbase") << vstream::newline();
  else vs << "# zbase: " << 0.5*t.getsize().z() << vstream::newline();
  
  if (t.is_meta("xnodes"))  vs << "# xnodes: "<< t.get_meta("xnodes") << vstream::newline();
  else vs << "# xnodes: "<< t.getnodes().x() << vstream::newline();
  
  if (t.is_meta("ynodes"))  vs << "# ynodes: "<< t.get_meta("ynodes") << vstream::newline();
  else vs << "# ynodes: "<< t.getnodes().y() << vstream::newline();
  
  if (t.is_meta("znodes"))  vs << "# znodes: "<< t.get_meta("znodes") << vstream::newline();
  else vs << "# znodes: "<< t.getnodes().z() << vstream::newline();
  
  if (t.is_meta("xstepsize"))  vs << "# xstepsize: "<< t.get_meta("xstepsize") << vstream::newline();
  else vs << "# xstepsize: "<< t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ystepsize"))  vs << "# ystepsize: "<< t.get_meta("ystepsize") << vstream::newline();
  else vs << "# ystepsize: "<< t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zstepsize"))  vs << "# zstepsize: "<< t.get_meta("zstepsize") << vstream::newline();
  else vs << "# zstepsize: "<< t.getsize().z() << vstream::newline();
  
  vs << "# End: Header" << vstream::newline();
  vs << "# Begin: Data Binary 4" << vstream::newline();
  if (format.get_v2())  vs.write(signle4,4);
  else                  vs.write(signbe4,4);
  
  bool scalar=false;
  if (t.is_meta("valuedim")) {
    if(t.get_val<int>("valuedim")==1) scalar=true;
    if(t.get_val<int>("valuedim")==3) scalar=false;
  }
  // TO-DO : Add little-endian / big-endian coding for V1
  int size = t.getnodes().volume();
  if (scalar) for (int idx=0;idx<size;idx++) vs.write(t[idx].z());
  else        vs.write(reinterpret_cast<const unsigned char*>(t.begin()),sizeof(fvector)*t.getnodes().volume());
  vs << "# End: Data Binary 4" << vstream::newline();
  vs << "# End: Segment" << vstream::newline();
  
  return;
}

void saveOVF2v(std::vector<unsigned char>& file,const ffield& t){saveOVF(file,t,OVF_format(1,1,0,0)); return;}
void saveOVF2vm(std::vector<unsigned char>& file,const ffield& t){saveOVF(file,t,OVF_format(1,1,0,0)); return;}
/*
void saveOVF2v(std::vector<unsigned char>& file,const ffield& t){
  rcl::vstream vs(file);
  const unsigned char signle4[]={0x38,0xB4,0x96,0x49};
  vs << "# OOMMF OVF 2.0" << vstream::newline();
  vs << "# Segment count: 1" << vstream::newline();
  vs << "# Begin: Segment" << vstream::newline();
  vs << "# Begin: Header" << vstream::newline();
  vs << "# Title: m" << vstream::newline();
  vs << "# meshtype: rectangular" << vstream::newline();
  vs << "# meshunit: m" << vstream::newline();
  vs << "# xmin: 0" << vstream::newline();
  vs << "# ymin: 0" << vstream::newline();
  vs << "# zmin: 0" << vstream::newline();
  vs << "# xmax: "<< t.getnodes().x()*t.getsize().x() << vstream::newline();
  vs << "# ymax: "<< t.getnodes().y()*t.getsize().y() << vstream::newline();
  vs << "# zmax: "<< t.getnodes().z()*t.getsize().z() << vstream::newline();
  vs << "# valuedim: 3" << vstream::newline();
  vs << "# valuelabels: m_x m_y m_z" << vstream::newline();
  vs << "# valueunits: 1 1 1" << vstream::newline();
  vs << "# Desc: " << vstream::newline();
  vs << "# xbase: " << 0.5*t.getsize().x() << vstream::newline();
  vs << "# ybase: " << 0.5*t.getsize().y() << vstream::newline();
  vs << "# zbase: " << 0.5*t.getsize().z() << vstream::newline();
  vs << "# xnodes: "<< t.getnodes().x() << vstream::newline();
  vs << "# ynodes: "<< t.getnodes().y() << vstream::newline();
  vs << "# znodes: "<< t.getnodes().z() << vstream::newline();
  vs << "# xstepsize: "<< t.getsize().x() << vstream::newline();
  vs << "# ystepsize: "<< t.getsize().y() << vstream::newline();
  vs << "# zstepsize: "<< t.getsize().z() << vstream::newline();
  vs << "# End: Header" << vstream::newline();
  vs << "# Begin: Data Binary 4" << vstream::newline();
  vs.write(signle4,4);
  vs.write(reinterpret_cast<const unsigned char*>(t.begin()),sizeof(fvector)*t.getnodes().volume());
  vs << "# End: Data Binary 4" << vstream::newline();
  vs << "# End: Segment" << vstream::newline();
}
void saveOVF2vm(std::vector<unsigned char>& file,const ffield& t){
  rcl::vstream vs(file);
  const unsigned char signle4[]={0x38,0xB4,0x96,0x49};
  vs << "# OOMMF OVF 2.0" << vstream::newline();
  vs << "# Segment count: 1" << vstream::newline();
  vs << "# Begin: Segment" << vstream::newline();
  vs << "# Begin: Header" << vstream::newline();
  
  if (t.is_meta("Title"))  vs << "# Title: "<< t.get_meta("Title") << vstream::newline();
  else                    vs << "# Title: m" << vstream::newline();
  
  if (t.is_meta("meshtype"))  vs << "# meshtype: "<< t.get_meta("meshtype") << vstream::newline();
  else    vs << "# meshtype: rectangular" << vstream::newline();
  
  if (t.is_meta("meshunit"))  vs << "# meshunit: "<< t.get_meta("meshunit") << vstream::newline();
  else vs << "# meshunit: m" << vstream::newline();
  
  if (t.is_meta("xmin"))  vs << "# xmin: "<< t.get_meta("xmin") << vstream::newline();
  else vs << "# xmin: 0" << vstream::newline();
  
  if (t.is_meta("ymin"))  vs << "# ymin: "<< t.get_meta("ymin") << vstream::newline();
  else vs << "# ymin: 0" << vstream::newline();
  
  if (t.is_meta("zmin"))  vs << "# zmin: "<< t.get_meta("zmin") << vstream::newline();
  else vs << "# zmin: 0" << vstream::newline();
  
  if (t.is_meta("xmax"))  vs << "# xmax: "<< t.get_meta("xmax") << vstream::newline();
  else vs << "# xmax: "<< t.getnodes().x()*t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ymax"))  vs << "# ymax: "<< t.get_meta("ymax") << vstream::newline();
  else vs << "# ymax: "<< t.getnodes().y()*t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zmax"))  vs << "# zmax: "<< t.get_meta("zmax") << vstream::newline();
  else vs << "# zmax: "<< t.getnodes().z()*t.getsize().z() << vstream::newline();
  
  if (t.is_meta("valuedim"))  vs << "# valuedim: "<< t.get_meta("valuedim") << vstream::newline();
  else vs << "# valuedim: 3" << vstream::newline();
  
  if (t.is_meta("valuelabels"))  vs << "# valuelabels: "<< t.get_meta("valuelabels") << vstream::newline();
  else vs << "# valuelabels: m_x m_y m_z" << vstream::newline();
  
  if (t.is_meta("valueunits"))  vs << "# valueunits: "<< t.get_meta("valueunits") << vstream::newline();
  else vs << "# valueunits: 1 1 1" << vstream::newline();
  
  if (t.is_meta("Desc"))  vs << "# Desc: "<< t.get_meta("Desc") << vstream::newline();
  else vs << "# Desc: " << vstream::newline();
  
  if (t.is_meta("xbase"))  vs << "# xbase: "<< t.get_meta("xbase") << vstream::newline();
  else vs << "# xbase: " << 0.5*t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ybase"))  vs << "# ybase: "<< t.get_meta("ybase") << vstream::newline();
  else vs << "# ybase: " << 0.5*t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zbase"))  vs << "# zbase: "<< t.get_meta("zbase") << vstream::newline();
  else vs << "# zbase: " << 0.5*t.getsize().z() << vstream::newline();
  
  if (t.is_meta("xnodes"))  vs << "# xnodes: "<< t.get_meta("xnodes") << vstream::newline();
  else vs << "# xnodes: "<< t.getnodes().x() << vstream::newline();
  
  if (t.is_meta("ynodes"))  vs << "# ynodes: "<< t.get_meta("ynodes") << vstream::newline();
  else vs << "# ynodes: "<< t.getnodes().y() << vstream::newline();
  
  if (t.is_meta("znodes"))  vs << "# znodes: "<< t.get_meta("znodes") << vstream::newline();
  else vs << "# znodes: "<< t.getnodes().z() << vstream::newline();
  
  if (t.is_meta("xstepsize"))  vs << "# xstepsize: "<< t.get_meta("xstepsize") << vstream::newline();
  else vs << "# xstepsize: "<< t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ystepsize"))  vs << "# ystepsize: "<< t.get_meta("ystepsize") << vstream::newline();
  else vs << "# ystepsize: "<< t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zstepsize"))  vs << "# zstepsize: "<< t.get_meta("zstepsize") << vstream::newline();
  else vs << "# zstepsize: "<< t.getsize().z() << vstream::newline();
  
  vs << "# End: Header" << vstream::newline();
  vs << "# Begin: Data Binary 4" << vstream::newline();
  vs.write(signle4,4);
  vs.write(reinterpret_cast<const unsigned char*>(t.begin()),sizeof(fvector)*t.getnodes().volume());
  vs << "# End: Data Binary 4" << vstream::newline();
  vs << "# End: Segment" << vstream::newline();
  
  return;
}
*/
void saveOVF2vm(std::vector<unsigned char>& file,const fsfield& t){
  rcl::vstream vs(file);
  const unsigned char signle4[]={0x38,0xB4,0x96,0x49};
  vs << "# OOMMF OVF 2.0" << vstream::newline();
  vs << "# Segment count: 1" << vstream::newline();
  vs << "# Begin: Segment" << vstream::newline();
  vs << "# Begin: Header" << vstream::newline();
  
  if (t.is_meta("Title"))  vs << "# Title: "<< t.get_meta("Title") << vstream::newline();
  else                    vs << "# Title: regions" << vstream::newline();
  
  if (t.is_meta("meshtype"))  vs << "# meshtype: "<< t.get_meta("meshtype") << vstream::newline();
  else    vs << "# meshtype: rectangular" << vstream::newline();
  
  if (t.is_meta("meshunit"))  vs << "# meshunit: "<< t.get_meta("meshunit") << vstream::newline();
  else vs << "# meshunit: m" << vstream::newline();
  
  if (t.is_meta("xmin"))  vs << "# xmin: "<< t.get_meta("xmin") << vstream::newline();
  else vs << "# xmin: 0" << vstream::newline();
  
  if (t.is_meta("ymin"))  vs << "# ymin: "<< t.get_meta("ymin") << vstream::newline();
  else vs << "# ymin: 0" << vstream::newline();
  
  if (t.is_meta("zmin"))  vs << "# zmin: "<< t.get_meta("zmin") << vstream::newline();
  else vs << "# zmin: 0" << vstream::newline();
  
  if (t.is_meta("xmax"))  vs << "# xmax: "<< t.get_meta("xmax") << vstream::newline();
  else vs << "# xmax: "<< t.getnodes().x()*t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ymax"))  vs << "# ymax: "<< t.get_meta("ymax") << vstream::newline();
  else vs << "# ymax: "<< t.getnodes().y()*t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zmax"))  vs << "# zmax: "<< t.get_meta("zmax") << vstream::newline();
  else vs << "# zmax: "<< t.getnodes().z()*t.getsize().z() << vstream::newline();
  
  //if (t.is_meta("valuedim"))  vs << "# valuedim: "<< t.get_meta("valuedim") << vstream::newline();
  /*else*/ vs << "# valuedim: 1" << vstream::newline();
  
  if (t.is_meta("valuelabels"))  vs << "# valuelabels: "<< t.get_meta("valuelabels") << vstream::newline();
  else vs << "# valuelabels: regions" << vstream::newline();
  
  if (t.is_meta("valueunits"))  vs << "# valueunits: "<< t.get_meta("valueunits") << vstream::newline();
  else vs << "# valueunits: 1" << vstream::newline();
  
  if (t.is_meta("Desc"))  vs << "# Desc: "<< t.get_meta("Desc") << vstream::newline();
  else vs << "# Desc: " << vstream::newline();
  
  if (t.is_meta("xbase"))  vs << "# xbase: "<< t.get_meta("xbase") << vstream::newline();
  else vs << "# xbase: " << 0.5*t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ybase"))  vs << "# ybase: "<< t.get_meta("ybase") << vstream::newline();
  else vs << "# ybase: " << 0.5*t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zbase"))  vs << "# zbase: "<< t.get_meta("zbase") << vstream::newline();
  else vs << "# zbase: " << 0.5*t.getsize().z() << vstream::newline();
  
  if (t.is_meta("xnodes"))  vs << "# xnodes: "<< t.get_meta("xnodes") << vstream::newline();
  else vs << "# xnodes: "<< t.getnodes().x() << vstream::newline();
  
  if (t.is_meta("ynodes"))  vs << "# ynodes: "<< t.get_meta("ynodes") << vstream::newline();
  else vs << "# ynodes: "<< t.getnodes().y() << vstream::newline();
  
  if (t.is_meta("znodes"))  vs << "# znodes: "<< t.get_meta("znodes") << vstream::newline();
  else vs << "# znodes: "<< t.getnodes().z() << vstream::newline();
  
  if (t.is_meta("xstepsize"))  vs << "# xstepsize: "<< t.get_meta("xstepsize") << vstream::newline();
  else vs << "# xstepsize: "<< t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ystepsize"))  vs << "# ystepsize: "<< t.get_meta("ystepsize") << vstream::newline();
  else vs << "# ystepsize: "<< t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zstepsize"))  vs << "# zstepsize: "<< t.get_meta("zstepsize") << vstream::newline();
  else vs << "# zstepsize: "<< t.getsize().z() << vstream::newline();
  
  vs << "# End: Header" << vstream::newline();
  vs << "# Begin: Data Binary 4" << vstream::newline();
  vs.write(signle4,4);
  vs.write(reinterpret_cast<const unsigned char*>(t.begin()),sizeof(fscalar)*t.getnodes().volume());
  vs << "# End: Data Binary 4" << vstream::newline();
  vs << "# End: Segment" << vstream::newline();
  
  return;
}
void saveOVF1Tvm(std::vector<unsigned char>& file,const ffield& t){
  rcl::vstream vs(file);
  const unsigned char signle4[]={0x38,0xB4,0x96,0x49};
  vs << "# OOMMF: rectangular mesh v1.0" << vstream::newline();
  vs << "# Segment count: 1" << vstream::newline();
  vs << "# Begin: Segment" << vstream::newline();
  vs << "# Begin: Header" << vstream::newline();
  
  if (t.is_meta("Desc")) vs << "# Desc: " << t.get_meta("Title") << vstream::newline();
  else                    vs << "# Desc: Time (s) :  0" << vstream::newline();
  
  if (t.is_meta("Title"))  vs << "# Title: "<< t.get_meta("Title") << vstream::newline();
  else                    vs << "# Title: m" << vstream::newline();
  
  if (t.is_meta("meshtype"))  vs << "# meshtype: "<< t.get_meta("meshtype") << vstream::newline();
  else    vs << "# meshtype: rectangular" << vstream::newline();
  
  if (t.is_meta("meshunit"))  vs << "# meshunit: "<< t.get_meta("meshunit") << vstream::newline();
  else vs << "# meshunit: m" << vstream::newline();
  
  if (t.is_meta("xbase"))  vs << "# xbase: "<< t.get_meta("xbase") << vstream::newline();
  else vs << "# xbase: " << 0.5*t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ybase"))  vs << "# ybase: "<< t.get_meta("ybase") << vstream::newline();
  else vs << "# ybase: " << 0.5*t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zbase"))  vs << "# zbase: "<< t.get_meta("zbase") << vstream::newline();
  else vs << "# zbase: " << 0.5*t.getsize().z() << vstream::newline();
  
  if (t.is_meta("xstepsize"))  vs << "# xstepsize: "<< t.get_meta("xstepsize") << vstream::newline();
  else vs << "# xstepsize: "<< t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ystepsize"))  vs << "# ystepsize: "<< t.get_meta("ystepsize") << vstream::newline();
  else vs << "# ystepsize: "<< t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zstepsize"))  vs << "# zstepsize: "<< t.get_meta("zstepsize") << vstream::newline();
  else vs << "# zstepsize: "<< t.getsize().z() << vstream::newline();
  
  if (t.is_meta("xmin"))  vs << "# xmin: "<< t.get_meta("xmin") << vstream::newline();
  else vs << "# xmin: 0" << vstream::newline();
  
  if (t.is_meta("ymin"))  vs << "# ymin: "<< t.get_meta("ymin") << vstream::newline();
  else vs << "# ymin: 0" << vstream::newline();
  
  if (t.is_meta("zmin"))  vs << "# zmin: "<< t.get_meta("zmin") << vstream::newline();
  else vs << "# zmin: 0" << vstream::newline();
  
  if (t.is_meta("xmax"))  vs << "# xmax: "<< t.get_meta("xmax") << vstream::newline();
  else vs << "# xmax: "<< t.getnodes().x()*t.getsize().x() << vstream::newline();
  
  if (t.is_meta("ymax"))  vs << "# ymax: "<< t.get_meta("ymax") << vstream::newline();
  else vs << "# ymax: "<< t.getnodes().y()*t.getsize().y() << vstream::newline();
  
  if (t.is_meta("zmax"))  vs << "# zmax: "<< t.get_meta("zmax") << vstream::newline();
  else vs << "# zmax: "<< t.getnodes().z()*t.getsize().z() << vstream::newline();
  
  if (t.is_meta("xnodes"))  vs << "# xnodes: "<< t.get_meta("xnodes") << vstream::newline();
  else vs << "# xnodes: "<< t.getnodes().x() << vstream::newline();
  
  if (t.is_meta("ynodes"))  vs << "# ynodes: "<< t.get_meta("ynodes") << vstream::newline();
  else vs << "# ynodes: "<< t.getnodes().y() << vstream::newline();
  
  if (t.is_meta("znodes"))  vs << "# znodes: "<< t.get_meta("znodes") << vstream::newline();
  else vs << "# znodes: "<< t.getnodes().z() << vstream::newline();
  
  if (t.is_meta("ValueRangeMinMag"))  vs << "# ValueRangeMinMag: "<< t.get_meta("ValueRangeMinMag") << vstream::newline();
  else vs << "# ValueRangeMinMag: "<< 1e-08 << vstream::newline();
  
  if (t.is_meta("ValueRangeMaxMag"))  vs << "# ValueRangeMaxMag: "<< t.get_meta("ValueRangeMaxMag") << vstream::newline();
  else vs << "# ValueRangeMaxMag: "<< 1 << vstream::newline();
  
  if (t.is_meta("valueunit"))  vs << "# valueunit: "<< t.get_meta("valueunit") << vstream::newline();
  else vs << "# valueunit: "<< 1 << vstream::newline();
  
  if (t.is_meta("valuemultiplier"))  vs << "# valuemultiplier: "<< t.get_meta("valuemultiplier") << vstream::newline();
  else vs << "# valuemultiplier: "<< 1 << vstream::newline();
  
  vs << "# End: Header" << vstream::newline();
  vs << "# Begin: Data Text" << vstream::newline();
  
  for (unsigned int idx=0;idx<t.size();idx++) 
  vs << t[idx].x() << " " << t[idx].y() << " " << t[idx].z() << vstream::newline();
  
  vs << "# End: Data Text" << vstream::newline();
  vs << "# End: Segment" << vstream::newline();
  
  return;
}

void loadOVF2v(ffield& t,const std::vector<unsigned char>& file){
  const std::string ovf2("# OOMMF OVF 2.0");
  const std::string ovfb("# Begin: Data Binary 4");
  const unsigned char signle4[]={0x38,0xB4,0x96,0x49};
  std::vector<unsigned char>::const_iterator cursor;
  utriad nodes;
  /*
  readfrombuffer(file,"xnodes",nodes.x());
  readfrombuffer(file,"ynodes",nodes.y());
  readfrombuffer(file,"znodes",nodes.z());
  readfrombuffer(file,"xstepsize",size.x());
  readfrombuffer(file,"ystepsize",size.y());
  readfrombuffer(file,"zstepsize",size.z());
  * */
  /* read all metadata */
  std::string bheader("# Begin: Header");
  std::string eheader("# End: Header");
  std::vector<unsigned char>::const_iterator end;
  cursor = search(file.begin(),file.end(),bheader.begin(),bheader.end());
  cursor += bheader.length() +1;
  if (cursor==file.end()) return;
  end    = search(file.begin(),file.end(),eheader.begin(),eheader.end());
  if (end==file.end()) return;
  std::vector<std::string> header;
  std::string line;
  while (cursor!=end){
    if ((*cursor)=='\n') {header.push_back(line); line=std::string();}
    else {line.push_back(*cursor);}
    cursor++;
    }
  //for (unsigned int idx=0;idx<header.size();idx++) std::cout << idx << " -> " << header[idx] << std::endl;
  for (unsigned int idx=0;idx<header.size();idx++){
    std::string key,val;
    unsigned int pos=2;
    if (header[idx][0]=='#') if (header[idx][1]==' '){
      while(header[idx][pos]!=':' && pos<header[idx].length()) key.push_back(header[idx][pos++]);
      pos+=2;
      while(pos<header[idx].length()) val.push_back(header[idx][pos++]);
      //std::cout << idx << " -> " << header[idx] << std::endl<< idx << " @@ # " << key << ": " << val << std::endl;
      t.reg_meta(key,val);
      }    
    }
  /*end readind the metadata*/
  t.parse_meta();
  nodes=t.getnodes();
  if (std::search(file.begin(),file.end(),ovf2.begin(),ovf2.end())==file.end()) return;
  if (std::search(file.begin(),file.end(),ovfb.begin(),ovfb.end())==file.end()) return;
  cursor = std::search(file.begin(),file.end(),signle4,signle4+4);
  if (cursor==file.end()) return;
  cursor+=4;
  if (!t.is_meta("valuedim")) t.reg_meta("valuedim","3");
  if (t.get_val<int>("valuedim")==3){
    const fvector* vcursor = reinterpret_cast<const fvector*>(&(*cursor));
    std::copy(vcursor,vcursor+nodes.volume(),t.begin());
  }
  if (t.get_val<int>("valuedim")==1){
    const float* vcursor = reinterpret_cast<const float*>(&(*cursor));
    std::copy(vcursor,vcursor+nodes.volume(),t.begin());
  }
  return;
  }

static void insert_meta_if_missing(rcl::ffield& m,const rcl::pair<std::string,std::string>& kv){
  if (!m.is_meta(kv.first)) m.reg_meta(kv.first,kv.second);
  return;
  }

void populate_meta(rcl::ffield& m,const std::string& title,const std::string& desc,const std::string& valuelabels,const std::string& valueunits){
  rcl::pair<std::string,std::string> kv;
  rcl::utriad cells = m.getnodes();
  rcl::ftriad size = m.getsize();
  
  kv.first = "Title"; if (title.empty()) kv.second = title; else kv.second = "Exported Vector";
  insert_meta_if_missing(m,kv);
  kv.first = "Desc"; if (!desc.empty()) kv.second = desc; else kv.second = "Exported File";
  insert_meta_if_missing(m,kv);
  kv.first = "meshtype"; kv.second = "rectangular";
  insert_meta_if_missing(m,kv);
  kv.first = "meshunit"; kv.second = "m";
  insert_meta_if_missing(m,kv);
  kv.first = "xmin"; kv.second = "0";
  insert_meta_if_missing(m,kv);
  kv.first = "ymin"; kv.second = "0";
  insert_meta_if_missing(m,kv);
  kv.first = "zmin"; kv.second = "0";
  insert_meta_if_missing(m,kv);
  kv.first = "xmax"; kv.second = rcl::to_str(cells[0]*size[0]);
  insert_meta_if_missing(m,kv);
  kv.first = "ymax"; kv.second = rcl::to_str(cells[1]*size[1]);
  insert_meta_if_missing(m,kv);
  kv.first = "zmax"; kv.second = rcl::to_str(cells[2]*size[2]);
  insert_meta_if_missing(m,kv);
  kv.first = "valuedim"; kv.second = "3";
  insert_meta_if_missing(m,kv);
  kv.first = "valuelabels"; if (!valuelabels.empty()) kv.second = valuelabels; else kv.second = "m_x m_y m_z";
  insert_meta_if_missing(m,kv);
  kv.first = "valueunits"; if (!valueunits.empty()) kv.second = valueunits; else kv.second = "1 1 1";
  insert_meta_if_missing(m,kv);
  kv.first = "xbase"; kv.second = rcl::to_str(0.5*size[0]);
  insert_meta_if_missing(m,kv);
  kv.first = "ybase"; kv.second = rcl::to_str(0.5*size[1]);
  insert_meta_if_missing(m,kv);
  kv.first = "zbase"; kv.second = rcl::to_str(0.5*size[2]);
  insert_meta_if_missing(m,kv);
  kv.first = "xnodes"; kv.second =  rcl::to_str(cells[0]);
  insert_meta_if_missing(m,kv);
  kv.first = "ynodes"; kv.second =  rcl::to_str(cells[1]);
  insert_meta_if_missing(m,kv);
  kv.first = "znodes"; kv.second =  rcl::to_str(cells[2]);
  insert_meta_if_missing(m,kv);
  kv.first = "xstepsize"; kv.second =  rcl::to_str(size[0]);
  insert_meta_if_missing(m,kv);
  kv.first = "ystepsize"; kv.second =  rcl::to_str(size[1]);
  insert_meta_if_missing(m,kv);
  kv.first = "zstepsize"; kv.second =  rcl::to_str(size[2]);
  insert_meta_if_missing(m,kv);
  return;
}

}
