#ifndef RCL_SKYRMION_MODEL
#define RCL_SKYRMION_MODEL

#include "tfield.hpp"
#include "tvector.hpp"
#include "tpoint.hpp"
#include "constants.hpp"

namespace rcl{
  template <typename T>
  class Flat_Neel_Skyrmion{
  public:
    typedef rcl::tpoint<T>      point;
    typedef T                   basetype;
    typedef rcl::tvector<T>     vector;
    typedef rcl::tfield<vector> field;
  private:
    point pos;
    point rdw;
    rcl::utriad nodes;
    rcl::ftriad cellsz;
    bool polarity = false;
    bool chirality = false;
  public:
    Flat_Neel_Skyrmion(){return;}
    Flat_Neel_Skyrmion(const point& pos,const point& rdw){
      this->pos = pos;
      this->rdw = rdw;
      cellsz = rcl::ftriad(1,1,1);
      return;
    }
    Flat_Neel_Skyrmion(const basetype& rad,const basetype& dww){
      pos = point(0,0);
      rdw = point(rad,dww);
      cellsz = rcl::ftriad(1,1,1);
      return;
      }
    point& getpos() { return pos;}
    point& getrdw() { return rdw;}
    rcl::utriad& getnodes() { return nodes;}
    rcl::ftriad& getcells() { return cellsz;}
    bool& get_polarity() {return polarity;}
    bool& get_chirality() {return chirality;}
    const point& getpos() const { return pos;}
    const point& getrdw() const { return rdw;}    
    const rcl::utriad& getnodes() const { return nodes;}
    const rcl::ftriad& getcells() const { return cellsz;}
    const bool& get_polarity()  const {return polarity;}
    const bool& get_chirality() const {return chirality;}    
    basetype dist(point r)const { r-=pos; return r.norm();}
    basetype theta(const basetype& r) const { 
      if (r<rdw.x()-rdw.y()/2.f) return 0;
      if (r>rdw.x()+rdw.y()/2.f) return rcl::PI<basetype>;
      return (r-rdw.x()+(rdw.y()/2))*rcl::PI<basetype>/rdw.y(); }
    basetype theta(const point& r) const { return theta(dist(r));}
    basetype phi(const point& r) const {if (chirality) return std::atan2(r.y()-pos.y(),r.x()-pos.x()); else return std::atan2(pos.y()-r.y(),pos.x()-r.x());}
    basetype z(const basetype& r) const { if (polarity) return std::cos(theta(r)); else return -std::cos(theta(r)); }
    basetype z(const point& r) const { if (polarity) return std::cos(theta(r)); else return -std::cos(theta(r)); }
    std::vector<point> render_z (const std::vector<point>& sample) const {
      std::vector<point> ans;
      for (typename std::vector<point>::const_iterator itr=sample.begin();itr!=sample.end();itr++){
        ans.push_back(point(itr->x(),z(itr->x())));
        }
      return ans;
      }
    vector operator[](const point& r)const {return v(r);}
    vector v(const point& r)const{
      basetype th = theta(r);
      point dir = r-pos;
      if (dir==point()) dir = point(1,1);
      dir.normalize();
      if (!polarity) th = rcl::PI<basetype> - th;
      if (chirality) dir*=-1;
      return vector(std::sin(th)*dir.x(),std::sin(th)*dir.y(),std::cos(th));
      }
    void render(std::vector<vector>& v) const;
    field render_field() const;
    std::vector<vector> render() const;
};

template<typename T> void rcl::Flat_Neel_Skyrmion<T>::render(std::vector<typename rcl::Flat_Neel_Skyrmion<T>::vector>& vdata)const{
  if (nodes.volume()==0) return;
  if (vdata.size()<nodes.volume()) vdata.resize(nodes.volume());
  T fx,fy,fz;
  for (unsigned int idz=0;idz<nodes.z();idz++){
    fz = static_cast<T>(idz)*cellsz.z();
    for (unsigned int idy=0;idy<nodes.y();idy++){
      fy = static_cast<T>(idy)*cellsz.y();
      for (unsigned int idx=0;idx<nodes.x();idx++){
        fx = static_cast<T>(idx)*cellsz.x();
          vdata[nodes.addr(rcl::utriad(idx,idy,idz))] = v(point(fx,fy));
        }
      }
    }
  return;
  }
template<typename T> std::vector<typename rcl::Flat_Neel_Skyrmion<T>::vector> rcl::Flat_Neel_Skyrmion<T>::render() const{
  std::vector<rcl::Flat_Neel_Skyrmion<T>::vector> ans;
  render(ans);
  return ans;
  }
template<typename T> typename rcl::Flat_Neel_Skyrmion<T>::field rcl::Flat_Neel_Skyrmion<T>::render_field() const{
  rcl::Flat_Neel_Skyrmion<T>::field ans(nodes,cellsz);
  render(ans.getdata());
  return ans;
  }
  
}// end namespace

#endif
