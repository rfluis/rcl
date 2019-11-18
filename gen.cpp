#include "busybox.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>

int gen_main(int argc,char **argv){
  std::string name(argv[1]),uname(argv[1]);
  for (auto & c: uname) c = std::toupper(c);
  std::fstream header(name+".hpp",std::fstream::out);
  std::fstream code(name+".cpp",std::fstream::out);
  
  header << "#ifndef " << uname << "_HEADER" << std::endl;
  header << "#define " << uname << "_HEADER" << std::endl;
  header << std::endl;
  header << "namespace rcl{" << std::endl;
  header << std::endl;
  header << "}" << std::endl;
  header << "#endif" << std::endl;
  
  code << "#include \""<<name << ".hpp"<< "\""  << std::endl;
  code << std::endl;
  code << "namespace rcl{" << std::endl;
  code << std::endl;
  code << "}" << std::endl;
  return 0;
}

static rcl::reg_app reg("gen_template_tool",gen_main);
