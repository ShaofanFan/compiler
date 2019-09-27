#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "syn.h"
#include "ir.h"

//全局变量
struct Table_node *p1,*q1;
struct Func_node *p2;
//IR
int i;//count the label
int j;//count newvar in ir
struct ir_node *ir_head;
struct ir_node *ir_pre;
struct ir_node *ir_next;
void addirnode(int k,char *op,char *arg1,char *arg2,char *result);
char *labeltrue,*labelfalse;
/*
void print_tablenode(struct Table_node *head){
    int i=0;
    struct Table_node *p=malloc(sizeof(struct Table_node));
    p=head;
    while(p!=NULL){
        printf("%d : %s %s %d\n",i,p->name,p->type,p->kind);
        i++;
        p=p->next;
    }
}*/

void traver_extdeflist(int i,struct TREE_NODE *extdeflist){
	if(extdeflist->child==NULL){
	    return;
	}
	if(i==1){//在第一次traver时给head分配内存并使p1=head；
		head=create_table_node("FunDec","read",0);
		head->kind=6;
		struct Func_node *readnode=create_func_node(0,"int");
		readnode->para_num=0;
		add_tf_node(head,readnode);

		p1=create_table_node("FunDec","write",0);
		p1->kind=6;
		struct Func_node *writenode=create_func_node(1,"int");
		writenode->para_num=1;
		add_tf_node(p1,writenode);
		head->next=p1;

		ir_head=malloc(sizeof(struct ir_node));
		ir_pre=malloc(sizeof(struct ir_node));
		ir_next=malloc(sizeof(struct ir_node));
		ir_head=create_ir_node(0,"","","","");
		ir_pre=ir_head;
		ir_next=ir_head;

		labelfalse=malloc(sizeof(char[10]));
		labeltrue=malloc(sizeof(char[10]));


	}
	traver_extdef(extdeflist->child);
	traver_extdeflist(2,extdeflist->child->sibling);
}

void traver_extdef(struct TREE_NODE *extdef){
	if(strcmp(extdef->child->sibling->name,"SEMI")==0){//specifier ;
		if(strcmp(extdef->child->child->name,"StructSpecifier")==0){
			traver_structspecifier(extdef->child->child);//struct定义
		}

	}else if(strcmp(extdef->child->sibling->name,"ExtDecList")==0){//Specifier ExtDecList;
		if(strcmp(extdef->child->child->name,"type")==0){//1,2用于区分type和struct
			traver_extdeclist(1,extdef->child->child->l,extdef->child->sibling);
		}else{//structspecifier->STRUCT tag;tag->id;id->l
			traver_extdeclist(2,extdef->child->child->child->sibling->child->l,extdef->child->sibling);
		}//struct tag id;

	}else if(strcmp(extdef->child->sibling->name,"FunDec")==0){//specifier fundec compst
		if(strcmp(extdef->child->child->name,"type")==0){
			traver_fundec(1,extdef->child->child->l,extdef->child->sibling);
		}else{//structspecifier->STRUCT tag;tag->id
			traver_fundec(2,extdef->child->child->child->sibling->child->l,extdef->child->sibling);
		}
	}
}
//struct的定义 struct opttag lc deflist rc
void traver_structspecifier(struct TREE_NODE *structspecifier){
	struct TREE_NODE *p,*q3,*q4=malloc(sizeof(struct TREE_NODE));
	p=structspecifier->child->sibling;//tag或者opttag
	if(strcmp(p->name,"OptTag")==0){//struct opttag{deflist}
		//创建struct类型的table节点,p是opttag
		if(p->child!=NULL){//opttag->id|null
			struct Table_node* result=malloc(sizeof(struct Table_node));
			result=search_name(head,p->child->l);
			if(result!=NULL){
				printf("Error type 16 at line %d:duplicated name '%s'\n",p->line,p->child->l);
			}
			//计算struct里面定义了几个变量
			q3=p->sibling->sibling;//deflist
			int count=0;
			while(q3->child!=NULL){
				q4=q3->child->child->sibling;//declsit
				count++;
				while(q4->child->sibling!=NULL){//declist->dec comma declist
					count++;
					q4=q4->child->sibling->sibling;//declist
				}
				q3=q3->child->sibling;//deflist->def deflist
			}
			q1=create_table_node("STRUCT",p->child->l,p->line);
			q1->kind=5;//struct类型
			q1->weishu=count;
			p1->next=q1;
			p1=q1;
			traver_deflist2(q1,p->sibling->sibling);
		}
	}
}

void traver_extdeclist(int kind,char *type,struct TREE_NODE *extdeclist){
	if(extdeclist==NULL){
	    return;
	}
	struct TREE_NODE *p=malloc(sizeof(struct TREE_NODE));
	struct Table_node *result=malloc(sizeof(struct Table_node));
	if(kind==1){//extdeclist->vardec|vardec,extdeclist;vardec->id|vardec[]
		//创建type(int/float类型的table节点 id:extdeclist->child->child
		if(strcmp(type,"int")){
			p=extdeclist->child;//vardec
			if(strcmp(p->child->name,"id")==0){//int 类型 kind=1
				//检查是否重定义
				result=search_name(head,p->child->l);
				if(result!=NULL){
					printf("Error type 3 at line %d:redefined variable '%s'\n",p->line,p->child->l);
				}
				q1=create_table_node("int",p->child->l,p->line);
				q1->kind=1;//int类型
				p1->next=q1;
				p1=q1;
			}else{//int 数组类型 kind=3
				int count=0;//记录维数
				while(strcmp(p->child->name,"id")!=0){
					count++;
					p=p->child;//vardec->vardec[]
				}//检查是否重定义
				result=search_name(head,p->child->l);
				if(result!=NULL){
					printf("Error type 3 at line %d:redefined variable '%s'\n",p->line,p->child->l);
				}
				char *newadd1=malloc(sizeof(char[4]));
				newadd1=newvar(j);
				j++;
				q1=create_table_node("int",p->child->l,p->line);
				strcpy(q1->addr,newadd1);
				q1->kind=3;//int类型
				q1->weishu=count;
				p1->next=q1;
				p1=q1;
				int pianyi=p->sibling->sibling->in;
				pianyi=pianyi*4;
				addirnode(9,"DEC",q1->addr,itoa(pianyi),"");
			}
		}else{//float
			p=extdeclist->child;//vardec
			if(strcmp(p->child->name,"id")==0){//float 类型 kind=2
				//检查是否重定义
				result=search_name(head,p->child->l);
				if(result!=NULL){
					printf("Error type 3 at line %d:redefined variable '%s'\n",p->line,p->child->l);
				}
				q1=create_table_node("float",p->child->l,p->line);
				q1->kind=2;//float类型
				p1->next=q1;
				p1=q1;
			}else{//float 数组类型 kind=4
				int count=0;//记录维数
				while(strcmp(p->child->name,"id")!=0){
					count++;
					p=p->child;//vardec->vardec[]
				}//检查是否重定义
				result=search_name(head,p->child->l);
				if(result!=NULL){
					printf("Error type 3 at line %d:redefined variable '%s'\n",p->line,p->child->l);
				}
				char *newadd1=newvar(j);
				j++;
				q1=create_table_node("float",p->child->l,p->line);
				strcpy(q1->addr,newadd1);
				q1->kind=4;//float数组类型
				q1->weishu=count;
				p1->next=q1;
				p1=q1;
				int pianyi=p->sibling->sibling->in;
				pianyi=pianyi*4;
				addirnode(9,"DEC",q1->addr,itoa(pianyi),"");
			}
		}
	}else{
		//创建struct类型的table节点 type为 struct 的tag  ;以定义好的struct
		//检查是否struct tag 是否定义过
		result=search_name(head,type);
		if(result==NULL){//未定义
			printf("Error type 17 at line %d:undefined structure '%s'\n",extdeclist->child->child->line,type);
		}else{
			result=search_name(head,extdeclist->child->child->l);
			if(result!=NULL){
				printf("Error type 3 at line %d:redefined variable '%s'\n",extdeclist->child->child->line,extdeclist->child->child->l);
			}
		}
		q1=create_table_node(type,extdeclist->child->child->l,extdeclist->child->child->line);
		q1->kind=5;
		p1->next=q1;
		p1=q1;
	}

	if(extdeclist->child->sibling!=NULL){//vardec comma extdeclist
		traver_extdeclist(kind,type,extdeclist->child->sibling->sibling);
	}
}

void traver_fundec(int kind,char *return_type,struct TREE_NODE *fundec){
	//printf("return_type %s\n",return_type);
	struct Table_node *result=malloc(sizeof(struct Table_node));
	result=search_name(head,fundec->child->l);
	if(result){

		printf("Error type 4 at line %d :redefined function '%s'\n",fundec->line,fundec->child->l);
	}

	q1=create_table_node("FunDec",fundec->child->l,fundec->line);
        //printf("q1->name%s\n",q1->name);
	q1->kind=6;
	p1->next=q1;
	//printf("P1->name1 %s\n",p1->name);
	p1=q1;
        //printf("P1->name2 %s\n",p1->name);
    addirnode(4,"FUNCTION",fundec->child->l,"","");

	//创建func_node
	if(strcmp(fundec->child->sibling->sibling->name,"VarList")==0){
		struct TREE_NODE *p3=malloc(sizeof(struct TREE_NODE));
		int count=1;
		p3=fundec->child->sibling->sibling;//varlist
		while(p3->child->sibling!=NULL){
			count++;
			p3=p3->child->sibling->sibling;//varlist;
		}
		//printf("count%d\n",count);
		p2=create_func_node(count,return_type);
		add_tf_node(p1,p2);
		traver_varlist(fundec->child->sibling->sibling);
	}else{
		p2=create_func_node(0,return_type);
		add_tf_node(p1,p2);
	}
	traver_compst(return_type,fundec->sibling);

}

void traver_compst(char * return_type,struct TREE_NODE *compst){
	if(compst==NULL){
		return ;
	}
	traver_deflist(compst->child->sibling);

	traver_stmtlist(return_type,compst->child->sibling->sibling);
}

void traver_deflist(struct TREE_NODE *deflist)
{
	if(deflist->child==NULL){
		return ;
	}
	traver_def(deflist->child);
	traver_deflist(deflist->child->sibling);

}

void traver_deflist2(struct Table_node* struc,struct TREE_NODE *deflist) {
	if(deflist->child==NULL){
		return;
	}else{
		traver_def2(struc,deflist->child);
		traver_deflist2(struc,deflist->child->sibling);
	}
}

void traver_def(struct TREE_NODE *def){
	if(strcmp(def->child->child->name,"type")==0){//specifier->type
		traver_declist(1,def->child->child->l,def->child->sibling);
	}else{//structspecifier
		traver_declist(5,def->child->child->child->sibling->child->l,def->child->sibling);
	}
}

void traver_def2(struct Table_node* struc,struct TREE_NODE *def){
	if(strcmp(def->child->child->name,"type")==0){//specifier->type
		traver_declist2(1,def->child->child->l,def->child->sibling,struc);
	}else{//structspecifier
		traver_declist2(5,def->child->child->child->sibling->child->l,def->child->sibling,struc);
	}
}



void traver_declist(int kind,char *type,struct TREE_NODE *declist){
	traver_dec(kind,type,declist->child);
	if(declist->child->sibling!=NULL){
		traver_declist(kind,type,declist->child->sibling->sibling);
	}

}

void traver_declist2(int kind,char *type,struct TREE_NODE *declist,struct Table_node* struc){
	traver_dec2(kind,type,declist->child,struc);
	if(declist->child->sibling!=NULL){
		traver_declist2(kind,type,declist->child->sibling->sibling,struc);
	}

}

void traver_dec(int kind,char *type ,struct TREE_NODE *dec){
	struct TREE_NODE* exp=malloc(sizeof(struct TREE_NODE));
	exp=NULL;
	if(dec->child->sibling!=NULL){
		traver_exp(dec->child->sibling->sibling);//exp
		exp=dec->child->sibling->sibling;
	}

	struct Table_node *result = malloc(sizeof(struct Table_node));
	if(kind==5){
		result=search_name(head,type);
		if(!result){
			printf("Error type 17 at line %d : undefined structure '%s'\n",dec->line,type);
		}

	}
	if(strcmp(dec->child->child->name,"id")==0){//dec->vardec->id
		result=search_name(head,dec->child->child->l);
		if(result!=NULL){
			printf("Error type 3 at line %d:redefined variable '%s'\n",dec->child->child->line,dec->child->child->l);
		}
		char *newaddr1=newvar(j);
		if(exp!=NULL){
			if(!((exp->in==1 && strcmp(type,"int")==0) || (exp->in==2 && strcmp(type,"float")==0))){
				printf("Error type 5 at line %d:type mismatched\n",exp->line);
			}


            j++;
			addirnode(1,"",exp->addr,"",newaddr1);
		}
		q1=create_table_node(type,dec->child->child->l,dec->child->child->line);
		q1->addr=malloc(sizeof(char[6]));
		strcpy(q1->addr,newaddr1);
		if(kind==5){
			q1->kind=5;
		}
		p1->next=q1;
		p1=q1;

	}else{//vardec lb int rb
		struct TREE_NODE *p=malloc(sizeof(struct TREE_NODE));
		int count=0;
		p=dec->child;//vardec
		while(strcmp(p->child->name,"id")!=0){
			count++;//数组的维数
			p=p->child;
		}
		result=search_name(head,p->child->l);
		if(result!=NULL){
			printf("Error type 3 at line %d:redefined variable '%s'\n",p->child->line,p->child->l);
		}
		char *newaddr1=newvar(j);
		j++;
		int pianyi=dec->child->child->sibling->sibling->in;
		pianyi=pianyi*4;
		q1=create_table_node(type,p->child->l,p->child->line);
		strcpy(q1->addr,newaddr1);
		q1->weishu=count;
		addirnode(9,"DEC",p->child->l,itoa(pianyi),"");

		if(strcmp(type,"int")==0){
			q1->kind=3;//int array
		}else if(strcmp(type,"float")==0){
			q1->kind=4;//float array
		}

		p1->next=q1;
		p1=q1;
	}
}

void traver_dec2(int kind,char *type ,struct TREE_NODE *dec,struct Table_node *struc){
	struct Table_node *result = malloc(sizeof(struct Table_node));
	if(dec->child->sibling!=NULL){
		printf("Error type 15 at line %d:illegal initialize\n",dec->line);
	}
	if(strcmp(dec->child->child->name,"id")==0){//dec->vardec->id
		result=search_name(struc->next,dec->child->child->l);
		if(result!=NULL){
			printf("Error type 15 at line %d:redefined field '%s'\n",dec->child->child->line,dec->child->child->l);
		}
		q1=create_table_node(type,dec->child->child->l,dec->child->child->line);
		if(kind==5){
			q1->kind=5;
		}
		p1->next=q1;
		p1=q1;
	}else{//vardec lb int rb
		struct TREE_NODE *p=malloc(sizeof(struct TREE_NODE));
		int count=0;
		p=dec->child;//vardec
		while(strcmp(p->child->name,"id")!=0){
			count++;//数组的维数
			p=p->child;
		}
		result=search_name(head,p->child->l);
		if(result!=NULL){
			printf("Error type 15 at line %d:redefined field '%s'\n",p->child->line,p->child->l);
		}
		q1=create_table_node(type,p->child->l,p->child->line);
		q1->weishu=count;

		if(strcmp(type,"int")==0){
			q1->kind=3;//int array
		}else if(strcmp(type,"float")==0){
			q1->kind=4;//float array
		}

		p1->next=q1;
		p1=q1;
	}
}
//func_node 的参数
void traver_varlist(struct TREE_NODE *varlist){//func_node
	struct TREE_NODE *p3,*p4=malloc(sizeof(struct TREE_NODE));
	struct Table_node *result=malloc(sizeof(struct Table_node));
	p3=varlist->child->child;//specifier (paramdec->specifier vardec
	if(strcmp(p3->child->name,"type")==0){
		if(strcmp(p3->sibling->child->name,"id")==0){//p3->sibling->child
			result=search_name(head,p3->sibling->child->l);
			if(result!=NULL){
				printf("Error type 3 at line %d:redefined variable '%s'\n",p3->sibling->line,p3->sibling->child->l);
			}
			char *newadd1=newvar(j);
			j++;
			strcpy(p3->sibling->child->addr,newadd1);
			q1=create_table_node(p3->child->l,p3->sibling->child->l,p3->sibling->line);
			q1->addr=malloc(sizeof(char[6]));
			strcpy(q1->addr,newadd1);
			p1->next=q1;
			p1=q1;
			addirnode(6,"PARAM",q1->addr,"","");
		}else{//数组
			int count=0;
			p4=p3->sibling;//vardec
			while(p4->child->sibling!=NULL){
				count++;
			}
			result=search_name(head,p4->child->l);
			if(result!=NULL){
				printf("Error type 3 at line %d:redefined variable '%s'\n",p4->child->line,p4->child->l);
			}
			q1=create_table_node(p3->child->l,p4->child->l,p4->child->line);
			q1->weishu=count;
			if(strcmp(q1->type,"int")==0){
				q1->kind=3;
			}else{
				q1->kind=4;
			}
			p1->next=q1;
			p1=q1;
		}
	}else{
		//struct,函数形参里面假装没有。。。
	}

	if(varlist->child->sibling!=NULL){
		traver_varlist(varlist->child->sibling->sibling);//varlist
	}

}

void traver_stmtlist(char * return_type,struct TREE_NODE *stmtlist){
	if(stmtlist->child==NULL){
		return ;
	}
	traver_stmt(return_type,stmtlist->child);
	traver_stmtlist(return_type,stmtlist->child->sibling);
}

void traver_stmt(char *return_type,struct TREE_NODE *stmt){
	if(strcmp(stmt->child->name,"Exp")==0){
		traver_exp(stmt->child);
	}else if(strcmp(stmt->child->name,"CompSt")==0){
		traver_compst(return_type,stmt->child);
	}else if(strcmp(stmt->child->name,"RETURN")==0){
		traver_exp(stmt->child->sibling);
		//printf("return_type in stmt %s\n",return_type);
		//printf("exp->in %d\n",stmt->child->sibling->in);
		if(!(strcmp(return_type,"int")==0 && stmt->child->sibling->in==1
		||strcmp(return_type,"float")==0 && stmt->child->sibling->in==2\
        )){

		printf("Error type 8 at line %d: type mismatched for return.\n",stmt->child->line);
		}
		addirnode(6,"RETURN",stmt->child->sibling->addr,"","");

	}else if(strcmp(stmt->child->name,"WHILE")==0){
        char *labelbegin=newlabel(i);
        i++;
        char *label=newlabel(i);
		i++;
		strcpy(labeltrue,label);
		label=newlabel(i);
        i++;
        strcpy(labelfalse,label);

        addirnode(4,"LABEL",labelbegin,"","");
		traver_exp(stmt->child->sibling->sibling);
		addirnode(4,"LABEL",labeltrue,"","");
		traver_stmt(return_type,stmt->child->sibling->sibling->sibling->sibling);
		addirnode(3,"GOTO",labelbegin,"","");
		addirnode(4,"LABEL",labelfalse,"","");

	}else{//if
	    char *label1=newlabel(i);
		i++;
		strcpy(labeltrue,label1);
		char *label2=newlabel(i);
		i++;
        strcpy(labelfalse,label2);
        char *label3=newlabel(i);
        i++;

		traver_exp(stmt->child->sibling->sibling);

		addirnode(4,"LABEL",labeltrue,"","");

		traver_stmt(return_type,stmt->child->sibling->sibling->sibling->sibling);
		addirnode(3,"GOTO",label3,"","");

		if(stmt->child->sibling->sibling->sibling->sibling->sibling!=NULL){//else sibling

            addirnode(4,"LABEL",labelfalse,"","");
			traver_stmt(return_type,stmt->child->sibling->sibling->sibling->sibling->sibling->sibling);
			addirnode(4,"LABEL",label3,"","");
		}else{
            addirnode(4,"LABEL",labelfalse,"","");
		}



	}
}

void traver_exp(struct TREE_NODE *exp){//用tree_node中的in存放exp的类型
	struct Table_node *result=malloc(sizeof(struct TREE_NODE));
	if(strcmp(exp->child->name,"INT")==0){
		exp->in=1;
		result=search_num(head,exp->child->in);
		if(result!=NULL){
                strcpy(exp->addr,result->name);
		}else{
            char *newvar1=newvar(j);
            //printf("newvar1(%d):%s",j,newvar(j));
            j++;
            q1=create_table_node("INT",newvar1,exp->child->in);
            q1->kind=7;
            p1->next=q1;
            p1=q1;
            addirnode(1,"#",itoa(exp->child->in),"",newvar1);//?????
            strcpy(exp->addr,newvar1);
		}

	}else if(strcmp(exp->child->name,"FLOAT")==0){
		exp->in=2;
		char *newvar1=newvar(j);
		j++;
        strcpy(exp->addr,newvar1);


	}else if(strcmp(exp->child->name,"id")==0){
		//exp->id
		if(exp->child->sibling==NULL){//exp->id
			result=search_name(head,exp->child->l);
			if(result==NULL){
				printf("Error type 1 at line %d: undefined variable '%s'\n",exp->line,exp->child->l);
			}else{
				exp->in=result->kind;
				if(result->addr==NULL){
                    char *newvar1=newvar(j);
                    j++;
                    result->addr=malloc(sizeof(char[6]));
                    strcpy(result->addr,newvar1);
				}
				exp->addr=result->addr;
			}

		}else if(exp->child->sibling->sibling->sibling==NULL){//Exp->ID LP RP
			result=search_name(head,exp->child->l);

			if(result==NULL){
				printf("Error type 2 at line %d: undefined function '%s'\n",exp->line,exp->child->l);
			}else{

				if(result->kind!=6){
					printf("Error type 11 at line %d: incorrect use of ()\n",exp->line);
				}else {
                    exp->in=type(result->func_node->return_type);
				}
				if(result->func_node->para_num!=0){

					printf("Error type 9 at line %d: arguments dismatched\n",exp->line);
				}
				char *newaddr1=newvar(j);
                j++;
                strcpy(exp->addr,newaddr1);
				if(strcmp(result->name,"read")==0){
                    addirnode(7,"READ",exp->addr,"","");
				}else{
                    addirnode(5,"CALL",result->name,"",exp->addr);

				}

			}

		}else{//exp->ID LP Args RP
            char *newaddr1=newvar(j);
            j++;
            strcpy(exp->addr,newaddr1);
			result=search_name(head,exp->child->l);//the name of function
			if(result==NULL){
				printf("Error type 2 at line %d: undefined function '%s'\n",exp->line,exp->child->l);
			}else{


				if(result->kind!=6){
					printf("Error type 11 at line %d: incorrect use of ()\n",exp->line);
				}else {
                    exp->in=type(result->func_node->return_type);

					if(strcmp(result->name,"write")==0){
					    traver_args(result,exp->child->sibling->sibling,0);
                        addirnode(8,"WRITE",exp->child->sibling->sibling->child->addr,"","");
					}else{
                        traver_args(result,exp->child->sibling->sibling,1);
                        addirnode(5,"CALL",result->name,"",exp->addr);

					}
				}
			}
		}
	}else if(strcmp(exp->child->name,"MINUS")==0){//minus exp
		traver_exp(exp->child->sibling);
		exp->in=exp->child->sibling->in;
		char *newvar1=newvar(j);
        j++;
        strcpy(exp->addr,newvar1);
		addirnode(2,"MINUS","#0",exp->child->sibling->addr,exp->addr);

	}else if(strcmp(exp->child->name,"NOT")==0){//not exp
		traver_exp(exp->child->sibling);
		exp->in=exp->child->sibling->in;
		char *temp=labeltrue;
		labeltrue=labelfalse;
		labelfalse=temp;

	}else if(strcmp(exp->child->name,"LP")==0){//lp exp rp
		traver_exp(exp->child->sibling);
		exp->in=exp->child->sibling->in;
		strcpy(exp->addr,exp->child->sibling->addr);
	}else{
		traver_exp(exp->child);
		char *newvar1=newvar(j);
        j++;
        strcpy(exp->addr,newvar1);
		if(strcmp(exp->child->sibling->name,"PLUS")==0 || strcmp(exp->child->sibling->name,"MINUS")==0\
		||strcmp(exp->child->sibling->name,"STAR")==0 || strcmp(exp->child->sibling->name,"DIV")==0){
			traver_exp(exp->child->sibling->sibling);
			if(exp->child->in != exp->child->sibling->sibling->in){
				printf("Error type 7 at line %d:type mismatched for operands\n",exp->line);
			}else if(exp->child->in !=1 && exp->child->in !=2){
				printf("Error type 7 at line %d:type mismatched for operands\n",exp->line);
			}
			exp->in=exp->child->in;
			addirnode(2,exp->child->sibling->name,exp->child->addr,\
			exp->child->sibling->sibling->addr,exp->addr);

		}else if(strcmp(exp->child->sibling->name,"AND")==0){
            char *labeland=newlabel(i);
            i++;
            addirnode(12,"GOTO",labeland,"","");
            addirnode(3,"GOTO",labelfalse,"","");
            addirnode(4,"LABEL",labeland,"","");
			traver_exp(exp->child->sibling->sibling);
			addirnode(12,"GOTO",labeltrue,"","");
			addirnode(3,"GOTO",labelfalse,"","");

		}else if(strcmp(exp->child->sibling->name,"OR")==0){
            addirnode(12,"GOTO",labeltrue,"","");
            traver_exp(exp->child->sibling->sibling);
            addirnode(12,"GOTO",labeltrue,"","");
            addirnode(3,"GOTO",labelfalse,"","");

		}else if(strcmp(exp->child->sibling->name,"RELOP")==0){
            traver_exp(exp->child->sibling->sibling);
            addirnode(11,exp->child->sibling->l,exp->child->addr,exp->child->sibling->sibling->addr,"IF");
            if(strcmp(exp->sibling->name,"RP")==0){
                addirnode(12,"GOTO",labeltrue,"","");
                addirnode(3,"GOTO",labelfalse,"","");
            }


		}else if(strcmp(exp->child->sibling->name,"ASSIGNOP")==0){
			traver_exp(exp->child->sibling->sibling);
			if(strcmp(exp->child->child->name,"id")!=0){
				printf("Error type 6 at line %d:the left-hand side error\n",exp->line);
			}else if(exp->child->in != exp->child->sibling->sibling->in){
				printf("Error type 5 at line %d:type mismatched for assignment\n",exp->line);
			}
			char *newvar1=newvar(j);
            j++;
            strcpy(exp->addr,newvar1);
            addirnode(1,"",exp->child->sibling->sibling->addr,"",exp->child->addr);

		}else if(strcmp(exp->child->sibling->name,"LB")==0){//数组 exp->exp[exp]
			traver_exp(exp->child->sibling->sibling);//exp
			if(exp->child->in !=3 &&exp->child->in!=4){
				printf("Error type 10 at line %d:'%s'is not an array\n",exp->line,exp->child->child->l);
			}else if(exp->child->sibling->sibling->in != 1){
				printf("Error type 12 at line %d:not interger\n",exp->line);
			}
			char *newvar1=newvar(j);
            j++;
            strcpy(exp->addr,newvar1);
			addirnode(10,"+",exp->child->addr,itoa(exp->child->sibling->sibling->child->in*4),exp->addr);

		}else if(strcmp(exp->child->sibling->name,"DOT")==0){//
			if(exp->child->in!=5){//struct 类型
				printf("Error type 13 at line %d:illegal use of '.'\n",exp->line);
			}else{
			result=search_name(head,exp->child->child->l);
			result=search_name(head,result->type);
			//printf("struct tag: %s\n",result->name);
			int num=result->weishu;
			//printf("weishu %d\n",num);
			result=result->next;
			int field=-1;
			while(num>0){
				if(strcmp(result->name,exp->child->sibling->sibling->l)==0){
					field=1;

				}
				num--;
			}
			if(field==-1){
				printf("Error type 14 at line %d:non-existent field\n",exp->line);
			}
		 }
		}
	}
}

void traver_args(struct Table_node* r,struct TREE_NODE *args,int kind){//实参
	struct TREE_NODE *p4=malloc(sizeof(struct TREE_NODE));
	p4=args;
	int count=1;
	traver_exp(p4->child);
	while(p4->child->sibling!=NULL){
		count++;
		p4=p4->child->sibling->sibling;
		traver_exp(p4->child);
	}
	if(count!=r->func_node->para_num){//数量不匹配
		printf("Error type 9 at line %d: arguments dismatched\n",args->line);
	}else{
		p4=args->child;//exp
		while(count>0){
			if(p4->in!=1){
				printf("Error type 9 at line %d: arguments dismatched\n",args->line);
				break;
			}
			if(count>1){
                r=r->next;
                p4=p4->sibling->sibling->child;//exp
			}
			if(kind==1){//ARG
                addirnode(3,"ARG",p4->addr,"","");
			}
			count--;
		}
	}
}

void addirnode(int k,char *op,char *arg1,char *arg2,char *result){
    ir_next=create_ir_node(k,op,arg1,arg2,result);
    ir_pre->next=ir_next;
    ir_next->prev=ir_pre;
    ir_pre=ir_next;
}


