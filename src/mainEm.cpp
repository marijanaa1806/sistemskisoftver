#include <iostream>
#include "../inc/emulator.h"

using namespace std;

int main(int argc, char* argv[]){

  if(argc != 2){
    cout<< "Pogresan unos, unesite => emulator <naziv_ulazne_datoteke>" << endl;
    return -1;
  }
  Emulator* em=new Emulator(argv[1]);
  em->ucitaj();
  em->emulate();
  em->ispis();
  return 0;
}