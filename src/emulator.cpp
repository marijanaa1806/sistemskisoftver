#include "../inc/emulator.h"
void Emulator::splitstr( std::string const &str, const char delim,  std::vector <std::string> &out ){
	std::stringstream s(str);  
  std::string s2;  
  while (std:: getline (s, s2, delim) ){  
    out.push_back(s2);  
  }  
}
void Emulator::ucitaj(){
	kodovi.insert(pair<int,string>(0,"halt"));
	kodovi.insert(pair<int,string>(16,"int"));
	kodovi.insert(pair<int,string>(32,"iret"));
	kodovi.insert(pair<int,string>(48,"call"));
	kodovi.insert(pair<int,string>(64,"ret"));
	kodovi.insert(pair<int,string>(80,"jmp"));
	kodovi.insert(pair<int,string>(81,"jeq"));
	kodovi.insert(pair<int,string>(82,"jne"));
	kodovi.insert(pair<int,string>(83,"jgt"));
	kodovi.insert(pair<int,string>(96,"xchg"));
	kodovi.insert(pair<int,string>(112,"add"));
	kodovi.insert(pair<int,string>(113,"sub"));
	kodovi.insert(pair<int,string>(114,"mul"));
	kodovi.insert(pair<int,string>(115,"div"));
	kodovi.insert(pair<int,string>(116,"cmp"));
	kodovi.insert(pair<int,string>(128,"not"));
	kodovi.insert(pair<int,string>(129,"and"));
	kodovi.insert(pair<int,string>(130,"or"));
	kodovi.insert(pair<int,string>(131,"xor"));
	kodovi.insert(pair<int,string>(132,"test"));
	kodovi.insert(pair<int,string>(144,"shl"));
	kodovi.insert(pair<int,string>(145,"shr"));
	kodovi.insert(pair<int,string>(160,"ldr"));
	kodovi.insert(pair<int,string>(176,"str"));
	for(int k=0;k<8;k++){
		CPU.reg[k]=0;
	}
  for(int u=0;u<65536;u++)mem[u]=0;
  
	CPU.ivt=0;
	inpt.open(ul);
	std::string linija;
	int i=0;
  while(getline(inpt,linija)){
		vector<string> reci;
		splitstr(linija,' ',reci);   
      for(int m=1;m<reci.size();m++){
        content.push_back(reci[m]);
      }
     for(int t=0;t<reci.size()-1;t++){
       char ubaci=stoi(reci[t+1],nullptr,16);
       mem[t+i]=ubaci;
      }
      i+=reci.size()-1;
	}
}
void Emulator::inc2b(){
  CPU.reg[PC]=CPU.reg[PC]+2;
}
void Emulator::inc3b(){
  CPU.reg[PC]=CPU.reg[PC]+3;
}
void Emulator::inc5b(){
  CPU.reg[PC]=CPU.reg[PC]+5;
}
void Emulator::exec2reg(){
      inc2b();
  		if(currIns.instruction=="xchg"){
        short temp=CPU.reg[currIns.regDes];
        CPU.reg[currIns.regDes]=CPU.reg[currIns.regSrc];
        CPU.reg[currIns.regSrc]=temp;
      }
      else if(currIns.instruction=="shl"){
        CPU.reg[currIns.regDes]=CPU.reg[currIns.regDes]<<CPU.reg[currIns.regSrc];
        if(CPU.reg[currIns.regDes]&0x8000)setC();
        if(CPU.reg[currIns.regDes]==0)setZ();
        if(CPU.reg[currIns.regDes]<0)setN();
      }
      else if(currIns.instruction=="shr"){
        CPU.reg[currIns.regDes]=CPU.reg[currIns.regDes]>>CPU.reg[currIns.regSrc];
        if(CPU.reg[currIns.regDes]&0x0001)setC();
        if(CPU.reg[currIns.regDes]==0)setZ();
        if(CPU.reg[currIns.regDes]<0)setN();
      }
      else if(currIns.instruction=="add"){
        CPU.reg[currIns.regDes]=CPU.reg[currIns.regDes]+CPU.reg[currIns.regSrc];
      }
      else if(currIns.instruction=="sub"){
         CPU.reg[currIns.regDes]=CPU.reg[currIns.regDes]-CPU.reg[currIns.regSrc];
      }
      else if(currIns.instruction=="mul"){
         CPU.reg[currIns.regDes]=CPU.reg[currIns.regDes]*CPU.reg[currIns.regSrc];  
      }
      else if(currIns.instruction=="div"){
         CPU.reg[currIns.regDes]=CPU.reg[currIns.regDes]/CPU.reg[currIns.regSrc];   
      }
      else if(currIns.instruction=="cmp"){
        short rez=CPU.reg[currIns.regDes]-CPU.reg[currIns.regSrc];  
        if(rez<0)setN();
        if(rez==0)setZ();
        if(rez>0 && CPU.reg[currIns.regDes]<0 && ~CPU.reg[currIns.regDes]<0)setO();
        if(rez<0 && CPU.reg[currIns.regDes]>0 && ~CPU.reg[currIns.regDes]>0)setO();

        //o i v namesti
      }
      else if(currIns.instruction=="and"){
        CPU.reg[currIns.regDes]=CPU.reg[currIns.regDes]&CPU.reg[currIns.regSrc];
      }
      else if(currIns.instruction=="or"){
        CPU.reg[currIns.regDes]=CPU.reg[currIns.regDes]|CPU.reg[currIns.regSrc];
      }
      else if(currIns.instruction=="xor"){
        CPU.reg[currIns.regDes]=CPU.reg[currIns.regDes]^CPU.reg[currIns.regSrc];  
      }
      else if(currIns.instruction=="test"){
        short rez=CPU.reg[currIns.regDes]&CPU.reg[currIns.regSrc]; 
        if(rez<0)setN();
        if(rez==0)setZ();
      }
      clearIns();
}
void Emulator::clearIns(){
  currIns.adrs=Adres::mind; //mind jer se ne koristi jedino nzm sto san stavila
  currIns.regDes=-1;
  currIns.regSrc=-1;
  currIns.instruction="";
  currIns.operand="";
}
void Emulator::exec1reg(){
  if(currIns.instruction=="int"){
    CPU.reg[SP]-=2;
    char low1;
    char high1;
    makeChars(CPU.reg[PC],high1,low1);
    mem[CPU.reg[SP]]=low1;
    mem[CPU.reg[SP]+1]=high1;
    CPU.reg[SP]-=2;
    char low;
    char high;
    makeChars(psw,high,low);
    mem[CPU.reg[SP]]=low;
    mem[CPU.reg[SP]+1]=high;
    makeShort(CPU.reg[PC],mem[(CPU.reg[currIns.regDes]%8)*2+1],mem[(CPU.reg[currIns.regDes]%8)*2]);
  }
  else if(currIns.instruction=="not"){
    CPU.reg[currIns.regDes]=~CPU.reg[currIns.regDes];
  }
  clearIns();
}
void Emulator::execLdr(){
  //cout<<"u exec ldr smo, operand je "<<currIns.operand<<"\n";
 /* if(currIns.regDes==3){
    cout<<"vrednost u reg 3 je "<<CPU.reg[currIns.regDes]<<"\n";
    cout<<"adresiranje je "<<currIns.adrs<<"\n";
    cout<<"vr reg je"<<CPU.reg[currIns.regSrc]<<" a registar je "<<currIns.regSrc<<"\n";

  }*/
  short toLoad;
  if(currIns.adrs==Adres::immed){
    toLoad=stoi(currIns.operand,0,16);
  }
  else if(currIns.adrs==Adres::regdir){
    toLoad=CPU.reg[currIns.regSrc];
  }
  else if(currIns.adrs==Adres::regind){
    makeShort(toLoad,mem[CPU.reg[currIns.regSrc]+1],mem[CPU.reg[currIns.regSrc]]);
  }
  else if(currIns.adrs==Adres::regpom){
     makeShort(toLoad,mem[(CPU.reg[currIns.regSrc]+stoi(currIns.operand,0,16))+1],mem[(CPU.reg[currIns.regSrc]+stoi(currIns.operand,0,16))]);
  }
  else if(currIns.adrs==Adres::apsol){
   // cout<<"memorija "<<ispisiznak(mem[(stoi(currIns.operand,0,16))+1])<<" a niza "<<ispisiznak(mem[(stoi(currIns.operand,0,16))])<<"\n";
    makeShort(toLoad,mem[(stoi(currIns.operand,0,16))+1],mem[(stoi(currIns.operand,0,16))]);
  }
  CPU.reg[currIns.regDes]=toLoad;
  clearIns();
}
string Emulator::ispisiznak(char c){
  char ss[10];
  sprintf(ss, "%02X", c);
  string sc(ss);
  return sc;
}
void Emulator::push(){
   CPU.reg[SP]-=2;
   char low;
   char high;
   //cout<<"pravimo char od "<<CPU.reg[currIns.regDes]<<"\n";
   makeChars(CPU.reg[currIns.regDes],high,low);
   mem[CPU.reg[currIns.regSrc]]=low;
  mem[CPU.reg[currIns.regSrc]+1]=high;
  clearIns();
}
void Emulator::pop(){
  short toLoad;
  makeShort(toLoad,mem[CPU.reg[currIns.regSrc]+1],mem[CPU.reg[currIns.regSrc]]);
  CPU.reg[SP]+=2;
  CPU.reg[currIns.regDes]=toLoad;
  clearIns();
}
void Emulator::makeShort(short& tb,char c1,char c2){
  tb = (((short)c1) << 8) | (0x00ff & c2);
}
void Emulator::makeChars(short tb,char& c1,char& c2){
  c1 = tb >> 8;
  c2 = tb & 0x00ff; 
}
void Emulator::checkUslov(){
  if(currIns.instruction=="jgt" && (~psw & pswN && ~psw & pswO))uslov=true;
  else if(currIns.instruction=="jgt" && (psw & pswN && psw & pswO))uslov=true;
  else if(currIns.instruction=="jeq" &&(psw & pswZ))uslov=true;
  else if(currIns.instruction=="jne" &&(~psw & pswZ))uslov=true;
  else if(currIns.instruction=="jmp")uslov=true;
}
void Emulator::execStr(){
  char low,high;
  if(currIns.adrs==Adres::immed)return;
  else if(currIns.adrs==Adres::regdir)CPU.reg[currIns.regSrc]=CPU.reg[currIns.regDes];
  else if(currIns.adrs==Adres::regind){
    makeChars(CPU.reg[currIns.regDes],high,low);
    mem[CPU.reg[currIns.regSrc]]=low;
    mem[CPU.reg[currIns.regSrc]+1]=high;
  }
  else if(currIns.adrs==Adres::regpom){
    makeChars(CPU.reg[currIns.regDes],high,low);
    mem[(CPU.reg[currIns.regSrc]+stoi(currIns.operand,0,16))]=low;
    mem[(CPU.reg[currIns.regSrc]+stoi(currIns.operand,0,16))+1]=high;
  }
  else if(currIns.adrs==Adres::apsol){
    makeChars(CPU.reg[currIns.regDes],high,low);
    mem[stoi(currIns.operand,0,16)]=low;
    mem[stoi(currIns.operand,0,16)+1]=high;
  }
  clearIns();
}
void Emulator::execJmps(){
  if(currIns.adrs==Adres::regpom && currIns.regSrc==PC){
   CPU.reg[PC]=CPU.reg[PC]+stoi(currIns.operand,0,16);
  }
  else{
    subroutine();
  }
  uslov=false;
  clearIns();
}
void Emulator::subroutine(){
  if(currIns.adrs==Adres::immed){
    CPU.reg[PC]=stoi(currIns.operand,0,16);
  }
  else if(currIns.adrs==Adres::regdir){
    CPU.reg[PC]=CPU.reg[currIns.regSrc];
  }
  else if(currIns.adrs==Adres::regind){
    makeShort(CPU.reg[PC],mem[CPU.reg[currIns.regSrc]+1],mem[CPU.reg[currIns.regSrc]]);
  }
  else if(currIns.adrs==Adres::regpom && currIns.regSrc==PC)CPU.reg[PC]=CPU.reg[PC]+stoi(currIns.operand,0,16);
  else if(currIns.adrs==Adres::regpom){
   makeShort(CPU.reg[PC],mem[(CPU.reg[currIns.regSrc]+stoi(currIns.operand,0,16))+1],mem[(CPU.reg[currIns.regSrc]+stoi(currIns.operand,0,16))]);
  }
  else if(currIns.adrs==Adres::apsol){
    makeShort(CPU.reg[PC],mem[stoi(currIns.operand,0,16)+1],mem[stoi(currIns.operand,0,16)]);
  }
}
void Emulator::execCall(){
  CPU.reg[SP]-=2;
  char low;
  char high;
  makeChars(CPU.reg[PC],high,low);
  mem[CPU.reg[SP]]=low;
  mem[CPU.reg[SP]+1]=high;
  subroutine();
  clearIns();
}
void Emulator::adrFaza(string aa){
  int i=stoi(aa);
  switch(i){
    case 0:
      currIns.adrs=Adres::immed;
      break;
    case 1:
      currIns.adrs=Adres::regdir;
      break;
    case 2:
      currIns.adrs=Adres::regind;
      break;
    case 3:
      currIns.adrs=Adres::regpom;
      break;
    case 4:
      currIns.adrs=Adres::apsol;
      break;
  }
}
void Emulator::setN(){
  psw=psw|pswN;
}
void Emulator::setZ(){
  psw=psw|pswZ;
}
void Emulator::setO(){
  psw=psw|pswO;
}
void Emulator::setC(){
  psw=psw|pswC;
}
void Emulator::emulate(){
  CPU.reg[PC]=92;
	while(!end){
    clearIns();
		string instrukcija;
    int pos=CPU.reg[PC];
   string nn=content[pos];
    int pret=stoi(nn,0,16);
		map<int,string>::iterator it = kodovi.find(pret);
		if (it != kodovi.end()){
			 instrukcija=it->second;
		}
    //cout<<"instrukcija jeee "<<instrukcija<<"\n";
		if(instrukcija=="ldr"){
     string regs=content[pos+1];
      currIns.regDes=stoi(regs.substr(0,1));
      currIns.regSrc=stoi(regs.substr(1,1),0,16);
      if(content[pos+2]=="42"){
        inc3b();
        pop();
      }else{
        adrFaza(content[pos+2]);
        if(currIns.adrs!=Adres::regdir && currIns.adrs!=Adres::regind){
          currIns.operand=content[pos+4]+content[pos+3];
          inc5b();
        }else inc3b();
        currIns.instruction=instrukcija;
        execLdr();
      }
		}
		else if(instrukcija=="str"){
			
      string regs=content[pos+1];
      currIns.regDes=stoi(regs.substr(0,1));
      currIns.regSrc=stoi(regs.substr(1,1),0,16);
      //cout<<"regd "<<currIns.regDes<<" regs "<<currIns.regSrc<<"\n";
      if(content[pos+2]=="12"){
        inc3b();
        push();
      }
      else{
        adrFaza(content[pos+2]);
        if(currIns.adrs!=Adres::regdir && currIns.adrs!=Adres::regind){
          currIns.operand=content[pos+4]+content[pos+3];
          inc5b();
        }else inc3b();
        currIns.instruction=instrukcija;
        execStr();
      } 
		}
		else if(instrukcija=="halt"){
      CPU.reg[PC]+=1;
      end=true;
      break;
    }
		else if(instrukcija=="iret"){
      char psw1=mem[CPU.reg[SP]];//low
      char psw2=mem[CPU.reg[SP]+1];//high
      CPU.reg[SP]=CPU.reg[SP]+2;
      char pc1=mem[CPU.reg[SP]];
      char pc2=mem[CPU.reg[SP]+1];
      CPU.reg[SP]=CPU.reg[SP]+2;
      makeShort(psw,psw2,psw1);
      makeShort(CPU.reg[PC],pc2,pc1);
    }
		else if(instrukcija=="int" || instrukcija=="not"){
      string regs=content[pos+1];
      currIns.regDes=stoi(regs.substr(0,1));
      currIns.instruction=instrukcija;
      inc2b();
      exec1reg();
    }
		else if(instrukcija=="call"){
      //treba da pushujemo pc nakon sto ucita celu instrukciju call
      string regs=content[pos+1];
      currIns.regDes=stoi(regs.substr(0,1),0,16);
      currIns.regSrc=stoi(regs.substr(1,1),0,16);
      adrFaza(content[pos+2]);
      if(currIns.adrs!=Adres::regdir && currIns.adrs!=Adres::regind){
          currIns.operand=content[pos+4]+content[pos+3];
          inc5b();
      }else inc3b();
      currIns.instruction=instrukcija;
      execCall();
    }
		else if(instrukcija=="ret"){
      char pc1=mem[CPU.reg[SP]];
      char pc2=mem[CPU.reg[SP]+1];
      CPU.reg[SP]=CPU.reg[SP]+2;
      makeShort(CPU.reg[PC],pc2,pc1);
    }
		else if(instrukcija=="xchg" || instrukcija=="shl" || instrukcija=="shr" || instrukcija=="add" || instrukcija=="sub" || instrukcija=="mul" || instrukcija=="div" || instrukcija=="cmp" || instrukcija=="and" || instrukcija=="or" || instrukcija=="xor" || instrukcija=="test"){
      string regs=content[pos+1];
      currIns.regDes=stoi(regs.substr(0,1));
      currIns.regSrc=stoi(regs.substr(1,1));
      currIns.instruction=instrukcija;
      exec2reg();
    }
		else if(instrukcija=="jmp" || instrukcija=="jeq" || instrukcija=="jne" || instrukcija=="jgt"){
      string regs=content[pos+1];
      currIns.regDes=stoi(regs.substr(0,1),0,16);
      currIns.regSrc=stoi(regs.substr(1,1),0,16);
      adrFaza(content[pos+2]);
      if(currIns.adrs!=Adres::regdir && currIns.adrs!=Adres::regind){
          currIns.operand=content[pos+4]+content[pos+3];
          inc5b();
      }else inc3b();
      currIns.instruction=instrukcija;
      checkUslov();
      if(uslov)execJmps();
    } 
	}
}
void Emulator::ispis(){
  cout<<"Emulated processor executed halt instruction\n";
  cout<<"Emulated processor state: psw=0b"<<std::bitset<16>(psw).to_string()<<"\n";
  cout<<"r0=0x"<<wrReg(CPU.reg[0])<<" r1=0x"<<wrReg(CPU.reg[1])<<" r2=0x"<<wrReg(CPU.reg[2])<<" r3=0x"<<wrReg(CPU.reg[3])<<"\n";
  cout<<"r4=0x"<<wrReg(CPU.reg[4])<<" r5=0x"<<wrReg(CPU.reg[5])<<" r6=0x"<<wrReg(CPU.reg[6])<<" r7=0x"<<wrReg(CPU.reg[7])<<"\n";
  if(inpt.is_open())inpt.close();
}
string Emulator::wrReg(short rr){
  std::stringstream stream;
  stream << std::hex << rr;
  return stream.str();
}