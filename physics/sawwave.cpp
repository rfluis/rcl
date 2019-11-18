#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

template<typename T>
std::vector<T> fix_toroidal_movement(const std::vector<T>& src,const T& limit);

int main(){
  std::vector<double> points;
  std::vector<double> cutted;
  std::vector<double> fixed;
  
  for (double x=0;x<10;x+=0.01)     points.push_back(10*std::sin(x));
  for (int x=0;x<points.size();x++) cutted.push_back(points[x]-std::floor(points[x]));
  fixed = fix_toroidal_movement(cutted,1.);
  
  for (int x=0;x<points.size();x++) {
    std::cout << std::setw(16) << x;
    std::cout << std::setw(16) << points[x] ;
    std::cout << std::setw(16) << cutted[x] ;
    std::cout << std::setw(16) << fixed[x] << std::endl;
  }
  
  return 0;
}

template<typename T>
std::vector<T> fix_toroidal_movement(const std::vector<T>& src,const T& limit){
  std::vector<T> ans;
  T offset=0,threshold = limit*static_cast<T>(0.5);
  ans.push_back(src[0]);
  for (int x=1;x<src.size();x++){
    if (src[x]-src[x-1]>threshold) offset-=static_cast<T>(1.);
    if (src[x-1]-src[x]>threshold) offset+=static_cast<T>(1.);
    ans.push_back(src[x]+offset);    
    }
  return ans;
}
