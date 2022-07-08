#ifndef TARM_H_
#define TARM_H_
#include "def.h"

typedef struct armcode_
{
    int op;
    int flag;
    bool is_s;
    armoper oper1;
    armoper oper2;
    armoper oper3;
    armoper oper4; //用来移位的
    armcode *next;
    armcode *pre;
} armcode;
typedef struct armoper_
{
    int type; //寄存器直接，立即数，带=号的立即数，寄存器间接，//TODO 变址寻址
    int i;

} armoper;
#endif