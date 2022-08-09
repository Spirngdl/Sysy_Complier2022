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

char *flag[6] = {"EQ", "NE", "GE", "LT", "GT", "LE"};

void printarm(armcode *armnode, FILE *fp)
{
    armcode *p = armnode->next;
    int tmp;
    fprintf(fp,"\t.text\n");
    fprintf(fp,"\t.code     32\n");
    while (p != NULL)
    {
        switch (p->op)
        {
        case ARMLABEL:
            fprintf(fp, "%s:\n", p->result.str_id);
            break;

        case FUNCLABEL:
            fprintf(fp,"\t.align    2\n");
            fprintf(fp,"\t.global   %s\n",p->result.str_id);
            fprintf(fp,"\t.type %s  ,%%function\n",p->result.str_id);
            fprintf(fp,"%s:\n",p->result.str_id);
            break;

        case ENDLABEL:
            fprintf(fp,"\t.size %s , .-%s\n",p->result.str_id,p->result.str_id);
            break;

        case GVAR_INT:
            fprintf(fp, "\t%s   %d\n", p->result.str_id, p->oper1.value);
            break;

        case GVAR_FLOAT:
            fprintf(fp, "\t%s    %x\n", p->result.str_id, p->oper1.value);
            break;

        case GVAR_LABEL:
            fprintf(fp, "\t%s    %s\n", p->result.str_id, p->oper1.str_id);
            break;

        case MOV:
            if (p->oper1.type == REG)
            {
                fprintf(fp, "\tMOV   R%d , R%d\n", p->result.value, p->oper1.value);
            }
            else if (p->oper1.type == IMME)
            {
                fprintf(fp, "\tMOV   R%d , #%d\n", p->result.value, p->oper1.value);
            }
            else
            {
                if (p->oper1.type == NUL)
                {
                }
                else
                {
                    printf("MOV node oper1.type error!\n");
                }
            }
            break;

        case ADD:
            if (p->oper2.type == REG)
            {
                if(p->oper3.type == LSL)
                {
                   fprintf(fp, "\tADD   R%d , R%d , R%d,LSL #%d\n", p->result.value, p->oper1.value, p->oper2.value,p->oper3.value); 
                }
                else
                {
                    fprintf(fp, "\tADD   R%d , R%d , R%d\n", p->result.value, p->oper1.value, p->oper2.value);
                }
                
            }
            else if (p->oper2.type == IMME)
            {
                fprintf(fp, "\tADD   R%d , R%d , #%d\n", p->result.value, p->oper1.value, p->oper2.value);
            }
            else
            {
                printf("ADD node oper2.type error!\n");
            }
            break;

        case SUB:
            if (p->oper2.type == REG)
            {
                fprintf(fp, "\tSUB   R%d , R%d , R%d\n", p->result.value, p->oper1.value, p->oper2.value);
            }
            else if (p->oper2.type == IMME)
            {
                fprintf(fp, "\tSUB   R%d , R%d , #%d\n", p->result.value, p->oper1.value, p->oper2.value);
            }
            else
            {
                printf("SUB node oper2.type error!\n");
            }
            break;

        case MUL:
            fprintf(fp, "\tMUL   R%d , R%d , R%d\n", p->result.value, p->oper1.value, p->oper2.value);
            break;

        case BL:
            fprintf(fp, "\tBL    %s\n", p->result.str_id);
            break;

        case BX:
            fprintf(fp, "\tBX    R%d\n", p->result.value);
            break;

        case LDR:
            if (p->oper1.type == MEM)
            {
                if(p->oper1.kind == immeindex)
                {
                    fprintf(fp, "\tLDR   R%d , [R%d,#%d]\n", p->result.value, p->oper1.value, p->oper1.index);
                }
                else 
                {
                    fprintf(fp, "\tLDR   R%d , [R%d,R%d]\n", p->result.value, p->oper1.value, p->oper1.index);
                }
            }
            else if (p->oper1.type == ILIMME)
            {
                fprintf(fp, "\tLDR   R%d , =%d\n", p->result.value, p->oper1.value);
            }
            else if (p->oper1.type == STRING)
            {
                fprintf(fp, "\tLDR   R%d , %s\n", p->result.value, p->oper1.str_id);
            }
            else
            {
                printf("LDR node oper1.type error!\n");
            }
            break;

        case STR:
            if(p->oper1.kind == immeindex)
            {
                if (p->oper1.index == 0)
                {
                    fprintf(fp, "\tSTR   R%d , [R%d]\n", p->result.value, p->oper1.value);
                }
                else
                {
                    fprintf(fp, "\tSTR   R%d , [R%d,#%d]\n", p->result.value, p->oper1.value, p->oper1.index);
                }
            }
            else if(p->oper1.kind == regindex)
            {
                fprintf(fp, "\tSTR   R%d , [R%d,R%d]\n", p->result.value, p->oper1.value, p->oper1.index);
            }
            
            break;

        case STMFD:
            fprintf(fp, "\tSTMFD R%d! , {R%d", p->result.value, p->oper1.reglist[0]);
            for (tmp = 1; tmp < p->regnum; tmp++)
            {
                fprintf(fp, ",R%d", p->oper1.reglist[tmp]);
            }
            fprintf(fp, "}\n");
            break;

        case LDMFD:
            fprintf(fp, "\tLDMFD R%d! , {R%d", p->result.value, p->oper1.reglist[0]);
            for (tmp = 1; tmp < p->regnum; tmp++)
            {
                fprintf(fp, ",R%d", p->oper1.reglist[tmp]);
            }
            fprintf(fp, "}\n");
            break;

        case B:
            if(p->flag == 0)
            {
                fprintf(fp,"\tB %s\n",p->result.str_id);
            }
            else
            {
                fprintf(fp,"\tB%s %s\n",flag[p->flag-EQU],p->result.str_id);
            }
            break;

        case CMP:
            if (p->oper2.type == IMME)
            {
                fprintf(fp, "\tCMP   R%d , #%d\n", p->oper1.value, p->oper2.value);
            }
            else if (p->oper2.type == REG)
            {
                fprintf(fp, "\tCMP   R%d , R%d\n", p->oper1.value, p->oper2.value);
            }
            break;

        default:
            break;
        }

        p = p->next;
    }
}

void arminterface(Blocks *blocks)
{
    FILE *fp = fopen(dst_path, "w");
    armcode *armcode = translatearm(blocks);
    printarm(armcode, fp);
}
