#include "../inc/lekser.h"


void Lekser::obradi(char c){
  //| c == '-' || c == '+' || c == '[' || c == ']' || c == '*' || c == '$' || c == '%'
  if(c=='\n' || c=='\t' || c==' ')this->blank=true;
  //else if(c==':' || c==',')this->znak=true;
 else if(c == '+' || c == ':' || c == '-' || c == ',' || c == '[' || c == ']' || c == '*' || c == '$' || c == '%')this->znak=true;
  else if(c== '_' || c == '.' || (c>='0' && c<='9') || (c >= 'a'&& c <= 'z') || (c >= 'A'&& c <= 'Z'))this->slovo=true;
  
}
void Lekser::resetuj(){
  this->blank=false;
  this->slovo=false;
  this->znak=false;
}
std::vector<string> Lekser::tokenize(string linija){
  std::vector<string> toks;
  string token="";
  for(auto karakter:linija){
    if(karakter=='#')break;
    this->obradi(karakter);
    if(slovo)token+=karakter;
    else if(blank){
      toks.push_back(token);
      token="";
    }
    else if(znak){
      if(token!=""){
        if(karakter==':')token+=karakter;
        else toks.push_back(token);
      }
      if(karakter!=':'){
        token=karakter;
        toks.push_back(token);
        token="";
      }
    } 
    this->resetuj();
  }
  if(token!="")toks.push_back(token);
  return toks;
}