#include "newdisplay.hpp"


namespace rcl{
image32 display_field(const ffield& data,const unsigned int& layer,std::string format){
  utriad nodes = data.getnodes();
  image32 img(nodes.x(),nodes.y());
  unsigned int yf = nodes.y()-1;
  unsigned int yy = nodes.y();
  unsigned int xx = nodes.x();
  float h,s,l;
  if (format=="") format="nrainbow";
  if (format=="rainbow") for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
    fvector v = data[utriad(x,y,layer)];
    h = v.phi()*ITPI<float>; if (h<0) h=h+1.;
    s = v.norm();
    l = v.z()*0.5f + 0.5f;
    if (s==0) img(x,yf-y)=pixel32(0);
    else      img(x,yf-y)=HSLtoRGB(h,s,l);    
    }
  if (format=="nrainbow"){
    ffield ndata=data;
    normpeak(ndata);
    for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
      fvector v = ndata[utriad(x,y,layer)];
      h = v.phi()*ITPI<float>; if (h<0.f) h=h+1.f;
      s = 1.f;
      l = (v.z()/v.norm())*0.5f + 0.5f;
      if (v.norm2()==0) img(x,yf-y)=pixel32(0);
      else              img(x,yf-y)=HSLtoRGB(h,s,l);    
      } 
    }
  if (format=="urainbow") for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
      fvector v = data[utriad(x,y,layer)];
      h = v.phi()*ITPI<float>; if (h<0) h=h+1.;
      s = 1.f;
      l = (v.z()/v.norm())*0.5 + 0.5;
      if (v.norm2()==0) img(x,yf-y)=pixel32(0);
      else              img(x,yf-y)=HSLtoRGB(h,s,l);    
      } 
  if (format=="prainbow") for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
    fvector v = data[utriad(x,y,layer)];
    h = v.phi()*ITPI<float>; if (h<0) h=h+1.;
    s = v.norm();
    l = 0.5;
    if (s==0) img(x,yf-y)=pixel32(0);
    else      img(x,yf-y)=HSLtoRGB(h,s,l);    
    }
  if (format=="xfrance")  for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
    fvector v = data[utriad(x,y,layer)];
    float h,s,l;
    s = v.norm2();
    if (s==0) img(x,yf-y)=pixel32(0);
    else      img(x,yf-y)=France(v.x());   
    }
  if (format=="yfrance")  for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
    fvector v = data[utriad(x,y,layer)];
    float h,s,l;
    s = v.norm2();
    if (s==0) img(x,yf-y)=pixel32(0);
    else      img(x,yf-y)=France(v.y());   
    }
  if (format=="zfrance")  for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
    fvector v = data[utriad(x,y,layer)];
    float h,s,l;
    s = v.norm2();
    if (s==0) img(x,yf-y)=pixel32(0);
    else      img(x,yf-y)=France(v.z());   
    }
  if (format=="xgray")    for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
    fvector v = data[utriad(x,y,layer)];
    float h,s,l;
    s = v.norm2();
    if (s==0) img(x,yf-y)=pixel32(0);
    else      img(x,yf-y)=GrayScale(v.x());   
    }  
  if (format=="ygray")    for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
    fvector v = data[utriad(x,y,layer)];
    float h,s,l;
    s = v.norm2();
    if (s==0) img(x,yf-y)=pixel32(0);
    else      img(x,yf-y)=GrayScale(v.y());   
    }  
  if (format=="zgray")    for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
    fvector v = data[utriad(x,y,layer)];
    float h,s,l;
    s = v.norm2();
    if (s==0) img(x,yf-y)=pixel32(0);
    else      img(x,yf-y)=GrayScale(v.z());   
    } 
  if (format=="zmono")    for(unsigned int y=0;y<yy;y++) for (unsigned int x=0;x<xx;x++){
    fvector v = data[utriad(x,y,layer)];
    float h,s,l;
    s = v.norm2();
    if (s==0) img(x,yf-y)=pixel32(0);
    else      img(x,yf-y)=UnitGrayScale(v.z());   
    } 
  return img;
}

image32 basic_plot(const rcl::upoint& size,const std::vector<rcl::fpoint>& list,const rcl::pixel32& color){
  rcl::font fnt = rcl::loadfont(0);
  rcl::image32 img(size.x(),size.y());
  std::vector<rcl::fpoint> worklist(list);
  std::sort(worklist.begin(),worklist.end(),[](const rcl::fpoint& a,const rcl::fpoint& b)->bool{return a.x()<b.x();});
  float minx = worklist[0].x();
  float maxx = worklist[worklist.size()-1].x();
  float miny = worklist[0].y();
  float maxy = worklist[0].y();
  for (unsigned int idx=1;idx<worklist.size();idx++){
    if (worklist[idx].y()<miny) miny = worklist[idx].y();
    if (worklist[idx].y()>maxy) maxy = worklist[idx].y();
    }
  float deltax = (maxx - minx)/static_cast<float>(size.x());
  float deltay = (maxy - miny)/static_cast<float>(size.y());
  for (unsigned int idx=1;idx<worklist.size();idx++)
  {
    unsigned int x1 = static_cast<unsigned int>((worklist[idx-1].x()-minx)/deltax);
    unsigned int y1 = static_cast<unsigned int>((worklist[idx-1].y()-miny)/deltay);
    unsigned int x2 = static_cast<unsigned int>((worklist[idx].x()-minx)/deltax);
    unsigned int y2 = static_cast<unsigned int>((worklist[idx].y()-miny)/deltay);
    img.line(x1,y1,x2,y2,color);
  }
  return img;
} 


}
