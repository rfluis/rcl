#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

struct point{
  double x,y;
  point(){return;}
  point(const double& xx,const double& yy){x=xx;y=yy; return;}
  point& operator+=(const point& rhs){ x+=rhs.x; y+=rhs.y; return *this;}
  point& operator-=(const point& rhs){ x-=rhs.x; y-=rhs.y; return *this;}
  point& operator*=(const double& rhs){ x*=rhs;   y*=rhs; return *this;}
  point& operator/=(const double& rhs){ x/=rhs; y/=rhs; return *this;}
  double norm2()const { return x*x+y*y;}
  double norm()const{return std::sqrt(norm2());}
  void normalize(){ (*this)/=norm(); return;}
  point normalized(){ return point(x/norm(),y/norm()); }
};

point operator+(point lhs, const point& rhs){return lhs+=rhs;}
point operator-(point lhs, const point& rhs){return lhs-=rhs;}
point operator*(point lhs, const double& rhs){return lhs*=rhs;}
point operator*(const double& rhs, point lhs){return lhs*=rhs;}
point operator/(point lhs, const double& rhs){return lhs/=rhs;}
point operator/(const double& rhs, point lhs){return lhs/=rhs;}
double operator*(const point& lhs,const point& rhs){ return (lhs.x*rhs.x)+(lhs.y*rhs.y);}
double operator^(const point& lhs,const point& rhs){ return (lhs.x*rhs.y)-(rhs.x*lhs.y);}
struct node{
  unsigned int src;
  double dist;
  node(){return;}
  node(const unsigned int& src,const double& dist){this->src=src;this->dist=dist;return;}  
};

double dist(const point& a,const point& b){
  return (a-b).norm();
}

std::vector<point> filter1(const std::vector<point>& data,const double& threshold){
  std::vector<point> ans;
  unsigned int size = data.size();
  ans.push_back(data[0]);
  const double& threshold2 = threshold*threshold;
  for (unsigned int idx=2;idx<size-2;idx++)
  {
    point vll = (data[idx]-data[idx-2]).normalized();
    point vl = (data[idx]-data[idx-1]).normalized();
    point vr = (data[idx+1]-data[idx]).normalized();
    point vrr = (data[idx+2]-data[idx]).normalized();
    double dp = vl*vr;
    double dpc = vll*vrr;
    if (dp<threshold) ans.push_back(data[idx]);
  }
  ans.push_back(data[size-1]);
  return ans;
}

std::vector<point> filter2(const std::vector<point>& data,const double& threshold){
  std::vector<point> ans;
  unsigned int size = data.size();
  point last = (data[1]-data[0]).normalized();
  ans.push_back(data[0]);
  for (unsigned int idx=1;idx<size-1;idx++){
    point vl = (data[idx]-data[idx-1]).normalized();
    double dp = vl*last;
    if (dp<threshold) {ans.push_back(data[idx]); last = vl;}
  }
  ans.push_back(data[size-1]);
  return ans;
}

std::vector<point> filter3(const std::vector<point>& data,const double& threshold){
  std::vector<point> ans;
  unsigned int size = data.size();
  point last = (data[1]-data[0]).normalized();
  ans.push_back(data[0]);
  for (unsigned int idx=1;idx<size-1;idx++){
    point vl = (data[idx]-data[idx-1]).normalized();
    double cp = std::fabs(vl^last);
    if (cp>threshold) {ans.push_back(data[idx]); last = vl;}
  }
  ans.push_back(data[size-1]);
  return ans;
}

int main(){
  std::vector<point> seno1;
  for (double x=0;x<5*6.28;x+=(0.0001+0.0001*std::fabs(std::cos(x)))) seno1.push_back(point(x,std::sin(x)));
  std::cout << "Calculados " << seno1.size() << " puntos" << std::endl;
  /*
  for (double threshold=0.9;threshold<1.;threshold+=0.001){
    std::stringstream name;
    name << "seno-" << threshold << ".txt";
    std::vector<point> f = filter2(seno1,threshold);
    std::fstream outfile(name.str(),std::fstream::out);
    std::cout << "Filtrado a  " << f.size() << " puntos con threshold = " << threshold << std::endl;
    for (std::vector<point>::const_iterator itr=f.begin();itr!=f.end();itr++)
      outfile << itr->x << " " << itr->y << std::endl;
    }
    */
  std::fstream cal("cal.txt",std::fstream::out);
  
  for (double threshold=0.05;threshold>0;threshold-=0.001){
    std::stringstream name;
    name << "seno-" << threshold << ".txt";
    std::vector<point> f = filter3(seno1,threshold);
    std::fstream outfile(name.str(),std::fstream::out);
    std::cout << "Filtrado a  " << f.size() << " puntos con threshold = " << threshold << " ( "<<std::asin(threshold)*180./3.1415927 << " )"<< std::endl;
    cal << threshold << " " << static_cast<double>(f.size())*threshold << std::endl;
    for (std::vector<point>::const_iterator itr=f.begin();itr!=f.end();itr++)
      outfile << itr->x << " " << itr->y << std::endl;
    }
  return 0;
}
