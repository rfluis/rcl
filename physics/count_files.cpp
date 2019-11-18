#include <iostream>
#include <sstream>
#include <iomanip>
#include "files.hpp"
#include "filepath.hpp"
// bool check_file(const std::string& filename);

int main(int argc,char **argv){
  std::vector<std::string> files;
  std::vector<std::string> paths(argv+1,argv+argc);
  for(unsigned int idx=0;idx<paths.size();idx++)
  {
    for(unsigned int counter=0;counter<1000000;counter++){
      std::stringstream ss;
      ss << paths[idx] << "/" << "m" << std::setw(6) << std::setfill('0') << counter << ".vf";
      if (!rcl::check_file(ss.str())) break;
      files.push_back(ss.str());
      }
  }
  
  for (auto itr=files.begin();itr!=files.end();itr++) std::cout << *itr << std::endl;
}
