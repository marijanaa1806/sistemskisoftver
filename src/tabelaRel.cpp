#include "../inc/tabelaRel.h"

void TabelaRel::dodaj(relZapis rr){
  tabelaR.push_back(rr);
}
relZapis* TabelaRel::dohvZapis(int i){
  return &tabelaR[i];
}
vector<relZapis>& TabelaRel::dohvTabelu(){
    return tabelaR;
}
