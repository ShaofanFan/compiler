#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ir.h"

struct ir_node* create_ir_node(int kind,char *op1,char *ar1,char *ar2,char *result){
    struct ir_node *newnode=malloc(sizeof(struct ir_node));
    newnode->kind=kind;
	newnode->op=malloc(sizeof(op1)+1);
    strcpy(newnode->op,op1);
	newnode->arg1=malloc(sizeof(ar1)+1);
    strcpy(newnode->arg1,ar1);
	newnode->arg2=malloc(sizeof(ar2)+1);
    strcpy(newnode->arg2,ar2);
	newnode->result=malloc(sizeof(result)+1);
    strcpy(newnode->result,result);
    newnode->prev=malloc(sizeof(struct ir_node));
    newnode->prev=NULL;
    newnode->next=malloc(sizeof(struct ir_node));
    newnode->next=NULL;
    return newnode;
}
void print_irnode(struct ir_node* head){
    struct ir_node * p=malloc(sizeof(head));
    p=head;
    while(p!=NULL){
        if(p->kind==1){
            printf("%s := %s%s\n",p->result,p->op,p->arg1);
        }else if(p->kind==2){
            char *op1=malloc(sizeof(char[4]));
            if(strcmp(p->op,"PLUS")==0){
                strcpy(op1,"+");
            }else if(strcmp(p->op,"MINUS")==0){
                strcpy(op1,"-");
            }else if(strcmp(p->op,"STAR")==0){
                strcpy(op1,"*");
            }else{
                strcpy(op1,"/");
            }
            printf("%s := %s %s %s\n",p->result,p->arg1,op1,p->arg2);
        }else if(p->kind==3){
            printf("%s %s\n",p->op,p->arg1);
        }else if(p->kind==4){
            printf("%s %s :\n",p->op,p->arg1);
        }else if(p->kind==5){
            printf("%s := %s %s\n",p->result,p->op,p->arg1);
        }else if(p->kind==6){
            printf("%s %s\n",p->op,p->arg1);
        }else if(p->kind==7){
            printf("%s %s\n",p->op,p->arg1);
        }else if(p->kind==8){
            printf("%s %s\n",p->op,p->arg1);
        }else if(p->kind==9){
            printf("%s %s [%s]\n",p->op,p->arg1,p->arg2);
        }else if(p->kind==10){
            printf("%s := %s %s #%s\n",p->result,p->arg1,p->op,p->arg2);
        }else if(p->kind==11){
            printf("%s %s %s %s ",p->result,p->arg1,p->op,p->arg2);
        }else if(p->kind==12){
            printf("%s %s\n",p->op,p->arg1);
        }
        p=p->next;
    }

}

char* itoa(int n){
    char *result=malloc(sizeof(char[6]));
    int i=0;
    if(n==0){
        result[i]='0';
        result[i+1]='\0';
        return result;
    }
    while(n>=1){
        result[i]=n%10+'0';
        n=n/10;
		i++;
    }
	result[i]='\0';
    return result;
}
char *newlabel(int i){
    char *newlabel=malloc(sizeof(char[10]));
    newlabel[0]='l';
    newlabel[1]='a';
    newlabel[2]='b';
    newlabel[3]='e';
    newlabel[4]='l';
    newlabel[5]='\0';
	strcat(newlabel,itoa(i));
    return newlabel;
}
char *newvar(int i){
    char *newvar1=malloc(sizeof(char[6]));
	newvar1[0]='v';
	newvar1[1]='\0';
	strcat(newvar1,itoa(i));
    return newvar1;
}
