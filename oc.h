#ifndef OC_H
#define OC_H
#include "ir.h"

char register_table[10][6][5];
char addr_table[20][6][5];

void ir_to_oc(struct ir_node *head);
char* get_register(char *arg);
char* add_addrtable(char *arg);
char* find_register(char *arg);
void* add_addr(char *arg,char *regis);
void* del_addr(char *arg,char *regis);
#endif // OC_H
