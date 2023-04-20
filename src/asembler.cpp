#include "../inc/asembler.h"
void Asembler::add2bytes(short val)
{
    section->dodaj(val&0xff);
    section->dodaj(val>>8);
}
relZapis Asembler::patch(int simbol,int offset,bool pcrel){
  Simbol* sim=tabSim->dohvSimbol(simbol);
  relZapis zap;
  zap.offset=offset;
  if(sim->bind==tipBind::GLOB){
    zap.simbol=simbol;
    zap.addend=-2;
  }else{
    zap.simbol=sim->sekcija;
    zap.addend=sim->vrednost-2;
  }
  if(pcrel){
    zap.tip=R_X86_PC;
    return zap;
  }else{
    zap.tip = R_X86_16;
    zap.addend=zap.addend+2;
  }
  return zap;
}
void Asembler::simbolize(string naziv,bool pcrel){
  Simbol* sim=tabSim->getSim(naziv);
  if(!sim){
    ubaciSimbol(section->getBr(),tipBind::LOC,naziv);
  }
  Simbol* nsim=tabSim->getSim(naziv);
  if(nsim->definisan==false ){
    backZapis bz;
    if(pcrel)bz.tip=backTip::BACK_REL;
    else bz.tip=backTip::BACK_APS;
    bz.simbol=tabSim->getSimbol(naziv);
    bz.offset=section->LC();
    bz.gotov=false;
    section->backP()->ubaciBack(bz);
    add2bytes(0);
    return;
  }else{
    if(pcrel){
      if(section->getBr()==nsim->sekcija){
        add2bytes(nsim->vrednost-(section->LC()+2));
        return;
      }else{
        relZapis rz;
        switch(nsim->bind){
          case tipBind::LOC:
            rz.offset=section->LC();
            rz.simbol=nsim->sekcija;
            rz.addend=-2+nsim->vrednost;
            rz.tip=relTip::R_X86_PC;
            section->relTab()->dodaj(rz);
            add2bytes(0);
            return;
            break;
          case tipBind::GLOB:
            rz.offset=section->LC();
            rz.simbol=tabSim->getSimbol(naziv);
            rz.addend=-2;
            rz.tip=relTip::R_X86_PC;
            section->relTab()->dodaj(rz);
            add2bytes(0);
            return;
            break;
        }
      }
    }else{
      relZapis rz;
      switch(nsim->bind){
          case tipBind::LOC:
              rz.offset=section->LC();
              rz.simbol=nsim->sekcija;
              rz.addend=nsim->vrednost;
              rz.tip=relTip::R_X86_16;
              section->relTab()->dodaj(rz);
              add2bytes(0);
              return;
            break;
          case tipBind::GLOB:
              rz.offset=section->LC();
              rz.simbol=tabSim->getSimbol(naziv);
              rz.addend=0;
              rz.tip=relTip::R_X86_16;
              section->relTab()->dodaj(rz);
              add2bytes(0);
              return;
            break;
        }
    }
  }
}
void Asembler::razresi(){
  for(int i=0;i<sekcije.size();i++){
    for(int j=0;j<sekcije.at(i)->backP()->dohvTabeluB().size();j++){
     if(sekcije.at(i)->backP()->dohvTabeluB()[j].tip==backTip::BACK_APS){
        relZapis zap=patch(sekcije.at(i)->backP()->dohvTabeluB()[j].simbol,sekcije.at(i)->backP()->dohvTabeluB()[j].offset,false);
        sekcije.at(i)->relTab()->dodaj(zap);
        sekcije.at(i)->backP()->dohvTabeluB().at(j).gotov=true;
      }else{
        Simbol* sim=tabSim->dohvSimbol(sekcije.at(i)->backP()->dohvTabeluB()[j].simbol);
        if(sekcije.at(i)->getBr()!=sim->sekcija){
          relZapis zap=patch(sekcije.at(i)->backP()->dohvTabeluB()[j].simbol,sekcije.at(i)->backP()->dohvTabeluB()[j].offset,true);
          sekcije.at(i)->relTab()->dodaj(zap);
        }else{
          //pc rel je u istoj sekciji mozemo odmah da izmenimo
          sekcije.at(i)->dohvSadrzaj()[sekcije.at(i)->backP()->dohvTabeluB()[j].offset] = sim->vrednost - sekcije.at(i)->backP()->dohvTabeluB()[j].offset - 2;
          sekcije.at(i)->dohvSadrzaj()[sekcije.at(i)->backP()->dohvTabeluB()[j].offset+1] = (sim->vrednost - sekcije.at(i)->backP()->dohvTabeluB()[j].offset - 2)>> 8; 
        }
        sekcije.at(i)->backP()->dohvTabeluB().at(j).gotov=true;   
      }
    }
  }
}
string Asembler::printchar(unsigned char cc){
  char ss[10];
  sprintf(ss, "%02X", cc);
  string cs(ss);
  return cs;
}
bool Asembler::is_blank(string str, int N)
{ 
    for (int i = 0; i < N; i++) {
        if(isspace(str[i]))continue;
        else return false;
    }
    return true;
}
void Asembler::asemble(){
  ifstream inpt;
  ofstream outpt;
  inpt.open(ulazniFajl);
  outpt.open(izlazniFajl);
  try{
  string linija;
   while (getline(inpt, linija)){
      if (linija.size() == 0 || linija==" "||linija=="\n" || linija=="\t" || linija.empty() || is_blank(linija,linija.length())  || linija.front()=='#') {
          continue;
      }
      vector<string> vec;
      vec=lex->tokenize(linija);//tokenize line
      Parsed parsovana=parser->parsuj(vec);
      switch(parsovana.id){
        case 0:
          {switch(parsovana.direktiva){
            case Direktiva::global:
              cout<<"global--------\n";
            case Direktiva::eXtern:{
              cout<<"extern--------\n";
               for(int i=0;i<parsovana.listaPar.size();i++){
                string simb=parsovana.listaPar[i].argm;
                if(tabSim->getSim(simb)==nullptr){
                  ubaciSimbol(-1,tipBind::GLOB,simb);
                }else throw "simbol vec definisan\n";
              }
              break;
            }
            case Direktiva::section:{
              cout<<"section--------\n";
              string simb=parsovana.listaPar[0].argm;
                if(tabSim->getSim(simb)==nullptr){
                  if(section!=nullptr){
                    Simbol* sekc=tabSim->dohvSimbol(section->getBr());
                    tabSim->updateSize(sekc->naziv,section->LC());
                    sekcije.push_back(section);  
                  }
                  this->otvoriSekciju(simb);
                }else throw "sekcija vec definisana\n";
              break;
            }
            case Direktiva::skip:{
              cout<<"skip-----\n";
              int br=parsovana.listaPar[0].pom;
              while(br>0){
                section->dodaj(0);
                br--;
              }
              break;
            }
            case Direktiva::word:{
               cout<<"word------\n";
                for(int j=0;j<parsovana.listaPar.size();j++){
                  if(parsovana.listaPar[j].pom==-1){
                    simbolize(parsovana.listaPar[j].argm,false);
                  }else{
                    add2bytes(parsovana.listaPar[j].pom);
                  }
                }
              break;
            }
            case Direktiva::end:{
              cout<<"zavrsavamo\n";
              zavrsi=true;
              if(section!=nullptr){
                Simbol* sekc=tabSim->dohvSimbol(section->getBr());
                tabSim->updateSize(sekc->naziv,section->LC());
                sekcije.push_back(section);  
              }
              razresi();
              outpt<<"#TABELASIMBOLA "<<endl;
              for(int i=0;i<tabSim->dohvTabelu().size();i++){
                outpt<<"naziv "<<tabSim->dohvSimbol(i)->naziv<<" vrednost "<<tabSim->dohvSimbol(i)->vrednost<<" velicina "<<tabSim->dohvSimbol(i)->velicina<<" tipB "<<tabSim->dohvSimbol(i)->bind<<" tip "<<tabSim->dohvSimbol(i)->tip<<" sekcija "<<tabSim->dohvSimbol(i)->sekcija<<" definisan "<<tabSim->dohvSimbol(i)->definisan<<endl;
              }
              for(int j=0;j<sekcije.size();j++){
                outpt<<"rela."<<tabSim->dohvSimbol(sekcije.at(j)->getBr())->naziv<<endl;
                for(int k=0;k<sekcije.at(j)->relTab()->dohvTabelu().size();k++){
                  outpt<<"offset "<<sekcije.at(j)->relTab()->dohvZapis(k)->offset<<" tip "<<sekcije.at(j)->relTab()->dohvZapis(k)->tip<<" simbol "<<sekcije.at(j)->relTab()->dohvZapis(k)->simbol<<" addend "<<sekcije.at(j)->relTab()->dohvZapis(k)->addend<<endl;
                }
              }
              for(int j=0;j<sekcije.size();j++){
                outpt<<"sekcija."<<tabSim->dohvSimbol(sekcije.at(j)->getBr())->naziv<<endl;
                for (unsigned char item : sekcije.at(j)->dohvSadrzaj()) {
                  outpt<<printchar(item)<<" ";
                }
                outpt<<endl;
              }
              break;
            }
            default:
              throw "nepostojeca direktiva\n";
              break;
          }
          break;}
        case 1:
         { 
          section->dodaj(stoi(parsovana.opCode,nullptr,16));
          switch(parsovana.instrukcija){
              case Instr::ldr:
              case Instr::str:{
                  string registar="000"+to_string(parsovana.listaPar[0].brojReg);                 
                if(parsovana.tipAdr==Adres::immed){
                  string rr=registar;
                  int dod=stoi((std::bitset<4>(stoi(rr)).to_string()+std::bitset<4>(15).to_string()),0,2);
                  section->dodaj(dod);
                  section->dodaj(0);
                  if(parsovana.listaPar[1].pom==-1){
                    simbolize(parsovana.listaPar[1].argm,false);
                  }else{//literal je
                    add2bytes(parsovana.listaPar[1].pom);
                  }
                }else if(parsovana.tipAdr==Adres::regdir){
                  string rr=registar;
                  int dod=stoi((std::bitset<4>(stoi(rr)).to_string()+std::bitset<4>(bitset<4>(parsovana.listaPar[1].brojReg).to_string()).to_string()),0,2);
                  section->dodaj(dod);
                  section->dodaj(1);
                 //cout<<"ldr +regd--------\n";
                }else if(parsovana.tipAdr==Adres::apsol){
                  string rr=registar;
                  int dod=stoi((std::bitset<4>(stoi(rr)).to_string()+std::bitset<4>(15).to_string()),0,2);
                  section->dodaj(dod);
                  section->dodaj(4);
                  if(parsovana.listaPar[1].pom==-1){
                     simbolize(parsovana.listaPar[1].argm,false);
                  }else{//literal je
                    add2bytes(parsovana.listaPar[1].pom);
                  }
                  //cout<<"ldr +aps--------\n";
                }else if(parsovana.tipAdr==Adres::pcrel){
                  string rr=registar;
                  int dod=stoi((std::bitset<4>(stoi(rr)).to_string()+std::bitset<4>(7).to_string()),0,2);
                  section->dodaj(dod);
                  section->dodaj(3);
                  simbolize(parsovana.listaPar[1].argm,true);
                  //cout<<"ldr +pcr--------\n";
                }else if(parsovana.tipAdr==Adres::regind){
                  //cout<<"ldr +regi--------\n";
                  string rr=registar;
                  int dod=stoi((std::bitset<4>(stoi(rr)).to_string()+std::bitset<4>(bitset<4>(parsovana.listaPar[1].brojReg).to_string()).to_string()),0,2);
                  section->dodaj(dod);
                  section->dodaj(2);
                }else{
                  string rr=registar;
                  int dod=stoi((std::bitset<4>(stoi(rr)).to_string()+std::bitset<4>(bitset<4>(parsovana.listaPar[1].brojReg).to_string()).to_string()),0,2);
                  section->dodaj(dod);
                  section->dodaj(3);
                  if(parsovana.listaPar[2].pom==-1){
                     simbolize(parsovana.listaPar[2].argm,false);
                  }else{//literal je
                    add2bytes(parsovana.listaPar[2].pom);
                  }
                  //cout<<"ldr +regp--------\n";
                }
                break;
              }
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
              case Instr::shr:{
                section->dodaj(stoi((bitset<4>(parsovana.listaPar[0].brojReg).to_string()+bitset<4>(parsovana.listaPar[1].brojReg).to_string()),0,2));
                break;
              }
              case Instr::jmp:
              case Instr::call:
              case Instr::jeq:
              case Instr::jne:
              case Instr::jgt:{
                 if(parsovana.tipAdr==Adres::apsol){
                  section->dodaj(255);
                  section->dodaj(0);
                  if(parsovana.listaPar[0].pom==-1){
                     simbolize(parsovana.listaPar[0].argm,false);       
                  }else{//literal je
                    add2bytes(parsovana.listaPar[0].pom);
                  }
                }else if(parsovana.tipAdr==Adres::mdir){
                  section->dodaj(255);
                  section->dodaj(4);
                  if(parsovana.listaPar[0].pom==-1){
                     simbolize(parsovana.listaPar[0].argm,false);
                  }else{//literal je
                    add2bytes(parsovana.listaPar[0].pom);
                  }
                }else if(parsovana.tipAdr==Adres::regdir){
                  section->dodaj((240+parsovana.listaPar[0].brojReg));
                  section->dodaj(1);
                }else if(parsovana.tipAdr==Adres::regind){
                  section->dodaj((240+parsovana.listaPar[0].brojReg));
                  section->dodaj(2);
                }else if(parsovana.tipAdr==Adres::pcrel){
                  section->dodaj(247);
                  section->dodaj(3);
                   simbolize(parsovana.listaPar[0].argm,true); 
                }else if(parsovana.tipAdr==Adres::regpom){
                  section->dodaj((240+parsovana.listaPar[0].brojReg));
                  section->dodaj(3);
                  if(parsovana.listaPar[1].tip==Tip::simbol){
                    if(parsovana.listaPar[0].brojReg!=7){
                      simbolize(parsovana.listaPar[1].argm,false);                     
                    }else{
                      simbolize(parsovana.listaPar[1].argm,true);                     
                    }
                  }else{//literal je
                    add2bytes(parsovana.listaPar[1].pom);
                  }
                }
                break;
              }
              case Instr::halt:
              case Instr::iret:
              case Instr::ret:
                {break;}
              case Instr::Not:
                {section->dodaj(stoi((bitset<4>(parsovana.listaPar[0].brojReg).to_string()+"0"),0,2));
                break;}
              case Instr::push:
                {string regista="000"+to_string(parsovana.listaPar[0].brojReg);
                regista+=bitset<4>(6).to_string();
                section->dodaj(stoi(regista,0,2));
                section->dodaj(18);
                break;}
              case Instr::pop:
                {string registar="000"+to_string(parsovana.listaPar[0].brojReg);
                registar+=bitset<4>(6).to_string();
                section->dodaj(stoi(registar,0,2));
                section->dodaj(66);
                break;}
              case Instr::Int:
                {section->dodaj(stoi((bitset<4>(parsovana.listaPar[0].brojReg).to_string()+"15"),0,2));
                break;}
          }
          break;}
        case 2:
         { Simbol* si=tabSim->getSim(parsovana.labela);
          if(si==nullptr){
            ubaciSimbol(section->getBr(),tipBind::LOC,parsovana.labela);
            tabSim->updateLc(parsovana.labela,section->LC(),section->getBr());
          }else{
            if(!si->definisan && si->vrednost==0)tabSim->updateLc(parsovana.labela,section->LC(),section->getBr());
            else "vec definisana labela\n";
          }
          break;}
        default:
         { break;}
      }
    }
  }catch(const char* exp){
    cout<<exp;
    exit(1);
  }
  if (outpt.is_open())outpt.close();
  if (inpt.is_open())inpt.close();
}
void Asembler::ubaciSimbol(int sec,tipBind tip,string naz){
  Simbol s;
  s.naziv=naz;
  s.bind=tip;
  s.tip=tipSim::NO_TYPE;
  s.vrednost=0;
  s.definisan=false;
  s.sekcija=sec;
  s.velicina=0;
  tabSim->dodaj(s);
}
void Asembler::otvoriSekciju(string naziv){
  Simbol s;
  s.naziv=naziv;
  s.bind=tipBind::LOC;
  s.tip=tipSim::SCTN;
  s.vrednost=0;
  s.definisan=true;
  int br=this->tabSim->dohvTabelu().size();
  s.sekcija=br;
  s.velicina=0;
  this->tabSim->dodaj(s);
  Sekcija* sek=new Sekcija();
  sek->setBr(br);
  section=sek;  
}
