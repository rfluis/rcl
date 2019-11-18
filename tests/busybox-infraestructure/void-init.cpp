#include <iostream>

class initializer{};

initializer pre(){
  std::cout << "Hello" << std::endl;
  return initializer();
}

static initializer exec = pre();
static initializer d = pre();

int main(){
  std::cout << "World!!" << std::endl;
  std::cout << sizeof(initializer) << std::endl;
  std::cout << static_cast<unsigned int>(reinterpret_cast<unsigned char&>(exec)) << std::endl;
  std::cout << static_cast<unsigned int>(reinterpret_cast<unsigned char&>(d)) << std::endl;
  return 0;
}
