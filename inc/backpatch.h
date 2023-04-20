#ifndef BACKPATCH_H
#define BACKPATCH_H

#include<iostream>
#include<string>
#include <stdio.h>
#include <vector>
#include "tabelaSim.h"
using namespace std;

enum backTip{BACK_REL,BACK_APS};
struct backZapis{
  
  int simbol;
  int offset;
  backTip tip;
  bool gotov;
};

class Backpatch{
  vector<backZapis> tabelaBp;
  public:
  void ubaciBack(backZapis bb);
  void zavrsiZapis(Simbol* s,int off);
  vector<backZapis>& dohvTabeluB();
  backZapis getZapis(int i);
 
};

#endif