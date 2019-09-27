%option yylineno noyywrap noinput nounput 

%{
#include "tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "syntax.tab.h"
int yycolumn =1;
#define YY_USER_ACTION \
	yylloc.first_line = yylloc.last_line =yylineno;\
	yylloc.first_column = yycolumn;\
	yylloc.last_column = yycolumn +yyleng-1;\
	yycolumn+=yyleng;

int error1=1;
%}

RELOP>|<|>=|<=|==|!=
INT([1-9]+[0-9]*)|(0[0-7]+)|([0x|0X][0-9|A-F]+)|0
FLOAT([0-9]*\.?[0-9]+|[0-9]+\.)([e|E][-|+]?[0-9]+)?
ID[a-zA-Z|_][a-zA-Z|_|0-9]*
NOTE\/\*.*|.*\*\/
%%

"struct" {
	yylval.r=createnode(yylineno,"STRUCT",0);
	return STRUCT;
	}
"return" {
	yylval.r=createnode(yylineno,"RETURN",0);
	return RETURN;
	}
"if" {
	yylval.r=createnode(yylineno,"IF",0);
	return IF;}
"else" {
	yylval.r=createnode(yylineno,"ELSE",0);
	return ELSE;}
"while" {
	yylval.r=createnode(yylineno,"WHILE",0);
	return WHILE;}
int|float {
	yylval.r=createnode(yylineno,"type",0);
 	yylval.r->l=malloc(sizeof(yyleng));
        strcpy(yylval.r->l,yytext);
	return TYPE;}
\( {
	yylval.r=createnode(yylineno,"LP",0);
	return LP;}
\) {
	yylval.r=createnode(yylineno,"RP",0);
	return RP;}
\[ {
	yylval.r=createnode(yylineno,"LB",0);
	return LB;}
\] {
	yylval.r=createnode(yylineno,"RB",0);
	return RB;}
\{ {
	yylval.r=createnode(yylineno,"LC",0);
	return LC;}
\} {
	yylval.r=createnode(yylineno,"RC",0);
	return RC;}
; {
	yylval.r=createnode(yylineno,"SEMI",0);
	return SEMI;}
, {
	yylval.r=createnode(yylineno,"COMMA",0);
	return COMMA;}
= {
	yylval.r=createnode(yylineno,"ASSIGNOP",0);
	return ASSIGNOP;}
{RELOP} {
	yylval.r=createnode(yylineno,"RELOP",0);
	yylval.r->l=malloc(sizeof(yytext)+1);
	strcpy(yylval.r->l,yytext);
	return RELOP;}
\+ {
	yylval.r=createnode(yylineno,"PLUS",0);
	return PLUS;}
\- {
	yylval.r=createnode(yylineno,"MINUS",0);
	return MINUS;}

\*  {
	yylval.r=createnode(yylineno,"STAR",0);
	return STAR;}
\/ {
	yylval.r=createnode(yylineno,"DIV",0);
	return DIV;}
&& {
	yylval.r=createnode(yylineno,"AND",0);
	return AND;}
\|\| {
	yylval.r=createnode(yylineno,"OR",0);
	return OR;}
! {
	yylval.r=createnode(yylineno,"NOT",0);
	return NOT;}
\. {
	yylval.r=createnode(yylineno,"DOT",0);
	return DOT;}

{INT} {
	yylval.r=createnode(yylineno,"INT",0);
	yylval.r->in=atoi(yytext);
	return INT;}
{FLOAT} {
	yylval.r=createnode(yylineno,"FLOAT",0);
	yylval.r->fn=atof(yytext);
	return FLOAT;}
{ID} {
	yylval.r=createnode(yylineno,"id",0);
	yylval.r->l=malloc(sizeof(yyleng));
        strcpy(yylval.r->l,yytext);
	return ID;}
{NOTE} {}
\n {yycolumn=1;}
[ \t\r]* {}
. {printf("Error type A at line %d :mysterious characters '%s';\n", yylineno, yytext);error1=0;}


%%
