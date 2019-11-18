#ifndef BUSYBOX_HEADER
#define BUSYBOX_HEADER

#include <vector>
#include <string>
#include <map>
#include <iostream>

class initializer{};

struct app{
  std::string alias;
  int (*ptr)(const std::vector<std::string>&);
};

class app_register{
  private:
  std::map<std::string,int (*ptr)(const std::vector<std::string>&)> data;
  public:
  static int callback(const std::vector<std::string>& params){
    std::cout << "Called function doesn't exists" << std::endl;
    for (unsigned int idx=0;idx<params.size();idx++){
      std::cout << params[idx] ;
      if (idx == params.size()-1) std::cout << std::endl;
      else std::cout << " , ";
      }
      return 1;
    }
  void push_back(const std::string& alias,int (*ptr)(const std::vector<std::string>&)){
    data[alias]=ptr;
    return;
    }
};



#ifndef BUSYBOX_REGISTER
#define BUSYBOX_REGISTER
extern app_register genreg;
#endif

#endif

// FIN
