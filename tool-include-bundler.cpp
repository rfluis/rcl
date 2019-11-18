#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>

#ifndef STANDALONE_TOOL
#include "files.hpp"
#include "text.hpp"
#include "busybox.hpp"
#endif


#ifdef STANDALONE_TOOL
namespace rcl {
typedef std::map<std::string,bool> stringcheck; 
typedef std::vector<std::string> stringvector;
typedef std::map<std::string,std::string> stringmap;
typedef std::map<std::string,stringvector> stringbook;
}
#endif


#ifdef STANDALONE_TOOL
namespace rcl {
static void load_file(std::vector<std::string>& buffer,const std::string& filename){
  std::fstream file(filename,std::fstream::in);
  while (!file.eof()){
    std::string line;
    std::getline(file,line);
    if (line.length()!=0) buffer.push_back(line);
    }
  return;
}

} //end namespace rcl
#endif
// g++ -c -fPIC -O3 -s -mtune=native -march=native -fno-exceptions -fno-rtti tool-include-bundler.cpp

static std::ostream& operator << (std::ostream& os,const std::vector<std::string>& data){
  for (typename std::vector<std::string>::size_type idx=0;idx<data.size();idx++) os << data[idx] << '\n';
  os << std::flush;
  return os;
  }
static inline bool match(const std::string& haystack,const std::string& needle){
  if (std::search(haystack.begin(),haystack.end(),needle.begin(),needle.end())!=haystack.begin()) return false;
  if (std::search(haystack.begin(),haystack.end(),needle.begin(),needle.end())!=haystack.end()) return true;
  return false;
}
enum kind_of_line{code,local_include,global_include,define,ifdef,ifndef,endif,comment};

static bool is_include(const std::string& line){return match(line,std::string("#include"));}
static bool is_define(const std::string& line){ return match(line,std::string("#define"));}
static bool is_ifndef(const std::string& line){ return match(line,std::string("#ifndef"));}
static bool is_ifdef(const std::string& line){  return match(line,std::string("#ifdef"));}
static bool is_endif(const std::string& line){  return match(line,std::string("#endif"));}
static bool is_global_include(const std::string& line){
  if (!is_include(line)) return false;
  std::string::const_iterator itr;
  for (itr=line.begin();itr!=line.end();itr++) if (*itr=='<') break;
  if (itr==line.end()) return false;
  for (itr=line.begin();itr!=line.end();itr++) if (*itr=='>') break;
  if (*itr=='>') return true;
  return false;
  }
static bool is_local_include(const std::string& line){
  if (!is_include(line)) return false;
  std::string::const_iterator itr;
  for (itr=line.begin();itr!=line.end();itr++) if (*itr=='\"') break;
  if (itr==line.end()) return false;
  for (itr=line.begin();itr!=line.end();itr++) if (*itr=='\"') break;
  if (*itr=='\"') return true;
  return false;
  }
static std::string get_included(const std::string& line){
  std::string included;
  if (!is_include(line)) return included;
  std::string::const_iterator itr;
  itr = line.begin();
  for (;itr!=line.end();itr++) if (*itr=='<' || *itr=='\"') break;
  if (*itr=='<' || *itr=='\"') itr++;
  for (;itr!=line.end();itr++) if (*itr=='>' || *itr=='\"') break; else included.push_back(*itr);
  return included;
  }
static std::string get_deflike(const std::string& line,const std::string& keyword){
  std::string defined;
  std::string::const_iterator itr;
  if (!match(line,keyword)) return defined;
  itr = std::search(line.begin(),line.end(),keyword.begin(),keyword.end());
  itr += keyword.length();
  while (*itr==' ') itr++;
  while (*itr!=' ' && *itr!='\n' && itr!=line.end()) defined.push_back(*(itr++));
  return defined;
  }
static std::string get_define(const std::string& line){return get_deflike(line,std::string("#define"));}
static std::string get_ifndef(const std::string& line){return get_deflike(line,std::string("#ifndef"));}
static std::string get_ifdef(const std::string& line){return get_deflike(line,std::string("#ifdef"));}

class header_task{
private:
  int level=0;
  bool write=true;
  bool write_level=0;
  std::map<std::string,bool> defines,global_includes,local_includes,hcache_check;
  std::map<std::string,rcl::stringvector> hcache;
  rcl::stringvector preamble,output;
public:
  void parse(const std::string& filename);
  rcl::stringvector& cachefile(const std::string& filename);
  void write_preamble();
  const rcl::stringvector& get_preamble()const{return preamble;}
  const rcl::stringvector& get_output()const{return output;}
  };

void header_task::write_preamble(){
  preamble = rcl::stringvector();
  for (std::map<std::string,bool>::const_iterator itr=global_includes.begin();itr!=global_includes.end();itr++)
    preamble.push_back(std::string("#include <")+itr->first+std::string(">"));
  return;
  }
rcl::stringvector& header_task::cachefile(const std::string& filename){
  if (hcache_check[filename]==false){
    rcl::load_file(hcache[filename],filename);
    hcache_check[filename] = true;
    std::cerr << "[X] ( CACHE  ) File:"<< filename << " - Lines: "  <<hcache[filename].size() << std::endl;
  }
  return hcache[filename];
}
void header_task::parse(const std::string& filename){
  rcl::stringvector& cache = cachefile(filename);
  for (unsigned int idx=0;idx<cache.size();idx++){
    const std::string& line = cache[idx];
    std::string label;
    kind_of_line kind = code;
    if (is_include(line)) {
      if (is_global_include(line)) kind = global_include; 
      else if (is_local_include(line)) kind = local_include;
      }
    else if (is_define(line)) kind = define;
    else if (is_ifdef(line)) kind = ifdef;
    else if (is_ifndef(line)) kind = ifndef;
    else if (is_endif(line)) kind = endif;
                                std::cerr << "[" << write << "]" ;
    if (kind == local_include)  std::cerr << " ( LOCAL  ) ";
    if (kind == global_include) std::cerr << " ( GLOBAL ) ";
    if (kind == code)           std::cerr << " ( CODE   ) ";
    if (kind == define)         std::cerr << " ( DEFINE ) ";
    if (kind == ifdef)          std::cerr << " ( IFDEF  ) ";
    if (kind == ifndef)         std::cerr << " ( IFNDEF ) ";
    if (kind == endif)          std::cerr << " ( ENDIF  ) ";
                                std::cerr << "File:" << filename << " - Line:" << std::setw(4) << idx << " - Lvl:" << level << " - " << line << std::endl;
    
    
      if (kind == code) {   
        //if (write) output.push_back(static_cast<std::stringstream&>(std::stringstream() << "/*" << std::setw(15) << filename << " line:" << std::setw(3) << idx << "*/ ").str()+line); 
        if (write) output.push_back(line);
        //std::cout << "File:" << filename << " - " << "line: " << idx << " lvl:"<< level << " - " << line << std::endl;
        }
      if (write) if (kind == local_include) { 
        label = get_included(line);
        local_includes[label] = true;
        parse(label);
         }
      if (write) if (kind == global_include){
        label = get_included(line);
        global_includes[label] = true;
        }
      if (write) if (kind == define){
        label = get_define(line);
        std::cerr << "               **" << "DEFINED \"" << label << "\"" << std::endl;
        defines[label]=true;}
      
      if (kind == ifndef){
        label = get_ifndef(line);
        if (defines[label]==true) std::cerr << "               **" << "IFNDEF \"" << label << "\" * " << std::endl;
        else                      std::cerr << "               **" << "IFNDEF \"" << label << "\" X " << std::endl;
        if (defines[label]==true){
          write_level=level;
          level++;
          write=false;
          }
        }
      if (kind == ifdef){
        label = get_ifdef(line);
        if (defines[label]==false)std::cerr << "               **" << "IFDEF \"" << label << "\" * " << std::endl;
        else                      std::cerr << "               **" << "IFDEF \"" << label << "\" X " << std::endl;
        if (defines[label]==false){
          write_level=level;
          level++;
          write=false;
          }
        }
      if (kind == endif){
        std::cerr << "               **" << "ENDIF makes level go up" << std::endl; 
        if (write==false) level--;
        if (write_level==level) if (write==false) write=true;
        }
    
  }
  return;
}


#ifdef STANDALONE_TOOL
int main(int argc,char **argv){
#else
int bundler_main(int argc,char **argv){
#endif
  rcl::stringvector args(argv,argv+argc);
  header_task task;
  for (int idx=1;idx<args.size();idx++) task.parse(args[idx]);
  task.write_preamble();
  std::cout << task.get_preamble();
  std::cout << task.get_output(); 
  return 0;
}

#ifndef STANDALONE_TOOL
static rcl::reg_app reg("include_bundle_tool",bundler_main);
#endif
