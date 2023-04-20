#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../inc/linker.h"
using namespace std;

int main(int argc, char *argv[])
{ 
  //${LINKER} -hex -o program.hex ivt.o math.o main.o isr_reset.o isr_terminal.o isr_timer.o isr_user0.o
  vector<string> fajlovi;
  int i = 0;
  for(int i=4;i<argc;i++)fajlovi.push_back((string)argv[i]);
  Linker *linker = new Linker(fajlovi,argv[3]);
  linker->pokreni();
  return 0;
}
