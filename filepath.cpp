#include "filepath-fn.hpp"
#include "filepath.hpp"

namespace rcl{
  
  FilePath::FilePath(const std::string& s){
    std::string filename;
    std::string::const_reverse_iterator c=std::find(s.rbegin(),s.rend(),'/');
    path = std::string(s.begin(),c.base());
    filename = std::string(c.base(),s.end());
    std::string::const_reverse_iterator le=std::find(filename.rbegin(),filename.rend(),'.');
    std::string::const_iterator         fe=std::find(filename.begin(),filename.end(),'.');
    while (is_number(*(fe+1))) 
      fe=std::find(static_cast<std::string::const_iterator>(fe+1),
                  static_cast<std::string::const_iterator>(filename.end()),'.');
    if (fe == filename.end()) {file = filename; return;}
    file = std::string((std::string::const_iterator)filename.begin(),fe);
    ext = std::string(fe,le.base()-1);
    lext = std::string(le.base()-1,(std::string::const_iterator)filename.end());
    return;
    }
  
  std::vector<std::string> getfilepath(const std::string& filename){
    std::vector<std::string> ans;
    rcl::FilePath fp(filename);
    ans.push_back(fp.GetPath());
    ans.push_back(fp.GetFile());
    ans.push_back(fp.GetExt());
    ans.push_back(fp.GetLext());
    return ans;
    }
    
  std::ostream& operator<< (std::ostream& os,const FilePath& fp){
  os << fp.GetPath();
  os << fp.GetFile();
  os << fp.GetExt();
  os << fp.GetLext();
  return os;
  }
}
