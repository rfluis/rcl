#include "graphics.hpp"

namespace rcl{

image32 image32::crop (const upoint& src,const upoint& csize) const {
    image32 ans(csize);
    for (unsigned int idy=0;idy<csize.y();idy++) 
      for (unsigned int idx=0;idx<csize.x();idx++) 
        ans(idx,idy) = (*this)(src.x()+idx,src.y()+idy);
    return ans;
  }
void image32::put(const image32& sprite,const upoint& pos){
    for (unsigned int idy=0;idy<sprite.GetY();idy++)
      for (unsigned int idx=0;idx<sprite.GetX();idx++)
        (*this)(idx+pos.x(),idy+pos.y()) = sprite(idx,idy);
    return;
    }
void image32::blend(const image32& sprite,const upoint& pos){
    for (unsigned int idy=0;idy<sprite.GetY();idy++)
      for (unsigned int idx=0;idx<sprite.GetX();idx++)
        (*this)(idx+pos.x(),idy+pos.y()).blend(sprite(idx,idy));
    return;
    }

image32 image32::flip() const {
  image32 ans(xx,yy);
  for (unsigned int idy=0;idy<yy;idy++)
  for (unsigned int idx=0;idx<xx;idx++)
  ans(idx,idy)=pget(idx,yy-idy-1);
  return ans;
  }
image32 image32::mirror() const {
  image32 ans(xx,yy);
  for (unsigned int idy=0;idy<yy;idy++)
  for (unsigned int idx=0;idx<xx;idx++)
  ans(idx,idy)=pget(xx-idx-1,idy);
  return ans;
  }
  
image32 image32::resize_nn(const unsigned int& x,const unsigned int& y) const{
  image32 ans(x,y);
  for (unsigned int idy=0;idy<y;idy++){
    unsigned int py;
    py = idy * (yy-1) / y;
    for (unsigned int idx=0;idx<x;idx++){
      unsigned int px;
      px = idx * (xx-1) / x;
      ans(idx,idy) = pget(px,py);
    }
  }
  return ans;
}

void image32::hline(const unsigned int& x1,const unsigned int& x2, const unsigned int& y,const rcl::pixel32& c){
  for (unsigned int x=x1;x<=x2;x++) pset(x,y,c);  return;  }
void image32::vline(const unsigned int& x,const unsigned int& y1, const unsigned int& y2,const rcl::pixel32& c){
  for (unsigned int y=y1;y<=y2;y++) pset(x,y,c);  return;  }
  
void image32::line(const unsigned int& x1,const unsigned int& y1,const unsigned int& x2,const unsigned int& y2, const rcl::pixel32& color){
  if(reinterpret_cast<const int&>(x1) < 0 || reinterpret_cast<const int&>(x1) > xx - 1 || reinterpret_cast<const int&>(x2) < 0 || reinterpret_cast<const int&>(x2) > xx - 1 || reinterpret_cast<const int&>(y1) < 0 || reinterpret_cast<const int&>(y1) > yy - 1 || reinterpret_cast<const int&>(y2) < 0 || reinterpret_cast<const int&>(y2) > yy - 1) return;

  int deltax = rcl::abs(reinterpret_cast<const int&>(x2) - reinterpret_cast<const int&>(x1)); 
  int deltay = rcl::abs(reinterpret_cast<const int&>(y2) - reinterpret_cast<const int&>(y1)); 
  int x = reinterpret_cast<const int&>(x1); 
  int y = reinterpret_cast<const int&>(y1); 
  int xinc1, xinc2, yinc1, yinc2, den, num, numadd, numpixels, curpixel;

  if(reinterpret_cast<const int&>(x2) >= reinterpret_cast<const int&>(x1))  {    xinc1 = 1;    xinc2 = 1;  }
  else          {    xinc1 = -1;    xinc2 = -1;  }
  if(reinterpret_cast<const int&>(y2) >= reinterpret_cast<const int&>(y1))  {    yinc1 = 1;    yinc2 = 1;  }
  else          {    yinc1 = -1;    yinc2 = -1;  }
  if (deltax >= deltay)   {    xinc1 = 0;     yinc2 = 0;     den = deltax;    num = deltax / 2;    numadd = deltay;    numpixels = deltax;   }
  else                    {    xinc2 = 0;     yinc1 = 0;     den = deltay;    num = deltay / 2;    numadd = deltax;    numpixels = deltay;   }
  for (curpixel = 0; curpixel <= numpixels; curpixel++)  {
    pset(x,y,color);  
    num += numadd;  
    if (num >= den) {      num -= den;       x += xinc1;       y += yinc1;     }
    x += xinc2; 
    y += yinc2; 
  }
  return;
}
void image32::circle(const unsigned int& xc,const unsigned int& yc,const unsigned int& radius,const rcl::pixel32& color){
  if(reinterpret_cast<const int&>(xc) - reinterpret_cast<const int&>(radius) < 0 || reinterpret_cast<const int&>(xc) + reinterpret_cast<const int&>(radius) >= xx || reinterpret_cast<const int&>(yc) - reinterpret_cast<const int&>(radius) < 0 || reinterpret_cast<const int&>(yc) + reinterpret_cast<const int&>(radius) >= yy) return;
  int x = 0;
  int y = reinterpret_cast<const int&>(radius);
  int p = 3 - (reinterpret_cast<const int&>(radius) << 1);
  int a, b, c, d, e, f, g, h;
  while (x <= y){
     a = reinterpret_cast<const int&>(xc) + x; //8 pixels can be calculated at once thanks to the symmetry
     b = reinterpret_cast<const int&>(yc) + y;
     c = reinterpret_cast<const int&>(xc) - x;
     d = reinterpret_cast<const int&>(yc) - y;
     e = reinterpret_cast<const int&>(xc) + y;
     f = reinterpret_cast<const int&>(yc) + x;
     g = reinterpret_cast<const int&>(xc) - y;
     h = reinterpret_cast<const int&>(yc) - x;
     pset(a, b, color);
     pset(c, d, color);
     pset(e, f, color);
     pset(g, f, color);
     if(x > 0) {
       pset(a, d, color);
       pset(c, b, color);
       pset(e, h, color);
       pset(g, h, color); }
     if(p < 0) p += (x++ << 2) + 6;
     else p += ((x++ - y--) << 2) + 10;
  }
  return ;
}
void image32::disk(const unsigned int& xc,const unsigned int& yc,const unsigned int& radius,const rcl::pixel32& color){
  if(reinterpret_cast<const int&>(xc) + reinterpret_cast<const int&>(radius) < 0 || reinterpret_cast<const int&>(xc) - reinterpret_cast<const int&>(radius) >= xx || reinterpret_cast<const int&>(yc) + reinterpret_cast<const int&>(radius) < 0 || reinterpret_cast<const int&>(yc) - reinterpret_cast<const int&>(radius) >= yy) return; //every single pixel outside screen, so don't waste time on it
  int x = 0;
  int y = reinterpret_cast<const int&>(radius);
  int p = 3 - (reinterpret_cast<const int&>(radius) << 1);
  int a, b, c, d, e, f, g, h;
  int pb = reinterpret_cast<const int&>(yc) + reinterpret_cast<const int&>(radius) + 1, pd = reinterpret_cast<const int&>(yc) + reinterpret_cast<const int&>(radius) + 1; 
  while (x <= y){
     a = reinterpret_cast<const int&>(xc) + x;
     b = reinterpret_cast<const int&>(yc) + y;
     c = reinterpret_cast<const int&>(xc) - x;
     d = reinterpret_cast<const int&>(yc) - y;
     e = reinterpret_cast<const int&>(xc) + y;
     f = reinterpret_cast<const int&>(yc) + x;
     g = reinterpret_cast<const int&>(xc) - y;
     h = reinterpret_cast<const int&>(yc) - x;
     if(b != pb) hline(b, a, c, color);
     if(d != pd) hline(d, a, c, color);
     if(f != b)  hline(f, e, g, color);
     if(h != d && h != f) hline(h, e, g, color);
     pb = b;
     pd = d;
     if(p < 0) p += (x++ << 2) + 6;
     else p += ((x++ - y--) << 2) + 10;
  }
  return ;
}

void image32::rect(const unsigned int& x1,const unsigned int& y1,const unsigned int& x2,const unsigned int& y2,const rcl::pixel32& color){
  for (unsigned int y=y1;y<=y2;y++) 
  for (unsigned int x=x1;x<=x2;x++) 
  pset(x,y,color);
  return;
}



ftriad RGBtoHSL(const pixel32& color){
  float r, g, b, h = 0, s = 0, l; //this function works with floats between 0 and 1
  r = color.r() / 255.0;
  g = color.g() / 255.0;
  b = color.b() / 255.0;

  float maxColor = std::max(r, std::max(g, b));
  float minColor = std::min(r, std::min(g, b));

  if(minColor == maxColor) { l = r;}
  else {
    l = (minColor + maxColor) / 2;
    if(l < 0.5) s = (maxColor - minColor) / (maxColor + minColor);
    if(l >= 0.5) s = (maxColor - minColor) / (2.0 - maxColor - minColor);
    if(r == maxColor) h = (g - b) / (maxColor - minColor);
    if(g == maxColor) h = 2.0 + (b - r) / (maxColor - minColor);
    if(b == maxColor) h = 4.0 + (r - g) / (maxColor - minColor);
    h /= 6; 
    if(h < 0) h += 1;
  }
  return ftriad(h,s,l);
}
pixel32 HSLtoRGB(const ftriad& color){
  float ur,ug,ub, t1, t2, tr, tg, tb;
  if(color.z() < 0.5f) t2 = color.z() * (1.f + color.y());    else t2 = (color.z() + color.y()) - (color.z() * color.y());
  t1 = 2.f * color.z() - t2;    tr=color.x() + 1.0f / 3.0f;
  if(tr > 1.0f) tr--;
  tg=color.x();    tb=color.x()-1.0f / 3.0f;
  if(tb < 0.0f) tb++;
  if(tr < 1.0f / 6.0f) ur = t1 + (t2 - t1) * 6.0f * tr;
  else if(tr < 0.5f) ur = t2;
  else if(tr < 2.0f / 3.0f) ur = t1 + (t2 - t1) * ((2.0f / 3.0f) - tr) * 6.0f;
  else ur = t1;
  if(tg < 1.0 / 6.0) ug = t1 + (t2 - t1) * 6.0 * tg;
  else if(tg < 0.5) ug=t2;
  else if(tg < 2.0 / 3.0) ug = t1 + (t2 - t1) * ((2.0 / 3.0) - tg) * 6.0;
  else ug = t1;
  if(tb < 1.0 / 6.0) ub = t1 + (t2 - t1) * 6.0 * tb;
  else if(tb < 0.5) ub = t2;
  else if(tb < 2.0 / 3.0) ub = t1 + (t2 - t1) * ((2.0 / 3.0) - tb) * 6.0;
  else ub = t1; 
  return pixel32 ((unsigned char) (255.f*ur),(unsigned char) (255.f*ug),(unsigned char) (255.f*ub));
}
pixel32 France(const float& x){
  if (x<-1.f||x>1.f) return pixel32(0,255,0); 
  float k;
  if (x<0.f) k=1.f+x; else k=1.f-x;
  if (x<0.f) return pixel32((unsigned char) (k*255.),(unsigned char) (k*255.),255); // azul
  else       return pixel32(255,(unsigned char) (k*255.),(unsigned char) (k*255.)); // rojo
}
pixel32 GrayScale(const float& x){return pixel32((unsigned char) ((x+1.f)*127.5f),(unsigned char) ((x+1.f)*127.5f),(unsigned char) ((x+1.f)*127.5f));}
pixel32 UnitGrayScale(const float& x){return pixel32((unsigned char) (x*254.5f),(unsigned char)  (x*254.5f),(unsigned char)  ((x)*254.5f) );}

/*
//Converts an RGB color to HSV color
ColorHSV RGBtoHSV(const ColorRGB& colorRGB){
  float r, g, b, h = 0.0, s = 0.0, v; //this function works with floats between 0 and 1
  r = colorRGB.r / 256.0;
  g = colorRGB.g / 256.0;
  b = colorRGB.b / 256.0;

  float maxColor = std::max(r, std::max(g, b));
  float minColor = std::min(r, std::min(g, b));

  v = maxColor;

  if(maxColor != 0.0) //avoid division by zero when the color is black
  {
    s = (maxColor - minColor) / maxColor;
  }

  if(s == 0.0)
  {
    h = 0.0; //it doesn't matter what value it has
  }
  else
  {
    if(r == maxColor) h = (g - b) / (maxColor - minColor);
    if(g == maxColor) h = 2.0 + (b - r) / (maxColor - minColor);
    if(b == maxColor) h = 4.0 + (r - g) / (maxColor - minColor);

    h /= 6.0; //to bring it to a number between 0 and 1
    if(h < 0.0) h++;
  }

  ColorHSV colorHSV;
  colorHSV.h = int(h * 255.0);
  colorHSV.s = int(s * 255.0);
  colorHSV.v = int(v * 255.0);
  return colorHSV;
}

//Converts an HSV color to RGB color
ColorRGB HSVtoRGB(const ColorHSV& colorHSV){
  float r, g, b, h, s, v; //this function works with floats between 0 and 1
  h = colorHSV.h / 256.0;
  s = colorHSV.s / 256.0;
  v = colorHSV.v / 256.0;

  //if saturation is 0, the color is a shade of grey
  if(s == 0.0) r = g = b = v;
  //if saturation > 0, more complex calculations are needed
  else
  {
    float f, p, q, t;
    int i;
    h *= 6.0; //to bring hue to a number between 0 and 6, better for the calculations
    i = int(floor(h)); //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
    f = h - i;//the fractional part of h

    p = v * (1.0 - s);
    q = v * (1.0 - (s * f));
    t = v * (1.0 - (s * (1.0 - f)));

    switch(i)
    {
      case 0: r=v; g=t; b=p; break;
      case 1: r=q; g=v; b=p; break;
      case 2: r=p; g=v; b=t; break;
      case 3: r=p; g=q; b=v; break;
      case 4: r=t; g=p; b=v; break;
      case 5: r=v; g=p; b=q; break;
      default: r = g = b = 0; break;
    }
  }
  ColorRGB colorRGB;
  colorRGB.r = int(r * 255.0);
  colorRGB.g = int(g * 255.0);
  colorRGB.b = int(b * 255.0);
  return colorRGB;
}
*/

}//end namespace rcl

  /*
bool 

//Bresenham circle with center at (xc,yc) with radius and red green blue color
bool drawCircle(int xc, int yc, int radius, const ColorRGB& color){
  if(xc - radius < 0 || xc + radius >= w || yc - radius < 0 || yc + radius >= h) return 0;
  int x = 0;
  int y = radius;
  int p = 3 - (radius << 1);
  int a, b, c, d, e, f, g, h;
  while (x <= y)
  {
     a = xc + x; //8 pixels can be calculated at once thanks to the symmetry
     b = yc + y;
     c = xc - x;
     d = yc - y;
     e = xc + y;
     f = yc + x;
     g = xc - y;
     h = yc - x;
     pset(a, b, color);
     pset(c, d, color);
     pset(e, f, color);
     pset(g, f, color);
     if(x > 0) //avoid drawing pixels at same position as the other ones
     {
       pset(a, d, color);
       pset(c, b, color);
       pset(e, h, color);
       pset(g, h, color);
     }
     if(p < 0) p += (x++ << 2) + 6;
     else p += ((x++ - y--) << 2) + 10;
  }

  return 1;
}


//Filled bresenham circle with center at (xc,yc) with radius and red green blue color
bool drawDisk(int xc, int yc, int radius, const ColorRGB& color){
  if(xc + radius < 0 || xc - radius >= w || yc + radius < 0 || yc - radius >= h) return 0; //every single pixel outside screen, so don't waste time on it
  int x = 0;
  int y = radius;
  int p = 3 - (radius << 1);
  int a, b, c, d, e, f, g, h;
  int pb = yc + radius + 1, pd = yc + radius + 1; //previous values: to avoid drawing horizontal lines multiple times  (ensure initial value is outside the range)
  while (x <= y)
  {
     // write data
     a = xc + x;
     b = yc + y;
     c = xc - x;
     d = yc - y;
     e = xc + y;
     f = yc + x;
     g = xc - y;
     h = yc - x;
     if(b != pb) horLine(b, a, c, color);
     if(d != pd) horLine(d, a, c, color);
     if(f != b)  horLine(f, e, g, color);
     if(h != d && h != f) horLine(h, e, g, color);
     pb = b;
     pd = d;
     if(p < 0) p += (x++ << 2) + 6;
     else p += ((x++ - y--) << 2) + 10;
  }

  return 1;
}

//Rectangle with corners (x1,y1) and (x2,y2) and rgb color
bool drawRect(int x1, int y1, int x2, int y2, const ColorRGB& color){
  if(x1 < 0 || x1 > w - 1 || x2 < 0 || x2 > w - 1 || y1 < 0 || y1 > h - 1 || y2 < 0 || y2 > h - 1) return 0;
  SDL_Rect rec;
  rec.x = x1;
  rec.y = y1;
  rec.w = x2 - x1 + 1;
  rec.h = y2 - y1 + 1;
  Uint32 colorSDL = SDL_MapRGB(scr->format, color.r, color.g, color.b);
  SDL_FillRect(scr, &rec, colorSDL);  //SDL's ability to draw a hardware rectangle is used for now
  return 1;
}


ColorHSL RGBtoHSL(const ColorRGB& colorRGB){
  float r, g, b, h = 0, s = 0, l; //this function works with floats between 0 and 1
  r = colorRGB.r / 256.0;
  g = colorRGB.g / 256.0;
  b = colorRGB.b / 256.0;

  float maxColor = std::max(r, std::max(g, b));
  float minColor = std::min(r, std::min(g, b));

  if(minColor == maxColor) //R = G = B, so it's a shade of grey
  {
    h = 0; //it doesn't matter what value it has
    s = 0;
    l = r; //doesn't matter if you pick r, g, or b
  }
  else
  {
    l = (minColor + maxColor) / 2;

    if(l < 0.5) s = (maxColor - minColor) / (maxColor + minColor);
    if(l >= 0.5) s = (maxColor - minColor) / (2.0 - maxColor - minColor);

    if(r == maxColor) h = (g - b) / (maxColor - minColor);
    if(g == maxColor) h = 2.0 + (b - r) / (maxColor - minColor);
    if(b == maxColor) h = 4.0 + (r - g) / (maxColor - minColor);

    h /= 6; //to bring it to a number between 0 and 1
    if(h < 0) h += 1;
  }

  ColorHSL colorHSL;
  colorHSL.h = int(h * 255.0);
  colorHSL.s = int(s * 255.0);
  colorHSL.l = int(l * 255.0);
  return colorHSL;
}

//Converts an HSL color to RGB color
ColorRGB HSLtoRGB(const ColorHSL& colorHSL){
  float r, g, b, h, s, l; //this function works with floats between 0 and 1
  float temp1, temp2, tempr, tempg, tempb;
  h = colorHSL.h / 256.0;
  s = colorHSL.s / 256.0;
  l = colorHSL.l / 256.0;

  //If saturation is 0, the color is a shade of grey
  if(s == 0) r = g = b = l;
  //If saturation > 0, more complex calculations are needed
  else
  {
    //set the temporary values
    if(l < 0.5) temp2 = l * (1 + s);
    else temp2 = (l + s) - (l * s);
    temp1 = 2 * l - temp2;
    tempr=h + 1.0 / 3.0;
    if(tempr > 1.0) tempr--;
    tempg=h;
    tempb=h-1.0 / 3.0;
    if(tempb < 0.0) tempb++;

    //red
    if(tempr < 1.0 / 6.0) r = temp1 + (temp2 - temp1) * 6.0 * tempr;
    else if(tempr < 0.5) r = temp2;
    else if(tempr < 2.0 / 3.0) r = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempr) * 6.0;
    else r = temp1;

     //green
    if(tempg < 1.0 / 6.0) g = temp1 + (temp2 - temp1) * 6.0 * tempg;
    else if(tempg < 0.5) g=temp2;
    else if(tempg < 2.0 / 3.0) g = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempg) * 6.0;
    else g = temp1;

    //blue
    if(tempb < 1.0 / 6.0) b = temp1 + (temp2 - temp1) * 6.0 * tempb;
    else if(tempb < 0.5) b = temp2;
    else if(tempb < 2.0 / 3.0) b = temp1 + (temp2 - temp1) * ((2.0 / 3.0) - tempb) * 6.0;
    else b = temp1;
  }

  ColorRGB colorRGB;
  colorRGB.r = int(r * 255.0);
  colorRGB.g = int(g * 255.0);
  colorRGB.b = int(b * 255.0);
  return colorRGB;
}

//Converts an RGB color to HSV color
ColorHSV RGBtoHSV(const ColorRGB& colorRGB){
  float r, g, b, h = 0.0, s = 0.0, v; //this function works with floats between 0 and 1
  r = colorRGB.r / 256.0;
  g = colorRGB.g / 256.0;
  b = colorRGB.b / 256.0;

  float maxColor = std::max(r, std::max(g, b));
  float minColor = std::min(r, std::min(g, b));

  v = maxColor;

  if(maxColor != 0.0) //avoid division by zero when the color is black
  {
    s = (maxColor - minColor) / maxColor;
  }

  if(s == 0.0)
  {
    h = 0.0; //it doesn't matter what value it has
  }
  else
  {
    if(r == maxColor) h = (g - b) / (maxColor - minColor);
    if(g == maxColor) h = 2.0 + (b - r) / (maxColor - minColor);
    if(b == maxColor) h = 4.0 + (r - g) / (maxColor - minColor);

    h /= 6.0; //to bring it to a number between 0 and 1
    if(h < 0.0) h++;
  }

  ColorHSV colorHSV;
  colorHSV.h = int(h * 255.0);
  colorHSV.s = int(s * 255.0);
  colorHSV.v = int(v * 255.0);
  return colorHSV;
}

//Converts an HSV color to RGB color
ColorRGB HSVtoRGB(const ColorHSV& colorHSV){
  float r, g, b, h, s, v; //this function works with floats between 0 and 1
  h = colorHSV.h / 256.0;
  s = colorHSV.s / 256.0;
  v = colorHSV.v / 256.0;

  //if saturation is 0, the color is a shade of grey
  if(s == 0.0) r = g = b = v;
  //if saturation > 0, more complex calculations are needed
  else
  {
    float f, p, q, t;
    int i;
    h *= 6.0; //to bring hue to a number between 0 and 6, better for the calculations
    i = int(floor(h)); //e.g. 2.7 becomes 2 and 3.01 becomes 3 or 4.9999 becomes 4
    f = h - i;//the fractional part of h

    p = v * (1.0 - s);
    q = v * (1.0 - (s * f));
    t = v * (1.0 - (s * (1.0 - f)));

    switch(i)
    {
      case 0: r=v; g=t; b=p; break;
      case 1: r=q; g=v; b=p; break;
      case 2: r=p; g=v; b=t; break;
      case 3: r=p; g=q; b=v; break;
      case 4: r=t; g=p; b=v; break;
      case 5: r=v; g=p; b=q; break;
      default: r = g = b = 0; break;
    }
  }
  ColorRGB colorRGB;
  colorRGB.r = int(r * 255.0);
  colorRGB.g = int(g * 255.0);
  colorRGB.b = int(b * 255.0);
  return colorRGB;
}

}
*/
