#ifndef PARSER_H
#define PARSER_H
#include<iostream>
#include<string>
#include <stdio.h>
#include <vector>
#include<algorithm>
#include<regex>
using namespace std;

enum Direktiva{global,eXtern,section,word,skip,end};
enum Instr{halt,Int,ret,iret,call,jmp,jeq,jne,jgt,push,pop,xchg,add,sub,mul,Div,cmp,Not,And,Or,Xor,test,shl,shr,ldr,str};
enum Tip{literal,simbol,registar};
struct Argum{
  Tip tip;
  string argm;
  int brojReg;
  int pom;
};
enum Adres{mdir,mind,pcrel,apsol,regdir,regind,regpom,immed};
struct Parsed{
  Direktiva direktiva;
  Instr instrukcija;
  vector<Argum> listaPar;
  string labela;
  Adres tipAdr;
  int id;// 0 je dir 1 je instr 2 je labela
  string opCode;
};
class Parser{
  vector<string> instruction={"halt","int","ret","iret","call","jmp","jeq","jne","jgt","push","pop","xchg","add","sub","mul","div","cmp","not","and","or","xor","test","shl","shr","ldr","str"};
  bool checkR(string s);
  bool checkL(string s);
  bool checkS(string s);
  Argum makeL(string s);
  Argum makeS(string s);
  Argum makeR(string s);
  bool checkList(vector<string> tok);
  vector<string> directive={".global",".extern",".section",".word",".skip",".end"};
  Parsed p;

  public:
  string reg="r[0-7]";
  string litOc="^(0[0-7])*$";
  string litHex="0[xX][0-9a-fA-F]+";
  string litBin="^([0-1])*$";
  string litDec="^(-|\\+)?\\d*\\.?\\d+$";
  string ssym="^[0-9a-zA-Z_]+";
  Parsed parsuj(vector<string> tokeni);
};
#endif