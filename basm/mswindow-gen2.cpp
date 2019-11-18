#include <iostream>
#include <string>
#include <sstream>
/*
row = 10: height = 100
col = 10: length = 200
*/

using namespace std;
struct var{
 string name;
 int offset;
 var(string s){name = s; offset=0;}
 var(string s,int x){name = s;offset=x;}
 var(int x){ name=""; offset = x;}
 //var operator=(var x){
 //cout << this->name << " = " << x.name << endl;
 //return *this;
 //}
 operator string()const {
   stringstream ss;
   if (offset==0) return name;
   if (offset<0) ss << name << "-" << -offset;
   else          ss << name << "+" << offset;
   return ss.str();
   }
};

bool operator==(var x,var y){if ((x.name==y.name)&&(x.offset==y.offset)) return true; return false;}



var operator+(var x,var y){
 stringstream ss;
 if ((x.name!="")&&(y.name!="")) ss << x.name << "+" << y.name;
 if (x.name=="") ss << y.name;
 if (y.name=="") ss << x.name;
 return var(ss.str(),x.offset+y.offset);
}


var operator-(var x,var y){
 stringstream ss;
 if ((x.name!="")&&(y.name!="")) ss << x.name << "-" << y.name;
 if (x.name=="") ss << y.name;
 if (y.name=="") ss << x.name;
 return var(ss.str(),x.offset-y.offset);
}



ostream& operator << (ostream& os,var& f){os<<(string)f; return os;}

void LINE(var x,var y,var xf,var yf,int c,string mode=""){
 if (x==xf){
 cout << "b.vline("<<x<<","<<y<<","<<yf<<","<<"p["<<c<<"]);" << endl;
 return;
 }
 if (y==yf){
 cout << "b.hline("<<x<<","<<xf<<","<<y<<","<<"p["<<c<<"]);" << endl;
 return;
 }
 if (mode=="B"){
 cout << "b.box("<<x<<","<<xf<<","<<y<<","<<yf<<","<<"p["<<c<<"]);" << endl;
 return;
 }
 if (mode=="BF"){
 cout << "b.fill("<<x<<","<<xf<<","<<y<<","<<yf<<","<<"p["<<c<<"]);" << endl;
 return;
 }
 cout << "// ===========================================================" << endl;
 cout << "// Line("<<x<<","<<y<<","<<xf<<","<<yf<<","<<c<<","<<mode<<")" << endl;
 cout << "// ===========================================================" << endl;
 return;
}

int main ()
{
 string B="B",BF="BF";
 var column("x");
 var row("y");
 var length("w");
 var height("h");
 LINE (column, row,column + length, row + height, 0, B);
 for (int i=1;i<=2;i++){
  row = row + 1;
  column = column + 1;
  height = height - 1;
  length = length - 1;
  LINE (column, row,column + length, row, 7);
  LINE (column, row,column, row + height, 7);
  }
 row = row + 1;
 column = column + 1;
 height = height - 1;
 length = length - 1;
 
 LINE (column, row,column + length, row, 0);
 LINE (column, row,column, row + height, 0);
 LINE (column, row,column + 18, row + 18, 7, BF);
 LINE (column, row,column + 18, row + 18, 0, B);
 LINE (column + 18, row - 2,column + 18, row + 5, 0);
 LINE (column - 2, row + 18,column + 5, row + 18, 0);
 LINE (column + 5, row + 7,column + 13, row + 9, 15, BF);
 LINE (column + 5, row + 7,column + 13, row + 9, 0, B);
 LINE (column + 14, row + 7,column + 14, row + 9, 8);
 LINE (column + 6, row + 10,column + 14, row + 10, 8, B);
 LINE (column + 18, row,column + length, row + 18, 9, BF);
 LINE (column + 18, row,column + length, row + 18, 0, B);
 LINE (column + length, row - 3,column + length + 2, row + height, 7, BF);
 LINE (column + length, row - 3,column + length + 3, row + height, 0, B);
 LINE (column + length, row - 2,column + length, row - 1, 7);
 LINE (column + length - 18, row - 2,column + length - 18, row - 1, 0);
 LINE (column + length, row + 18,column + length + 2, row + 18, 0);
 LINE (column, row + height - 3,column + length, row + height - 3, 0, B);
 LINE (column, row + height - 2,column + length + 2, row + height, 7, BF);
 LINE (column - 2, row + height,column + length + 2, row + height, 0, B);
 LINE (column - 2, row + height - 18,column, row + height - 18, 0);
 LINE (column + length - 2, row + height - 18,column + length + 2, row + height - 18, 0);
 LINE (column + 18, row + height,column + 18, row + height - 2, 0);
 LINE (column + length - 18, row + height,column + length - 18, row + height - 2, 0);
 LINE (column + 1, row + 19,column + length - 1, row + height - 4, 7, BF);
 return 0;
}
