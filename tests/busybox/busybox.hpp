#ifndef BUSYBOX_HEADER
#define BUSYBOX_HEADER

#include <vector>
#include <string>
#include <map>
#include <iostream>

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
  void add(const std::string& name,int (*const func)(int,char**)){
    apps[name] = func;
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
#endif

#ifndef REGAPPS_DECLARE
class reg_tester{
  public:
  reg_tester(){
    std::vector<std::string> list = regApps.app_list();
    for (unsigned int idx=0;idx<list.size();idx++)
    std::cout << idx << " : " << list[idx] << std::endl;
    return;
    }
  reg_tester(const std::string& name,int argc,char** argv){
    int res = regApps[name](argc,argv); 
    std::cout << "Called function \""<< name << "\" and returned " << res << std::endl;
    return;}
};
#endif

#endif
