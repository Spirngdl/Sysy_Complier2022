#ifndef TARM_H_
#define TARM_H_
#include "../include/def.h"
typedef struct armcode_ armcode;

typedef enum
{
    NUL, //空值
    IMME,
    ILIMME,
    REG,
    REGLIST,
    STRING,//字符串类型
    MEM,
   
} optype;

typedef enum
{
    MOV,
    ADD,
    SUB,
    MUL,
    BL,
    BX,
    LDR,
    STR,
    STMFD,
    LDMFD,
    ARMLABEL,

} armop;

typedef enum
{
    // EQ,
    NE=25,
    GE,
    LT,
    GT,
    LE,

} opflag;

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

} armoper;

struct armcode_
{
    armop op;
    opflag flag;
    bool is_s;
    armoper result; // result
    armoper oper1;  //···||在全局变量声明时，oper1.type为STRING，str_id为字符串值
    armoper oper2;       //oper2.type 为imme，oper2.value为初始值
    armoper oper3; //用来移位的
    struct armcode_ *next;
    struct armcode_ *pre;
   
    int regnum;//寄存器列表寄存器个数
};

typedef enum
{
    regindex,
    memindex,
}indexkind;

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
#define R11 11
#define R12 12
#define R13 13
#define LR 14


void arminterface();
void translate(armcode *newnode, struct codenode *p, armop armop);
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


#endif