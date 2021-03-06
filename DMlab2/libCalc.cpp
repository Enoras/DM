#include "libCalc.hpp"
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include <sstream>
#include <vector>


using namespace std;


unsigned int CalcComb::CalcU(unsigned int m, unsigned int n)
{
    if (m == 0)
    {
        if (n == 0)
            return 1;
        else
            return 0;
    }
    if (m == 1)
        return 1;
    unsigned int acc = 1;
    for (unsigned int i = 1; i <= n; i++)
    {
        acc = notZero(acc, m);
    }
    return acc;
}


unsigned int CalcComb::CalcA(unsigned int m, unsigned int n)
{
    if (n == 0)
        return 1;
    if (n > m)
        return 0;

    unsigned int acc = 1;
    for (unsigned int i = m - n + 1; i <= m; i++)
    {
        acc = notZero(acc, i);
    }
    return acc;
}

CalcComb::com_t* CalcComb::readCom(istream& istrm)
{
  com_t* command = nullptr;
  string* commandBuffer = new string(31, '\0');
  istrm >> *commandBuffer;
  if (commandBuffer->size() != 0)
  {
    for (auto& cmd : *com)
    {
      if (commandBuffer->compare(cmd.str) == 0)
      {
        command = &cmd;
        break;
      }
    }
  }
  delete commandBuffer;
  return command;
}

CalcComb::CalcComb()
{
  com = new vector<com_t>
  {
    { helpFunc, "H" },
    { nullptr, "Q" },
    { uFunc, "U" },
    { aFunc, "A" },
    { pFunc, "P" },
    { cFunc, "C" },
    { sFunc, "S" },
    { bFunc, "B" }
  };
}

CalcComb::~CalcComb()
{
  delete com;
}

bool CalcComb::procCom(string& commandStr, ostream& ostrm)
{
  istringstream istrm = istringstream(commandStr);
  com_t* command_p = readCom(istrm);
  if (command_p == nullptr)
  {
    throw exception("Unknown command");
  }
  if (string("Q").compare(command_p->str) == 0)
  {
    return false;
  }
  else if (command_p->func == nullptr)
  {
    throw exception("Command function not found.");
  }
  else
  {
    command_p->func(istrm, ostrm);
    return true;
  }
}

unsigned int CalcComb::readArg(istream& istrm)
{
    unsigned int arg;
    istrm >> arg;
    if (istrm.fail())
    {
        throw exception("Incorrect command");
    }
    return arg;
}



unsigned int CalcComb::CalcP(unsigned int m)
{
  unsigned int acc = 1;
  for (unsigned int i = 2; i <= m; i++)
  {
    acc = notZero(acc, i);
  }
  return acc;
}

unsigned int CalcComb::CalcC(unsigned int m, unsigned int n)
{
  if (n > m)
    return 0;
  if (n > m - n)
    n = m - n;
  if (n == 1)
    return m;
  if (n == 0)
    return 1;
  if (n > 40)
    throw exception("integer overflow.");

  vector<unsigned int> vec = vector<unsigned int>(n + 1);
  for (unsigned int i = 0; i <= n; i++)
  {
    vec[i] = 1;
  }
  for (unsigned int i = 0; i < m - n; i++)
  {
    for (unsigned int j = 1; j <= n; j++)
    {
      vec[j] = Add(vec[j], vec[j - 1]);
    }
  }
  return vec[n];
}

unsigned int CalcComb::CalcS(unsigned int m, unsigned int n)
{
  if (m == n)
    return 1;
  if (n == 0)
    return 0;
  if (n > m)
    return 0;
  if (n == 1)
    return 1;

  unsigned int d = min(n, m - n + 1);
  unsigned int s = max(n, m - n + 1);
  vector<unsigned int> vec = vector<unsigned int>(d);
  for (unsigned int i = 0; i < d; i++)
  {
    vec[i] = 1;
  }
  if (d == n)
  {
    for (unsigned int i = 1; i < s; i++)
    {
      for (unsigned int j = 1; j < d; j++)
      {
        vec[j] = Add(vec[j - 1], notZero(j + 1, vec[j]));
      }
    }
  }
  else
  {
    for (unsigned int i = 1; i < s; i++)
    {
      for (unsigned int j = 1; j < d; j++)
      {
        vec[j] = Add(vec[j], notZero(i + 1, vec[j - 1]));
      }
    }
  }
  return vec[d - 1];
}

unsigned int CalcComb::CalcB(unsigned int m)
{
  if (m == 0)
    return 1;

  vector<unsigned int> vec = vector<unsigned int>(m);
  vec[0] = 1;
  for (unsigned int n = 2; n <= m; n++)
  {
    unsigned int t = vec[0];
    vec[0] = vec[n - 2];
    for (unsigned int k = 1; k < n; k++)
    {
      unsigned int s = vec[k];
      vec[k] = Add(vec[k - 1], t);
      t = s;
    }
  }
  return vec[m - 1];
}

void CalcComb::helpFunc(istream& istrm, ostream& ostrm)
{
    ostrm
    << "****************************************************************************************************" << endl
    << "H - show help." << endl
    << "Q - quit the programm." << endl
    << "U <M_arg> <N_arg> - calculates the number of <N_arg>-permutations of <M_arg> with repetitions." << endl
    << "A <M_arg> <N_arg> - calculates the number of <N_arg>-permutations of <M_arg> without repetitions." << endl
    << "P <M_arg> - calculates the number of permutations of <M_arg>." << endl
    << "C <M_arg> <N_arg> - calculates the binomial coefficient." << endl
    << "S <M_arg> <N_arg> - calculates the Stirling numbers of the second kind." << endl
    << "B <M_ar> - calculates the Bell number." << endl
    << "****************************************************************************************************" << endl
    ;
}

void CalcComb::uFunc(istream& istrm, ostream& ostrm)
{
  unsigned int m = readArg(istrm);
  unsigned int n = readArg(istrm);
  try
  {
    ostrm << "U(" << m << ", "<< n <<") =" << endl << CalcU(m, n) << endl;
  }
  catch (const std::exception&)
  {
    throw exception("Too big numbers.");
  }
}

void CalcComb::aFunc(istream& istrm, ostream& ostrm)
{
  unsigned int m = readArg(istrm);
  unsigned int n = readArg(istrm);
  try
  {
  ostrm << "A(" << m << ", " << n << ") =" << endl << CalcA(m, n) << endl;
  }
  catch (const std::exception&)
  {
    throw exception("Too big numbers.");
  }
}

void CalcComb::pFunc(istream& istrm, ostream& ostrm)
{
  unsigned int m = readArg(istrm);
  try
  {
    ostrm << "P(" << m << ") =" << endl << CalcP(m) << endl;
  }
  catch (const std::exception&)
  {
    throw exception("Too big numbers.");
  }
}

void CalcComb::cFunc(istream& istrm, ostream& ostrm)
{
  unsigned int m = readArg(istrm);
  unsigned int n = readArg(istrm);
  try
  {
    ostrm << "C(" << m << ", " << n << ") =" << endl << CalcC(m, n) << endl;
  }
  catch (const std::exception&)
  {
    throw exception("Too big numbers.");
  }
}

void CalcComb::sFunc(istream& istrm, ostream& ostrm)
{
  unsigned int m = readArg(istrm);
  unsigned int n = readArg(istrm);
  try
  {
    ostrm << "S(" << m << ", " << n << ") =" << endl << CalcS(m, n) << endl;
  }
  catch (const std::exception&)
  {
    throw exception("Too big numbers.");
  }
}

void CalcComb::bFunc(istream& istrm, ostream& ostrm)
{
  unsigned int m = readArg(istrm); try
  {
    ostrm << "B(" << m << ") =" << endl << CalcB(m) << endl;
  }
  catch (const std::exception&)
  {
    throw exception("Too big numbers.");
  }
}
