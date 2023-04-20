#include <iostream>
#include <fstream>
#include <regex>
#include "../inc/asembler.h"
using namespace std;

int main(int argc, const char *argv[])
{
    if (("-o" == (string)argv[1]) && argc==4){
        Asembler* as=new Asembler(argv[3],argv[2]);
        as->asemble();
    }
    else{
        cout<< "Greska prilikom pozivanja asemblera";
        return -1;
    }
    return 0;
}