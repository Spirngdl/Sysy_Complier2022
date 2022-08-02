#include "../include/def.h"
// char *Oper[5] = {"MOV", "ADD", "SUB", "MUL", "LDR"};

// void printarm(armcode *armnode, FILE *fp)
// {

//     armcode *p = armnode->next;
//     while (p != NULL)
//     {
//         if (p->oper1.type == REG)
//         {
//             switch (p->oper2.type)
//             {
//             case IMME:
//                 fprintf(fp, "\t%S  R%d , R%d , #%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value, p->oper2.value);
//                 break;
//             case REG:
//                 fprintf(fp, "\t%s  R%d , R%d , R%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value, p->oper2.value);
//                 break;
//             case NUL:
//                 fprintf(fp, "\t%s  R%d , R%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value);
//                 break;
//             default:
//                 printf("p->oper2.type error!\n");
//             }
//         }
//         else if (p->oper1.type == IMME)
//         {
//             fprintf(fp, "\t%s  R%d , #%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value);
//         }
//         else if (p->oper1.type == ILIMME)
//         {
//             fprintf(fp, "\t%s  R%d , =%d\n", Oper[p->op - MOV], p->result.value, p->oper1.value); // the op should be LDR
//         }
//         else if(p->oper1.type == NUL)
//         {
//             if(p->op == ARMLABEL)
//             {
//                 fprintf(fp,"%s\n",p->result.str_id);
//             }
//         }
//     }
// }

void printarm(armcode *armnode, FILE *fp)
{
    armcode * p = armnode->next;
    int tmp;
    while(p != NULL)
    {
        switch (p->op)
        {
        case ARMLABEL:
            fprintf(fp,"%s\n",p->result.str_id);
            if(p->oper1.type == STRING)
            {
                fprintf(fp,"\t%s    %d\n",p->oper1.str_id,p->oper2.value);
            }
            break;

        case MOV:
            if(p->oper1.type == REG)
            {
                fprintf(fp,"\tMOV   R%d , R%d\n",p->result.value,p->oper1.value);
            }
            else if(p->oper1.type == IMME)
            {
                fprintf(fp,"\tMOV   R%d , #%d\n",p->result.value,p->oper1.value);
            }
            else
            {
                if(p->oper1.type == NUL)
                {

                }
                else
                {
                    printf("MOV node oper1.type error!\n");
                }
            }
            break;

        case ADD:
            if(p->oper2.type == REG)
            {
                fprintf(fp,"\tADD   R%d , R%d , R%d\n",p->result.value,p->oper1.value,p->oper2.value);
            }
            else if(p->oper2.type == IMME)
            {
                fprintf(fp,"\tADD   R%d , R%d , #%d\n",p->result.value,p->oper1.value,p->oper2.value);
            }
            else
            {
                printf("ADD node oper2.type error!\n");
            }
            break;

        case SUB:
            if(p->oper2.type == REG)
            {
                fprintf(fp,"\tSUB   R%d , R%d , R%d\n",p->result.value,p->oper1.value,p->oper2.value);
            }
            else if(p->oper2.type == IMME)
            {
                fprintf(fp,"\tSUB   R%d , R%d , #%d\n",p->result.value,p->oper1.value,p->oper2.value);
            }
            else
            {
                printf("SUB node oper2.type error!\n");
            }
            break;

        case MUL:
            fprintf(fp,"\tMUL   R%d , R%d , R%d\n",p->result.value,p->oper1.value,p->oper2.value);
            break;

        case BL:
            fprintf(fp,"\tBL    %s\n",p->result.str_id);
            break;

        case BX:
            fprintf(fp,"\tBX    R%d\n",p->result.value);
            break;

        case LDR:
            if(p->oper1.type == MEM)
            {
                fprintf(fp,"\tLDR   R%d , [R%d,#%d]\n",p->result.value,p->oper1.value,p->oper1.index);
            }
            else if(p->oper1.type == ILIMME)
            {
                fprintf(fp,"\tLDR   R%d , =%d\n",p->result.value,p->oper1.value);
            }
            else
            {
                printf("LDR node oper1.type error!\n");
            }
            break;

        case STR:
            fprintf(fp,"\tSTR   R%d , [R%d,#%d]\n",p->result.value,p->oper1.value,p->oper1.index);
            break;
        
        case STMFD:
            fprintf(fp,"\tSTMFD R%d! , {R%d",p->result.value,p->oper1.reglist[0]);
            for(tmp=1;tmp<p->regnum;tmp++)
            {
                fprintf(fp,",R%d",p->oper1.reglist[tmp]);
            }
            fprintf(fp,"}\n");
            break;
        
        case LDMFD:
            fprintf(fp,"\tLDMFD R%d! , {R%d",p->result.value,p->oper1.reglist[0]);
            for(tmp=1;tmp<p->regnum;tmp++)
            {
                fprintf(fp,",R%d",p->oper1.reglist[tmp]);
            }
            fprintf(fp,"}\n");
            break;
        
        default:
            break;
        }
        
        p=p->next;
    }
}

void arminterface(Blocks *blocks)
{
    FILE *fp = fopen("./a.asm", "w");
    armcode *armcode = translatearm(blocks);
    printarm(armcode, fp);
}
