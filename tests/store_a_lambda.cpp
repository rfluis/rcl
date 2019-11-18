#include <iostream>

template <typename FN> void execute(const FN& fn);

int main()
{
  auto const& hi = []()->void{std::cout << "Hi :-)" << std::endl; return;};
  auto const& code = []()->void{std::cout << "<Some Code Here>" << std::endl; return;};
  auto const& bye = []()->void{std::cout << "Byeeeee :-)" << std::endl; return;};
  execute(hi);
  execute(code);
  execute(code);
  execute(code);
  execute(code);
  execute(bye);
  return 0;
  
}

template <typename FN>
void execute(const FN& fn){
  std::cout << " Executing" << std::endl;
  std::cout << "  ";
  fn();
  std::cout << " End of execution" << std::endl<< std::endl;
  return;
  }
