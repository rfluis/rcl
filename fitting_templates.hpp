#ifndef RCL_FITTING_TEMPLATES_HEADER
#define RCL_FITTING_TEMPLATES_HEADER

#include "fitting.hpp"

namespace rcl{
  
// Guessing 
// template<typename T> void sumplex(std::vector<T>& data);
template <typename T> T get_majoritarty_mz(const rcl::tfield<rcl::tvector<T> >& mag,unsigned int& layer){
  std::vector<T> ans;
  rcl::utriad nodes = mag.getnodes();
  ans.resize(nodes.x()*nodes.y());
  for (unsigned int idy=0;idy<nodes.y();idy++)
    for (unsigned int idx=0;idx<nodes.x();idx++)
      ans[idx+idy*nodes.x()]=mag[rcl::utriad(idx,idy,layer)].z();
  rcl::sumplex(ans);
  return ans[0]/static_cast<T>(nodes.x()*nodes.y());
  }

template <typename T> rcl::tfield<rcl::tscalar<T> > get_topologycal_charge_density(const rcl::tfield<rcl::tvector<T> >& mag,unsigned int& layer){
  rcl::tfield<rcl::tvector<T> > ml,dx,dy,cp;
  rcl::tfield<rcl::tscalar<T> > ans;
  ml = mag.get_slice(layer);
  dx = rcl::diffx(ml);
  dy = rcl::diffy(ml);
  cp = dx^dy;
  ans = ml*cp;
  return ans;
  }

// Codes
template <typename T>
rcl::triad<T> locate_skyrmion_PBC(const rcl::tfield<rcl::tvector<T> >& mag){
  rcl::triad<T> ans; 
  rcl::utriad nodes = mag.getnodes();
  const unsigned int& layer =0;
  T spx = rcl::TPI<T>/static_cast<T>(nodes.x()); // Scale Periodic X
  T spy = rcl::TPI<T>/static_cast<T>(nodes.y()); // Scale Periodic Y
  unsigned int cells(0);
  std::vector<T> sumxc,sumxs,sumyc,sumys;
  for (unsigned int idy=0;idy<nodes.y();idy++){
    for (unsigned int idx=0;idx<nodes.x();idx++){
      if (mag[rcl::utriad(idx,idy,layer)].z()<0.) {
        cells++;
        sumxc.push_back(std::cos(idx*spx));
        sumxs.push_back(std::sin(idx*spx));
        sumyc.push_back(std::cos(idy*spy));
        sumys.push_back(std::sin(idy*spy));
      }
    }
  }
  rcl::sumplex(sumxc);
  rcl::sumplex(sumxs);
  rcl::sumplex(sumyc);
  rcl::sumplex(sumys);
  ans.x() = std::atan2(sumxs[0],sumxc[0]);
  ans.y() = std::atan2(sumys[0],sumyc[0]);
  ans.z() = std::sqrt(static_cast<T>(cells)*rcl::IPI<T>);
  if (ans.x()<0) ans.x()+=rcl::TPI<T>;
  if (ans.y()<0) ans.y()+=rcl::TPI<T>;
  ans.x()/=spx;
  ans.y()/=spy;
  return ans;
}

template <typename T>
rcl::pair<rcl::tpoint<T>,rcl::tpoint<T> > get_skyrmion_sizes(const rcl::tfield<rcl::tvector<T> >& mag){
  std::vector<rcl::tpoint<T> > skysample;
  rcl::triad<T> pos = rcl::estimate_skyrmion(mag); // En celdas
  rcl::tpoint<T> ppos(pos.x(),pos.y());
  bool cx=false,cmx=false,cy=false,cmy=false,done=false;
  if (pos.x()+static_cast<T>(1.5)*pos.z()>static_cast<T>(mag.getnodes().x()-1)) cx = true;
  if (pos.x()-static_cast<T>(1.5)*pos.z()<static_cast<T>(0))                   cmx = true;
  if (pos.y()+static_cast<T>(1.5)*pos.z()>static_cast<T>(mag.getnodes().y()-1)) cy = true;
  if (pos.y()-static_cast<T>(1.5)*pos.z()<static_cast<T>(0))                   cmy = true;
  
  /*
  std::cout << "(x,y,r)=("<< pos.x()<<","<< pos.y()<<","<<pos.z() <<") " << std::endl;
  * 
  std::cout << "X:" << 0 << " >? " << pos.x()-static_cast<T>(2)*pos.z() << " , " ;
  std::cout << static_cast<T>(mag.getnodes().x()) << " <? " << pos.x()+static_cast<T>(2)*pos.z()<< std::endl;
  
  std::cout << "Y:" << 0 << " >? " << pos.y()-static_cast<T>(2)*pos.z() << " , " ;
  std::cout << static_cast<T>(mag.getnodes().y()) << " <? " << pos.y()+static_cast<T>(2)*pos.z()<< std::endl;
  */
  
  if (cx==false){
    /*
     std::cout << "fit in x+ :" << pos.x() << " , " << pos.x()+static_cast<T>(2)*pos.z() << std::endl;
    */
    for (T cursor=pos.x();cursor<pos.x()+static_cast<T>(2)*pos.z();cursor=cursor+0.1) 
      skysample.push_back(rcl::tpoint<T>(cursor-pos.x(),mag(cursor,pos.y(),0).z()));
    done=true;
    }
    
  if (done==false) if (cmx==false){
    /*std::cout << "fit in x-" << std::endl;*/
    for (T cursor=pos.x();cursor<pos.x()-static_cast<T>(2)*pos.z();cursor=cursor-0.1) 
      skysample.push_back(rcl::tpoint<T>(pos.x()-cursor,mag(cursor,pos.y(),0).z()));
    done=true;
    }
  if (done==false) if (cy==false){
    /*std::cout << "fit in y+" << std::endl;*/
    for (T cursor=pos.y();cursor<pos.y()+static_cast<T>(2)*pos.z();cursor=cursor+0.1) 
      skysample.push_back(rcl::tpoint<T>(cursor-pos.y(),mag(pos.x(),cursor,0).z()));
    done=true;
    }
  if (done==false) if (cmy==false){
    /*std::cout << "fit in y-" << std::endl;*/
    for (T cursor=pos.y();cursor<pos.y()-static_cast<T>(2)*pos.z();cursor=cursor-0.1) 
      skysample.push_back(rcl::tpoint<T>(pos.y()-cursor,mag(pos.x(),cursor,0).z()));
    done=true;
    }
  if (done==false) std::cout << "ERROR: unable to fit" << std::endl;

  if (skysample.size()==0) return rcl::pair<rcl::tpoint<T>,rcl::tpoint<T> >();
  rcl::pair<rcl::pair<T,T>,rcl::pair<T,T> > solution;
  auto const& norm = [&skysample = static_cast<const std::vector<rcl::tpoint<T> >&>(skysample)](const T& rad,const T& dww)->T{
    std::vector<rcl::tpoint<T> > analytic = rcl::Flat_Neel_Skyrmion<T>(rad,dww).render_z(skysample);
    std::vector<T> sums = make_trapezoids(analytic,skysample);
    rcl::sumplex(sums);
    return sums[0];
    };
  auto const& lessthan = [](const T& a,const T& b)->bool{return a<b;};
  rcl::pair<T,T> radp(static_cast<T>(0.1)*pos.z(),static_cast<T>(2)*pos.z());
  rcl::pair<T,T> dwwp(static_cast<T>(0.1)*pos.z(),static_cast<T>(2)*pos.z());
  rcl::pair<unsigned int,unsigned int> samples(100,100);
  
  solution = findval(radp,dwwp,samples,norm,lessthan);
    
  solution = findval(solution.first,solution.second,samples,norm,lessthan);

  solution = findval(solution.first,solution.second,samples,norm,lessthan);

  rcl::pair<T,T> rdw(solution.x().x(),solution.y().x());
  return rcl::pair<rcl::tpoint<T>,rcl::tpoint<T> >(ppos,rdw);
}

// First Estimations

template <typename T>
rcl::triad<T> locate_skyrmion(const rcl::tfield<rcl::tvector<T> >& mag){
  const unsigned int& layer =0;
  rcl::triad<T> est = rcl::estimate_skyrmion(mag);
  rcl::triad<T> ans;  
  T xi=est.x()-1.5f*est.z();
  T yi=est.y()-1.5f*est.z();
  T xf=est.x()+1.5f*est.z();
  T yf=est.y()+1.5f*est.z();
  if (xi<0.f) xi=0.f;
  if (yi<0.f) yi=0.f;
  if (xf>static_cast<T>(mag.getnodes().x()-1)) xf = static_cast<T>(mag.getnodes().x()-1);
  if (yf>static_cast<T>(mag.getnodes().y()-1)) yf = static_cast<T>(mag.getnodes().y()-1);  
  const T& h=0.125;
  unsigned int cells(0);
  
  std::vector<rcl::tpoint<T> > sums;
  for (float fy=yi;fy<yf;fy+=h)
    for (float fx=xi;fx<xf;fx+=h){
      if (mag(fx,fy,layer).z()<0.){
        sums.push_back(rcl::tpoint<T>(fx,fy));
      }
    }
  sums.resize(cells);
  rcl::sumplex(sums);
  sums[0]/=static_cast<T>(cells);
  ans.x() = sums[0].x();
  ans.y() = sums[0].y();
  ans.z() = std::sqrt(static_cast<T>(cells)*rcl::IPI<T>*h*h);
  return ans;
}

template <typename T>
rcl::triad<T> estimate_skyrmion(const rcl::tfield<rcl::tvector<T> >& mag){
  const unsigned int& layer =0;
  rcl::triad<T> ans; // px,py,r in CELL units
  unsigned int cells(0);
  std::vector<rcl::tpoint<T> > sums;
  rcl::utriad nodes = mag.getnodes();
  rcl::ftriad size = mag.getsize();
  if (layer>nodes.z()-1) return ans;
  for (unsigned int idy=0;idy<nodes.y();idy++){
    for (unsigned int idx=0;idx<nodes.x();idx++){
      if (mag[rcl::utriad(idx,idy,layer)].z()<0.) {
        cells++; 
        sums.push_back(rcl::tpoint<T>(static_cast<T>(idx),static_cast<T>(idy)));
        }                  
      }
    }
  rcl::sumplex(sums);

  sums[0]/=static_cast<T>(cells);
  ans.x() = sums[0].x();
  ans.y() = sums[0].y();
  ans.z() = std::sqrt(static_cast<T>(cells)*rcl::IPI<T>);
  return ans;
}  

// Integration Methods

template <typename T>
std::vector<T> make_trapezoids (const std::vector<rcl::tpoint<T> >& a, const std::vector<rcl::tpoint<T> >& b){
  std::vector<T> ans;
  if (a.size()!=b.size()) return ans;
  unsigned int size = a.size();
  ans.resize(size-1);
  for (unsigned int idx=0;idx<size-1;idx++){
    ans[idx]=rcl::sqr(a[idx].y()+a[idx+1].y()-b[idx].y()-b[idx+1].y())*(a[idx+1].x()-a[idx].x())*static_cast<T>(0.5);
    }
  return ans;
  }  
  
}


#endif
