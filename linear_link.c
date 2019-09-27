#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linear_link.h"

//the function of create node
struct Table_node* create_table_node(char *type,char *name,int line)
{
	struct Table_node *head1=malloc(sizeof(struct Table_node));
	head1->type=malloc(sizeof(type)+1);
	strcpy(head1->type,type);
	head1->name=malloc(sizeof(name)+1);
	strcpy(head1->name,name);
	head1->line=line;
	head1->func_node=NULL;
	head1->next=NULL;
	head1->addr=NULL;
	if(strcmp(type,"int")==0){//type:int/float
	    head1->kind=1;
	}
	else if(strcmp(type,"float")==0){
	    head1->kind=2;
        }
	return head1;
}

struct Func_node* create_func_node(int num,char *type){
	struct Func_node *head1=malloc(sizeof(struct Func_node));
	head1->para_num=num;
	head1->return_type=malloc(sizeof(type)+1);
	head1->return_type=type;
	return head1;
}

//the function of add node
void add_tt_node(struct Table_node *p,struct Table_node *q){
	p->next=q;
}

void add_tf_node(struct Table_node *p,struct Func_node *q){
	p->func_node=q;
}

//search name
struct Table_node* search_name(struct Table_node *h ,char *name){
	struct Table_node *head1=malloc(sizeof(struct Table_node));
	head1=h;
	//printf("search for %s\n",name);
	//printf("name %s\n",name);
	while(head1!=NULL){
		//printf("search %s %s\n",head1->name,head1->type);
		//printf("name %s\n",name);
		if(strcmp(head1->name,name)==0){
			//printf("find %s\n",name);
			return head1;
		}
		head1=head1->next;
	}
	return NULL;//Î´¶¨Òå
}

struct Table_node* search_num(struct Table_node *h,int num){
    struct Table_node *head1=malloc(sizeof(struct Table_node));
	head1=h;
	while(head1!=NULL){
		if(head1->kind==7 && head1->line==num){
			return head1;
		}
		head1=head1->next;
	}
	return NULL;
}
//1int;2float;3int array;4float array;5struct;6func
int type(char *type){
    if(strcmp(type,"int")==0){
        return 1;
    }
    else if(strcmp(type,"float")==0)
    {
        return 2;
    }
}
