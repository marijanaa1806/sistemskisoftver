#ifndef LINKER_H
#define LINKER_H
#include <string>
#include <exception>
#include <iostream>
#include <vector>
#include "stdio.h"
#include <fstream>
#include <algorithm>
#include <ctype.h>
#include <sstream>
#include "sekcija.h"
#include "tabelaRel.h"
#include "tabelaSim.h"

using namespace std;

class SadrzajFajla{
  public:
  vector<Sekcija*> sekcije;
  TabelaSim* tabelaSim=new TabelaSim();
  vector<string> imenaSekcija;
  int fajl;
};
struct sectionOffset{
  int offset;
  int file;
  string naziv;
};
class Linker{
  ifstream inpt;
  ofstream outpt;
  vector<string> fajlovi;
  string izlaz;
  bool simboli=false;
  bool rels=false;
  bool sects=false;
  int currOffset=0;
  public:
  int offMoje(string naziv,int fajl);
  void fixRelas();
  vector<SadrzajFajla*> fileContent;
  vector<sectionOffset> offSekc;
  vector<Sekcija*> noveSekcije;
  TabelaSim* eksterni=new TabelaSim();
  TabelaSim* globalni=new TabelaSim();
  TabelaSim* noviSimboli=new TabelaSim();
  vector<unsigned char> ceoSadrzaj;
  vector<int> fajloviEks;
  Linker(vector<string> ulazniFs,string izlazniF){
    fajlovi=ulazniFs;
    izlaz=izlazniF;
   
  }
  void kopiraj();
  void pokreni();
  void ispis();
  int isSubstring(string s1, string s2);
  int brojsek(string naziv);
  bool exists(string naziv);
  void izmeni(int mesto,int vrednost);
  Sekcija* findSek(string naziv);
  void spojiSekcije();
  int nadjiSekciju(string naziv,int mojf);
  void ubaciuSekciju(string naz,char c);
  int globVrednost(string naz);
  void ucitajFajlove();
  int nadjiSimbol(string naziv);
  void split_str( std::string const &str, const char delim,  std::vector <std::string> &out );
  bool poveziSimbole();
  SadrzajFajla* nadjiFajl(int ind);

};

#endif