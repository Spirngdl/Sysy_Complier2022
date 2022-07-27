#include "../include/def.h"

char *Reg[16];

bool MULFLAG = false;
// bool FIRSTARG = false;//第一次遇到ARG时，插入压栈操作后置true，遇到call时再置false
int ARGTIM = 0;

armcode *initnewnode()
{
    armcode *newnode = (armcode *)malloc(sizeof(struct armcode_));
    memset(newnode, 0, sizeof(struct armcode_));
    newnode->result.type = REG;

    return newnode;
}

void translate(armcode *newnode, struct codenode *p, armop armop)
{
    int rn0, rn1, rn2, rn3;
    if ((rn0 = search_var(p->result.id)) >= 0)
    {
        Reg[rn0] = p->result.id;
        newnode->op = armop;
        newnode->result.value = rn0;

        if (p->opn1.kind == LITERAL)
        {
            // armcode *movcode = (armcode *)malloc(sizeof(struct armcode_));
            armcode *movcode = initnewnode();
            movcode->op = MOV;
            movcode->result.value = R4;
            movcode->oper1.type = IMME;
            movcode->oper1.value = p->opn1.const_int;

            newnode->pre->next = movcode;
            movcode->pre = newnode->pre;
            movcode->next = newnode;
            newnode->pre = movcode;

            newnode->oper1.type = REG;
            newnode->oper1.value = R4;
            if (p->opn2.kind == LITERAL)
            {
                // printf("ADD R%d ,R0 ,#%d\n", rn0, p->opn2.const_int);
                newnode->oper2.type = IMME;
                newnode->oper2.value = p->opn2.const_int;
            }
            else if (p->opn2.kind == ID)
            {
                if ((rn2 = search_var(p->opn2.id)) >= 0)
                {
                    // printf("ADD R%d ,R%d ,#%d", rn0, rn1, p->opn1.const_int);
                    Reg[rn2] = p->opn2.id;
                    newnode->oper2.type = REG;
                    newnode->oper2.value = rn2;
                }
                else
                {
                    printf("Failed to allocate register for opn2\n");
                }
            }
            else if (p->opn2.kind == FLOAT_LITERAL)
            {
                // TODO
            }
            else
            {
                printf("p->opn2.kind error!\n");
            }
        }
        else if (p->opn1.kind == ID)
        {
            if ((rn1 = search_var(p->opn1.id)) >= 0)
            {
                if (MULFLAG && rn0 == rn1)
                {
                    MULFLAG = false;
                    // armcode *movcode = (armcode *)malloc(sizeof(struct armcode_));
                    armcode *movcode = initnewnode();
                    movcode->op = MOV;
                    movcode->result.value = R4;
                    movcode->oper1.type = REG;
                    movcode->oper1.value = rn1;

                    newnode->pre->next = movcode;
                    movcode->pre = newnode->pre;
                    movcode->next = newnode;
                    newnode->pre = movcode;

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R4;
                }
                else
                {
                    Reg[rn1] = p->opn1.id;
                    newnode->oper1.type = REG;
                    newnode->oper1.value = rn1;
                }

                if (p->opn2.kind == LITERAL)
                {
                    // printf("ADD R%d ,R%d ,#%d", rn0, rn1, p->opn2.const_int);
                    newnode->oper2.type = IMME;
                    newnode->oper2.value = p->opn2.const_int;
                }
                else if (p->opn2.kind == ID)
                {
                    if ((rn2 = search_var(p->opn2.id)) > 0)
                    {
                        // printf("ADD R%d ,R%d ,R%d", rn0, rn1, rn2);
                        Reg[rn2] = p->opn2.id;
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else
                    {
                        printf("TOK_:Failed to allocate register for opn2\n");
                    }
                }
                else if (p->opn2.kind == FLOAT_LITERAL)
                {
                    // TODO
                }
                else
                {
                    printf("TOK_:p->opn2.kind error!\n");
                }
            }
            else
            {
                printf("TOK_:Failed to allocate register for opn1\n");
            }
        }
        else if (p->opn2.kind == FLOAT_LITERAL)
        {
            // TODO
        }
        else
        {
            printf("p->opn1.kind error!\n");
            newnode->oper2.type = NUL;
        }
    }
}

armcode *translatearm(Blocks *blocks)
{
    struct codenode *p;
    armcode *q = NULL;
    armcode *newnode = NULL;
    armcode *first = (armcode *)malloc(sizeof(struct armcode_)); // header node
    memset(first, 0, sizeof(struct armcode_));

    q = first;
    int rn0, rn1, rn2, rn3;
    Blocks *cur_blocks = blocks;
    struct codenode *result = NULL;
    while (cur_blocks != NULL) //遍历所有基本块
    {
        for (int i = 0; i < cur_blocks->count; i++)
        {
            struct codenode *tcode = cur_blocks->block[i]->tac_list;
            p = tcode;
            while (p->next != NULL)
            {
                // newnode = (armcode *)malloc(sizeof(struct armcode_));
                //  memset(newnode, 0, sizeof(struct armcode_));
                newnode = initnewnode();
                q->next = newnode;
                newnode->pre = q;
                q = newnode;
                switch (p->op)
                {
                case FUNCTION:
                    newnode->op = ARMLABEL;
                    newnode->result.type = STR;
                    strcpy(newnode->result.str_id, p->result.id);
                    newnode->oper1.type = NUL;
                    newnode->oper2.type = NUL;
                    break;
                case TOK_ASSIGN:
                    if ((rn0 = search_var(p->result.id)) >= 0) // The result is stored in the register
                    {
                        Reg[rn0] = p->result.id;
                        newnode->op = MOV;
                        // newnode->oper1.type = REG;
                        newnode->result.value = rn0;

                        if (p->opn1.kind == LITERAL)
                        {
                            printf("MOV R%d ,#%d\n", rn0, p->opn1.const_int);
                            newnode->oper1.type = IMME;
                            newnode->oper1.value = p->opn1.const_int;
                        }
                        else if (p->opn1.kind == ID)
                        {
                            if ((rn1 = search_var(p->opn1.id)) >= 0)
                            {
                                Reg[rn1] = p->opn1.id;
                                printf("MOV R%d ,R%d\n", rn0, rn1);
                                newnode->oper2.type = REG;
                                newnode->oper2.value = rn1;
                            }
                        }
                        else if (p->opn1.kind == FLOAT_LITERAL)
                        {
                            // TODO
                        }
                        else
                        {
                            newnode->oper2.type = NUL;
                        }
                    }

                    break;

                case TOK_LDR:
                    if ((rn0 = search_var(p->result.id)) >= 0)
                    {
                        Reg[rn0] = p->result.id;
                        newnode->op = LDR;
                        newnode->result.value = rn0;

                        if (p->opn1.kind == LITERAL)
                        {
                            newnode->oper1.type = ILIMME;
                            newnode->oper1.value = p->opn1.const_int;
                        }
                        else
                        {
                            printf("p->opn1.kind must be LITERAL!\n");
                        }
                    }
                    break;

                case TOK_ADD:
                    translate(newnode, p, ADD);
                    break;

                case TOK_MUL:
                    MULFLAG = true;
                    translate(newnode, p, MUL);
                    break;

                case TOK_SUB:
                    translate(newnode, p, SUB);
                    break;

                case ARG:
                    if (!ARGTIM)
                    {
                        int reg[5] = {0, 1, 2, 3, 14};
                        armcode *snode = stmnode(R13, reg, 5);
                        newnode->pre->next = snode;
                        snode->pre = newnode->pre;
                        snode->next = newnode;
                        newnode->pre = snode;
                    }
                    if (ARGTIM > 3) //超过四个参数，压栈
                    {

                    }
                    else            //四个参数以内，寄存器传值
                    {
                        
                        newnode->result.value = ARGTIM;
                        if(p->result.kind == ID)
                        {
                            newnode->op = MOV;
                           if(( rn1 = search_var(p->result.id))>=0)
                           {
                                Reg[rn0] = p->result.id;
                                newnode->oper1.type = REG;
                                newnode->oper1.value = rn1;
                           }
                           else
                           {
                            //todo
                           }
                        }
                        else if(p->result.kind == LITERAL)
                        {
                            if(check_imme(p->result.const_int)==0)  //合法立即数
                            {
                                newnode->op = MOV;
                                newnode->oper1.type = IMME;
                                newnode->oper1.value = p->result.const_int;
                            }
                            else
                            {
                                newnode->op = LDR;
                                newnode->oper1.type = ILIMME;
                                newnode->oper1.value = p->result.const_int;

                            }
                        }
                    }
                    ARGTIM += 1;
                    break;

                case CALL:
                    ARGTIM = 0;
                    break;

                default:
                    break;
                }
                p = p->next;
            }
        }
        cur_blocks = cur_blocks->next;
    }
    return first;
}

armcode *stmnode(int stkreg, int reg[], int regnum)
{
    // armcode * snode = (armcode*)malloc(sizeof(struct armcode_));
    armcode *snode = initnewnode();
    snode->op = STMFD;
    snode->regnum = regnum;
    snode->result.value = stkreg;
    snode->oper1.type = REGLIST;
    for (int i = 0; i < regnum; i++)
    {
        snode->oper1.reglist[i] = reg[i];
    }

    return snode;
}

int regcount(short reg)
{
    int temp = 0, count = 0;
    for (int i = 0; i < 4; i++)
    {
        temp = (reg && (0xF << i * 4));
        temp = temp >> i * 4;
        count += regcountmask[temp];
    }
    return count;
}