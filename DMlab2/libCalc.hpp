#pragma once
#include <string>
#include <vector>
using namespace std;


class CalcComb
{

private:
  using comFunc = void(*)(istream &istrm, ostream &ostrm);

  static unsigned int CalcU(unsigned int m, unsigned int n);
  static unsigned int CalcA(unsigned int m, unsigned int n);
  static unsigned int CalcP(unsigned int m);
  static unsigned int CalcC(unsigned int m, unsigned int n);
  static unsigned int CalcS(unsigned int m, unsigned int n);
  static unsigned int CalcB(unsigned int m);

  static void helpFunc(istream& istrm, ostream& ostrm);
  static void uFunc(istream& istrm, ostream& ostrm);
  static void aFunc(istream& istrm, ostream& ostrm);
  static void pFunc(istream& istrm, ostream& ostrm);
  static void cFunc(istream& istrm, ostream& ostrm);
  static void sFunc(istream& istrm, ostream& ostrm);
  static void bFunc(istream& istrm, ostream& ostrm);

  static unsigned int notZero(unsigned int arg1, unsigned int arg2)
  {
    unsigned int res = arg1 * arg2;
    if (res / arg1 != arg2 || res % arg1 != 0) throw exception("Unsigned integer overflow.");
    return res;
  }

  static unsigned int Add(unsigned int arg1, unsigned int arg2)
  {
    unsigned int res = arg1 + arg2;
    if (res < arg1 || res < arg2) throw exception("Unsigned integer overflow.");
    return res;
  }


  using com_t = struct
  {
    comFunc func;
    char str[21];
  };

  vector<com_t>* com;

  static unsigned int readArg(istream& istrm);
  com_t* readCom(istream& istrm);

public:
  CalcComb();
  ~CalcComb();
  bool procCom(string& commandStr, ostream &ostrm);
};