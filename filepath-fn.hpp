#ifndef FILEPATH_FN_HEADER
#define FILEPATH_FN_HEADER

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

namespace rcl{
  std::vector<std::string> getfilepath(const std::string& filename);
}

#endif
