#ifndef _LINEAR_LINK_H
#define _LINEAR_LINK_H



struct Table_node{
	int kind;//1int;2float;3int array;4float array;5struct;6func;7INTvalue for ir_code
	int weishu;//数组的维数 或者struct内部定义变量的个数
	char *type;//token word\struct的tag
	char *name;//the name of id
	int line;
	char *addr;
	struct Func_node *func_node;
	struct Table_node *next;
};

//some information of function
struct Func_node{
	int para_num;
	char *return_type;
};

struct Table_node* create_table_node(char *type,char *name,int line);

struct Func_node* create_func_node(int num,char *return_type);

//search
struct Table_node* search_name(struct Table_node *h ,char *name);
struct Table_node* search_num(struct Table_node *h,int num);
//add
void add_tt_node(struct Table_node *p,struct Table_node *q);
void add_tf_node(struct Table_node *p,struct Func_node *q);
int type(char *type);
#endif
