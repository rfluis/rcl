#include "busybox.hpp"
#include "filepath-fn.hpp"
#include "filepath.hpp"
/* const std::string& GetPath() const {return path;}
   const std::string& GetFile() const {return file;}
   const std::string& GetExt() const {return ext;}
   const std::string& GetLext() const {return lext;}
*/

int filepath_test_main(int argc,char** argv){
  std::string sample="m0.1s2a3e4r5t6F7D8S9";
  for (int idx=0;idx<sample.length();idx++) std::cout << sample[idx];
  std::cout << std::endl;
  for (int idx=0;idx<sample.length();idx++) std::cout << rcl::FilePath::is_number(sample[idx]);
  std::cout << std::endl;
  for (int idx=1;idx<argc;idx++){
    rcl::FilePath fp(argv[idx]);
    std::cout << "Path :" << fp.GetPath() << std::endl;
    std::cout << "File :" << fp.GetFile() << std::endl;
    std::cout << "Ext  :" << fp.GetExt()  << std::endl;
    std::cout << "Lext :" << fp.GetLext() << std::endl;
    std::cout << " ==================================================================" << std::endl;
    }
  return 0;
}

static rcl::reg_app reg("filepath_test",filepath_test_main);
