
#ifndef IR_H
#define IR_H

struct ir_node{
    int kind;
    char *op;
    char *arg1;
    char *arg2;
    char *result;
    struct ir_node *prev;
    struct ir_node *next;
};

struct ir_node* create_ir_node(int kind,char *op1,char *ar1,char *ar2,char *result);
void print_irnode(struct ir_node* head);

char *newlabel(int i);
char *newvar(int i);
char* itoa(int n);
#endif // IR_H
