#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "oc.h"
#include "ir.h"

char register_table[10][6][5]={
    {"t0","","","","","0"},
    {"t1","","","","","0"},
    {"t2","","","","","0"},
    {"t3","","","","","0"},
    {"t4","","","","","0"},
    {"t5","","","","","0"},
    {"t6","","","","","0"},
    {"t7","","","","","0"},
    {"t8","","","","","0"},
    {"t9","","","","","0"}
    };


void ir_to_oc(struct ir_node *head){
    int temp=0;//arg
    char *param=malloc(sizeof(char[4]));
    struct ir_node *p=head;
    //.data
    char* headstr=".data\n_prompt: .asciiz \"enter an interger:\"\n_ret: .asciiz \"\\n\"\n.globl main";
    printf("%s\n.text\n",headstr);
    //read
    printf("read:\nli $v0,4\nla $a0,_prompt\nsyscall\n");
    printf("li $v0,5\nsyscall\njr $ra\n");
    //write
    printf("write:\nli $v0,1\n");
    printf("syscall\nli $v0,4\nla $a0, _ret\nsyscall\n");
    printf("move $v0,$0\njr $ra\n");
    while(p!=NULL){
        if(p->kind==1){
            char *result1=find_register(p->result);
            if(strcmp(p->op,"")==0){
                char *arg1=find_register(p->arg1);
                printf("move $%s, $%s\n",result1,arg1);
            }else if(strcmp(p->op,"#")==0){
                printf("li $%s,%s\n",result1,p->arg1);
            }
        }else if(p->kind==2){
            char *result1=find_register(p->result);
            char *arg1=find_register(p->arg1);
            char *arg2=find_register(p->arg2);
            if(strcmp(p->op,"PLUS")==0){
                printf("add $%s,$%s,$%s\n",result1,arg1,arg2);
            }else if(strcmp(p->op,"MINUS")==0){
                printf("sub $%s,$%s,$%s\n",result1,arg1,arg2);
            }else if(strcmp(p->op,"STAR")==0){
                printf("mul $%s,$%s,$%s\n",result1,arg1,arg2);
            }else if(strcmp(p->op,"DIV")==0){
                printf("div $%s,$%s\n",arg1,arg2);
                printf("mflo $%s\n",result1);
            }

        }else if(p->kind==3){
            if(strcmp(p->op,"GOTO")==0){
                printf("j %s\n",p->arg1);
            }else if(strcmp(p->op,"ARG")==0){
                if(temp!=0){
                    printf("add $sp,$sp,-4\n");
                    printf("sw $%s,0($sp)\n",param);
                }
                char *arg1=find_register(p->arg1);
                printf("move $a0,$%s\n",arg1);


            }

        }else if(p->kind==4){
            printf("%s:\n",p->arg1);

        }else if(p->kind==5){
            char *result1=find_register(p->result);
            printf("add $sp,$sp,-4\n");
            printf("sw $ra,0($sp)\n");
            printf("jal %s\n",p->arg1);
            printf("lw $ra,0($sp)\n");
            printf("add $sp,$sp,4\n");
            printf("move $%s,$v0\n",result1);
            if(temp!=0){

                printf("lw $%s,0($sp)\n",param);
                printf("add $sp,$sp,4\n");
                temp--;
            }

        }else if(p->kind==6){
            char *result=find_register(p->arg1);
            if(strcmp(p->op,"RETURN")==0){
                printf("move $v0,$%s\njr $ra\n\n",result);
            }else{//param
                printf("move $%s,$a0\n",result);

                temp++;
                strcpy(param,result);
            }

        }else if(p->kind==7){
            char *result1=find_register(p->arg1);
            printf("add $sp,$sp,-4\n");
            printf("sw $ra,0($sp)\n");
            printf("jal read\n");
            printf("lw $ra,0($sp)\n");
            printf("add $sp,$sp,4\n");
            printf("move $%s,$v0\n",result1);

        }else if(p->kind==8){
            char *arg1=find_register(p->arg1);
            printf("move $a0,$%s\n",arg1);
            printf("add $sp,$sp,-4\n");
            printf("sw $ra,0($sp)\n");
            printf("jal write\n");
            printf("lw $ra,0($sp)\n");
            printf("add $sp,$sp,4\n");

        }else if(p->kind==9){


        }else if(p->kind==10){


        }else if(p->kind==11){
            char *arg1=find_register(p->arg1);
            char *arg2=find_register(p->arg2);
            if(strcmp(p->op,"==")==0){
                printf("beq $%s,$%s,",arg1,arg2);
            }else if(strcmp(p->op,"!=")==0){
                printf("bne $%s,$%s,",arg1,arg2);
            }else if(strcmp(p->op,">")==0){
                printf("bgt $%s,$%s,",arg1,arg2);
            }else if(strcmp(p->op,"<")==0){
                printf("blt $%s,$%s,",arg1,arg2);
            }else if(strcmp(p->op,">=")==0){
                printf("bge $%s,$%s,",arg1,arg2);
            }else if(strcmp(p->op,"<=")==0){
                printf("ble $%s,$%s,",arg1,arg2);
            }

        }else if(p->kind==12){
            printf("%s\n",p->arg1);

        }
        p=p->next;

    }

}
char* get_register(char *arg){
    for(int i=0;i<10;i++){//search for empty register
        if(strcmp(&register_table[i][5][0],"0")==0){
            int reg=register_table[i][5][0]-47;
            strcpy(register_table[i][reg],arg);
            register_table[i][5][0]++;
            add_addr(arg,&register_table[i][0][0]);
            return &register_table[i][0][0];

        }
    }
    //no empty register
    for(int i=0;i<10;i++){
        if(strcmp(&register_table[i][5][0],"1")==0){
            del_addr(&register_table[i][1][0],&register_table[i][0][0]);

            //store the old
            //printf("sw %s,num\n",&register_table[i][1][0]);
            //strcpy(register_table[i][1],arg);
        }
    }
}

char* add_addrtable(char *arg){
    for(int i=0;i<20;i++){
        if(strcmp(&addr_table[i][0][0],"")==0){
            strcpy(addr_table[i][0],arg);
            return get_register(arg);
        }
    }

}

void* add_addr(char *arg,char *regis){
    for(int i=0;i<20;i++){
        if(strcmp(&addr_table[i][0][0],arg)==0){
            for(int j=1;j<5;j++){
                if(strcmp(&addr_table[i][j][0],"")==0){
                    strcpy(addr_table[i][j],regis);
                }
            }
        }
    }
}

void* del_addr(char *arg,char *regis){
    for(int i=0;i<20;i++){
        if(strcmp(&addr_table[i][0][0],arg)==0){
            for(int j=1;j<5;j++){
                if(strcmp(&addr_table[i][j][0],regis)==0){
                    strcpy(addr_table[i][j],"");
                }
            }
            //strcpy(addr_table[i][5],"num");
        }
    }
}

char* find_register(char *arg){
    for(int i=0;i<20;i++){
        if(strcmp(&addr_table[i][0][0],arg)==0){
            for(int j=1;j<5;j++){
                if(strcmp(&addr_table[i][j][0],"")!=0){
                    return &addr_table[i][j][0];
                }
            }
            return get_register(arg);
        }
    }
    return add_addrtable(arg);
}
