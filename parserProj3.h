#ifndef par
#define par
#include <string>
#include <iostream>
#include "lexer.h"
using std::string;
using std::stoi;
using std::istream;
enum dattype{erron,stringer,integ};
extern void err(int numline,const string& dialog);
enum mathtype{ad,mul,divi,sub,nullif};
class PT {
   int lines,somenum,dinum,tesnum,adnum,mulnum;
   PT *l;
   PT *r;
public:
   PT(int x, PT *left=0, PT *right=0) : lines(x),l(left),r(right)
   {
      somenum=0,dinum=0,tesnum=0,adnum=0,mulnum=0;
   }
   virtual ~PT(){}
   PT* gl() const {return l;}
   PT* gr() const {return r;}
   int getlines() const {return lines;}
   virtual dattype gt() const {return erron;}
   virtual int giv() const {throw "no integer value";}
   virtual string gsv() const {throw "no string value";}
   virtual mathtype mt() const {return nullif;}
   int gnum() const {return somenum;}
   int gdinum() const {return dinum;}
   int gtesnum() const {return tesnum;}
   int gadnum() const {return adnum;}
   int gmulnum() const {return mulnum;}
};
class slist : public PT {
public:
   slist(PT *leader, PT *other) : PT(0,leader,other){}
};
class sta : public PT {
public:
   sta(PT *leader, PT *other) : PT(0,leader,other){}
};
class printer : public PT {
public:
   printer(PT *leader, PT *other) : PT(0,leader,other){}
};
class setting : public PT {
public:
   setting(PT *leader, PT *other) : PT(0,leader,other){}
};
class dc : public PT {
public:
   dc(PT *leader, PT *other) : PT(0,leader,other){}
};
class exr : public PT {
public:
   exr(PT *leader, PT *other) : PT(0,leader,other){}
};
class mret : public PT {
public:
   mret(PT *leader, PT *other) : PT(0,leader,other){}
};

class ic : public PT {
   int soe;
public:
   ic(const Token& coin) : PT(coin.GetLinenum())
   {
      soe=stoi(coin.GetLexeme());
   }
   dattype gt() const {return integ;}
   int giv() const {return soe;}
};
class sc : public PT{
   string soe;
public:
   sc(const Token& coin) : PT(coin.GetLinenum())
   {
      soe=coin.GetLexeme();
   }
   dattype gt() const {return stringer;}
   string gsv() const {return soe;}
};
class stastr : public PT {
public:
   stastr(Token pen) : PT(pen.GetLinenum()){}
};
class ist : public PT {
public:
   ist(Token pen) : PT(pen.GetLinenum()){}
};
class iden : public PT {
public:
   iden(Token pen) : PT(pen.GetLinenum()){}
};
void route(PT * apath);
extern PT * prog(istream* in);
extern PT * sl(istream* in);
extern PT * st(istream* in);
extern PT * dec(istream* in);
extern PT * setit(istream* in);
extern PT * pr(istream* in);
extern PT * express(istream* in);
extern PT * ter(istream* in);
extern PT * prim(istream* in);
#endif
