// C++11
// g++ -Werror -Wall -Wpedantic --std=c++11 funvm.cc && ./a.out
#include <iostream>
#include <string>
#include <deque>
#include <memory>
#include <map>
using namespace std;
#include "Opcode.cc"
#include "Data.cc"
#include "Machine.cc"

int main() {
  Machine m;
  map<P, P> dval;
  dval[new Data("a")] = new Data("b");
  cout << dval[new Data("a")]->sval << endl;
}
