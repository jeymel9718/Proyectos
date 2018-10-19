%{

    #include <stdio.h>
    #include <iostream>
    #include <string.h>
    #include <bitset>
    #include <vector>
    #include <map>
    #include <stdlib.h>
    #include <algorithm>
    #include <cstdlib>
    #include <fstream>
    std::map<std::string,int> labels; //Etiquetas y valores
    std::map<std::string,int> futureLabels; //En caso de encontrar una etiqueta antes de ser declarada
    std::fstream fs;
    std::fstream fs2;
    std::string final_message="Compiler success";
    int memCount=0;

    int yylex();
    extern int yylineno;
    void encondig_instruccion(std::string op,std::string rd,std::string rs,std::string rt);
    void encondig_instruccion1(std::string op,std::string rt,std::string rs,std::string imme);
    void encondig_instruccion2(std::string op,std::string rt,std::string rs,std::string tag);
    void encondig_instruccion3(std::string op,std::string rt,std::string rs,std::string imme);
    void encondig_instruccion4(std::string op,std::string rt,std::string imme);
    std::string regtobin(std::string regis);
    std::string immtobin(std::string in,int type);
    void procces_label(std::string tag,std::string g,int type);
    void variablestobin(int val);
    std::string current_type="DCD";
    int data_memory=0x10000000;
    int text_memory=0x00000000;
    void yyerror(std::string S);
    void printt(std::string s);

%}

%union{
  char* id;
  int num;
}
//lex tokens
%token <id> add addi sub subi beq bne lw lui sw
%token <id> reg
%token <id> imm
%token <id> tags labe
%token <id> comm
//yacc tokens
%type <id> operation
%%

program : program line {;}
        |
        ;

line : {;}
     | tags  {procces_label($1,"",1);}
     | comm  {;}
     | instruction  {;}
     | line '\n' {;}
     ;

instruction : operation reg ',' reg ',' reg {encondig_instruccion($1,$2,$4,$6);}
            | operation reg ',' reg ',' imm {encondig_instruccion1($1,$2,$4,$6);}
            | operation reg ',' reg ',' labe {encondig_instruccion2($1,$2,$4,$6);}
            | operation reg ',' imm  '(' reg ')' {encondig_instruccion3($1,$2,$6,$4);}
            | operation reg ',' imm {encondig_instruccion4($1,$2,$4);}
            ;

operation : add {;}
          | addi {;}
          | sub {;}
          | subi {;}
          | beq {;}
          | bne {;}
          | lw  {;}
          | lui {;}
          | sw  {;}
          ;

%%

extern int yyparse();
extern FILE *yyin;
std::string ruta="";

//Instruccion op rd,rs,rm
void encondig_instruccion(std::string op,std::string rd,std::string rs,std::string rt){
  std::string binIns="";
  text_memory+=0x4;
  if(op.compare("Add")==0 || op.compare("ADD")==0 || op.compare("add")==0){
    binIns+="000000";
    binIns+=regtobin(rs);
    binIns+=regtobin(rt);
    binIns+=regtobin(rd);
    binIns+="00000100000";
    fs<<binIns<<'\n';
  }else if(op.compare("Sub")==0 || op.compare("sub")==0 || op.compare("SUB")==0){
    binIns+="000000";
    binIns+=regtobin(rs);
    binIns+=regtobin(rt);
    binIns+=regtobin(rd);
    binIns+="00000100010";
    fs<<binIns<<'\n';
  }else{
    yyerror("Error: instruccion no reconocida");
  }
}

void encondig_instruccion1(std::string op,std::string rt,std::string rs,std::string imme){
  std::string binIns="";
  text_memory+=0x4;
  if(op.compare("Addi")==0 || op.compare("ADDI")==0 || op.compare("addi")==0){
    binIns+="001000";
    binIns+=regtobin(rs);
    binIns+=regtobin(rt);
    binIns+=immtobin(imme,1);
    fs<<binIns<<'\n';
  }else if(op.compare("Subi")==0 || op.compare("SUBI")==0 || op.compare("subi")==0){
    binIns+="001000";
    binIns+="00000";
    binIns+="00001";
    binIns+=immtobin(imme,1);
    fs<<binIns<<'\n';
    text_memory+=0x4;
    binIns="";
    binIns+="000000";
    binIns+=regtobin(rs);
    binIns+="00001";
    binIns+=regtobin(rt);
    binIns+="00000100010";
    fs<<binIns<<'\n';
  }else{
    yyerror("Error: instruccion no reconocida");
  }

}

void encondig_instruccion2(std::string op,std::string rt,std::string rs,std::string tag){
  std::string binIns="";
  text_memory+=0x4;
  if(op.compare("Beq")==0 || op.compare("BEQ")==0 || op.compare("beq")==0){
    int index=labels.find(tag)->second;
    if(index == 0){
      futureLabels[tag]=fs.tellp();
      index=0;
    }
    int result=(index-text_memory)/4;
    binIns+="000100";
    binIns+=regtobin(rt);
    binIns+=regtobin(rs);
    binIns+=std::bitset<16>(result).to_string();
    fs<<binIns<<'\n';
  }else if(op.compare("Bne")==0 || op.compare("BNE")==0 || op.compare("bne")==0){
    int index=labels.find(tag)->second;
    if(index == 0){
      futureLabels[tag]=fs.tellp();
      index=0;
    }
    int result=(index-text_memory)/4;
    binIns+="000101";
    binIns+=regtobin(rt);
    binIns+=regtobin(rs);
    binIns+=std::bitset<16>(result).to_string();
    fs<<binIns<<'\n';
  }else{
    yyerror("Error: instruccion no reconocida");
  }
}

void encondig_instruccion3(std::string op,std::string rt,std::string rs,std::string imme){
  std::string binIns="";
  text_memory+=0x4;
  if(op.compare("Lw")==0 || op.compare("LW")==0 || op.compare("lw")==0){
    binIns+="100011";
    binIns+=regtobin(rs);
    binIns+=regtobin(rt);
    binIns+=immtobin(imme,1);
    fs<<binIns<<'\n';
  }else if(op.compare("Sw")==0 || op.compare("SW")==0 || op.compare("sw")==0){
    binIns+="101011";
    binIns+=regtobin(rs);
    binIns+=regtobin(rt);
    binIns+=immtobin(imme,1);
    fs<<binIns<<'\n';
  }else{
    yyerror("Error: instruccion no reconocida");
  }
}

void encondig_instruccion4(std::string op,std::string rt,std::string imme){
  std::string binIns="";
  text_memory+=0x4;
  if(op.compare("Lui")==0 || op.compare("LUI")==0 || op.compare("lui")==0){
    binIns+="001111";
    binIns+="00000";
    binIns+=regtobin(rt);
    binIns+=immtobin(imme,1);
    fs<<binIns<<'\n';
  }else{
    yyerror("Error: instruccion no reconocida");
  }
}

std::string regtobin(std::string regis){
  regis.erase(0,1);
  std::string bin=std::bitset<5>(atoi(regis.c_str())).to_string();
  return bin;
}

void variablestobin(int val){
  if(current_type.compare("DCB")==0){
    std::string bin=std::bitset<8>(val).to_string();
    if(memCount>2){
      memCount=0;
      fs2<<bin<<'\n';
    }else{
      memCount++;
      fs2<<bin;
    }
  }else if(current_type.compare("DCW")==0){
    std::string bin=std::bitset<16>(val).to_string();
    if(memCount>1){
      memCount=0;
      fs2<<bin<<'\n';
    }else{
      memCount++;
      fs2<<bin;
    }
  }else if(current_type.compare("DCD")==0){
    std::string bin=std::bitset<32>(val).to_string();
    fs2<<bin<<'\n';
  }
}

void procces_label(std::string tag,std::string g,int type){
  tag=tag.substr(0, tag.size()-1);
  int tmp=futureLabels.find(tag)->second;
  if(tmp > 0){ //Se encontro una etiqueta usada por una instruccion antes de declararse
    int tposition=fs.tellp();
    int result=(0x4*tmp/33);
    result=(text_memory-result)/4;
    fs.seekp(tmp+16);
    fs<<std::bitset<16>(result).to_string();
    fs.seekp(tposition);
  }
  if(type==1){
    labels[tag]=text_memory; //Valor de la etiqueta
  }else if(type==2){
    current_type=g;
    labels[tag]=data_memory;
  }
}

//Se guarda el inmediato en binario
//Type= De que tamaño sera el inmediato
std::string immtobin(std::string in,int type){
  if(type==1){
    return std::bitset<16>(atoi(in.c_str())).to_string();
  }else if(type==2){
    return std::bitset<26>(atoi(in.c_str())).to_string();
  }
}

void printt(std::string s){
  std::cout << s << std::endl;
}

void yyerror(std::string S){
  final_message="Compiler failed";
  std::cout << S <<" at line: "<<yylineno<<'\n';
}

int main(void) {
  std::cout<<"Ruta del archivo a compilar"<<'\n';
  fs.open ("text.txt", std::ios::out | std::ios::trunc); //Intrucciones
  fs2.open ("data.txt", std::ios::out | std::ios::trunc); //Datos
  std::cin>>ruta;
  FILE *myfile = fopen(ruta.c_str(), "r");
	//se verifica si es valido
	if (!myfile) {
		std::cout << "No es posible abrir el archivo" << std::endl;
		return -1;
	}
  fs<<"11100001101000000000000000000000"<<'\n';
	yyin = myfile;
	do {
		yyparse();
	} while (!feof(yyin));
  fs.close();
  fs2.close();
  std::cout<<final_message<<'\n';
  for(int i=0;i<100;++i);
}
