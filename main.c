#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree.h"
#include "syntax.tab.h"
#include "syn.h"
#include "ir.h"
#include "oc.h"

extern FILE *yyin;
extern FILE *yyout;
extern struct TREE_NODE *root;
extern struct Table_node *head;
extern struct ir_node * ir_head;

int main(int argc,char **argv){
    yyin = fopen(argv[1],"r");
    yyparse();
    fclose(yyin);
    //printtree(root,0);
    traver_extdeflist(1,root->child);
    //print_tablenode(head);
    print_irnode(ir_head->next);
	stdout=freopen(argv[2],"w",stdout);

    //freetree(root);
    ir_to_oc(ir_head->next);

    return 0;
}


struct TREE_NODE *createnode(int line1,char *name,int n1,...){
	struct TREE_NODE *head =malloc(sizeof(struct TREE_NODE));
	struct TREE_NODE *temp =malloc(sizeof(struct TREE_NODE));
	head->name=malloc(sizeof(name)+1);
	head->name=name;
	head->child=NULL;
	head->sibling=NULL;
	head->line=line1;
	head->addr=malloc(sizeof(char[6]));
	va_list ap;
	va_start(ap,n1);
	if(n1>0){
	   temp=va_arg(ap,struct TREE_NODE *);
	   head->child = temp;
	   if(n1>1){
		for(int i=1;i<n1;i++){
		   temp->sibling=malloc(sizeof(struct TREE_NODE));
		   temp->sibling=va_arg(ap,struct TREE_NODE *);
		   temp=temp->sibling;
 		}
	   }
        }
	va_end(ap);
	return head;
}
void printnode(struct TREE_NODE *r){
	if(!strcmp(r->name,"INT")){
	  printf("%s(%d):%d\n",r->name,r->line,r->in);}
	else if(!strcmp(r->name,"FLOAT")){
	  printf("%s(%d):%f\n",r->name,r->line,r->fn);}
	else if(!strcmp(r->name,"type")){
	  printf("%s(%d):%s\n",r->name,r->line,r->l);
	}
	else{
	  printf("%s(%d)\n",r->name,r->line);}
}

void printtree(struct TREE_NODE *root,int nlayer){
	if(!root){
	   return ;
	}
	//print node
	for(int j=1;j<=nlayer;j++){
	  printf("  ");
	}
	printnode(root);

	printtree(root->child,nlayer+1);
	printtree(root->sibling,nlayer);
}

void freetree(struct TREE_NODE *r){
	if(!r)return;
        freetree(r->child);
	free(r);
	freetree(r->sibling);
}


