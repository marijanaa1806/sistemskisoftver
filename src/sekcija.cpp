#include "../inc/sekcija.h"
void Sekcija::setBr(int n){
      broj=n;
}
int Sekcija::getBr(){
    return broj;
}
TabelaRel* Sekcija::relTab(){
    return mojaRel;
}
void Sekcija::dodaj(char c){
    sadrzaj.push_back(c);
}
int Sekcija::LC(){
    return sadrzaj.size();
}
vector<unsigned char>& Sekcija::dohvSadrzaj(){
    return sadrzaj;
}
Backpatch* Sekcija::backP(){
    return mojBackp;
}
  