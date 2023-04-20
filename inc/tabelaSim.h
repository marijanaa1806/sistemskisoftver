#ifndef TABELASIM_H
#define TABELASIM_H
#include<iostream>
#include<string>
#include <stdio.h>
#include <vector>
using namespace std;

enum tipBind {GLOB,LOC};
enum tipSim{NO_TYPE,SCTN};
class Simbol{
  public:
  string naziv;
  int vrednost;
  int velicina;
  int sekcija;
  tipBind bind;
  tipSim tip;
  bool definisan;
  int fajlDef;
};

class TabelaSim{
  vector<Simbol> tabelaS;
  public:
  void setFajl(string naziv,int ff);
  void dodaj(Simbol rr);
  void updateLc(string name,int val,int sec);
  Simbol* dohvSimbol(int i);
  Simbol* getSim(string name);
  int getSimbol(string naziv);
  vector<Simbol>& dohvTabelu();
  void updateSize(string sekcija,int vel);
};

#endif