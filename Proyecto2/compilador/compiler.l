%{
  #include <string>
  #include <string.h>
  #include <stdlib.h>
  #include "y.tab.h"
  void yyerror(std::string S);
%}

%option yylineno

comment #.+
tag    [a-zA-Z_][a-zA-Z0-9_]*\:
label  [a-zA-Z_][a-zA-Z0-9_]*
imme   -?(0x)?[0-9a-fA-F]+

%%

    /*Operation Keywords*/
(a|A)(d|D)(d|D)         {yylval.id=strdup(yytext); return add;}
(a|A)(d|D)(d|D)(i|I)         {yylval.id=strdup(yytext); return addi;}
(s|S)(u|U)(b|B)         {yylval.id=strdup(yytext); return sub;}
(s|S)(u|U)(b|B)(i|I)         {yylval.id=strdup(yytext); return subi;}
(b|B)(e|E)(q|Q)         {yylval.id=strdup(yytext); return beq;}
(b|B)(n|N)(e|E)         {yylval.id=strdup(yytext); return bne;}
(l|L)(w|W)         {yylval.id=strdup(yytext); return lw;}
(l|L)(u|U)(i|I)         {yylval.id=strdup(yytext); return lui;}
(s|S)(w|W)         {yylval.id=strdup(yytext); return sw;}
    /*Types*/


    /*Registers*/
(\$)[0-3]?[0-9]         {yylval.id=strdup(yytext); return reg;}

{imme}                  {yylval.id=strdup(yytext); return imm;}

  /*Tags*/
{tag}                    {yylval.id=strdup(yytext); return tags;}
{label}                  {yylval.id=strdup(yytext); return labe;}
  /*Comments*/
{comment}                {yylval.id=strdup(yytext); return comm;}


[,\(\)\n\=\!]                 {return yytext[0];}
[\t \s]                   {;}
.                  {yyerror("caracter invalido");}
%%
int yywrap(void){
  return 1;
}
