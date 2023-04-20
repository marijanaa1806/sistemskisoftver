#ifndef LEXER_H
#define LEXER_H
#include<iostream>
#include<string>
#include <stdio.h>
#include <vector>
using namespace std;
 
class Lekser{
  std::vector<std::string> tokeni;
  bool slovo=false;
  bool znak=false;
  bool blank=false;
  void obradi(char c);
  void resetuj();
  public:
  std::vector<string> tokenize(string linija);
};


#endif