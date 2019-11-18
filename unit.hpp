#include <string>
#include <iostream>
#include <iomanip> 
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

/* Α α, Β β, Γ γ, Δ δ, Ε ε, Ζ ζ, Η η, Θ θ, Ι ι, Κ κ, Λ λ, Μ μ, Ν ν, Ξ ξ, Ο ο, Π π, Ρ ρ, Σ σ/ς, Τ τ, Υ υ, Φ φ, Χ χ, Ψ ψ, Ω ω */

namespace rcl{

class unit{
private:
  short int l; // longitud
  short int t; // tiempo 
  short int m; // masa
  short int q; // temperatura
  short int c; // carga
  short int f; // luminosidad
public:
  unit():l(0),t(0),m(0),q(0),c(0),f(0){return;}
  unit(const short int& l ,const short int& t,const short int& m,const short int& q,const short int& c,const short int& f){
    this->l=l;
    this->t=t;
    this->m=m;     
    this->q=q;     
    this->c=c;     
    this->f=f;
    return;     
    }
  unit& operator*=(const unit& rhs){
    this->l+=rhs.l;
    this->t+=rhs.t;
    this->m+=rhs.m;
    this->q+=rhs.q;
    this->c+=rhs.c;
    this->f+=rhs.f;
    return *this;
    }
  unit& operator/=(const unit& rhs){
    this->l-=rhs.l;
    this->t-=rhs.t;
    this->m-=rhs.m;
    this->q-=rhs.q;
    this->c-=rhs.c;
    this->f-=rhs.f;
    return *this;
    }
  const short int& operator[](const unsigned int& idx)const{return reinterpret_cast<const short int*>(this)[idx];}
  const short int& GetL()const{ return l;}
  const short int& GetT()const{ return t;}
  const short int& GetM()const{ return m;}
  const short int& GetQ()const{ return q;}
  const short int& GetC()const{ return c;}
  const short int& GetF()const{ return f;}
  
  
  //bool operator<(const unit& rhs) const{
  //  if (rhs.l<l) return true;
  //  if (rhs.t<t) return true;
  //  if (rhs.m<m) return true;
  //  if (rhs.q<q) return true;
  //  if (rhs.c<c) return true;
  //  if (rhs.f<f) return true;
  //  return false;
  //}
  
};

inline bool operator<(const unit& lhs,const unit& rhs){
  if (lhs.GetL()<rhs.GetL()) return true;
  if (rhs.GetL()<lhs.GetL()) return false;
  if (lhs.GetT()<rhs.GetT()) return true;
  if (rhs.GetT()<lhs.GetT()) return false;
  if (lhs.GetM()<rhs.GetM()) return true;
  if (rhs.GetM()<lhs.GetM()) return false;
  if (lhs.GetQ()<rhs.GetQ()) return true;
  if (rhs.GetQ()<lhs.GetQ()) return false;
  if (lhs.GetC()<rhs.GetC()) return true;
  if (rhs.GetC()<lhs.GetC()) return false;
  if (lhs.GetF()<rhs.GetF()) return true;
  return false; 
}

inline unit operator*(unit lhs,const unit& rhs){return lhs*=rhs;}
inline unit operator/(unit lhs,const unit& rhs){return lhs/=rhs;}
inline std::ostream& operator<< (std::ostream& os,const unit& u){
  os << "(" << std::setw(2) << u[0] ; 
  os << "," << std::setw(2) << u[1] ; 
  os << "," << std::setw(2) << u[2] ; 
  os << "," << std::setw(2) << u[3] ; 
  os << "," << std::setw(2) << u[4] ; 
  os << "," << std::setw(2) << u[5] << ")"; 
  return os;
}

namespace units{
const unit& adim = unit();
const unit& lenght = unit(1,0,0,0,0,0);
const unit& mass = unit(0,0,1,0,0,0);
const unit& time = unit(0,0,1,0,0,0);
const unit& charge = unit(0,0,0,0,1,0);

const unit& speed = lenght/time;
const unit& acceleration = speed/time;
const unit& momentum = mass*speed;
const unit& force = mass*acceleration;
const unit& energy = force*lenght;
const unit& power = force*speed;
const unit& surface = lenght*lenght;
const unit& presure = force/surface;
const unit& volume = surface*lenght;
const unit& density = mass/volume;

const unit& charge_density = charge/volume;
const unit& current_intensity = charge/time;
const unit& current = current_intensity / surface;
const unit& displacement_field = charge_density*lenght;
const unit& electric_field = force/charge;
const unit& electric_potential = energy/charge;
const unit& electric_capacity = charge/electric_potential;

const unit& magnetic_field = current*lenght;
const unit& magnetic_induction = force/speed/charge;
const unit& magnetic_potential = magnetic_induction*lenght;

const unit& vacuum_permitivity = displacement_field/electric_field;
const unit& vacuum_permeability = magnetic_induction/magnetic_field;
const unit& gravity_constant = acceleration*surface/mass;

const unit& wave_number = adim/lenght;
const unit& frequency = adim/time;

};

}
