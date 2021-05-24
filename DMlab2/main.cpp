#include <stdlib.h>
#include <crtdbg.h>
#include "libCalc.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(void)
{
  CalcComb* cM = new CalcComb();

  bool ok = true;
  string* strCom = new string(101, '\0');

  cout << "H for help" << endl ;

  while (ok && getline(cin, *strCom))
  {
    cout << endl;

    try
    {
      ok = cM->procCom(*strCom, cout);
    }
    catch (const std::exception& err)
    {
      cout << err.what() << endl;
    }
    cout << endl;
  }

  delete strCom;
  delete cM;

  _CrtDumpMemoryLeaks();

  return 0;
}