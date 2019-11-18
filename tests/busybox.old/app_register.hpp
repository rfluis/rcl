#ifndef TEST_APP_REGISTER
#define TEST_APP_REGISTER

#include <iostream>
#include <string>
#include <vector>

class Register{
  private:
  std::vector<std::string> keys,values;
  public:
  Register(){return;}
  void add(const std::string& key,const std::string& val){
    keys.push_back(key);
    values.push_back(val);
    dshow();
    return;
    }
  void show()const{
    std::cout << " ///////////////////////////////////////////" << std::endl;
    for (unsigned int idx=0;idx<keys.size();idx++)
      std::cout << idx << " : " << keys[idx] << " -- " << values[idx] << std::endl;
    std::cout << " ///////////////////////////////////////////" << std::endl;
    return;
  } 
  void dshow()const{
    std::cout << " *******************************************" << std::endl;
    for (unsigned int idx=0;idx<keys.size();idx++)
      std::cout << " *** " << idx << " : " << keys[idx] << " -- " << values[idx] << std::endl;
    std::cout << " *******************************************" << std::endl;
    return;
  } 
};


extern Register GReg; 


class Register_Caller{
  public:
  Register_Caller(const std::string& k,const std::string& v){GReg.add(k,v); return;}  
  Register& get_reg() const{ return GReg;}
};



#endif
