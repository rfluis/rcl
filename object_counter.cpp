#include "busybox.hpp"
#include "files.hpp"
#include "lib-png.hpp"
#include "tpoint.hpp"
#include "tstack.hpp"
#include "fonts.hpp"
#include "numerical.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include <map>
#include <chrono>
//shuffle (foo.begin(), foo.end(), std::default_random_engine(seed));

std::map<rcl::upoint,int> count(const rcl::image32& src,const rcl::pixel32& bg);
void floodfillcount(std::map<rcl::upoint,int>& catalog,const int& id,const rcl::upoint& sp,const rcl::image32& src,const rcl::pixel32& bg);

int object_counter_test_main(int argc,char **argv){
  std::string infile,outfile;
  infile = std::string("./resources/test-circles.png");
  outfile = std::string("test-circles-colour.png");
  if (argc>2){
    infile = std::string(argv[1]);
    outfile = std::string(argv[2]);
    }
  std::chrono::high_resolution_clock::time_point t1,t2;
  std::chrono::duration<double> time_span;
  std::map<rcl::upoint,int> recon;
  std::vector<rcl::fpoint> centers;
  std::vector<std::vector<rcl::fpoint> >coordseq;
  std::vector<unsigned char> data;
  rcl::image32 img,dst;
  rcl::upoint sz;
  std::vector<rcl::pixel32> palette;
  unsigned int counter=0;
  
  t1 = std::chrono::high_resolution_clock::now();
  rcl::load_file(data,infile);
  t2 = std::chrono::high_resolution_clock::now();
  time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "Lectura de disco: " << time_span.count() << " s." << std::endl;
  
  t1 = std::chrono::high_resolution_clock::now();
  rcl::decode_png(img,data);
  t2 = std::chrono::high_resolution_clock::now();
  time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "Decodificacion en RAM: " << time_span.count() << " s."<< std::endl;
  
  sz = img.GetXY();
  
  t1 = std::chrono::high_resolution_clock::now();
  std::map<rcl::upoint,int> catalog = count(img,rcl::pixel32(255,255,255));
  t2 = std::chrono::high_resolution_clock::now();
  time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "Reconocimiento: " << time_span.count() << " s."<< std::endl;
  
  t1 = std::chrono::high_resolution_clock::now();
  for (auto itr=catalog.begin();itr!=catalog.end();itr++) if (itr->second>counter) counter=itr->second;
  t2 = std::chrono::high_resolution_clock::now();
  time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "Cuento de puntos: " << time_span.count() << " s."<< std::endl;
  
  t1 = std::chrono::high_resolution_clock::now();
  palette.push_back(rcl::pixel32(255,255,255));
  std::default_random_engine generator(counter);
  std::uniform_int_distribution<int> distribution(0,255);
  for (unsigned int idx=0;idx<counter;idx++) 
    palette.push_back(rcl::pixel32(distribution(generator),distribution(generator),distribution(generator)));
  t2 = std::chrono::high_resolution_clock::now();
  time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "Generacion de paleta: " << time_span.count() << " s."<< std::endl;
  
  dst.fit(sz);
  
  t1 = std::chrono::high_resolution_clock::now();
  for (auto itr=catalog.begin();itr!=catalog.end();itr++) dst[itr->first]=palette[itr->second];
  t2 = std::chrono::high_resolution_clock::now();
  time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "Dibujado de la salida: " << time_span.count() << " s."<< std::endl;
  
  t1 = std::chrono::high_resolution_clock::now();
  data.clear();
  rcl::encode_png(data,dst);
  rcl::save_file(outfile,data);
  t2 = std::chrono::high_resolution_clock::now();
  time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  std::cout << "Codificando en PNG la salida: " << time_span.count() << " s."<< std::endl;
  return 0;  
}
static rcl::reg_app reg("object_counter_test",object_counter_test_main);

std::map<rcl::upoint,int> count(const rcl::image32& src,const rcl::pixel32& bg){
  int counter=1;
  std::map<rcl::upoint,int> catalog;
  rcl::upoint size(src.GetX(),src.GetY());
  for (unsigned int idy=0;idy<size.y();idy++)
  for (unsigned int idx=0;idx<size.x();idx++)
  {
    if (catalog.find(rcl::upoint(idx,idy))==catalog.end()){
      if (src(rcl::upoint(idx,idy))==bg) 
        catalog[rcl::upoint(idx,idy)]=0;
      else{
        floodfillcount(catalog,counter,rcl::upoint(idx,idy),src,bg); counter++;
        }
      }
  }
  return catalog;
}

void floodfillcount(std::map<rcl::upoint,int>& catalog,
                    const int& id,
                    const rcl::upoint& sp,
                    const rcl::image32& src,
                    const rcl::pixel32& bg){
  rcl::tstack<rcl::upoint> stk;
  rcl::upoint p;
  rcl::upoint size(src.GetX(),src.GetY());
  stk.push(sp);
  while(stk.pop(p)){
    catalog[p]=id;
    rcl::upoint neighbor;
    
    neighbor=p;
    if (p.x()==0) neighbor.x()=size.x()-1; else neighbor.x()--;
    if (catalog.find(neighbor)==catalog.end()) if (src(neighbor)!=bg) stk.push(neighbor);
    
    neighbor=p;
    if (p.x()==size.x()-1) neighbor.x()=0; else neighbor.x()++;
    if (catalog.find(neighbor)==catalog.end()) if (src(neighbor)!=bg) stk.push(neighbor);
    
    neighbor=p;
    if (p.y()==0) neighbor.y()=size.y()-1; else neighbor.y()--;
    if (catalog.find(neighbor)==catalog.end()) if (src(neighbor)!=bg) stk.push(neighbor);
    
    neighbor=p;
    if (p.y()==size.y()-1) neighbor.y()=0; else neighbor.y()++;
    if (catalog.find(neighbor)==catalog.end()) if (src(neighbor)!=bg) stk.push(neighbor);
    
    }
  return;
}
