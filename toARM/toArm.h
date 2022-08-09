#ifndef TARM_H_
#define TARM_H_
#include "../include/def.h"
typedef struct armcode_ armcode;

typedef enum _optype
{
    NUL, //空值
    IMME,
    ILIMME,
    REG,
    REGLIST,
    STRING,//字符串类型
    MEM,
    LSL,

   
} optype;

typedef enum _armop
{
    MOV,
    ADD,
    SUB,
    MUL,
    CMP,
    B,
    BL,
    BX,
    LDR,
    STR,
    STMFD,
    LDMFD,
    ARMLABEL,
    FUNCLABEL,
    ENDLABEL,
    GVAR_INT,   //整型全局变量
    GVAR_FLOAT, //浮点型全局变量
    GVAR_LABEL,  //全局变量地址

} armop;

typedef enum _opflag
{
    EQU=25,
    NE,
    GE,
    LT,
    GT,
    LE,

} opflag;

typedef enum _indexkind
{   
    immeindex,
    regindex,
    memindex,
}indexkind;

typedef struct armoper_
{
    optype type; //寄存器直接，立即数，带=号的立即数，寄存器间接，//TODO 变址寻址
    union 
    {
        int value;   //立即数（合法&非法），寄存器编号
        char str_id[33];//label
        short reglist[14];//stmfd,ldrfd寄存器列表
    };
    int index;      //相对寻址，指针偏移
    indexkind kind;

} armoper;

struct armcode_
{
    armop op;
    opflag flag;
    bool is_s;
    armoper result; // result
    armoper oper1;  //···||全局变量初始值,全局变量标号名
    armoper oper2;       
    armoper oper3; //用来移位的
    struct armcode_ *next;
    struct armcode_ *pre;
   
    int regnum;//寄存器列表寄存器个数
};



typedef struct _varnode
{
    char name[33];
    int index;
    indexkind kind;
}varnode;

typedef struct _vartable
{
    varnode table[10000];
    int nodecmt;
}vartable;                  //记录所有参数，局部变量



#define R0 0
#define R1 1
#define R2 2
#define R4 4
#define R5 5
#define R10 10
#define R11 11
#define R12 12
#define R13 13
#define LR 14


void arminterface();
void translate(armcode *newnode, struct codenode *p, armop armop,armcode *q);
armcode *initnewnode();
armcode *translatearm(Blocks *blocks);
void printarm(armcode *armnode, FILE *fp);


void armlink_insert(armcode * newnode,armcode* inode);

int vartable_insert(vartable* table,char *varname,indexkind kind,int index);    //成功返回下标，-1失败
int vartable_select(vartable* table,char *varname);                              //成功返回下标，-1失败
int vartable_update(vartable* table,char *varname,indexkind kind,int index);    //成功返回修改后下标，-1失败
int vartable_update_all(vartable* table,int stkamt);              //修改整个表中，栈中变量的偏移量
vartable * vartable_create();

armcode *mul_reg_node(armop opt,int stkreg, int reg[], int regnum); //生成LDMFD、STMFD节点

armcode *search_global_var();
armcode *traverse_List(List *value_list,int arysize);

armcode * gvar_node_list(int func_gvar_num,char **gvartable);
armcode * gvar_node(char ** gvartable,int i,int func_index);

armcode * create_ldrnode(int Rn,char * gvarname,int Rm,int index);
void init_strnode(armcode * snode,int R_res,int Rm,int index,int indexkind);
armcode * create_movnode(int R_res,optype type,int value);
armcode* create_strnode(int Rn,int Rm,int index);
armcode * create_addnode(int R_res,optype type_op1,int op1,int type_op2,int op2,optype type_op3,int op3);
void init_ldrnode(armcode * lnode,int Rn,char * gvarname,int Rm,int index,int indexkind);

void init_myreg();
int alloc_myreg();

int get_other_reg(int i, int j);


#endif