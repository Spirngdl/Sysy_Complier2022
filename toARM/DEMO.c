#include "../include/def.h"

char *Reg[16];

char *Oper[5] = {"MOV", "ADD", "SUB", "MUL", "LDR"};

bool MULFLAG = false;

armcode *initnewnode(armcode *newnode)
{
    memset(newnode, 0, sizeof(struct armcode_));
    newnode->result.type = REG;

    return newnode;
}

void translate(armcode *newnode, struct codenode *p, armop armop)
{
    int rn0, rn1, rn2, rn3;
    if ((rn0 = search_var(p->result.id)) > 0)
    {
        Reg[rn0] = p->result.id;
        newnode->op = armop;
        newnode->result.value = rn0;

        if (p->opn1.kind == LITERAL)
        {
            armcode *movcode = (armcode *)malloc(sizeof(struct armcode_));
            initnewnode(movcode);
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
                if ((rn2 = search_var(p->opn2.id)) > 0)
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
            if ((rn1 = search_var(p->opn1.id)) > 0)
            {
                if (MULFLAG && rn0 == rn1)
                {
                    MULFLAG = false;
                    armcode *movcode = (armcode *)malloc(sizeof(struct armcode_));
                    initnewnode(movcode);
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
                newnode = (armcode *)malloc(sizeof(struct armcode_));
                // memset(newnode, 0, sizeof(struct armcode_));
                initnewnode(newnode);
                q->next = newnode;
                newnode->pre = q;
                q = newnode;
                switch (p->op)
                {
                case FUNCTION:
                    newnode->op = ARMLABEL;
                    newnode->result.type = STR;
                    strcpy(newnode->result.str_id , p->result.id);
                    newnode->oper1.type = NUL;
                    newnode->oper2.type = NUL;
                    break;
                case TOK_ASSIGN:
                    if ((rn0 = search_var(p->result.id)) > 0) // The result is stored in the register
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
                            if ((rn1 = search_var(p->opn1.id)) > 0)
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
                    if ((rn0 = search_var(p->result.id)) > 0)
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
