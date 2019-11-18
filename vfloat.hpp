#ifndef VFLOAT_HEADER
#define VFLOAT_HEADER

namespace rcl{
  
class vfloat{
private:
  unsigned int mant:23;
  unsigned int exp:8;
  unsigned int sign:1;
  class mreference{
  public:
    mreference(vfloat& orig):master(orig){return;}
  private:
    vfloat& master;
  public:
    mreference& operator =(const unsigned int& rhs){ master.mant=rhs; return *this;} 
    mreference& operator =(const vfloat& rhs){master.mant = rhs.mant; return *this;}
    mreference& operator =(const vfloat::mreference& rhs) {master.mant = rhs.master.mant; return *this;}
    operator unsigned int()const{ return master.mant;}   
    };
  class ereference{
  public:
    ereference(vfloat& orig):master(orig){return;}
  private:
    vfloat& master;
  public:
    ereference& operator =(const unsigned int& rhs) { master.exp=rhs; return *this;}    
    ereference& operator =(const vfloat& rhs){master.exp = rhs.exp; return *this;}  
    ereference& operator =(const vfloat::ereference& rhs) {master.exp = rhs.master.exp;  return *this;}
    operator unsigned int()const{ return master.exp;}   
    };
  class sreference{
  public:
    sreference(vfloat& orig):master(orig){return;}
  private:
    vfloat& master;
  public:
    sreference& operator =(const unsigned int& rhs) { master.sign=rhs; return *this;}    
    sreference& operator =(const vfloat& rhs){master.sign = rhs.sign; return *this;}   
    sreference& operator =(const vfloat::sreference& rhs) {master.sign = rhs.master.sign; return *this;}
    operator unsigned int()const{ return master.sign;}
    };
public:
  vfloat(){return;}
  vfloat(const float& orig){ reinterpret_cast<float&>(*this) = orig; return;}
  mreference m(){ return mreference(*this);}
  ereference e(){ return ereference(*this);}
  sreference s(){ return sreference(*this);}
  unsigned int m()const { return mant;}
  unsigned int e()const { return exp;}
  unsigned int s()const { return sign;}
  operator float&(){ return reinterpret_cast<float&>(*this);}
  operator const float&()const{ return reinterpret_cast<float const&>(*this);}
};

}

#endif
