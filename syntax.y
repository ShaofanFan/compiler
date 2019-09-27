%{
#include <stdio.h>
#include "tree.h"
extern int yylineno;
struct TREE_NODE *root;
int yylex();
void yyerror (char *msg);
extern int error1;
%}
%token INT FLOAT TYPE 
%token ASSIGNOP RELOP PLUS MINUS1 MINUS2 STAR DIV 
%token AND OR NOT 
%token SEMI COMMA 
%token LP RP LB RB LC RC DOT
%token ID STRUCT RETURN IF ELSE WHILE

%union{
   struct TREE_NODE* r;
}

%locations

%type <r> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag 
Tag VarDec FunDec VarList Dec ParamDec CompSt StmtList Stmt DefList DecList Def Exp INT FLOAT TYPE ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR NOT SEMI COMMA LP RP LB RB LC RC DOT ID STRUCT RETURN IF ELSE WHILE Args

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT  

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%%
Program:ExtDefList{root=createnode(@$.first_line,"Program",1,$1);}
  ;
ExtDefList:/*empty*/
        {$$=createnode(@$.first_line,"ExtDefList",0);}
  |ExtDef ExtDefList{$$=createnode(@$.first_line,"ExtDefList",2,$1,$2);}
  ;
ExtDef:Specifier ExtDecList SEMI
	{$$=createnode(@$.first_line,"ExtDef",3,$1,$2,$3);}
  |Specifier SEMI
	{$$=createnode(@$.first_line,"ExtDef",2,$1,$2);}
  |Specifier FunDec CompSt
	{$$=createnode(@$.first_line,"ExtDef",3,$1,$2,$3);}
  |Specifier error SEMI
  |Specifier ExtDecList error SEMI
  ;
ExtDecList:VarDec
	{$$=createnode(@$.first_line,"ExtDecList",1,$1);}
  |VarDec COMMA ExtDecList
	{$$=createnode(@$.first_line,"ExtDecList",3,$1,$2,$3);}
  ;

Specifier:TYPE
	{$$=createnode(@$.first_line,"Specifier",1,$1);}
  |StructSpecifier
	{$$=createnode(@$.first_line,"Specifier",1,$1);}
  ;
StructSpecifier:STRUCT OptTag LC DefList RC
	{$$=createnode(@$.first_line,"StructSpecifier",5,$1,$2,$3,$4,$5);}
  |STRUCT Tag
	{$$=createnode(@$.first_line,"StructSpecifier",2,$1,$2);}
  |STRUCT OptTag LC DefList error RC
  ;
OptTag:/*empty*/
        {$$=createnode(@$.first_line,"OptTag",0);}
  |ID
	{$$=createnode(@$.first_line,"OptTag",1,$1);}
  ;
Tag:ID
	{$$=createnode(@$.first_line,"Tag",1,$1);}
  ;

VarDec:ID
	{$$=createnode(@$.first_line,"VarDec",1,$1);}
  |VarDec LB INT RB
	{$$=createnode(@$.first_line,"VarDec",4,$1,$2,$3,$4);}
  |VarDec LB INT error RB
  ;
FunDec:ID LP VarList RP
	{$$=createnode(@$.first_line,"FunDec",4,$1,$2,$3,$4);}
  |ID LP RP
	{$$=createnode(@$.first_line,"FunDec",3,$1,$2,$3);}
  |ID LP error RP
  ;
VarList:ParamDec COMMA VarDec
	{$$=createnode(@$.first_line,"VarList",3,$1,$2,$3);}
  |ParamDec
	{$$=createnode(@$.first_line,"VarList",1,$1);}
  ;
ParamDec:Specifier VarDec
	{$$=createnode(@$.first_line,"ParamDec",2,$1,$2);}
  ;

CompSt:LC DefList StmtList RC
	{$$=createnode(@$.first_line,"CompSt",4,$1,$2,$3,$4);}
  |LC DefList StmtList error RC
  ;
StmtList:/*empty*/
        {$$=createnode(@$.first_line,"StmtList",0);}
  |Stmt StmtList
	{$$=createnode(@$.first_line,"StmtList",2,$1,$2);}
  ;
Stmt:Exp SEMI
	{$$=createnode(@$.first_line,"Stmt",2,$1,$2);}
  |CompSt
	{$$=createnode(@$.first_line,"Stmt",1,$1);}
  |RETURN Exp SEMI
	{$$=createnode(@$.first_line,"Stmt",3,$1,$2,$3);}
  |IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
	{$$=createnode(@$.first_line,"Stmt",5,$1,$2,$3,$4,$5);}
  |IF LP Exp RP Stmt ELSE Stmt
	{$$=createnode(@$.first_line,"Stmt",7,$1,$2,$3,$4,$5,$6,$7);}
  |WHILE LP Exp RP Stmt
	{$$=createnode(@$.first_line,"Stmt",5,$1,$2,$3,$4,$5);}
  |Exp error SEMI
  ;

DefList:/*empty*/
        {$$=createnode(@$.first_line,"DefList",0);}
  |Def DefList
	{$$=createnode(@$.first_line,"DefList",2,$1,$2);}
  ;
Def:Specifier DecList SEMI
	{$$=createnode(@$.first_line,"Def",3,$1,$2,$3);}
  |Specifier DecList error SEMI
  ;
DecList:Dec
	{$$=createnode(@$.first_line,"DecList",1,$1);}
  |Dec COMMA DecList
	{$$=createnode(@$.first_line,"DecList",3,$1,$2,$3);}
  ;
Dec:VarDec
	{$$=createnode(@$.first_line,"Dec",1,$1);}
  |VarDec ASSIGNOP Exp
	{$$=createnode(@$.first_line,"Dec",3,$1,$2,$3);}
  ;
Exp:Exp ASSIGNOP Exp
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |Exp AND Exp
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |Exp OR Exp
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |Exp RELOP Exp
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |Exp PLUS Exp
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |Exp MINUS Exp
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |Exp STAR Exp
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |Exp DIV Exp
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |LP Exp RP
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |MINUS Exp
	{$$=createnode(@$.first_line,"Exp",2,$1,$2);}
  |NOT Exp
	{$$=createnode(@$.first_line,"Exp",2,$1,$2);}
  |ID LP Args RP
	{$$=createnode(@$.first_line,"Exp",4,$1,$2,$3,$4);}
  |ID LP RP
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |Exp LB Exp RB
	{$$=createnode(@$.first_line,"Exp",4,$1,$2,$3,$4);}
  |Exp DOT ID
	{$$=createnode(@$.first_line,"Exp",3,$1,$2,$3);}
  |ID	{$$=createnode(@$.first_line,"Exp",1,$1);}
  |INT	{$$=createnode(@$.first_line,"Exp",1,$1);}
  |FLOAT{$$=createnode(@$.first_line,"Exp",1,$1);}
  ;

Args:Exp COMMA Args {$$=createnode(@$.first_line,"Args",3,$1,$2,$3);}
  |Exp {$$=createnode(@$.first_line,"Args",1,$1);}
%%
/*
int main(int argc,char **argv){
    yyin = fopen(argv[1],"r");   
    int i=yyparse();
    fclose(yyin);
    if(i==0 && error1==1){
	printtree(root,0);
    }
    freetree(root);
    return 0;
}
*/
void yyerror(char *msg){
    printf("ERROR TYPE B %s:at line %d\n",msg,yylineno);
}
