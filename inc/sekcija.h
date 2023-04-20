#ifndef SEKCIJA_H
#define SEKCIJA_H
#include<iostream>
#include<string>
#include <stdio.h>
#include <vector>
#include "tabelaRel.h"
#include "backpatch.h"
using namespace std;

class Sekcija{
  int broj;
  vector<unsigned char> sadrzaj;
  TabelaRel* mojaRel=new TabelaRel();
  Backpatch* mojBackp=new Backpatch();

  public:
  string ime;
  void setBr(int n);
  int getBr();
  TabelaRel* relTab();
  void dodaj(char c);
  int LC();
  Backpatch* backP();
  vector<unsigned char>& dohvSadrzaj();
};

#endif