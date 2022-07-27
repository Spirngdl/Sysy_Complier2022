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
    STR,
   
} optype;

typedef enum
{
    MOV,
    ADD,
    SUB,
    MUL,
    LDR,
    STMFD,
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
    

} armoper;

struct armcode_
{
    armop op;
    opflag flag;
    bool is_s;
    armoper result; // result
    armoper oper1;
    armoper oper2;
    armoper oper3; //用来移位的
    struct armcode_ *next;
    struct armcode_ *pre;
   
    int regnum;//寄存器列表寄存器个数
};

#define R0 0
#define R1 1
#define R2 2
#define R4 4
#define R5 5
#define R13 13

int regcountmask[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};

void arminterface();
void translate(armcode *newnode, struct codenode *p, armop armop);
armcode *initnewnode();
armcode *translatearm(Blocks *blocks);
void printarm(armcode *armnode, FILE *fp);
int regcount(short reg)
#endif