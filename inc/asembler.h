#ifndef ASEMBLER_H
#define ASEMBLER_H
#include "lekser.h"
#include "parser.h"
#include "sekcija.h"
#include "tabelaRel.h"
#include "tabelaSim.h"
#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include "stdio.h"
#include <fstream>
#include <algorithm>
#include <ctype.h>
#include <sstream>
class Asembler{
  Lekser* lex=new Lekser();
  Parser* parser=new Parser();
  Sekcija* section=nullptr;
  TabelaSim* tabSim=new TabelaSim();
  vector<Sekcija*> sekcije;
  vector<string> tokeni;
  bool zavrsi=false;
  bool istaSek=false;
  string ulazniFajl;
  string izlazniFajl;
  public:
  Asembler(string ulaz, string izlaz){
    ulazniFajl=ulaz;
    izlazniFajl=izlaz;
  } 
  bool is_blank(string str, int N);
  void asemble();
  void ubaciSimbol(int sec,tipBind tip,string naziv);
  void otvoriSekciju(string naziv);
  void add2bytes(short val);
  string findSim(string naziv);
  void simbolize(string naziv,bool pcrel);
  void razresi();
  relZapis patch(int simbol,int offset,bool pcrel);
  string printchar(unsigned char cc);

};
#endif