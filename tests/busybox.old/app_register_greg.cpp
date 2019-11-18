#include <iostream>
#include <string>
#include <vector>

class Register{
  private:
  std::string name;
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
    for (unsigned int idx=0;idx<keys.size();idx++)
      std::cout << idx << " : " << keys[idx] << " -- " << values[idx] << std::endl;
    return;
  } 
  void dshow()const{
    for (unsigned int idx=0;idx<keys.size();idx++)
      std::cout << " *** " << idx << " : " << keys[idx] << " -- " << values[idx] << std::endl;
    return;
  } 
} GReg;
