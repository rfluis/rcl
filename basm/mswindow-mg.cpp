
class buffer{
public:
  void box(const unsigned int& x,const unsigned int& xf,const unsigned int& y,const unsigned int& yf,const unsigned int& color);
  void fill(const unsigned int& x,const unsigned int& xf,const unsigned int& y,const unsigned int& yf,const unsigned int& color);
  void vline(const unsigned int& x,const unsigned int& y,const unsigned int& yf,const unsigned int& color);
  void hline(const unsigned int& x,const unsigned int& xf,const unsigned int& y,const unsigned int& color);
  
};
typedef std::vector<unsigned int> palette;

void draw(buffer& b,int x,int y,int w,int h,palette& p){
  b.box(x,x+w,y,y+h,p[0]);
  b.hline(x+1,x+w,y+1,p[7]);
  b.vline(x+1,y+1,y+h,p[7]);
  b.hline(x+2,x+w,y+2,p[7]);
  b.vline(x+2,y+2,y+h,p[7]);
  b.hline(x+3,x+w,y+3,p[0]);
  b.vline(x+3,y+3,y+h,p[0]);
  b.fill(x+3,x+21,y+3,y+21,p[7]);
  b.box(x+3,x+21,y+3,y+21,p[0]);
  b.vline(x+21,y+1,y+8,p[0]);
  b.hline(x+1,x+8,y+21,p[0]);
  b.fill(x+8,x+16,y+10,y+12,p[15]);
  b.box(x+8,x+16,y+10,y+12,p[0]);
  b.vline(x+17,y+10,y+12,p[8]);
  b.hline(x+9,x+17,y+13,p[8]);
  b.fill(x+21,x+w,y+3,y+21,p[9]);
  b.box(x+21,x+w,y+3,y+21,p[0]);
  b.fill(x+w,x+w+2,y,y+h,p[7]);
  b.box(x+w,x+w+3,y,y+h,p[0]);
  b.vline(x+w,y+1,y+2,p[7]);
  b.vline(x+w-18,y+1,y+2,p[0]);
  b.hline(x+w,x+w+2,y+21,p[0]);
  b.hline(x+3,x+w,y+h-3,p[0]);
  b.fill(x+3,x+w+2,y+h-2,y+h,p[7]);
  b.hline(x+1,x+w+2,y+h,p[0]);
  b.hline(x+1,x+3,y+h-18,p[0]);
  b.hline(x+w-2,x+w+2,y+h-18,p[0]);
  b.vline(x+21,y+h,y+h-2,p[0]);
  b.vline(x+w-18,y+h,y+h-2,p[0]);
  b.fill(x+4,x+w-1,y+22,y+h-4,p[7]);
  return;
 }
