#include "par.h"
#include "lexer.h"
#include <string>
using namespace std;
#include <map>
#include <iostream>
#include <fstream>
string w = "";
int dinum=0;
int tesnum=0;
int mulnum=0;
int adnum=0;
bool b = false;
void err(int numline,const string& dialog)
{
   cout << w << ":" << numline+1 << ":" << dialog << endl;
}
int main(int argc, char *argv[])
{
   for(int x=1;x<argc;x++)
   {
      if(argv[x][0]!='-')
      {
         continue;
      }
      w=argv[x];
      if(w=="-t")
      {
         b=true;
      }
      else
      {
         cout << w << " UNRECOGNIZED FLAG" << endl;
         return 0;
      }
   }
   ifstream f;
   istream *in = &cin;
   bool c=false;
   for(int y=1;y<argc;y++)
   {
      if(argv[y][0]=='-')
      {
         continue;
      }
      if(c)
      {
         cout << "TOO MANY FILES" << endl;
         return 0;
      }
      w=argv[y];
      c=true;
      f.open(w);
      if(!f.is_open())
      {
         cout << w << " FILE NOT FOUND" << endl;
         return 0;
      }
      in = &f;
   }
   PT *branch=prog(in);
   if(branch==0)
   {
      return 0;
   }
   if(b)
   {
      route(branch);
      cout << endl;
   }
   cout << "Total number of identifiers: " << dinum << endl;
   cout << "Total number of set: " << tesnum << endl;
   cout << "Total number of +: " << adnum << endl;
   cout << "Total number of *: " << mulnum << endl;
   return 0;
}
