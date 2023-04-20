#include "../inc/backpatch.h"
void Backpatch::ubaciBack(backZapis bb){
  tabelaBp.push_back(bb);
}
void Backpatch::zavrsiZapis(Simbol* s,int off){
 for(int i=0;i<tabelaBp.size();i++){
    
    int offset=tabelaBp.at(i).offset;
    if(off==offset){
      tabelaBp.at(i).gotov=true;
      break;
    }
  }
}
vector<backZapis>& Backpatch::dohvTabeluB(){
    return tabelaBp;
  }
backZapis Backpatch::getZapis(int i){
  return tabelaBp[i];
}