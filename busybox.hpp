#ifndef BUSYBOX_HEADER
#define BUSYBOX_HEADER

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "filepath.hpp"

namespace rcl{

class app_container{
private:
  std::map<std::string,int (*)(int,char**)> apps;
public:
  app_container(){return;}
  static int callback(int argc,char** argv);
  bool exists(const std::string& name)const {
    std::map<std::string,int (*)(int,char**)>::const_iterator itr;
    itr = apps.find(name);
    if (itr!=apps.end()) return true;
    return false;
  }
  std::vector<std::string> app_list()const{
    std::vector<std::string> list;
    for (std::map<std::string,int (*)(int,char**)>::const_iterator itr=apps.begin();itr!=apps.end();itr++) list.push_back(itr->first);
    return list;
  }
  void add(const std::string& name,int (* func)(int,char**)){
    //std::cout << "Loading ... " << name << std::endl;
    apps.insert(std::pair<std::string,int (*)(int,char**)>(name,func));
    return;
    }
  int (*const operator[](const std::string& name))(int,char**){
    if (exists(name)) return apps[name];
    else return callback;
    }
};

#ifndef REGAPPS_DECLARE
extern app_container regApps;

class reg_app{
  public:
  reg_app(const std::string& name,int (*const func)(int,char**)){regApps.add(name,func);}
};
#endif // end REGAPPS_DECLARE

}// end namespace

/*
#include <vector>
#include <string>
#include "filepath.hpp"
#include <iostream>

namespace rcl{

class busybox_app{
public:
  busybox_app(const std::string& n,int (&p)(int,char**)):name(n),mptr(p){return;}
private:
  std::string name;
  int (& mptr)(int,char**);
public:
  int operator()(int argc,char** argv){ return mptr(argc,argv); }
  operator const std::string&() {return name;}
};

}
*/

#endif
