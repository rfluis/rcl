#ifndef TSTACK_HEADER
#define TSTACK_HEADER

#include <vector>

namespace rcl{

template<typename T>
class tstack{
  private:
  std::vector<T> data;
  public:
  void push(const T& x){
    data.push_back(x);
    }
  bool pop(T& x){
    if (data.size() == 0) return false;
    x = data.back();
    data.pop_back();
    return true;
    }
};


//void push(std::vector<int>& stack, int x, int y)
//{
//  // C++'s std::vector can act as a stack and manage memory for us
//  stack.push_back(x);
//  stack.push_back(y);
//}
//
//bool pop(std::vector<int>& stack, int& x, int& y)
//{
//  if(stack.size() < 2) return false; // it's empty
//  y = stack.back();
//  stack.pop_back();
//  x = stack.back();
//  stack.pop_back();
//  return true;
//}

}

#endif
