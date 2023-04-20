#ifndef EMULATOR_H
#define EMULATOR_H
#include <string>
#include "parser.h"
#include <exception>
#include <iostream>
#include <vector>
#include "stdio.h"
#include <fstream>
#include <map>
#include <algorithm>
#include <ctype.h>
#include <bitset>
#include <sstream>
using namespace std;

struct processor{
	short reg[8];
	char ivt;
  
};
struct instData{
  int regDes;
  int regSrc;
  string instruction;
  Adres adrs;
  string operand;
};
 
class Emulator{
	processor CPU;
	char mem[65536];
  short psw;
	static const int PC = 7;
	static const int SP = 6;
  static const int pswZ= 1;
  static const int pswO = 1 << 1;
  static const int pswC = 1 << 2;
  static const int pswN = 1 << 3;
  static const int pswI= 1 << 15; 
  instData currIns;
	ifstream inpt;
	vector<string> content;
	string ul;
	map<int,string> kodovi;
  bool uslov=false;
	bool end=false;
  string wrReg(short rr);
  void makeShort(short& tb,char c1,char c2);
  void makeChars(short tb,char& c1,char& c2);
	void splitstr( std::string const &str, const char delim,  std::vector <std::string> &out );
  string ispisiznak(char c);

  void exec2reg();
  void exec1reg();
  void execLdr();
  void execStr();
  void execJmps();
  void execCall();

  void setN();
  void setZ();
  void setO();
  void setC();
  
  void inc2b();
  void inc3b();
  void inc5b();
  void subroutine();
  void checkUslov();
  void adrFaza(string aa);
  void push();
  void pop();
  void clearIns();
	public:
	Emulator(string u){
		ul=u;
	}
	void ucitaj();
	void emulate();
  void ispis();
};
#endif