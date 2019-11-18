#include "tfield.hpp"
#include "tvector.hpp"

#ifndef UM_FLAT_HEADER
#define UM_FLAT_HEADER
namespace rcl{
  
template <typename T>
class umfproblem{
private:
  rcl::utriad nodes;
  rcl::ftriad csize;
  
  rcl::tfield<rcl::tvector<T> > dir_anisotropy;
  rcl::tfield<rcl::tscalar<T> > str_anisotropy;
  rcl::tfield<rcl::tscalar<T> > str_exchange;
  rcl::tfield<rcl::tscalar<T> > str_dmi;
  rcl::tfield<rcl::tscalar<T> > str_mag_sat;
  rcl::tfield<rcl::tscalar<T> > str_damping;
  
  rcl::tfield<rcl::tvector<T> > mag;
  rcl::tfield<rcl::tvector<T> > mag_old;
  rcl::tfield<rcl::tvector<T> > h_ex;
  rcl::tfield<rcl::tvector<T> > h_dmi;
  rcl::tfield<rcl::tvector<T> > h_pma;
  rcl::tfield<rcl::tvector<T> > h_ext;
  rcl::tfield<rcl::tvector<T> > h_eff;
  rcl::tfield<rcl::tvector<T> > h_old;
  rcl::tfield<rcl::tvector<T> > k1,k2,k3,k4;  
  
public:
  void fit(const rcl::utriad& n,const rcl::ftriad& s){
    nodes=n;
    csize=s;
    fit();
    return;
    }
  void fit(){
    dir_anisotropy.fit(nodes,csize);
    str_anisotropy.fit(nodes,csize);
    str_exchange.fit(nodes,csize);
    str_dmi.fit(nodes,csize);
    str_mag_sat.fit(nodes,csize);
    str_damping.fit(nodes,csize);
    mag.fit(nodes,csize);
    mag_old.fit(nodes,csize);
    h_ex.fit(nodes,csize);
    h_dmi.fit(nodes,csize);
    h_pma.fit(nodes,csize);
    h_ext.fit(nodes,csize);
    h_eff.fit(nodes,csize);
    h_old.fit(nodes,csize);
    k1.fit(nodes,csize);
    k2.fit(nodes,csize);
    k3.fit(nodes,csize);
    k4.fit(nodes,csize);
    return; 
    }
  void calculate_hex();
  void calculate_pma();
  void calculate_dmi();
};

template <typename T> void umfproblem<T>::calculate_hex(){
  unsigned int nx = nodes.x()-1;
  unsigned int ny = nodes.y()-1;
  unsigned int nz = nodes.z()-1;
  
  //!\ optimizar usando indice lineal y sobreescribir al final los datos malos :-)
  
  for (unsigned int idy=1;idy<ny;idy++)  for (unsigned int idx=1;idx<nx;idx++)
    h_ex[rcl::utriad(idx,idy,0)]=2*str_exchange[rcl::utriad(idx,idy,0)]*(mag[rcl::utriad(idx+1,idy,0)]+mag[rcl::utriad(idx,idy+1,0)]-mag[rcl::utriad(idx-1,idy,0)]-mag[rcl::utriad(idx,idy-1,0)]);
 
  for (unsigned int idy=1;idy<ny;idy++) h_ex[rcl::utriad( 0,idy,0)]=2*str_exchange[rcl::utriad(0 ,idy,0)]*(mag[rcl::utriad( 1,idy, 0)] +mag[rcl::utriad(0,idy+1,0)] -mag[rcl::utriad(0,idy-1,0)]);
  for (unsigned int idy=1;idy<ny;idy++) h_ex[rcl::utriad(nx,idy,0)]=2*str_exchange[rcl::utriad(nx,idy,0)]*(mag[rcl::utriad(nx,idy+1,0)]-mag[rcl::utriad(nx-1,idy,0)]-mag[rcl::utriad(nx,idy-1,0)]);
  for (unsigned int idx=1;idx<nx;idx++) h_ex[rcl::utriad(idx, 0,0)]=2*str_exchange[rcl::utriad(idx, 0,0)]*(mag[rcl::utriad(idx+1,0,0)] +mag[rcl::utriad(idx,1,0)]-mag[rcl::utriad(idx-1,0,0)] );
  for (unsigned int idx=1;idx<nx;idx++) h_ex[rcl::utriad(idx,ny,0)]=2*str_exchange[rcl::utriad(idx,ny,0)]*(mag[rcl::utriad(idx+1,ny,0)]-mag[rcl::utriad(idx-1,ny,0)]-mag[rcl::utriad(idx,ny-1,0)]);
  
  h_ex[rcl::utriad( 0, 0, 0)]=2*str_exchange[rcl::utriad( 0, 0, 0)]*(mag[rcl::utriad( 1, 0,0)]+mag[rcl::utriad(0,1,0)]);
  h_ex[rcl::utriad(nx, 0, 0)]=2*str_exchange[rcl::utriad(nx, 0, 0)]*(mag[rcl::utriad(nx, 1,0)]-mag[rcl::utriad(nx-1,0,0)]);
  h_ex[rcl::utriad( 0,ny, 0)]=2*str_exchange[rcl::utriad( 0,ny, 0)]*(mag[rcl::utriad( 1,ny,0)]-mag[rcl::utriad(0,ny-1,0)]);
  h_ex[rcl::utriad(nx,ny, 0)]=2*str_exchange[rcl::utriad(nx,ny, 0)]*(-mag[rcl::utriad(nx-1,ny,0)]-mag[rcl::utriad(nx,ny-1,0)]);
  
  return;
}
template <typename T> void umfproblem<T>::calculate_pma(){
  unsigned int nx = nodes.x()-1;
  unsigned int ny = nodes.y()-1;
  
  //!\ optimizar usando indice lineal
  
  rcl::tvector<T> uz(0,0,1);
  for (unsigned int idy=0;idy<=ny;idy++) for (unsigned int idx=0;idx<=nx;idx++)
    h_pma[rcl::utriad(idx,idy,0)]=(uz*mag[rcl::utriad(idx,idy,0)])*uz*str_mag_sat[rcl::utriad(idx,idy,0)]+
    (dir_anisotropy[rcl::utriad(idx,idy,0)]*mag[rcl::utriad(idx,idy,0)])*dir_anisotropy[rcl::utriad(idx,idy,0)]*str_anisotropy[rcl::utriad(idx,idy,0)];
  return;
  }
  
template <typename T> void umfproblem<T>::calculate_dmi(){
  unsigned int nx = nodes.x()-1;
  unsigned int ny = nodes.y()-1;
  unsigned int nz = nodes.z()-1;
  
  //!\ optimizar usando indice lineal y sobreescribir al final los datos malos :-)
  
  for (unsigned int idy=1;idy<ny;idy++)  for (unsigned int idx=1;idx<nx;idx++)
    h_dmi[rcl::utriad(idx,idy,0)]=str_dmi[rcl::utriad(idx,idy,0)]*
    rcl::tvector<T>(mag[rcl::utriad(idx+1,idy,0)].z()-mag[rcl::utriad(idx-1,idy,0)].z(),
                    mag[rcl::utriad(idx,idy+1,0)].z()-mag[rcl::utriad(idx,idy-1,0)].z(),);
  return;  
  }

}
#endif
