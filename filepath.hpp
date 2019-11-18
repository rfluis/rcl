#ifndef FILEPATH_HEADER
#define FILEPATH_HEADER

#include "filepath-fn.hpp"

namespace rcl{

class FilePath{
  std::string path,file,ext,lext;
public:
  FilePath(){return;}
  FilePath(const std::string& s);
  static bool is_number(const char& c){
    if (c>='0' && c<='9') return true;
    return false;
    }
  const std::string& GetPath() const {return path;}
  const std::string& GetFile() const {return file;}
  const std::string& GetExt() const {return ext;}
  const std::string& GetLext() const {return lext;}
        std::string  GetRoot() const {return path+file;}
        std::string  GetName() const {return file+ext;}
        std::string  GetFullFile() const {return file+ext+lext;}
        std::string  GetFullPath() const {return path+file+ext+lext;}
        std::string  GetFullExt() const {return ext+lext;}
};

std::ostream& operator<< (std::ostream& os,const FilePath& fp);

}
#endif
