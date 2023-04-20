#include "../inc/linker.h"
//kad se dodaje u listu novi simboli onda se proveri da li je globalni i definisan ako jedte dodaje se ako nije ne jer je eksterni negde drugde definisan
void Linker::pokreni(){
  ucitajFajlove();
  if(!poveziSimbole()){
    cout<<"error!! nisu svi simboli razreseni \n";
    return;
  }
  spojiSekcije();
  //cout<<"fixing relas\n";
  kopiraj();
  fixRelas();
  ispis();
}
void Linker::ispis(){
  outpt.open(izlaz);
  int k=0;
  for(unsigned char item:ceoSadrzaj){
    if(k==0){
      std::stringstream stream;
        stream << std::hex << k;
        outpt<<stream.str()<<": ";
    }
    else if(k%8==0){
        std::stringstream stream;
        stream << std::hex << k;
        outpt<<endl;
        outpt<<stream.str()<<": ";
    }
   char str[10];
   sprintf(str, "%02X", item);
   string out(str);
   outpt<<out<<" ";
    k++;
  }
  if (outpt.is_open())outpt.close();
}
int Linker::isSubstring(string s1, string s2)
{
    int M = s1.length();
    int N = s2.length();
    for (int i = 0; i <= N - M; i++) {
      int j;
      for (j = 0; j < M; j++)
          if (s2[i + j] != s1[j])break;
      if (j == M)return i;
    }
    return -1;
}
void Linker::ucitajFajlove(){
  try{
    for(int i=0;i<fajlovi.size();i++){
    SadrzajFajla* sadrzaj=new SadrzajFajla();
    sadrzaj->fajl=i;
    inpt.open(fajlovi[i]);
    string trenSek;
    std::string linija;
    vector<string> reci;
    while(getline(inpt,linija)){
      std::string rr="rela";
      std::string ssk="sekcija";
      if(isSubstring(rr, linija)!=-1){
       // cout<<"reeels\n";
        simboli=false;
        rels=true;
        vector<string> xx;
        split_str(linija,'.',xx);
        trenSek=xx.at(1);
        continue;
      }
      if(isSubstring(ssk, linija)!=-1 && !simboli){
       // cout<<"sekciccc\n";
        rels=false;
        sects=true;
        vector<string> xx;
        split_str(linija,'.',xx);
        trenSek=xx.at(1);
        continue;
      }
      if(linija=="#TABELASIMBOLA "){
        simboli=true;
        continue;
      }
      if(simboli){
        reci.clear();
        split_str(linija,' ',reci);
         Simbol sim;
         sim.naziv=reci[1];
         sim.vrednost=stoi(reci[3]);
         sim.velicina=stoi(reci[5]);
         int vez=stoi(reci[7]);
         if(vez==1)sim.bind=tipBind::LOC;
         else sim.bind=tipBind::GLOB;
         int tips=stoi(reci[9]);
         if(tips){
           sim.tip=tipSim::SCTN;
           Sekcija* sek=new Sekcija();
           sek->setBr(stoi(reci[11]));
           sek->ime=sim.naziv;
           sadrzaj->sekcije.push_back(sek);
         }
         else sim.tip=tipSim::NO_TYPE;
         sim.sekcija=stoi(reci[11]);
         int da=stoi(reci[13]);
         if(da==1)sim.definisan=true;
         else sim.definisan=false;
         //nalazimo u kom fajlu je definisan simbol 
         int isDef=nadjiSimbol(sim.naziv);
         sadrzaj->tabelaSim->dodaj(sim);
         if(sim.bind==tipBind::GLOB){
           if(sim.definisan){
             //proveri da ne postoji vec taj simbol ako postoji greska
             if(isDef==-1){
                sim.fajlDef=i;
                globalni->dodaj(sim);
             }else throw "vec je definisan simbol"; 
           }
           else {
             //ako je vec dodat u eksterni ne mora vise puta da ga dodajemo
             Simbol* check=eksterni->getSim(sim.naziv);
             if(check==nullptr){
                sim.fajlDef=-1;
                eksterni->dodaj(sim);
                fajloviEks.push_back(i);
             }
            }
         }
         continue;
      }
      if(rels){
        vector<string> reci;
        split_str(linija,' ',reci);
        relZapis rz;
        rz.offset=stoi(reci[1]);
        rz.simbol=stoi(reci[5]);
        int tiprel=stoi(reci[3]);
        if(tiprel==1)rz.tip=relTip::R_X86_16;
        else rz.tip=relTip::R_X86_PC;
        rz.addend=stoi(reci[7]);
        Simbol* trSek=sadrzaj->tabelaSim->getSim(trenSek);
        for(int k=0;k<sadrzaj->sekcije.size();k++){
          if(sadrzaj->sekcije[k]->getBr()==trSek->sekcija){
            sadrzaj->sekcije[k]->relTab()->dodaj(rz);
          }
        }
        continue;
      }
      if(sects){
        vector<string> reci;
        split_str(linija,' ',reci);
        Simbol* trSek=sadrzaj->tabelaSim->getSim(trenSek);
        for(int k=0;k<sadrzaj->sekcije.size();k++){
          if(sadrzaj->sekcije[k]->getBr()==trSek->sekcija){
            for(string rs:reci){
              sadrzaj->sekcije[k]->dodaj(stoi(rs,nullptr,16));
            }
          }
        }
        sects=false;
        continue;
      }
    }
    fileContent.push_back(sadrzaj);
    if (inpt.is_open())inpt.close();
  }
  }catch(const char* exp){
    cout<<exp;
    exit(1);
  }
}
int Linker::nadjiSimbol(string naziv){
  int f=-1;// -1 nije def -2 visestruko def 
  for(int i=0;i<fileContent.size();i++){
    Simbol* s=fileContent[i]->tabelaSim->getSim(naziv);
    if(s!=nullptr && s->definisan && f==-1)f=fileContent[i]->fajl;
    else if(s!=nullptr && s->definisan && f!=-1)f=-2;
  }
  return f;
}
void Linker::kopiraj(){
  for(int i=0;i<noveSekcije.size();i++){
    vector<unsigned char> sadr=noveSekcije[i]->dohvSadrzaj();
    for(int k=0;k<sadr.size();k++){
      ceoSadrzaj.push_back(sadr[k]);
    }
  }
}
int Linker::globVrednost(string naziv){
  for(int i=0;i<noviSimboli->dohvTabelu().size();i++){
    if(noviSimboli->dohvSimbol(i)->naziv==naziv)return noviSimboli->dohvSimbol(i)->vrednost;
  }
  return -1;
}
void Linker::izmeni(int mesto, int vrednost){
  ceoSadrzaj[mesto]=vrednost& 0xff;
  ceoSadrzaj[mesto+1]=vrednost>>8;
}
void Linker::fixRelas(){
  for(int i=0;i<fileContent.size();i++){
    for(int j=0;j<fileContent[i]->sekcije.size();j++){
      string imesek=fileContent[i]->sekcije[j]->ime;
      Sekcija* ss=findSek(imesek);
      for(int k=0;k<fileContent[i]->sekcije[j]->relTab()->dohvTabelu().size();k++){
        relZapis* rz=fileContent[i]->sekcije[j]->relTab()->dohvZapis(k);
        int P=rz->offset+offMoje(imesek,i);
        int S=globVrednost(fileContent[i]->tabelaSim->dohvSimbol(rz->simbol)->naziv);
        int A=rz->addend;
        if(rz->tip==relTip::R_X86_PC)izmeni(P,(S+A-P));
        else izmeni(P,(S+A));
      }
   }
  }
}
void Linker::spojiSekcije(){
  for(int i=0;i<fileContent.size();i++){
    for(int j=0;j<fileContent[i]->sekcije.size();j++){
      string imesek=fileContent[i]->sekcije[j]->ime;
      if(!exists(imesek)){
        //pravimo novu sekciju jer ne postoji s tim imenom
        Sekcija* sek=new Sekcija();
        sek->ime=fileContent[i]->sekcije[j]->ime;
        sek->setBr(noviSimboli->dohvTabelu().size()+1);
        sectionOffset sekof;
        sekof.file=i;
        sekof.offset=currOffset;
        sekof.naziv=imesek;
        offSekc.push_back(sekof);
        //ubacujemo njen sadrzaj
        vector<unsigned char> sadrzaj=fileContent[i]->sekcije[j]->dohvSadrzaj();
        for(auto ccc:sadrzaj){
          sek->dodaj(ccc);
        }
        //dodajemo je u tabelu novih sekcija
        noveSekcije.push_back(sek);
        //ubacujemo sekciju u tabelu simbola
        Simbol sim;
        sim.naziv=sek->ime;
        sim.sekcija=noviSimboli->dohvTabelu().size()+1;
        sim.tip=tipSim::SCTN;
        sim.bind=tipBind::LOC;
        sim.vrednost=currOffset;
        noviSimboli->dodaj(sim);
        //ubacujemo sve simbole koji su definisani u toj sekciji u novoj tabeli simbola
        for(int tt=0;tt<fileContent[i]->tabelaSim->dohvTabelu().size();tt++){
          if(fileContent[i]->tabelaSim->dohvSimbol(tt)->sekcija==fileContent[i]->sekcije[j]->getBr()){
            Simbol* dohvaceni=fileContent[i]->tabelaSim->dohvSimbol(tt);
            Simbol ubaci;
            ubaci.naziv=dohvaceni->naziv;
            ubaci.vrednost=dohvaceni->vrednost+currOffset;
            ubaci.sekcija=sim.sekcija;
            ubaci.bind=dohvaceni->bind;
            ubaci.tip=dohvaceni->tip;
            ubaci.definisan=true;
            ubaci.fajlDef=i;
            noviSimboli->dodaj(ubaci);
          }
        }
        //povecavamo offset za sadrzaj sekcije koju smo dodali
        currOffset+=sadrzaj.size();
        //trazimo u fajlovima posle ovog da li postoji sekcija sa trenutnim imenom da bi je nadovezali
        for(int k=i+1;k<fileContent.size();k++){
          for(int y=0;y<fileContent[k]->sekcije.size();y++){
            if(imesek==fileContent[k]->sekcije[y]->ime){
              sectionOffset sekof2;
              sekof2.file=k;
              sekof2.offset=currOffset;
              sekof2.naziv=imesek;
              offSekc.push_back(sekof2);
              vector<unsigned char> sadrzaj=fileContent[k]->sekcije[y]->dohvSadrzaj();
              //dodaj sadrzaj ove sekcije na kraj postojece
              for(auto ccc:sadrzaj){
                ubaciuSekciju(imesek,ccc);
              }
              //azuriraj tabelu simbola sa simbolima iz ove sekcije
              for(int tt=0;tt<fileContent[k]->tabelaSim->dohvTabelu().size();tt++){
                if(fileContent[k]->tabelaSim->dohvSimbol(tt)->sekcija==fileContent[k]->sekcije[y]->getBr()){
                  Simbol* dohvaceni=fileContent[k]->tabelaSim->dohvSimbol(tt);
                  Simbol ubaci;
                  ubaci.naziv=dohvaceni->naziv;
                  ubaci.vrednost=dohvaceni->vrednost+currOffset;
                  ubaci.sekcija=brojsek(imesek);
                  ubaci.bind=dohvaceni->bind;
                  ubaci.tip=dohvaceni->tip;
                  ubaci.definisan=true;
                  ubaci.fajlDef=i;
                  noviSimboli->dodaj(ubaci);
                }
              }
              currOffset+=sadrzaj.size();
            }
          }
       }
      }
    }
  }
}
Sekcija* Linker::findSek(string naziv){
  for(int i=0;i<noveSekcije.size();i++){
    if(noveSekcije[i]->ime==naziv)return noveSekcije[i];
  }
  return nullptr;
}
void Linker::ubaciuSekciju(string naziv,char c){
  for(int i=0;i<noveSekcije.size();i++){
    if(noveSekcije[i]->ime==naziv){
      noveSekcije[i]->dodaj(c);
      return;
    }
  }
}
bool Linker::exists(string naziv){
  for(int i=0;i<noveSekcije.size();i++){
    if(noveSekcije[i]->ime==naziv)return true;
  }
  return false;
}
void Linker::split_str( std::string const &str, const char delim,  std::vector <std::string> &out )  
{  
  std::stringstream s(str);  
  std::string s2;  
  while (std:: getline (s, s2, delim) ){  
    out.push_back(s2);  
  }  
} 
int Linker::nadjiSekciju(string naziv,int mojf){
  int offs=0;
  for(int i=0;i<noveSekcije.size();i++){
    if(noveSekcije.at(i)->ime==naziv)offs+=noveSekcije.at(i)->LC();
  }
  return 0;
}
int Linker::brojsek(string naziv){
  for(int i=0;i<noveSekcije.size();i++){
    if(noveSekcije[i]->ime==naziv)return i;
  }
  return -1;
}
int Linker::offMoje(string naziv,int fajl){
  for(int i=0;i<offSekc.size();i++){
    if(offSekc[i].naziv==naziv && offSekc[i].file==fajl)return offSekc[i].offset;
  }
  return -1;
}
bool Linker::poveziSimbole(){
  for(int i=0;i<eksterni->dohvTabelu().size();i++){
    Simbol* ss=eksterni->dohvSimbol(i);
    int pronasao=nadjiSimbol(ss->naziv);
    if(pronasao>=0){
      eksterni->setFajl(ss->naziv,pronasao);
    }else return false;
  }
  return true;
}
SadrzajFajla* Linker::nadjiFajl(int ind){
  for(int i=0;i<fileContent.size();i++){
    if(fileContent[i]->fajl==ind)return fileContent[i];
  }
  return nullptr;
}