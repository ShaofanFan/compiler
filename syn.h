#include "linear_link.h"
#include "tree.h"
#ifndef _SYN_H
#define _SYN_H

struct Table_node *head;

void traver_extdeflist(int i,struct TREE_NODE *extdeflist);
void traver_extdef(struct TREE_NODE *extdef);
void traver_structspecifier(struct TREE_NODE *structspecifier);
void traver_extdeclist(int kind,char *type,struct TREE_NODE *extdeclist);
void traver_fundec(int kind,char *return_type,struct TREE_NODE *fundec);
void traver_compst(char * return_type,struct TREE_NODE *compst);
void traver_deflist(struct TREE_NODE *deflist);
void traver_deflist2(struct Table_node* struc,struct TREE_NODE *deflist);
void traver_def(struct TREE_NODE *def);
void traver_def2(struct Table_node* struc,struct TREE_NODE *def);
void traver_declist(int kind,char *type,struct TREE_NODE *declist);
void traver_declist2(int kind,char *type,struct TREE_NODE *declist,struct Table_node* struc);
void traver_dec(int kind,char *type ,struct TREE_NODE *dec);
void traver_dec2(int kind,char *type ,struct TREE_NODE *dec,struct Table_node *struc);
void traver_varlist(struct TREE_NODE *varlist);
void traver_stmtlist(char * return_type,struct TREE_NODE *stmtlist);
void traver_stmt(char *return_type,struct TREE_NODE *stmt);
void traver_exp(struct TREE_NODE *exp);
void traver_args(struct Table_node* r,struct TREE_NODE *args,int kind);
//void print_tablenode(struct Table_node * head);
#endif
