#include "../include/def.h"
char *Oper[5] = {"MOV", "ADD", "SUB", "MUL", "LDR"};

void printarm(armcode *armnode, FILE *fp)
{

    armcode *p = armnode->next;
    while (p->next != NULL)
    {
        if (p->oper1.type == REG)
        {
            switch (p->oper2.type)
            {
            case IMME:
                fprintf(fp, "\t%S  R%d , R%d , #%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value, p->oper2.value);
                break;
            case REG:
                fprintf(fp, "\t%s  R%d , R%d , R%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value, p->oper2.value);
                break;
            case NUL:
                fprintf(fp, "\t%s  R%d , R%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value);
                break;
            default:
                printf("p->oper2.type error!\n");
            }
        }
        else if (p->oper1.type == IMME)
        {
            fprintf(fp, "\t%s  R%d , #%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value);
        }
        else if (p->oper1.type == ILIMME)
        {
            fprintf(fp, "\t%s  R%d , =%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value); // the op should be LDR
        }
        else if(p->oper1.type == NUL)
        {
            if(p->op == ARMLABEL)
            {
                fprintf(fp,"%s\n",p->result.str_id);
            }
        }
    }
}

void arminterface(Blocks *blocks)
{
    FILE *fp = fopen("./a.asm", "w+");
    armcode *armcode = translatearm(blocks);
    printarm(armcode, fp);
}