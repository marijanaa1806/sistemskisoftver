#include "../inc/tabelaSim.h"
void TabelaSim::setFajl(string name,int vr){
  for(int i=0;i<tabelaS.size();i++){
    if(tabelaS[i].naziv==name){
      tabelaS[i].fajlDef=vr;
      break;
    }
  }
}
void TabelaSim::dodaj(Simbol rr){
  tabelaS.push_back(rr);
}
void TabelaSim::updateLc(string name,int val,int sec){
  for(int i=0;i<tabelaS.size();i++){
    if(tabelaS[i].naziv==name){
      tabelaS[i].vrednost=val;
      tabelaS[i].definisan=true;
      tabelaS[i].sekcija=sec;
      break;
    }
  }
}
Simbol* TabelaSim::dohvSimbol(int i){
  return &tabelaS[i];
}
Simbol* TabelaSim::getSim(string nam){
  for(int i=0;i<tabelaS.size();i++){
    if(tabelaS[i].naziv==nam)return &tabelaS[i];
  }
  return nullptr;
}
void TabelaSim::updateSize(string name,int vr){
  for(int i=0;i<tabelaS.size();i++){
    if(tabelaS[i].naziv==name){
      tabelaS[i].velicina=vr;
      break;
    }
  }
}
vector<Simbol>& TabelaSim::dohvTabelu(){
    return tabelaS;
}
int TabelaSim::getSimbol(string naziv){
  for(int i=0;i<tabelaS.size();i++){
    if(tabelaS[i].naziv==naziv){
      return i;
    }
  }
  return -1;
}

