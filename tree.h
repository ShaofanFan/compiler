#ifndef _TREE_H
#define _TREE_H
#include<stdarg.h>


struct TREE_NODE{
    int line;
    char *name;
    char *l;
    int in;
    float fn;
    char *addr;
    struct TREE_NODE* child;
    struct TREE_NODE* sibling;
};



struct TREE_NODE* createnode(int line1,char *name,int n1,...);

void printtree(struct TREE_NODE *root,int nlayer);

void printnode(struct TREE_NODE *r);

void freetree(struct TREE_NODE *r);
#endif


