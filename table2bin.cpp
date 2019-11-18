#include "busybox.hpp"
#include <fstream>
#include <iostream>
#include <vector>

int table2bin_main(int argc,char **argv){
  std::ifstream ifs(argv[1],std::ifstream::in);
  std::ofstream ofs(argv[2],std::ofstream::out|std::ofstream::binary);
  std::vector<float> data;
  while (!ifs.eof()) { float datum=0; ifs >> datum; if (!ifs.eof()) data.push_back(datum); }
  ofs.write(reinterpret_cast<char*>(&data[0]),data.size()*sizeof(data[0]));
  std::cout << "Wrote " << data.size() << " values" << std::endl;
  return 0;  
}

static rcl::reg_app reg("table2bin",table2bin_main);
