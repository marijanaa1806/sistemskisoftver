#ifndef TABELAREL_H
#define TABELAREL_H
#include<iostream>
#include<string>
#include <stdio.h>
#include <vector>
using namespace std;

enum relTip {R_X86_PC,R_X86_16};
struct relZapis{
  int offset;
  relTip tip;
  int simbol;
  //addend je dec
  int addend;
};
class TabelaRel{
  vector<relZapis> tabelaR;
  public:
  void dodaj(relZapis rr);
  vector<relZapis>& dohvTabelu();
  relZapis* dohvZapis(int i);

};

#endif