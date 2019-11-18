#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

int main(){
  std::vector<double> points;
  std::vector<double> cutted;
  std::vector<double> fixed;
  std::vector<double> points3;
  std::vector<double> cutted3;
  std::vector<double> fixed3;
  
  for (double x=0;x<10;x+=0.01) points.push_back(10*std::sin(x));
  for (double x=0;x<10;x+=0.01) points3.push_back(3*std::sin(x));
  for (int x=0;x<points.size();x++) cutted.push_back(points[x]-std::floor(points[x]));
  for (int x=0;x<points.size();x++) cutted3.push_back(points3[x]-std::floor(points3[x]));
  double offset = 0;
  fixed.push_back(cutted[0]);
  for (int x=1;x<points.size();x++){
    if (cutted[x]-cutted[x-1]>0.8) offset--;
    if (cutted[x]-cutted[x-1]<-0.8) offset++;
    fixed.push_back(cutted[x]+offset);    
    }
  
  for (int x=0;x<points.size();x++) {
    std::cout << std::setw(16) << x;
    std::cout << std::setw(16) << points[x] ;
    std::cout << std::setw(16) << cutted[x] ;
    std::cout << std::setw(16) << fixed[x] << std::endl;
  }
  return 0;
}
