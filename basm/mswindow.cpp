void draw(buffer& b,int x,int y,int w,int h,palette& p)
{
  
  b.box(x,y,x+w,y+h,p[0]);
  x++;y++;h--;w--;
  b.hline(x,x+w,y,p[7]);
  b.vline(x,y,y+h,p[7]);
  
  x++;y++;h--;w--;
  b.hline(x,x+w,y,p[7]);
  b.vline(x,y,y+h,p[7]);
  
  x++;y++;h--;w--;
  b.hline(x,x+w,y,p[0]);
  b.vline(x,y,y+h,p[0]);
                                 
  b.fill(x,y,x+18,y+18,p[7]);
  b.box(x,y,x+18,y+18,p[0]);
  
  b.vline(x+18,y-2,y+5,p[0]);
  b.hline(x-2,x+5,y+18,p[0]);
  b.fill (x + 5, y + 7,x + 13, y + 9, 15, BF
  b.box (x + 5, y + 7,x + 13, y + 9, 0, B
  b.vline (x + 14, y + 7,x + 14, y + 9, 8
  b.hline (x + 6, y + 10,x + 14, y + 10, 8, B
  b.fill(x + 18, y,x + w, y + 18, 9, BF
  b.box (x + 18, y,x + w, y + 18, 0, B
  b.fill (x + w, y - 3,x + w + 2, y + h, 7, BF
  b.box (x + w, y - 3,x + w + 3, y + h, 0, B
  b.vline(x + w, y - 2,x + w, y - 1, 7
  b.vline(x + w - 18, y - 2,x + w - 18, y - 1, 0
  b.hline(x + w, y + 18,x + w + 2, y + 18, 0
  b.box (x, y + h - 3,x + w, y + h - 3, 0, B
  b.fill (x, y + h - 2,x + w + 2, y + h, 7, BF
  b.box (x - 2, y + h,x + w + 2, y + h, 0, B
  b.hline (x - 2, y + h - 18,x, y + h - 18, 0
  b. (x + w - 2, y + h - 18,x + w + 2, y + h - 18, 0
  b. (x + 18, y + h,x + 18, y + h - 2, 0
  b. (x + w - 18, y + h,x + w - 18, y + h - 2, 0
  b. (x + 1, y + 19,x + w - 1, y + h - 4, 7, BF
}
