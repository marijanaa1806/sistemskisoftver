#include "../inc/parser.h"
#include "stdio.h"

bool Parser::checkR(string s){
  return regex_match(s,regex(reg))||s=="psw";
}
bool Parser::checkL(string s){
  return regex_match(s,regex(litBin)) || regex_match(s,regex(litOc)) || regex_match(s,regex(litDec)) || regex_match(s,regex(litHex));
}
bool Parser::checkS(string s){
  return regex_match(s,regex(ssym));
}
Argum Parser::makeL(string s){
  Argum argum;
  argum.argm=s;
  argum.tip=Tip::literal;
  int n;
  if(regex_match(s,regex(litDec))){
    n=stoi(s);
  }else if(regex_match(s,regex(litHex))){
    n=stoi(s,nullptr,16);
  }else if(regex_match(s,regex(litOc))){
    n=stoi(s,nullptr,8);
  }else if(regex_match(s,regex(litBin))){
    n=stoi(s,nullptr,2);
  }
  argum.pom=n;
  argum.brojReg=-1;
  return argum;
}
Argum Parser::makeS(string s){
  Argum argum;
  argum.argm=s;
  argum.tip=Tip::simbol;
  argum.brojReg=-1;
  argum.pom=-1;
  return argum;
}
Argum Parser::makeR(string s){
  Argum argum;
  if(s=="psw")argum.brojReg=8;
  else argum.brojReg=stoi(s.substr(1,1));
  argum.tip=Tip::registar;
  argum.pom=-1;
  argum.argm=s;
  return argum;
}
bool Parser::checkList(vector<string> tok){
  if(p.listaPar.size()!=0)p.listaPar.clear();
  int zar=0;
  for(int i=1;i<tok.size();i++){
    if(i%2){
      if(checkS(tok.at(i))){
        p.listaPar.push_back(makeS(tok.at(i)));
      }else return false;
    }else{
      if(tok.at(i)==","){
        zar++;
      }else return false;
    }
  }
  if(zar!=p.listaPar.size()-1)return false;
  return true;
}
Parsed Parser::parsuj(vector<string> tokeni){
  Parsed par;
  par.id=-1;//nije nista na pocetku
  int i;
  bool ok=false;
  for (auto i = tokeni.begin(); i != tokeni.end(); ++i) {
    if (*i=="") {
      tokeni.erase(i);
      i--;
    }
  }
  int n=tokeni.size();
  try{
    if(tokeni.at(n-1).back()==':'){
      for(i=0;i<n-1;i++){
        //proveravamo da li su ispred nje samo blanko znaci, ako nisu onda labela nije na prvom mestu
        if(tokeni.at(i)!=""){
          throw "labela nije na prvom mesty\n";
        }
      }
      //posto smo izbacili blanko onda je prvi
      par.labela=tokeni.at(0).substr(0,tokeni.at(0).length()-1);
      par.id=2;
    }
  //nije labela, proveravamo da li je naredba ili direktiva
  else{
    for(i=0;i<n;i++){
      string s=tokeni.at(i);
      //direktiva
      if(s.front()=='.' && i==0){
         par.id=0;
         auto it = find(directive.begin(), directive.end(), s);
        int index=-1;
        if (it != directive.end()) 
        {
          index = it - directive.begin(); 
        }
        switch(index){
        case 0:
          par.direktiva=Direktiva::global;
          break;
        case 1:
          par.direktiva=Direktiva::eXtern;
          break;
        case 2:
          par.direktiva=Direktiva::section;
          break;
        case 3:
          par.direktiva=Direktiva::word;
          break;
        case 4:
          par.direktiva=Direktiva::skip;
          break;
        case 5:
          par.direktiva=Direktiva::end;
          break;
        default:
          throw "nepostojeca direktiva\n";
          break;
        }
      }
      //naredba
      else if(s.front()!='.' && i==0){
        par.id=1;  
        auto it = find(instruction.begin(), instruction.end(), s);
        int index=-1;
        if (it != instruction.end()) 
        {
          index = it - instruction.begin(); 
        }
        cout<<"inst je"<<index<<"\n";
        switch(index){
          case 11:
            par.instrukcija=Instr::xchg;
            par.opCode="0x60";
            break;
          case 12:
              par.instrukcija=Instr::add;
              par.opCode="0x70";
              break;
          case 13:
              par.instrukcija=Instr::sub;
              par.opCode="0x71";
              break;
          case 14:
              par.instrukcija=Instr::mul;
              par.opCode="0x72";
              break;
          case 15:
              par.instrukcija=Instr::Div;
              par.opCode="0x73";
              break;
          case 16:
              par.instrukcija=Instr::cmp;
              par.opCode="0x74";
              break;
          case 18:
              par.instrukcija=Instr::And;
              par.opCode="0x81";
              break;
          case 19:
             par.instrukcija=Instr::Or;
              par.opCode="0x82";
              break;
          case 20:
              par.instrukcija=Instr::Xor;
              par.opCode="0x83";
              break;
          case 21:
              par.instrukcija=Instr::test;
              par.opCode="0x84";
              break;
          case 22:
              par.instrukcija=Instr::shl;
              par.opCode="0x90";
              break;
          case 23:
              par.instrukcija=Instr::shr;
              par.opCode="0x91";
              break;
          case 24:
              par.instrukcija=Instr::ldr;
              par.opCode="0xA0";
              break;
          case 25:
              par.instrukcija=Instr::str;
              par.opCode="0xB0";
              break;
          case 5:
              par.instrukcija=Instr::jmp;
              par.opCode="0x50";
              break;
          case 4:
              par.instrukcija=Instr::call;
              par.opCode="0x30";
              break;
          case 6:
              par.instrukcija=Instr::jeq;
              par.opCode="0x51";
              break;
          case 7:
              par.instrukcija=Instr::jne;
              par.opCode="0x52";
              break;
          case 8:
              par.instrukcija=Instr::jgt;
              par.opCode="0x53";
              break;
          case 0:
              par.instrukcija=Instr::halt;
              par.opCode="0x00";
              break;
          case 3:
              par.instrukcija=Instr::iret;
              par.opCode="0x20";
              break;
          case 2:
              par.instrukcija=Instr::ret;
              par.opCode="0x40";
              break;
          case 17:
              par.instrukcija=Instr::Not;
              par.opCode="0x80";
              break;
          case 9:
              par.instrukcija=Instr::push;
              par.opCode="0xB0";//isto kao za str
              break;
          case 10:
              par.instrukcija=Instr::pop;
              par.opCode="0xA0";//isto kao za ldr
              break;
          case 1:
              par.instrukcija=Instr::Int;
              par.opCode="0x10";
              break;
          default:
              throw "nepostojeca instr\n";
        }
      }
      //direktiva, prikupljamo listu arg
      if(par.id==0 && i!=0){
         switch (par.direktiva){
          case Direktiva::global:
            if(checkList(tokeni)){
              par.listaPar=p.listaPar;
              return par;
            }
            else throw "nije lista simbola\n";
            break;
          case Direktiva::eXtern:
             if(checkList(tokeni)){
              par.listaPar=p.listaPar;
              return par;
            }
            else throw "nije lista simbola\n";
            break;
          case Direktiva::section:
            if(tokeni.size()>2)throw "treba da bude samo ime sekcije\n";
            else{
              Argum ar;
              ar.argm=s;
              par.listaPar.push_back(ar);
              return par;
            }
            break;
          case Direktiva::word:
            {
              if(tokeni.size()==2){
                if(checkL(s)){
                    par.listaPar.push_back(makeL(s));
                    return par;
                  }else if(checkS(s)){
                    par.listaPar.push_back(makeS(s));
                    return par;
                  }else throw "nije ni lit ni sim word\n";
              }else if(tokeni.size()>2){
                int zz=0;
                for(int j=1;j<tokeni.size();j++){
                  if(j%2==0){
                    if(tokeni.at(j)==",")zz++;
                    else throw "nije lista\n";
                  }else{
                    if(checkL(tokeni.at(j))){
                      par.listaPar.push_back(makeL(tokeni.at(j)));    
                    }else if(checkS(tokeni.at(j))){
                      par.listaPar.push_back(makeS(tokeni.at(j)));   
                    }else throw "nije ni lit ni sim word\n";
                  }
                }
                if(zz==(par.listaPar.size()-1))return par;
                else throw "nije lista word\n";
              }
              break;
            }
          case Direktiva::skip:
            if(tokeni.size()>2)throw ".skip preskace samo 1 literal\n";
            else{
              if(checkL(s)){
                par.listaPar.push_back(makeL(s));
                return par;
              }else throw ".skip mora da ima arg literal\n";
            }
            break;
          case Direktiva::end:
            if(tokeni.size()>1)throw ".end nema argumenata\n";
            break;
          default:
            throw "nepostojeca direktiva\n";
            break;
        }
      }
      //instrukcija
      else if(par.id==1 && i!=0){
        switch (par.instrukcija)
        {
        case Instr::ldr:
        case Instr::str:
          if(i==1){
            if(this->checkR(s)){
              par.listaPar.push_back(this->makeR(s));
            }
            else {
              throw "error while loading ldr instruction RegisterNotValid\n";
            }
          }else if(i==2){
            if(s!=","){ 
               throw "error while loading ldr instruction CommaNotValid\n"; 
            }
          }else{
            switch(n){
              case 4:
                if(this->checkL(s)){
                  par.listaPar.push_back(this->makeL(s));
                  par.tipAdr=Adres::apsol;
                  return par;
                }else if(this->checkR(s)){
                  par.listaPar.push_back(this->makeR(s));
                  par.tipAdr=Adres::regdir;
                  return par;
                }else if(this->checkS(s)){
                  par.listaPar.push_back(this->makeS(s));
                  par.tipAdr=Adres::apsol;
                  return par;
                }else{
                  throw "nije ni literal ni reg ni sim operand kod ldr reg,op";
                }
              break;
              case 5:
                if(i==3){
                  if(s=="%"){
                    string s2=tokeni.at(i+1);
                    if(this->checkS(s2)){
                      par.listaPar.push_back(this->makeS(s2));
                      par.tipAdr=Adres::pcrel;
                      return par;
                    }
                    else {
                     throw "error while loading symbol %\n";
                    }
                  }else if(s=="$"){
                     string s2=tokeni.at(i+1);
                    if(this->checkL(s2)){
                      par.tipAdr=Adres::immed;
                      par.listaPar.push_back(this->makeL(s2));
                      return par;
                    }else if(this->checkS(s2)){
                      par.tipAdr=Adres::immed;
                      par.listaPar.push_back(this->makeS(s2));
                      return par;
                    }
                    else {
                      throw "error while loading symbol or literal $\n";
                    }
                  }
                }
              break;
              case 6:
                if(tokeni.at(3)=="[" && this->checkR(tokeni.at(4)) && tokeni.at(5)=="]"){
                   par.listaPar.push_back(this->makeR(tokeni.at(4)));
                   par.tipAdr=Adres::regind;
                  return par;
                }else{
                  throw "error while loading [reg]\n";
                }
              break;
              case 8:
              if(tokeni.at(3)=="[" && this->checkR(tokeni.at(4)) && tokeni.at(5)=="+" && tokeni.at(7)=="]"){
                   par.listaPar.push_back(this->makeR(tokeni.at(4)));
                if(this->checkL(tokeni.at(6))){
                  par.listaPar.push_back(this->makeL(tokeni.at(6)));
                }else if(this->checkS(tokeni.at(6))){
                  par.listaPar.push_back(this->makeS(tokeni.at(6)));
                }else{
                  par.listaPar.pop_back();
                  throw "error while loading [reg+s/l]\n";
                }
                par.tipAdr=Adres::regpom;
                return par;
                }else{
                 throw "error while loading [reg+s/l]\n";
                }
              break;
              default:
                throw "bad instruction ldr/str";
              break;
            }
          }
          break;
        case Instr::xchg:
        case Instr::add:
        case Instr::sub:
        case Instr::mul:
        case Instr::Div:
        case Instr::cmp:
        case Instr::And:
        case Instr::Or:
        case Instr::Xor:
        case Instr::test:
        case Instr::shl:
        case Instr::shr:
          if(this->checkR(tokeni.at(1)) && tokeni.at(2)=="," && this->checkR(tokeni.at(3))){
            par.listaPar.push_back(this->makeR(tokeni.at(1)));
            par.listaPar.push_back(this->makeR(tokeni.at(3)));
            return par;
            break;
          }else{
            throw "error while loading logic opp\n";
          }
          break;
        case Instr::jmp:
        case Instr::call:
        case Instr::jeq:
        case Instr::jne:
        case Instr::jgt:
          if(i==1){
            switch(n){
              case 2:
                 if(checkL(s)){
                    par.listaPar.push_back(makeL(s));
                    par.tipAdr=Adres::apsol;
                    return par;
                  }else if(checkS(s)){
                    par.listaPar.push_back(makeS(s));
                    par.tipAdr=Adres::apsol;
                    return par;
                  }else{
                    throw "2 bad jump\n";
                  }
                break;
              case 3:
                  if(s=="%" && checkS(tokeni.at(i+1))){
                    par.listaPar.push_back(makeS(tokeni.at(i+1)));
                    par.tipAdr=Adres::pcrel;
                    return par;
                  }else if(s=="*"){
                    if(checkR(tokeni.at(i+1))){
                      par.listaPar.push_back(makeR(tokeni.at(i+1)));
                      par.tipAdr=Adres::regdir;
                      return par;
                    }
                    else if(checkL(tokeni.at(i+1))){
                      par.listaPar.push_back(makeL(tokeni.at(i+1)));
                      par.tipAdr=Adres::mdir;
                      return par;
                    }else if(checkS(tokeni.at(i+1))){
                      par.listaPar.push_back(makeS(tokeni.at(i+1)));
                      par.tipAdr=Adres::mdir;
                      return par;
                    }else throw "bad 3 jump\n";}

                break;
              case 5:
                   if(checkR(tokeni.at(i+2)) && tokeni.at(i+1)=="[" && tokeni.at(i+3)=="]" && s=="*"){
                    par.listaPar.push_back(makeR(tokeni.at(i+2)));
                    par.tipAdr=Adres::regind;
                    return par;
                  }else throw "bad 5 tokens juump\n";
                break;
              case 7:
                if(tokeni.at(i+3)=="+" && tokeni.at(i+5)=="]" && tokeni.at(i+1)=="["  && s=="*" && checkR(tokeni.at(i+2))){
                  par.listaPar.push_back(makeR(tokeni.at(i+2)));
                  bool provera=checkL(tokeni.at(i+4));
                  bool provs=checkS(tokeni.at(i+4));
                    if(provera){
                      par.listaPar.push_back(makeL(tokeni.at(i+4)));
                      par.tipAdr=Adres::regpom;
                      return par;
                    }else if(provs){
                      par.listaPar.push_back(makeS(tokeni.at(i+4)));
                      par.tipAdr=Adres::regpom;
                      return par;
                    }else{
                      throw "bad instruction *[reg+ nije ni lit ni sim\n";
                    }
                }else throw "bad 7 arg jump\n";
                break;
              default:
                throw "bad params jump\n";
            } 
          }
          break;
        case Instr::halt:
        case Instr::iret:
        case Instr::ret:
          if(tokeni.size()>1){
            throw "halt iret i ret moraju da stoje same\n";
          }
          return par;
          break;
        case Instr::Not:
        case Instr::push:
        case Instr::pop:
        case Instr::Int:
           if(tokeni.size()>2){
            throw "push pop not i int imaju samo 1 arg\n";
          }else{
            if(checkR(s)){
              par.listaPar.push_back(makeR(s));
              return par;
            }else{
              throw "push pop not i int imaju samo imaju registar kao parametar\n";
            }
          }
          break;
        default:
          throw "nepostojeca instrukcija\n";
          break;
        }
      }
    }
  }
  
  }catch(const char* exp){
    cout<<exp;
    exit(1);
  }
  return par;
}