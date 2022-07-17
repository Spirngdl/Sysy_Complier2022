#ifndef TARM_H_
#define TARM_H_
#include "../include/def.h"
typedef struct armcode_ armcode;

typedef enum
{
    IMME,
    ILIMME,
    REG,
    NUL, //空值
} optype;

typedef enum
{
    MOV,
    ADD,
    SUB,
    MUL,
    LDR,

} armop;

typedef enum
{
    // EQ,
    NE,
    GE,
    LT,
    GT,
    LE,

} opflag;

typedef struct armoper_
{
    optype type; //寄存器直接，立即数，带=号的立即数，寄存器间接，//TODO 变址寻址
    int value;   //立即数（合法&非法），寄存器编号

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
    short int reg[14];
    int num;
};

#define R0 0
#define R1 1
#define R2 2
#define R4 4
#define R5 5


void arminterface();
void translate(armcode *newnode, struct codenode *p, armop armop);
armcode *initnewnode(armcode *newnode);
armcode *translatearm(Blocks *blocks);
void printarm(armcode *armnode, FILE *fp);
#endif