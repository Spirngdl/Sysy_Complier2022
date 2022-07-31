#include "../include/def.h"

char *Reg[16];
char *funcname[33];

bool MULFLAG = false;
// bool FIRSTARG = false;//第一次遇到ARG时，插入压栈操作后置true，遇到call时再置false
int ARGTIM = 0;
vartable *vartbl;

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
    if ((rn0 = search_var(funcname, p->result.id)) >= 0)
    {
        Reg[rn0] = p->result.id;
        newnode->op = armop;
        newnode->result.value = rn0;

        if (p->opn1.kind == LITERAL)
        {
            // armcode *movcode = (armcode *)malloc(sizeof(struct armcode_));
            armcode *movcode = initnewnode();
            movcode->op = MOV;
            movcode->result.value = R11;
            movcode->oper1.type = IMME;
            movcode->oper1.value = p->opn1.const_int;

            newnode->pre->next = movcode;
            movcode->pre = newnode->pre;
            movcode->next = newnode;
            newnode->pre = movcode;

            newnode->oper1.type = REG;
            newnode->oper1.value = R11;
            if (p->opn2.kind == LITERAL)
            {
                // printf("ADD R%d ,R0 ,#%d\n", rn0, p->opn2.const_int);
                newnode->oper2.type = IMME;
                newnode->oper2.value = p->opn2.const_int;
            }
            else if (p->opn2.kind == ID)
            {
                if ((rn2 = search_var(funcname, p->opn2.id)) >= 0)
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
            if ((rn1 = search_var(funcname, p->opn1.id)) >= 0)
            {
                if (MULFLAG && rn0 == rn1)
                {
                    MULFLAG = false;
                    // armcode *movcode = (armcode *)malloc(sizeof(struct armcode_));
                    armcode *movcode = initnewnode();
                    movcode->op = MOV;
                    movcode->result.value = R11;
                    movcode->oper1.type = REG;
                    movcode->oper1.value = rn1;

                    newnode->pre->next = movcode;
                    movcode->pre = newnode->pre;
                    movcode->next = newnode;
                    newnode->pre = movcode;

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R11;
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
                    if ((rn2 = search_var(funcname, p->opn2.id)) > 0)
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
                    vartbl = vartable_create();
                    strcpy(funcname, p->result.id);
                    newnode->op = ARMLABEL;
                    newnode->result.type = STRING;
                    strcpy(newnode->result.str_id, p->result.id);
                    newnode->oper1.type = NUL;
                    newnode->oper2.type = NUL;

                    int paranum = search_func(funcname);
                    if (paranum > 4)
                    {
                        // struct codenode* m = p->next;   //第一个参数
                        for (int i = 0; i < 4; i++)
                        {
                            p = p->next;
                        }

                        for (int i = 0; i < paranum - 4; i++)
                        {
                            p = p->next;
                            vartable_insert(vartbl, p->result.id, memindex, i * 4);
                        }
                    }
                    break;
                case TOK_ASSIGN:
                    if ((rn0 = search_var(funcname, p->result.id)) >= 0) // The result is stored in the register
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
                            if ((rn1 = search_var(funcname, p->opn1.id)) >= 0)
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
                    if ((rn0 = search_var(funcname, p->result.id)) >= 0)
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
                    struct codenode *m = p;
                    while (m->op != CALL)
                    {
                        m = m->next;
                    }
                    int paranum = search_func(m->opn1.id);

                    int reg[5] = {0, 1, 2, 3, 14};
                    armcode *snode = stmnode(R13, reg, 5);
                    newnode->pre->next = snode;
                    snode->pre = newnode->pre;
                    snode->next = newnode;
                    newnode->pre = snode;

                    vartable_update_all(vartbl, 20);

                    vartable_insert(vartbl, "R0", memindex, 0);
                    vartable_insert(vartbl, "R1", memindex, 4);
                    vartable_insert(vartbl, "R2", memindex, 8);
                    vartable_insert(vartbl, "R3", memindex, 12);
                    vartable_insert(vartbl, "R14", memindex, 16);

                    if (paranum > 4)
                    {
                        int index = (paranum - 4) * 4;

                        armcode *subnode = initnewnode();
                        subnode->op = SUB;
                        subnode->result.value = R13;
                        subnode->oper1.type = REG;
                        subnode->oper1.value = R13;
                        subnode->oper2.type = IMME;
                        subnode->oper2.value = index;
                        // newnode->pre->next = subnode;
                        // subnode->pre = newnode->pre;
                        // subnode->next = newnode;
                        // newnode->pre = subnode;
                        armlink_insert(newnode, subnode);

                        vartable_update_all(vartbl, index);

                        m = p;
                        for (int i = 0; i < 4; i++)
                            m = m->next; //指向第五个参数

                        for (int i = 0; i < paranum - 4; i++)
                        {
                            if (m->result.kind == ID)
                            {
                                rn2 = search_var(funcname, m->result.id);
                                if (rn2 >= 0)
                                {
                                    armcode *strnode = initnewnode();
                                    strnode->op = STR;
                                    strnode->result.value = rn2;
                                    strnode->oper1.type = MEM;
                                    strnode->oper1.value = R13;
                                    strnode->oper1.index = i * 4;
                                    armlink_insert(newnode, strnode);
                                }
                                else if (rn2 == -1)
                                {
                                    // TODO 变量溢出
                                }
                                else if (rn2 == -2)
                                {
                                    // TODO全局变量
                                }
                                else
                                {
                                    //无用赋值
                                }
                            }
                            else if (m->result.kind == LITERAL)
                            {
                                if (check_imme(m->result.const_int) == 0)
                                {
                                    armcode *movnode = initnewnode();
                                    movnode->op = MOV;
                                    movnode->result.value = R11;
                                    movnode->oper1.type = IMME;
                                    movnode->oper1.value = m->result.const_int;
                                    armlink_insert(newnode, movnode);

                                    armcode *strnode = initnewnode();
                                    strnode->op = STR;
                                    strnode->result.value = R11;
                                    strnode->oper1.type = MEM;
                                    strnode->oper1.value = R13;
                                    strnode->oper1.index = i * 4;
                                    armlink_insert(newnode, strnode);
                                }
                                else
                                {
                                    armcode *movnode = initnewnode();
                                    movnode->op = LDR;
                                    movnode->result.value = R11;
                                    movnode->oper1.type = ILIMME;
                                    movnode->oper1.value = m->result.const_int;
                                    armlink_insert(newnode, movnode);

                                    armcode *strnode = initnewnode();
                                    strnode->op = STR;
                                    strnode->result.value = R11;
                                    strnode->oper1.type = MEM;
                                    strnode->oper1.value = R13;
                                    strnode->oper1.index = i * 4;
                                    armlink_insert(newnode, strnode);
                                }
                            }
                            m = m->next;
                        }
                        m = m->prior; // m指向最后一个参数
                    }

                    for (int i = 0; i < paranum && i < 4; i++) //四个参数以内，寄存器传值
                    {
                        if (p->result.kind == ID)
                        {
                            rn1 = search_var(funcname, p->result.id);
                            if (rn1 >= 0)
                            {
                               if(rn1<i)        //被覆盖，需从栈中取
                               {
                                    char * regname[10]={0};
                                    sprintf(regname,"R%d",rn1);
                                    int index = vartable_select(vartbl,regname);
                                    int stkindex = vartbl->table[index].index;

                                    armcode *strnode = initnewnode();
                                    strnode->op = LDR;
                                    strnode->result.value = i;
                                    strnode->oper1.type = MEM;
                                    strnode->oper1.value = R13;
                                    strnode->oper1.index = stkindex;
                                    armlink_insert(newnode, strnode);
                               }
                               else
                               {
                                    armcode * movnode = initnewnode();
                                    movnode->op = MOV;
                                    movnode->result.value = i;
                                    movnode->oper1.type = REG;
                                    movnode->oper1.value = rn1;
                                    armlink_insert(newnode,movnode);
                               }
                            }
                            else if (rn2 == -1)
                            {
                                // TODO 变量溢出
                            }
                            else if (rn2 == -2)
                            {
                                // TODO全局变量
                            }
                            else
                            {
                                //无用赋值
                            }
                        }
                        else if (p->result.kind == LITERAL)
                        {
                            if (check_imme(p->result.const_int) == 0) //合法立即数
                            {
                                armcode *movnode = initnewnode();
                                movnode->op = MOV;
                                movnode->result.value = i;
                                movnode->oper1.type = IMME;
                                movnode->oper1.value = m->result.const_int;
                                armlink_insert(newnode, movnode);
                            }
                            else
                            {
                                armcode *movnode = initnewnode();
                                movnode->op = LDR;
                                movnode->result.value = i;
                                movnode->oper1.type = ILIMME;
                                movnode->oper1.value = m->result.const_int;
                                armlink_insert(newnode, movnode);
                            }
                        }
                    }
                    p = m; //指向最后一个参数
                    q = newnode->pre;
                    free(newnode);
                    break;

                case CALL:
                    ARGTIM = 0;
                    if (search_func(p->opn1.id) == 0)
                    {
                        int reg[5] = {0, 1, 2, 3, 14};
                        armcode *snode = stmnode(R13, reg, 5);
                        newnode->pre->next = snode;
                        snode->pre = newnode->pre;
                        snode->next = newnode;
                        newnode->pre = snode;
                        // vartable_insert(vartbl,"R0",memindex,0);
                        // vartable_insert(vartbl,"R1",memindex,4);
                        // vartable_insert(vartbl,"R2",memindex,8);
                        // vartable_insert(vartbl,"R3",memindex,12);
                        // vartable_insert(vartbl,"R14",memindex,16);
                    }
                    break;

                case END:

                    free(vartbl);
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

void armlink_insert(armcode *newnode, armcode *inode)
{
    newnode->pre->next = inode;
    inode->pre = newnode->pre;
    inode->next = newnode;
    newnode->pre = inode;
}
