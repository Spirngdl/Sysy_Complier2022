#include "../include/def.h"

char *Reg[16];
char funcname[33];
const int reg[5] = {0, 1, 2, 3, 14};
// int regcountmask[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};

int R11R12 = 3;

bool MULFLAG = false;
// bool FIRSTARG = false;//第一次遇到ARG时，插入压栈操作后置true，遇到call时再置false
vartable *vartbl;

int func_enter_subindex;
int func_call_subindex;
int func_enter_reg[16];
int func_enter_regnum;

int cur_stk_offset; //从高往低压，压局部溢出变量和数组时，手动改变改指针,满递减，先减再压

int func_gvar_num;
char *func_gvar_ary[500];
int func_index = 0;

armcode *initnewnode()
{
    armcode *newnode = (armcode *)malloc(sizeof(struct armcode_));
    memset(newnode, 0, sizeof(struct armcode_));
    newnode->result.type = REG;

    return newnode;
}

void translate(armcode *newnode, struct codenode *p, armop armop, armcode *q)
{
    int rn0, rn1, rn2, rn3;  //通过寄存器分配接口，返回的寄存器号
    int R_res, R_op1, R_op2; //通过自己的寄存器分配函数返回的寄存器号
    int vartbl_index;
    armcode *ldrnode, *strnode, *movnode, *snode, *lnode;
    int reg[1] = {0};
    int regnum;

    rn0 = search_var(funcname, p->result.id);
    if (rn0 >= 0)
    {
        Reg[rn0] = p->result.id;
        newnode->op = armop;
        newnode->result.value = rn0;

        if (p->opn1.kind == LITERAL)
        {
            R_op1 = alloc_myreg();
            // movcode = initnewnode();
            // movcode->op = MOV;
            // movcode->result.value = R_op1;
            // movcode->oper1.type = IMME;
            // movcode->oper1.value = p->opn1.const_int;

            // newnode->pre->next = movcode;
            // movcode->pre = newnode->pre;
            // movcode->next = newnode;
            // newnode->pre = movcode;

            movnode = create_movnode(R_op1, IMME, p->opn1.const_int);
            armlink_insert(newnode, movnode);

            newnode->oper1.type = REG;
            newnode->oper1.value = R_op1;
            if (p->opn2.kind == LITERAL)
            {
                // printf("ADD R%d ,R0 ,#%d\n", rn0, p->opn2.const_int);
                newnode->oper2.type = IMME;
                newnode->oper2.value = p->opn2.const_int;
            }
            else if (p->opn2.kind == ID)
            {
                rn2 = search_var(funcname, p->opn2.id);
                if (rn2 >= 0)
                {
                    // printf("ADD R%d ,R%d ,#%d", rn0, rn1, p->opn1.const_int);
                    Reg[rn2] = p->opn2.id;
                    newnode->oper2.type = REG;
                    newnode->oper2.value = rn2;
                }
                else if (rn2 == -1)
                {
                    R_op2 = alloc_myreg();
                    vartbl_index = vartable_select(vartbl, p->opn2.id);
                    ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper2.type = REG;
                    newnode->oper2.value = R_op2;
                }
                else if (rn2 == -2)
                {
                    R_op2 = alloc_myreg();
                    ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                    armlink_insert(newnode, ldrnode);

                    ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper2.type = REG;
                    newnode->oper2.value = R_op2;
                }
                else
                {
                    printf("%d  %s REG ALLOC ERROR!\n", p->UID, p->opn2.id);
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
            rn1 = search_var(funcname, p->opn1.id);
            if (rn1 >= 0)
            {
                if (MULFLAG && rn0 == rn1)
                {

                    R_op1 = alloc_myreg();

                    movnode = create_movnode(R_op1, REG, rn1);

                    armlink_insert(newnode, movnode);

                    // newnode->pre->next = movcode;
                    // movcode->pre = newnode->pre;
                    // movcode->next = newnode;
                    // newnode->pre = movcode;

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;
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
                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn2.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn1);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (p->opn2.kind == ID)
                {
                    rn2 = search_var(funcname, p->opn2.id);
                    if (rn2 >= 0)
                    {
                        // printf("ADD R%d ,R%d ,R%d", rn0, rn1, rn2);
                        Reg[rn2] = p->opn2.id;
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else if (rn2 == -1)
                    {
                        R_op2 = alloc_myreg();
                        vartbl_index = vartable_select(vartbl, p->opn2.id);
                        if (R_op2 != -1)
                        {
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn1);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            ldrnode = create_ldrnode(reg[0], NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else if (rn2 == -2)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn1);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            ldrnode = create_ldrnode(reg[0], p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(reg[0], NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else
                    {
                        printf("%d  %s REG ALLOC ERROR!\n", p->UID, p->opn2.id);
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
            else if (rn1 == -1)
            {
                R_op1 = alloc_myreg();

                vartbl_index = vartable_select(vartbl, p->opn1.id);
                ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartbl_index].index);
                armlink_insert(newnode, ldrnode);

                newnode->oper1.type = REG;
                newnode->oper1.value = R_op1;

                if (p->opn2.kind == ID)
                {
                    rn2 = search_var(funcname, p->opn2.id);
                    if (rn2 >= 0)
                    {
                        // printf("ADD R%d ,R%d ,R%d", rn0, rn1, rn2);
                        Reg[rn2] = p->opn2.id;
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else if (rn2 == -1)
                    {
                        R_op2 = alloc_myreg();
                        vartbl_index = vartable_select(vartbl, p->opn2.id);
                        if (R_op2 != -1)
                        {
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn1);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            ldrnode = create_ldrnode(reg[0], NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else if (rn2 == -2)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn1);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            ldrnode = create_ldrnode(reg[0], p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(reg[0], NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else
                    {
                        printf("TOK_:Failed to allocate register for opn2\n");
                    }
                }
                else if (p->opn2.kind == LITERAL)
                {
                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn2.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn1);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (p->opn2.kind == FLOAT_LITERAL)
                {
                }
            }
            else if (rn1 == -2)
            {
                R_op1 = alloc_myreg();
                ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                armlink_insert(newnode, ldrnode);
                ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                armlink_insert(newnode, ldrnode);

                newnode->oper1.type = REG;
                newnode->oper1.value = R_op1;

                if (p->opn2.kind == ID)
                {
                    rn2 = search_var(funcname, p->opn2.id);
                    if (rn2 >= 0)
                    {
                        // printf("ADD R%d ,R%d ,R%d", rn0, rn1, rn2);
                        Reg[rn2] = p->opn2.id;
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else if (rn2 == -1)
                    {
                        R_op2 = alloc_myreg();
                        vartbl_index = vartable_select(vartbl, p->opn2.id);
                        if (R_op2 != -1)
                        {
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn1);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            ldrnode = create_ldrnode(reg[0], NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else if (rn2 == -2)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn1);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            ldrnode = create_ldrnode(reg[0], p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(reg[0], NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else
                    {
                        printf("TOK_:Failed to allocate register for opn2\n");
                    }
                }
                else if (p->opn2.kind == LITERAL)
                {
                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn2.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn1);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (p->opn2.kind == FLOAT_LITERAL)
                {
                }
            }
            else
            {
                printf("TOK_:Failed to allocate register for opn1\n");
            }
        }
        else if (p->opn1.kind == FLOAT_LITERAL)
        {
            // TODO
        }
        else
        {
            printf("p->opn1.kind error!\n");
            newnode->oper2.type = NUL;
        }
    }
    else if (rn0 == -1) //结果溢出，需要将结果压栈
    {
        R_res = alloc_myreg();
        newnode->op = armop;
        newnode->result.value = R_res;

        if (p->opn1.kind == LITERAL)
        {
            if (p->opn2.kind == ID)
            {
                rn2 = search_var(funcname, p->opn2.id);
                if (rn2 >= 0)
                {
                    newnode->oper1.type = REG;
                    newnode->oper1.value = rn2;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn1.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (rn2 == -1)
                {
                    R_op1 = alloc_myreg();
                    vartbl_index = vartable_select(vartbl, p->opn2.id);
                    ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartbl_index].index);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn1.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (rn2 == -2)
                {
                    R_op1 = alloc_myreg();
                    ldrnode = create_ldrnode(R_op1, p->opn2.id, 0, 0);
                    armlink_insert(newnode, ldrnode);
                    ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn1.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else
                {
                    printf("无用赋值\n");
                }
            }
            else if (p->opn2.kind == LITERAL)
            {
                R_op1 = alloc_myreg();

                movnode = create_movnode(R_op1, IMME, p->opn1.const_int);
                armlink_insert(newnode, movnode);
                newnode->oper1.type = REG;
                newnode->oper1.value = R_op1;

                R_op2 = alloc_myreg();
                if (R_op2 != -1)
                {
                    movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                    armlink_insert(newnode, movnode);
                    newnode->oper2.type = REG;
                    newnode->oper2.value = R_op2;
                }
                else
                {
                    reg[0] = get_other_reg(rn0, rn2);

                    snode = mul_reg_node(STMFD, R13, reg, 1);
                    armlink_insert(newnode, snode);

                    movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                    armlink_insert(newnode, movnode);
                    newnode->oper2.type = REG;
                    newnode->oper2.value = reg[0];

                    lnode = mul_reg_node(LDMFD, R13, reg, 1);
                    q->next = lnode;
                    lnode->pre = q;
                    q = lnode;
                }
            }
            else if (p->opn2.kind == FLOAT_LITERAL)
            {
            }
        }
        else if (p->opn1.kind == ID)
        {
            if (p->opn2.kind == ID)
            {
                rn1 = search_var(funcname, p->opn1.id);
                if (rn1 >= 0)
                {
                    newnode->oper1.type = REG;
                    newnode->oper1.value = rn1;

                    rn2 = search_var(funcname, p->opn2.id);
                    if (rn2 >= 0)
                    {
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else if (rn2 == -1)
                    {
                        R_op2 = alloc_myreg();

                        vartbl_index = vartable_select(vartbl, p->opn2.id);

                        ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                        armlink_insert(newnode, ldrnode);

                        newnode->oper2.type = REG;
                        newnode->oper2.value = R_op2;
                    }
                    else if (rn2 == -2)
                    {
                        R_op2 = alloc_myreg();
                        ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                        armlink_insert(newnode, ldrnode);
                        ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                        armlink_insert(newnode, ldrnode);

                        newnode->oper2.type = REG;
                        newnode->oper2.value = R_op2;
                    }
                    else
                    {
                        printf("ERROR!\n");
                    }
                }
                else if (rn1 == -1)
                {
                    R_op1 = alloc_myreg();
                    vartbl_index = vartable_select(vartbl, p->opn1.id);

                    ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartbl_index].index);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    rn2 = search_var(funcname, p->opn2.id);
                    if (rn2 >= 0)
                    {
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else if (rn2 == -1)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            vartbl_index = vartable_select(vartbl, p->opn2.id);

                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            vartbl_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(reg[0], NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else if (rn2 == -2)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            vartbl_index = vartable_select(vartbl, p->opn2.id);

                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            vartbl_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else
                    {
                        printf("ERROR!\n");
                    }
                }
                else if (rn1 == -2)
                {
                    R_op1 = alloc_myreg();

                    vartbl_index = vartable_select(vartbl, p->opn1.id);
                    ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                    armlink_insert(newnode, ldrnode);
                    ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                    armlink_insert(newnode, ldrnode);

                    rn2 = search_var(funcname, p->opn2.id);
                    if (rn2 >= 0)
                    {
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else if (rn2 == -1)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            vartbl_index = vartable_select(vartbl, p->opn2.id);

                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            vartbl_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(reg[0], NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else if (rn2 == -2)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            vartbl_index = vartable_select(vartbl, p->opn2.id);

                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            vartbl_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else
                    {
                        printf("ERROR!\n");
                    }
                }
                else
                {
                    printf("Error!\n");
                }
            }
            else if (p->opn2.kind == LITERAL)
            {
                rn1 = search_var(funcname, p->opn1.id);
                if (rn1 >= 0)
                {
                    newnode->oper1.type = REG;
                    newnode->oper1.value = rn1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn2.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (rn1 == -1)
                {
                    R_op1 = alloc_myreg();
                    vartbl_index = vartable_select(vartbl, p->opn1.id);
                    ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartbl_index].index);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn2.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (rn1 == -2)
                {
                    R_op1 = alloc_myreg();
                    ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                    armlink_insert(newnode, ldrnode);
                    ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn2.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else
                {
                    printf("ERROR\n");
                }
            }
            else if (p->opn2.kind == FLOAT_LITERAL)
            {
            }
        }
        else if (p->opn1.kind == FLOAT_LITERAL)
        {
        }

        cur_stk_offset -= 4;
        strnode = create_strnode(R_res, R13, cur_stk_offset);
        vartable_insert(vartbl, p->result.id, memindex, cur_stk_offset);
        q->next = strnode;
        strnode->pre = q;
        q = strnode;
    }
    else if (rn0 == -2)
    {
        R_res = alloc_myreg();
        newnode->op = armop;
        newnode->result.value = R_res;

        if (p->opn1.kind == LITERAL)
        {
            if (p->opn2.kind == ID)
            {
                rn2 = search_var(funcname, p->opn2.id);
                if (rn2 >= 0)
                {
                    newnode->oper1.type = REG;
                    newnode->oper1.value = rn2;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn1.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (rn2 == -1)
                {
                    R_op1 = alloc_myreg();
                    vartbl_index = vartable_select(vartbl, p->opn2.id);
                    ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartbl_index].index);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn1.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (rn2 == -2)
                {
                    R_op1 = alloc_myreg();
                    ldrnode = create_ldrnode(R_op1, p->opn2.id, 0, 0);
                    armlink_insert(newnode, ldrnode);
                    ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn1.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn1.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else
                {
                    printf("无用赋值\n");
                }
            }
            else if (p->opn2.kind == LITERAL)
            {
                R_op1 = alloc_myreg();

                movnode = create_movnode(R_op1, IMME, p->opn1.const_int);
                armlink_insert(newnode, movnode);
                newnode->oper1.type = REG;
                newnode->oper1.value = R_op1;

                R_op2 = alloc_myreg();
                if (R_op2 != -1)
                {
                    movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                    armlink_insert(newnode, movnode);
                    newnode->oper2.type = REG;
                    newnode->oper2.value = R_op2;
                }
                else
                {
                    reg[0] = get_other_reg(rn0, rn2);

                    snode = mul_reg_node(STMFD, R13, reg, 1);
                    armlink_insert(newnode, snode);

                    movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                    armlink_insert(newnode, movnode);
                    newnode->oper2.type = REG;
                    newnode->oper2.value = reg[0];

                    lnode = mul_reg_node(LDMFD, R13, reg, 1);
                    q->next = lnode;
                    lnode->pre = q;
                    q = lnode;
                }
            }
            else if (p->opn2.kind == FLOAT_LITERAL)
            {
            }
        }
        else if (p->opn1.kind == ID)
        {
            if (p->opn2.kind == ID)
            {
                rn1 = search_var(funcname, p->opn1.id);
                if (rn1 >= 0)
                {
                    newnode->oper1.type = REG;
                    newnode->oper1.value = rn1;

                    rn2 = search_var(funcname, p->opn2.id);
                    if (rn2 >= 0)
                    {
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else if (rn2 == -1)
                    {
                        R_op2 = alloc_myreg();

                        vartbl_index = vartable_select(vartbl, p->opn2.id);

                        ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                        armlink_insert(newnode, ldrnode);

                        newnode->oper2.type = REG;
                        newnode->oper2.value = R_op2;
                    }
                    else if (rn2 == -2)
                    {
                        R_op2 = alloc_myreg();
                        ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                        armlink_insert(newnode, ldrnode);
                        ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                        armlink_insert(newnode, ldrnode);

                        newnode->oper2.type = REG;
                        newnode->oper2.value = R_op2;
                    }
                    else
                    {
                        printf("ERROR!\n");
                    }
                }
                else if (rn1 == -1)
                {
                    R_op1 = alloc_myreg();
                    vartbl_index = vartable_select(vartbl, p->opn1.id);

                    ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartbl_index].index);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    rn2 = search_var(funcname, p->opn2.id);
                    if (rn2 >= 0)
                    {
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else if (rn2 == -1)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            vartbl_index = vartable_select(vartbl, p->opn2.id);

                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            vartbl_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(reg[0], NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else if (rn2 == -2)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            vartbl_index = vartable_select(vartbl, p->opn2.id);

                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            vartbl_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else
                    {
                        printf("ERROR!\n");
                    }
                }
                else if (rn1 == -2)
                {
                    R_op1 = alloc_myreg();

                    vartbl_index = vartable_select(vartbl, p->opn1.id);
                    ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                    armlink_insert(newnode, ldrnode);
                    ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                    armlink_insert(newnode, ldrnode);

                    rn2 = search_var(funcname, p->opn2.id);
                    if (rn2 >= 0)
                    {
                        newnode->oper2.type = REG;
                        newnode->oper2.value = rn2;
                    }
                    else if (rn2 == -1)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            vartbl_index = vartable_select(vartbl, p->opn2.id);

                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            vartbl_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(reg[0], NULL, R13, vartbl->table[vartbl_index].index);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else if (rn2 == -2)
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            vartbl_index = vartable_select(vartbl, p->opn2.id);

                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            vartbl_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                    else
                    {
                        printf("%d  %s REG ALLOC ERROR!\n", p->UID, p->opn2.id);
                    }
                }
                else
                {
                    printf("%d  %s REG ALLOC ERROR!\n", p->UID, p->opn1.id);
                }
            }
            else if (p->opn2.kind == LITERAL)
            {
                rn1 = search_var(funcname, p->opn1.id);
                if (rn1 >= 0)
                {
                    newnode->oper1.type = REG;
                    newnode->oper1.value = rn1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn2.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (rn1 == -1)
                {
                    R_op1 = alloc_myreg();
                    vartbl_index = vartable_select(vartbl, p->opn1.id);
                    ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartbl_index].index);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn2.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else if (rn1 == -2)
                {
                    R_op1 = alloc_myreg();
                    ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                    armlink_insert(newnode, ldrnode);
                    ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                    armlink_insert(newnode, ldrnode);

                    newnode->oper1.type = REG;
                    newnode->oper1.value = R_op1;

                    if (!MULFLAG)
                    {
                        newnode->oper2.type = IMME;
                        newnode->oper2.value = p->opn2.const_int;
                    }
                    else
                    {
                        R_op2 = alloc_myreg();
                        if (R_op2 != -1)
                        {
                            movnode = create_movnode(R_op2, IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;
                        }
                        else
                        {
                            reg[0] = get_other_reg(rn0, rn2);

                            snode = mul_reg_node(STMFD, R13, reg, 1);
                            armlink_insert(newnode, snode);

                            movnode = create_movnode(reg[0], IMME, p->opn2.const_int);
                            armlink_insert(newnode, movnode);
                            newnode->oper2.type = REG;
                            newnode->oper2.value = reg[0];

                            lnode = mul_reg_node(LDMFD, R13, reg, 1);
                            q->next = lnode;
                            lnode->pre = q;
                            q = lnode;
                        }
                    }
                }
                else
                {
                    printf("ERROR\n");
                }
            }
            else if (p->opn2.kind == FLOAT_LITERAL)
            {
            }
        }
        else if (p->opn1.kind == FLOAT_LITERAL)
        {
        }
        //后插
        ldrnode = create_ldrnode(R11, p->result.id, 0, 0);
        q->next = ldrnode;
        ldrnode->pre = q;
        q = ldrnode;

        strnode = create_strnode(R_res,R11,0);
        q->next = strnode;
        strnode->pre = q;
        q = strnode;
    }
    else
    {
        printf("无用赋值\n");
    }

    MULFLAG = false;

    init_myreg();
}

armcode *translatearm(Blocks *blocks)
{
    struct codenode *p;
    struct codenode *m;
    armcode *q = NULL;
    armcode *newnode = NULL;
    armcode *first = (armcode *)malloc(sizeof(struct armcode_)); // header node
    memset(first, 0, sizeof(struct armcode_));

    q = first;
    int R_res, R_op1, R_op2;
    int rn0, rn1, rn2, rn3;
    int index, stkindex;
    int vartable_index;
    char regname[10]={0};
    int reg[1];
    // char labelname[33];
    int paranum, spil_var_num;
    armcode *snode, *subnode, *addnode, *strnode, *ldrnode, *movnode, *ldmnode, *bnode;
    Blocks *cur_blocks = blocks;
    struct codenode *result = NULL;
    while (cur_blocks != NULL) //遍历所有基本块
    {
        for (int i = 0; i < cur_blocks->count; i++)
        {
            struct codenode *tcode = cur_blocks->block[i]->tac_list;
            p = tcode;
            while (p != NULL)
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
                    newnode->op = FUNCLABEL;
                    newnode->result.type = STRING;
                    strcpy(newnode->result.str_id, p->result.id);
                    newnode->oper1.type = NUL;
                    newnode->oper2.type = NUL;

                    paranum = search_func(funcname);
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

                    // int reg[16] = {0};
                    func_enter_regnum = one_fun_reg(funcname, func_enter_reg);
                    if (func_enter_regnum != 0)
                    {
                        snode = mul_reg_node(STMFD, R13, func_enter_reg, func_enter_regnum);
                        newnode->next = snode;
                        snode->pre = newnode;
                        q = snode;
                        vartable_update_all(vartbl, func_enter_regnum * 4);
                    }

                    spil_var_num = search_fun_spilling(funcname);

                    func_enter_subindex = (spil_var_num + 4) * 4; //+4为宏区

                    cur_stk_offset = func_call_subindex - 4; //从高往低压，压局部溢出变量和数组时，手动改变改指针

                    subnode = initnewnode();
                    subnode->op = SUB;
                    subnode->result.value = R13;
                    subnode->oper1.type = REG;
                    subnode->oper1.value = R13;
                    subnode->oper2.type = IMME;
                    subnode->oper2.value = func_enter_subindex;
                    q->next = subnode;
                    subnode->pre = q;
                    q = subnode;

                    vartable_update_all(vartbl, func_enter_subindex);

                    init_myreg();
                    break;

                case LABEL:
                    // memset(labelname,0,sizeof(labelname));
                    newnode->op = ARMLABEL;
                    newnode->result.type = STRING;
                    strcpy(newnode->result.str_id, p->result.id);
                    break;

                case TOK_ASSIGN:

                    rn0 = search_var(funcname, p->result.id);
                    if (rn0 >= 0) // The result is stored in the register
                    {
                        newnode->op = MOV;
                        Reg[rn0] = p->result.id;

                        // newnode->oper1.type = REG;
                        newnode->result.value = rn0;

                        if (p->opn1.kind == LITERAL)
                        {
                            // printf("MOV R%d ,#%d\n", rn0, p->opn1.const_int);
                            newnode->oper1.type = IMME;
                            newnode->oper1.value = p->opn1.const_int;
                        }
                        else if (p->opn1.kind == ID)
                        {
                            rn1 = search_var(funcname, p->opn1.id);
                            if (rn1 >= 0)
                            {
                                Reg[rn1] = p->opn1.id;
                                // printf("MOV R%d ,R%d\n", rn0, rn1);
                                newnode->oper1.type = REG;
                                newnode->oper1.value = rn1;
                            }
                            else if (rn1 == -1)
                            {
                                R_op1 = alloc_myreg();

                                vartable_index = vartable_select(vartbl, p->opn1.id);
                                ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);

                                newnode->oper1.type = REG;
                                newnode->oper1.value = R_op1;
                            }
                            else if (rn1 == -2)
                            {
                                R_op1 = alloc_myreg();
                                ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                newnode->oper1.type = REG;
                                newnode->oper1.value = R_op1;
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
                    else if (rn0 == -1)
                    {
                        R_res = alloc_myreg();
                        if (p->opn1.kind == LITERAL)
                        {
                            newnode->op = MOV;
                            newnode->result.value = R_res;
                            newnode->oper1.type = IMME;
                            newnode->oper1.value = p->opn1.const_int;

                            cur_stk_offset -= 4;
                            strnode = create_strnode(R_res, R13, cur_stk_offset);
                            vartable_insert(vartbl, p->result.id, memindex, cur_stk_offset);
                            q->next = strnode;
                            strnode->pre = q;
                            q = strnode;
                        }
                    }
                    else if (rn0 == -2)
                    {
                        R_res = alloc_myreg();

                        ldrnode = create_ldrnode(R_res, p->result.id, 0, 0);
                        armlink_insert(newnode, ldrnode);

                        if (p->opn1.kind == LITERAL)
                        {
                            R_op1 = alloc_myreg();

                            movnode = create_movnode(R_op1, IMME, p->opn1.const_int);
                            // movnode = initnewnode();
                            // movnode->op = MOV;
                            // movnode->result.value = R_op1;
                            // movnode->oper1.type = IMME;
                            // movnode->oper1.value = p->opn1.const_int;
                            armlink_insert(newnode, movnode);

                            init_strnode(newnode,R_op1,R_res,0,immeindex);

                            // newnode->op = STR;
                            // newnode->result.type = REG;
                            // newnode->result.value = R_op1;
                            // newnode->oper1.type = MEM;
                            // newnode->oper1.value = R_res;
                            // newnode->oper1.index = 0;
                        }
                        else if (p->opn1.kind == ID)
                        {
                            rn1 = search_var(funcname, p->opn1.id);
                            if (rn1 >= 0)
                            {
                                init_strnode(newnode,rn1,R_res,0,immeindex);
                            }
                            else if (rn1 == -1)
                            {
                                R_op1 = alloc_myreg();
                                vartable_index = vartable_select(vartbl, p->opn1.id);
                                ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);

                                init_strnode(newnode,R_op1,R_res,0,immeindex);
                            }
                            else if (rn1 == -2)
                            {
                                R_op1 = alloc_myreg();
                                ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                                armlink_insert(newnode, ldrnode);

                                ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                                armlink_insert(newnode, ldrnode);

                                init_strnode(newnode,R_op1,R_res,0,immeindex);

                                // newnode->op = STR;
                                // newnode->result.type = REG;
                                // newnode->result.value = R_op1;
                                // newnode->oper1.type = MEM;
                                // newnode->oper1.value = R_res;
                                // newnode->oper1.index = 0;
                            }
                        }
                        else if (p->opn1.kind == FLOAT_LITERAL)
                        {
                        }
                    }
                    init_myreg();
                    break;

                case TOK_LDR:
                    rn0 = search_var(funcname, p->result.id);
                    if (rn0 >= 0)
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
                            printf("TOK_LDR:p->opn1.kind must be LITERAL!\n");
                        }
                    }
                    else if (rn0 == -1)
                    {
                        R_res = alloc_myreg();
                        newnode->op = LDR;
                        newnode->result.value = R_res;

                        if (p->opn1.kind == LITERAL)
                        {
                            newnode->oper1.type = ILIMME;
                            newnode->oper1.value = p->opn1.const_int;
                        }
                        else
                        {
                            printf("TOK_LDR:p->opn1.kind must be LITERAL!\n");
                        }

                        cur_stk_offset -= 4;
                        strnode = create_strnode(R_res, R13, cur_stk_offset);
                        newnode->next = strnode;
                        strnode->pre = newnode;
                        q = strnode;

                        vartable_insert(vartbl, p->result.id, memindex, cur_stk_offset);
                    }
                    else if (rn0 == -2)
                    {
                        R_res = alloc_myreg();
                        ldrnode = create_ldrnode(R_res, p->result.id, 0, 0);
                        armlink_insert(newnode, ldrnode);

                        if (p->opn1.kind == LITERAL)
                        {
                            R_op1 = alloc_myreg();
                            ldrnode = initnewnode();
                            ldrnode->op = LDR;
                            ldrnode->result.value = R_op1;
                            ldrnode->oper1.type = ILIMME;
                            ldrnode->oper1.value = p->opn1.const_int;

                            armlink_insert(newnode, ldrnode);
                        }
                        else
                        {
                            printf("TOK_LDR:p->opn1.kind must be LITERAL!\n");
                        }

                        init_strnode(newnode,R_op1,R_res,0,immeindex);

                    }
                    else
                    {
                        printf("无用赋值\n");
                    }

                    init_myreg();
                    break;

                case TOK_ADD:
                    translate(newnode, p, ADD,q);
                    while(q->next)
                    {
                        q = q->next;
                    }
                    break;

                case TOK_MUL:
                    MULFLAG = true;
                    translate(newnode, p, MUL,q);
                    while(q->next)
                    {
                        q = q->next;
                    }
                    break;

                case TOK_SUB:
                    translate(newnode, p, SUB,q);
                    while(q->next)
                    {
                        q = q->next;
                    }
                    break;

                case ARG:
                    m = p;
                    while (m->op != CALL)
                    {
                        m = m->next;
                    }
                    paranum = search_func(m->opn1.id);

                    m = m->prior; //指向最后一个参数

                    // int reg[5] = {0, 1, 2, 3, 14};
                    snode = mul_reg_node(STMFD, R13, reg, 5);
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
                        func_call_subindex = (paranum - 4) * 4;

                        subnode = initnewnode();
                        subnode->op = SUB;
                        subnode->result.value = R13;
                        subnode->oper1.type = REG;
                        subnode->oper1.value = R13;
                        subnode->oper2.type = IMME;
                        subnode->oper2.value = func_call_subindex;
                        // newnode->pre->next = subnode;
                        // subnode->pre = newnode->pre;
                        // subnode->next = newnode;
                        // newnode->pre = subnode;
                        armlink_insert(newnode, subnode);

                        vartable_update_all(vartbl, func_call_subindex);

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
                                    strnode = create_strnode(rn2, R13, i * 4);
                                    // strnode = initnewnode();
                                    // strnode->op = STR;
                                    // strnode->result.value = rn2;
                                    // strnode->oper1.type = MEM;
                                    // strnode->oper1.value = R13;
                                    // strnode->oper1.index = i * 4;
                                    armlink_insert(newnode, strnode);
                                }
                                else if (rn2 == -1)
                                {
                                    // TODO 变量溢出
                                    R_op2 = alloc_myreg();

                                    vartable_index = vartable_select(vartbl, m->result.id);
                                    ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                    armlink_insert(newnode, ldrnode);
                                    strnode = create_strnode(R_op2, R13, i * 4);
                                    armlink_insert(newnode, strnode);
                                }
                                else if (rn2 == -2)
                                {
                                    R_op2 = alloc_myreg();

                                    ldrnode = create_ldrnode(R_op2, m->result.id, 0, 0);
                                    armlink_insert(newnode, ldrnode);
                                    ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                    armlink_insert(newnode, ldrnode);
                                    strnode = create_strnode(R_op2, R13, i * 4);
                                    armlink_insert(newnode, strnode);
                                }
                                else
                                {
                                    printf("ERROR!\n");
                                }
                            }
                            else if (m->result.kind == LITERAL)
                            {
                                if (check_imme(m->result.const_int) == 0)
                                {
                                    movnode = initnewnode();
                                    movnode->op = MOV;
                                    movnode->result.value = R11;
                                    movnode->oper1.type = IMME;
                                    movnode->oper1.value = m->result.const_int;
                                    armlink_insert(newnode, movnode);

                                    strnode = initnewnode();
                                    strnode->op = STR;
                                    strnode->result.value = R11;
                                    strnode->oper1.type = MEM;
                                    strnode->oper1.value = R13;
                                    strnode->oper1.index = i * 4;
                                    armlink_insert(newnode, strnode);
                                }
                                else
                                {
                                    movnode = initnewnode();
                                    movnode->op = LDR;
                                    movnode->result.value = R11;
                                    movnode->oper1.type = ILIMME;
                                    movnode->oper1.value = m->result.const_int;
                                    armlink_insert(newnode, movnode);

                                    strnode = initnewnode();
                                    strnode->op = STR;
                                    strnode->result.value = R11;
                                    strnode->oper1.type = MEM;
                                    strnode->oper1.value = R13;
                                    strnode->oper1.index = i * 4;
                                    armlink_insert(newnode, strnode);
                                }
                            }
                            m = m->next;
                            init_myreg();
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
                                if (rn1 < i) //被覆盖，需从栈中取
                                {

                                    sprintf(regname, "R%d", rn1);
                                    index = vartable_select(vartbl, regname);
                                    stkindex = vartbl->table[index].index;

                                    strnode = create_ldrnode(i, NULL, R13, stkindex);

                                    // strnode = initnewnode();
                                    // strnode->op = LDR;
                                    // strnode->result.value = i;
                                    // strnode->oper1.type = MEM;
                                    // strnode->oper1.value = R13;
                                    // strnode->oper1.index = stkindex;
                                    armlink_insert(newnode, strnode);
                                }
                                else
                                {
                                    movnode = initnewnode();
                                    movnode->op = MOV;
                                    movnode->result.value = i;
                                    movnode->oper1.type = REG;
                                    movnode->oper1.value = rn1;
                                    armlink_insert(newnode, movnode);
                                }
                            }
                            else if (rn1 == -1)
                            {
                                // TODO 变量溢出
                                R_op2 = alloc_myreg();

                                vartable_index = vartable_select(vartbl, p->result.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);

                                movnode = create_movnode(i, REG, R_op2);
                                armlink_insert(newnode, movnode);
                            }
                            else if (rn1 == -2)
                            {
                                // TODO全局变量
                                R_op2 = alloc_myreg();

                                ldrnode = create_ldrnode(R_op2, p->result.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);

                                movnode = create_movnode(i, REG, R_op2);
                                armlink_insert(newnode, movnode);
                            }
                            else
                            {
                                printf("%d  %s REG ALLOC ERROR!\n", p->UID, p->result.id);
                            }
                        }
                        else if (p->result.kind == LITERAL)
                        {
                            if (check_imme(p->result.const_int) == 0) //合法立即数
                            {
                                // movnode = initnewnode();
                                // movnode->op = MOV;
                                // movnode->result.value = i;
                                // movnode->oper1.type = IMME;
                                // movnode->oper1.value = p->result.const_int;

                                movnode = create_movnode(i, IMME, p->result.const_int);
                                armlink_insert(newnode, movnode);
                            }
                            else
                            {
                                movnode = initnewnode();
                                movnode->op = LDR;
                                movnode->result.value = i;
                                movnode->oper1.type = ILIMME;
                                movnode->oper1.value = p->result.const_int;
                                armlink_insert(newnode, movnode);
                            }
                        }
                        p = p->next;
                        init_myreg();
                    }
                    p = m; //指向最后一个参数
                    q = newnode->pre;
                    free(newnode);
                    init_myreg();
                    break;

                case CALL:

                    if (search_func(p->opn1.id) == 0)
                    {
                        // int reg[5] = {0, 1, 2, 3, 14};
                        snode = mul_reg_node(STMFD, R13, reg, 5);
                        // newnode->pre->next = snode;
                        // snode->pre = newnode->pre;
                        // snode->next = newnode;
                        // newnode->pre = snode;
                        armlink_insert(newnode, snode);
                        // vartable_insert(vartbl,"R0",memindex,0);
                        // vartable_insert(vartbl,"R1",memindex,4);
                        // vartable_insert(vartbl,"R2",memindex,8);
                        // vartable_insert(vartbl,"R3",memindex,12);
                        // vartable_insert(vartbl,"R14",memindex,16);
                    }

                    newnode->op = BL;
                    newnode->result.type = STRING;
                    strcpy(newnode->result.str_id, p->opn1.id);

                    if (p->result.kind == ID)
                    {
                        rn0 = search_var(funcname, p->result.id);
                        if (rn0 >= 0)
                        {
                            // movnode = initnewnode();
                            // movnode->op = MOV;
                            // movnode->result.value = rn0;
                            // movnode->oper1.type = REG;
                            // movnode->oper1.value = R0;

                            movnode = create_movnode(rn0, REG, R0);

                            newnode->next = movnode;
                            movnode->pre = newnode;
                            q = movnode;
                        }
                        else if (rn0 == -1)
                        {
                            vartable_index = vartable_select(vartbl, p->result.id);
                            strnode = create_strnode(R0, R13, vartbl->table[vartable_index].index);
                            q->next = strnode;
                            strnode->pre = q;
                            q = strnode;
                        }
                        else if (rn0 == -2)
                        {
                            R_op2 = alloc_myreg();

                            ldrnode = create_ldrnode(R_op2, p->result.id, 0, 0);
                            q->next = ldrnode;
                            ldrnode->pre = q;
                            q = ldrnode;

                            strnode = create_strnode(R0, R_op2, 0);
                            q->next = strnode;
                            strnode->pre = q;
                            q = strnode;
                        }
                        
                    }

                    if (func_call_subindex != 0)
                    {
                        addnode = initnewnode();
                        addnode->op = ADD;
                        addnode->result.value = R13;
                        addnode->oper1.type = REG;
                        addnode->oper1.value = R13;
                        addnode->oper2.type = IMME;
                        addnode->oper2.value = func_call_subindex;
                        q->next = addnode;
                        addnode->pre = q;
                        q = addnode;
                        vartable_update_all(vartbl, -func_call_subindex);
                    }

                    // int reg[5] = {0, 1, 2, 3, 14};
                    ldmnode = mul_reg_node(LDMFD, R13, reg, 5);
                    q->next = ldmnode;
                    ldmnode->pre = q;
                    q = ldmnode;

                    vartable_update_all(vartbl, -20);
                    init_myreg();
                    break;

                case TOK_RETURN:
                    if (p->result.kind == ID)
                    {
                        rn0 = search_var(funcname, p->result.id);
                        if (rn0 >= 0)
                        {
                            newnode->op = MOV;
                            newnode->result.value = R0;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;
                        }
                        else if (rn0 == -1)
                        {
                            R_op2 = alloc_myreg();

                            vartable_index = vartable_select(vartbl, p->result.id);
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            newnode->op = MOV;
                            newnode->result.value = R0;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;
                        }
                        else if (rn0 == -2)
                        {
                            R_op2 = alloc_myreg();

                            ldrnode = create_ldrnode(R_op2, p->result.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            newnode->op = MOV;
                            newnode->result.value = R0;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;
                        }
                        else
                        {
                            printf("ERROR!\n");
                        }
                    }
                    else if (p->result.kind == LITERAL)
                    {
                        if (check_imme(p->result.const_int) == 0)
                        {
                            newnode->op = MOV;
                            newnode->result.value = R0;
                            newnode->oper1.type = IMME;
                            newnode->oper1.value = p->result.const_int;
                        }
                        else
                        {
                            newnode->op = LDR;
                            newnode->result.value = R0;
                            newnode->oper1.type = ILIMME;
                            newnode->oper1.value = p->result.const_int;
                        }
                    }
                    else if (p->result.kind == FLOAT_LITERAL)
                    {
                        // TODO
                    }
                    else
                    {
                        q = newnode->pre; //无返回值
                    }

                    bnode = initnewnode();
                    bnode->op = BX;
                    bnode->result.value = LR;
                    q->next = bnode;
                    bnode->pre = q;
                    q = bnode;

                    addnode = initnewnode();
                    addnode->op = ADD;
                    addnode->result.value = R13;
                    addnode->oper1.type = REG;
                    addnode->oper1.value = R13;
                    addnode->oper2.type = IMME;
                    addnode->oper2.value = func_enter_subindex;
                    armlink_insert(q, addnode);
                    vartable_update_all(vartbl, -func_enter_subindex);

                    if (func_enter_regnum != 0)
                    {
                        ldmnode = mul_reg_node(LDMFD, R13, func_enter_reg, func_enter_regnum);
                        armlink_insert(q, ldmnode);
                        vartable_update_all(vartbl, -(func_enter_regnum * 4));
                    }

                    init_myreg();
                    break;

                case END:
                    // newnode->op = BX;
                    // newnode->result.value = LR;

                    // addnode = initnewnode();
                    // addnode->op = ADD;
                    // addnode->result.value = R13;
                    // addnode->oper1.type = REG;
                    // addnode->oper1.value = R13;
                    // addnode->oper2.type = IMME;
                    // addnode->oper2.value = func_enter_subindex;
                    // armlink_insert(newnode,addnode);
                    // vartable_update_all(vartbl,-func_enter_subindex);

                    // if(func_enter_regnum != 0)
                    // {
                    //     ldmnode = mul_reg_node(LDMFD,R13,func_enter_reg,func_enter_regnum);
                    //     armlink_insert(newnode,ldmnode);
                    //     vartable_update_all(vartbl,-(func_enter_regnum*4));
                    // }

                    func_gvar_num = search_func_gvar(funcname, func_gvar_ary);
                    armcode *gnode = gvar_node_list(func_gvar_num, func_gvar_ary);

                    if (gnode != NULL)
                    {
                        newnode->pre->next = gnode;
                        gnode->pre = newnode->pre;

                        while (gnode->next != NULL)
                        {
                            gnode = gnode->next;
                        }
                        q = gnode;
                    }

                    newnode->op = ENDLABEL;
                    newnode->result.type = STRING;
                    strcpy(newnode->result.str_id,funcname);
                    newnode->pre = q;
                    q->next = newnode;
                    q = newnode;
                   

                    free(vartbl);
                    init_myreg();
                    break;

                case ARRAY_DEF:
                    cur_stk_offset -= (p->opn1.const_int) * 4;        //数组标号低在低地址;从低往高增长
                    vartable_insert(vartbl,p->result.id,memindex,cur_stk_offset);
                    
                    break;

                case ARRAY_ASSIGN:
                    rn0 = search_var(funcname,p->result.id);
                    if(rn0 >= 0)                        //局部数组，且首址在寄存器中
                    {
                        if(p->opn1.kind == LITERAL)
                        {
                            if(p->opn2.kind == LITERAL)
                            {
                                R_op2 = alloc_myreg();
                                movnode = create_movnode(R_op2,IMME,p->opn2.const_int);
                                armlink_insert(newnode,movnode);

                                addnode = create_addnode(rn0,REG,rn0,IMME,p->opn1.const_int*4);     //计算出相当于SP的偏移
                                armlink_insert(newnode,addnode);

                                init_strnode(newnode,R_op2,R13,rn0,regindex);
                                // vartable_index = vartable_select(vartbl,p->result.id);
                                // if(check_imme(vartable_index) == 0)
                                // {
                                //     addnode = initnewnode();
                                //     addnode->op = ADD;
                                //     addnode->result.type = REG;
                                //     addnode->result.value = R_op2;
                                //     addnode->oper1.type = REG;
                                //     addnode->oper1.value = R_op2;
                                //     addnode->oper2.type = IMME;
                                //     addnode->oper2.value = vartable_index;

                                //     armlink_insert(newnode,addnode)
                                // }
                                // else
                                // {

                                // }
                               
                               
                                
                                

                            }
                            else if(p->opn2.kind == ID)
                            {
                                addnode = create_addnode(rn0,REG,rn0,IMME,p->opn1.const_int*4);     //计算出相当于SP的偏移
                                armlink_insert(newnode,addnode);

                                rn2 = search_var(funcname,p->opn2.id);
                                if(rn2 >= 0)
                                {
                                    init_strnode(newnode,rn2,R13,rn0,regindex);
                                }
                                else if(rn2 == -1)
                                {
                                    R_op2 = alloc_myreg();

                                    vartable_index = vartable_select(vartbl,p->opn2.id);
                                    ldrnode = create_ldrnode(R_op2,NULL,R13,vartbl->table[vartable_index].index);
                                    armlink_insert(newnode,ldrnode);

                                    init_strnode(newnode,R_op2,R13,rn0,regindex);
                                }
                                else if(rn2 == -2)
                                {
                                    R_op2 = alloc_myreg();

                                    ldrnode =create_ldrnode(R_op2,p->opn2.id,0,0);
                                    armlink_insert(newnode,ldrnode);
                                    ldrnode =create_ldrnode(R_op2,NULL,R_op2,0);
                                    armlink_insert(newnode,ldrnode);

                                    init_strnode(newnode,R_op2,R13,rn0,regindex);
                                }
                                else
                                {
                                    printf("%d  %s type ERROR!\n",p->UID,p->opn2.id);
                                }

                            }
                            else if(p->opn2.kind == FLOAT_LITERAL)
                            {

                            }
                            else
                            {
                               printf("%d  p->opn2.kind ERROR!\n",p->UID);
                            }
                        }
                        else if(p->opn1.kind == ID)
                        {
                            if(p->opn2.kind == LITERAL)
                            {
                                R_op2 = alloc_myreg();
                                movnode = create_movnode(R_op2,IMME,p->opn2.const_int);
                                armlink_insert(newnode,movnode);

                                rn1 = search_var(funcname,p->opn1.id);
                                if(rn1 >= 0)
                                {
                                    addnode = create_addnode(rn0,REG,rn0,REG,rn1);     //计算出相当于SP的偏移
                                    armlink_insert(newnode,addnode);

                                    init_strnode(newnode,R_op2,R13,rn0,regindex);
                                }
                                else if(rn1 == -1)
                                {
                                    R_op1 = alloc_myreg();
                                    vartable_index = vartable_select(vartbl,p->opn1.id);
                                    ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                    armlink_insert(newnode,ldrnode);

                                    addnode = create_addnode(rn0,REG,rn0,REG,R_op1);     //计算出相当于SP的偏移
                                    armlink_insert(newnode,addnode);

                                    init_strnode(newnode,R_op2,R13,rn0,regindex);
                                }
                                else if(rn1 == -2)
                                {
                                    R_op1 = alloc_myreg();
                                    ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                    armlink_insert(newnode,ldrnode);
                                    ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                    armlink_insert(newnode,ldrnode);

                                    addnode = create_addnode(rn0,REG,rn0,REG,R_op1);     //计算出相当于SP的偏移
                                    armlink_insert(newnode,addnode);

                                    init_strnode(newnode,R_op2,R13,rn0,regindex);
                                }
                                else
                                {
                                    printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                }
                            }
                            else if(p->opn2.kind == ID)
                            {
                                rn2 = search_var(funcname,p->opn2.id);
                                if(rn2 >= 0)
                                {
                                    rn1 = search_var(funcname,p->opn1.id);
                                    if(rn1 >= 0)
                                    {
                                        addnode = create_addnode(rn0,REG,rn0,REG,rn1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,rn2,R13,rn0,regindex);
                                    }
                                    else if(rn1 == -1)
                                    {
                                        R_op1 = alloc_myreg();
                                        vartable_index = vartable_select(vartbl,p->opn1.id);
                                        ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(rn0,REG,rn0,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,rn2,R13,rn0,regindex);
                                    }
                                    else if(rn1 == -2)
                                    {
                                        R_op1 = alloc_myreg();
                                        ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                        armlink_insert(newnode,ldrnode);
                                        ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(rn0,REG,rn0,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,rn2,R13,rn0,regindex);
                                    }
                                    else
                                    {
                                        printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                    }
                                }
                                else if(rn2 == -1)
                                {
                                    R_op2 = alloc_myreg();
                                    vartable_index = vartable_select(vartbl,p->opn2.id);
                                    ldrnode = create_ldrnode(R_op2,NULL,R13,vartbl->table[vartable_index].index);
                                    armlink_insert(newnode,ldrnode);

                                    rn1 = search_var(funcname,p->opn1.id);
                                    if(rn1 >= 0)
                                    {
                                        addnode = create_addnode(rn0,REG,rn0,REG,rn1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,rn0,regindex);
                                    }
                                    else if(rn1 == -1)
                                    {
                                        R_op1 = alloc_myreg();
                                        vartable_index = vartable_select(vartbl,p->opn1.id);
                                        ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(rn0,REG,rn0,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,rn0,regindex);
                                    }
                                    else if(rn1 == -2)
                                    {
                                        R_op1 = alloc_myreg();
                                        ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                        armlink_insert(newnode,ldrnode);
                                        ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(rn0,REG,rn0,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,rn0,regindex);
                                    }
                                    else
                                    {
                                        printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                    }
                                }
                                else if(rn2 == -2)
                                {
                                    R_op2 = alloc_myreg();
                                    ldrnode = create_ldrnode(R_op2,p->opn2.id,0,0);
                                    armlink_insert(newnode,ldrnode);
                                    ldrnode = create_ldrnode(R_op2,NULL,R_op2,0);
                                    armlink_insert(newnode,ldrnode);

                                    rn1 = search_var(funcname,p->opn1.id);
                                    if(rn1 >= 0)
                                    {
                                        addnode = create_addnode(rn0,REG,rn0,REG,rn1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,rn0,regindex);
                                    }
                                    else if(rn1 == -1)
                                    {
                                        R_op1 = alloc_myreg();
                                        vartable_index = vartable_select(vartbl,p->opn1.id);
                                        ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(rn0,REG,rn0,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,rn0,regindex);
                                    }
                                    else if(rn1 == -2)
                                    {
                                        R_op1 = alloc_myreg();
                                        ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                        armlink_insert(newnode,ldrnode);
                                        ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(rn0,REG,rn0,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,rn0,regindex);
                                    }
                                    else
                                    {
                                        printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                    }
                                }
                                else
                                {
                                    printf("%d  ALLOC REG for p->opn2 ERROR!\n",p->UID);
                                }
                            }
                            else if(p->opn2.kind == FLOAT_LITERAL)
                            {

                            }
                            else 
                            {
                                printf("%d  p->opn2.kind ERROR!\n",p->UID);
                            }
                        }
                        else
                        {
                            printf("%d  p->opn1.kind ERROR!\n",p->UID);
                        }
                    }
                    else if(rn0 == -1)                  //局部数组，首址在内存中
                    {
                        R_res = alloc_myreg();
                        vartable_index = vartable_select(vartbl,p->result.id);
                        ldrnode = create_ldrnode(R_res,NULL,R13,vartbl->table[vartable_index].index);
                        armlink_insert(newnode,ldrnode);


                        if(p->opn1.kind == LITERAL)
                        {
                            if(p->opn2.kind == LITERAL)
                            {
                                R_op2 = alloc_myreg();
                                movnode = create_movnode(R_op2,IMME,p->opn2.const_int);
                                armlink_insert(newnode,movnode);

                                addnode = create_addnode(R_res,REG,R_res,IMME,p->opn1.const_int*4);     //计算出相当于SP的偏移
                                armlink_insert(newnode,addnode);

                                init_strnode(newnode,R_op2,R13,R_res,regindex);
                               
                            }
                            else if(p->opn2.kind == ID)
                            {
                                addnode = create_addnode(R_res,REG,R_res,IMME,p->opn1.const_int*4);     //计算出相当于SP的偏移
                                armlink_insert(newnode,addnode);

                                rn2 = search_var(funcname,p->opn2.id);
                                if(rn2 >= 0)
                                {
                                    init_strnode(newnode,rn2,R13,R_res,regindex);
                                }
                                else if(rn2 == -1)
                                {
                                    R_op2 = alloc_myreg();

                                    vartable_index = vartable_select(vartbl,p->opn2.id);
                                    ldrnode = create_ldrnode(R_op2,NULL,R13,vartbl->table[vartable_index].index);
                                    armlink_insert(newnode,ldrnode);

                                    init_strnode(newnode,R_op2,R13,R_res,regindex);
                                }
                                else if(rn2 == -2)
                                {
                                    R_op2 = alloc_myreg();

                                    ldrnode =create_ldrnode(R_op2,p->opn2.id,0,0);
                                    armlink_insert(newnode,ldrnode);
                                    ldrnode =create_ldrnode(R_op2,NULL,R_op2,0);
                                    armlink_insert(newnode,ldrnode);

                                    init_strnode(newnode,R_op2,R13,R_res,regindex);
                                }
                                else
                                {
                                    printf("%d  %s type ERROR!\n",p->UID,p->opn2.id);
                                }

                            }
                            else if(p->opn2.kind == FLOAT_LITERAL)
                            {

                            }
                            else
                            {
                               printf("%d  p->opn2.kind ERROR!\n",p->UID);
                            }
                        }
                        else if(p->opn1.kind == ID)
                        {
                            if(p->opn2.kind == LITERAL)
                            {
                                R_op2 = alloc_myreg();
                                movnode = create_movnode(R_op2,IMME,p->opn2.const_int);
                                armlink_insert(newnode,movnode);

                                rn1 = search_var(funcname,p->opn1.id);
                                if(rn1 >= 0)
                                {
                                    addnode = create_addnode(R_res,REG,R_res,REG,rn1);     //计算出相当于SP的偏移
                                    armlink_insert(newnode,addnode);

                                    init_strnode(newnode,R_op2,R13,R_res,regindex);
                                }
                                else if(rn1 == -1)
                                {
                                    R_op1 = alloc_myreg();

                                    if(R_op1 != -1)
                                    {
                                        vartable_index = vartable_select(vartbl,p->opn1.id);
                                        ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);
                                    }
                                    else
                                    {
                                        reg[0] = get_other_reg(rn0,rn2);
                                        
                                        snode = mul_reg_node(STMFD,R13,reg,1);
                                        armlink_insert(newnode,snode);

                                        vartable_index = vartable_select(vartbl,p->opn1.id);
                                        ldrnode = create_ldrnode(reg[0],NULL,R13,vartbl->table[vartable_index].index);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);

                                        ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                        q->next = ldmnode;
                                        ldmnode->pre = q;
                                        q = ldmnode;

                                    }
                                }
                                else if(rn1 == -2)
                                {
                                    R_op1 = alloc_myreg();

                                    if(R_op1 != -1)
                                    {
                                        ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                        armlink_insert(newnode,ldrnode);
                                        ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);
                                    }
                                    else
                                    {
                                        reg[0] = get_other_reg(rn0,rn2);
                                        
                                        snode = mul_reg_node(STMFD,R13,reg,1);
                                        armlink_insert(newnode,snode);

                                        ldrnode = create_ldrnode(reg[0],p->opn1.id,0,0);
                                        armlink_insert(newnode,ldrnode);
                                        ldrnode = create_ldrnode(reg[0],NULL,reg[0],0);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);

                                        ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                        q->next = ldmnode;
                                        ldmnode->pre = q;
                                        q = ldmnode;

                                    }
                                }
                                else
                                {
                                    printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                }
                            }
                            else if(p->opn2.kind == ID)
                            {
                                rn2 = search_var(funcname,p->opn2.id);
                                if(rn2 >= 0)
                                {
                                    rn1 = search_var(funcname,p->opn1.id);
                                    if(rn1 >= 0)
                                    {
                                        addnode = create_addnode(R_res,REG,R_res,REG,rn1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,rn2,R13,R_res,regindex);
                                    }
                                    else if(rn1 == -1)
                                    {
                                        R_op1 = alloc_myreg();
                                        vartable_index = vartable_select(vartbl,p->opn1.id);
                                        ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,rn2,R13,R_res,regindex);
                                    }
                                    else if(rn1 == -2)
                                    {
                                        R_op1 = alloc_myreg();
                                        ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                        armlink_insert(newnode,ldrnode);
                                        ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,rn2,R13,R_res,regindex);
                                    }
                                    else
                                    {
                                        printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                    }
                                }
                                else if(rn2 == -1)
                                {
                                    R_op2 = alloc_myreg();
                                    vartable_index = vartable_select(vartbl,p->opn2.id);
                                    ldrnode = create_ldrnode(R_op2,NULL,R13,vartbl->table[vartable_index].index);
                                    armlink_insert(newnode,ldrnode);

                                    rn1 = search_var(funcname,p->opn1.id);
                                    if(rn1 >= 0)
                                    {
                                        addnode = create_addnode(R_res,REG,R_res,REG,rn1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);
                                    }
                                    else if(rn1 == -1)
                                    {
                                        R_op1 = alloc_myreg();                                       

                                        if(R_op1 != -1)
                                        {
                                            vartable_index = vartable_select(vartbl,p->opn1.id);
                                            ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);
                                        }
                                        else
                                        {
                                            reg[0] = get_other_reg(rn0,rn2);
                                            
                                            snode = mul_reg_node(STMFD,R13,reg,1);
                                            armlink_insert(newnode,snode);

                                            vartable_index = vartable_select(vartbl,p->opn1.id);
                                            ldrnode = create_ldrnode(reg[0],NULL,R13,vartbl->table[vartable_index].index);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);

                                            ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                            q->next = ldmnode;
                                            ldmnode->pre = q;
                                            q = ldmnode;

                                        }
                                    }
                                    else if(rn1 == -2)
                                    {
                                        R_op1 = alloc_myreg();                                       

                                        if(R_op1 != -1)
                                        {
                                            ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                            armlink_insert(newnode,ldrnode);
                                            ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);
                                        }
                                        else
                                        {
                                            reg[0] = get_other_reg(rn0,rn2);
                                            
                                            snode = mul_reg_node(STMFD,R13,reg,1);
                                            armlink_insert(newnode,snode);

                                            ldrnode = create_ldrnode(reg[0],p->opn1.id,0,0);
                                            armlink_insert(newnode,ldrnode);
                                            ldrnode = create_ldrnode(reg[0],NULL,reg[0],0);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);

                                            ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                            q->next = ldmnode;
                                            ldmnode->pre = q;
                                            q = ldmnode;

                                        }
                                    }
                                    else
                                    {
                                        printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                    }
                                }
                                else if(rn2 == -2)
                                {
                                    R_op2 = alloc_myreg();
                                    ldrnode = create_ldrnode(R_op2,p->opn2.id,0,0);
                                    armlink_insert(newnode,ldrnode);
                                    ldrnode = create_ldrnode(R_op2,NULL,R_op2,0);
                                    armlink_insert(newnode,ldrnode);

                                    rn1 = search_var(funcname,p->opn1.id);
                                    if(rn1 >= 0)
                                    {
                                        addnode = create_addnode(R_res,REG,R_res,REG,rn1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);
                                    }
                                    else if(rn1 == -1)
                                    {
                                        R_op1 = alloc_myreg();
                                        

                                        if(R_op1 != -1)
                                        {
                                            vartable_index = vartable_select(vartbl,p->opn1.id);
                                            ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);
                                        }
                                        else
                                        {
                                            reg[0] = get_other_reg(rn0,rn2);
                                            
                                            snode = mul_reg_node(STMFD,R13,reg,1);
                                            armlink_insert(newnode,snode);

                                            vartable_index = vartable_select(vartbl,p->opn1.id);
                                            ldrnode = create_ldrnode(reg[0],NULL,R13,vartbl->table[vartable_index].index);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);

                                            ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                            q->next = ldmnode;
                                            ldmnode->pre = q;
                                            q = ldmnode;

                                        }
                                    }
                                    else if(rn1 == -2)
                                    {
                                        R_op1 = alloc_myreg();

                                        if(R_op1 != -1)
                                        {
                                            ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                            armlink_insert(newnode,ldrnode);
                                            ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);
                                        }
                                        else
                                        {
                                            reg[0] = get_other_reg(rn0,rn2);
                                            
                                            snode = mul_reg_node(STMFD,R13,reg,1);
                                            armlink_insert(newnode,snode);

                                            ldrnode = create_ldrnode(reg[0],p->opn1.id,0,0);
                                            armlink_insert(newnode,ldrnode);
                                            ldrnode = create_ldrnode(reg[0],NULL,reg[0],0);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);

                                            ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                            q->next = ldmnode;
                                            ldmnode->pre = q;
                                            q = ldmnode;

                                        }
                                    }
                                    else
                                    {
                                        printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                    }
                                }
                                else
                                {
                                    printf("%d  ALLOC REG for p->opn2 ERROR!\n",p->UID);
                                }
                            }
                            else if(p->opn2.kind == FLOAT_LITERAL)
                            {

                            }
                            else 
                            {
                                printf("%d  p->opn2.kind ERROR!\n",p->UID);
                            }
                        }
                        else
                        {
                            printf("%d  p->opn1.kind ERROR!\n",p->UID);
                        }

                    }
                    else if(rn0 == -2)                  //全局数组
                    {
                        R_res = alloc_myreg();
                        ldrnode = create_ldrnode(R_res,p->result.id,0,0);
                        armlink_insert(newnode,ldrnode);
                        ldrnode = create_ldrnode(R_res,NULL,R_res,0);
                        armlink_insert(newnode,ldrnode);


                        if(p->opn1.kind == LITERAL)
                        {
                            if(p->opn2.kind == LITERAL)
                            {
                                R_op2 = alloc_myreg();
                                movnode = create_movnode(R_op2,IMME,p->opn2.const_int);
                                armlink_insert(newnode,movnode);

                                addnode = create_addnode(R_res,REG,R_res,IMME,p->opn1.const_int*4);     //计算出相当于SP的偏移
                                armlink_insert(newnode,addnode);

                                init_strnode(newnode,R_op2,R13,R_res,regindex);
                               
                            }
                            else if(p->opn2.kind == ID)
                            {
                                addnode = create_addnode(R_res,REG,R_res,IMME,p->opn1.const_int*4);     //计算出相当于SP的偏移
                                armlink_insert(newnode,addnode);

                                rn2 = search_var(funcname,p->opn2.id);
                                if(rn2 >= 0)
                                {
                                    init_strnode(newnode,rn2,R13,R_res,regindex);
                                }
                                else if(rn2 == -1)
                                {
                                    R_op2 = alloc_myreg();

                                    vartable_index = vartable_select(vartbl,p->opn2.id);
                                    ldrnode = create_ldrnode(R_op2,NULL,R13,vartbl->table[vartable_index].index);
                                    armlink_insert(newnode,ldrnode);

                                    init_strnode(newnode,R_op2,R13,R_res,regindex);
                                }
                                else if(rn2 == -2)
                                {
                                    R_op2 = alloc_myreg();

                                    ldrnode =create_ldrnode(R_op2,p->opn2.id,0,0);
                                    armlink_insert(newnode,ldrnode);
                                    ldrnode =create_ldrnode(R_op2,NULL,R_op2,0);
                                    armlink_insert(newnode,ldrnode);

                                    init_strnode(newnode,R_op2,R13,R_res,regindex);
                                }
                                else
                                {
                                    printf("%d  %s type ERROR!\n",p->UID,p->opn2.id);
                                }

                            }
                            else if(p->opn2.kind == FLOAT_LITERAL)
                            {

                            }
                            else
                            {
                               printf("%d  p->opn2.kind ERROR!\n",p->UID);
                            }
                        }
                        else if(p->opn1.kind == ID)
                        {
                            if(p->opn2.kind == LITERAL)
                            {
                                R_op2 = alloc_myreg();
                                movnode = create_movnode(R_op2,IMME,p->opn2.const_int);
                                armlink_insert(newnode,movnode);

                                rn1 = search_var(funcname,p->opn1.id);
                                if(rn1 >= 0)
                                {
                                    addnode = create_addnode(R_res,REG,R_res,REG,rn1);     //计算出相当于SP的偏移
                                    armlink_insert(newnode,addnode);

                                    init_strnode(newnode,R_op2,R13,R_res,regindex);
                                }
                                else if(rn1 == -1)
                                {
                                    R_op1 = alloc_myreg();

                                    if(R_op1 != -1)
                                    {
                                        vartable_index = vartable_select(vartbl,p->opn1.id);
                                        ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);
                                    }
                                    else
                                    {
                                        reg[0] = get_other_reg(rn0,rn2);
                                        
                                        snode = mul_reg_node(STMFD,R13,reg,1);
                                        armlink_insert(newnode,snode);

                                        vartable_index = vartable_select(vartbl,p->opn1.id);
                                        ldrnode = create_ldrnode(reg[0],NULL,R13,vartbl->table[vartable_index].index);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);

                                        ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                        q->next = ldmnode;
                                        ldmnode->pre = q;
                                        q = ldmnode;

                                    }
                                }
                                else if(rn1 == -2)
                                {
                                    R_op1 = alloc_myreg();

                                    if(R_op1 != -1)
                                    {
                                        ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                        armlink_insert(newnode,ldrnode);
                                        ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);
                                    }
                                    else
                                    {
                                        reg[0] = get_other_reg(rn0,rn2);
                                        
                                        snode = mul_reg_node(STMFD,R13,reg,1);
                                        armlink_insert(newnode,snode);

                                        ldrnode = create_ldrnode(reg[0],p->opn1.id,0,0);
                                        armlink_insert(newnode,ldrnode);
                                        ldrnode = create_ldrnode(reg[0],NULL,reg[0],0);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);

                                        ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                        q->next = ldmnode;
                                        ldmnode->pre = q;
                                        q = ldmnode;

                                    }
                                }
                                else
                                {
                                    printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                }
                            }
                            else if(p->opn2.kind == ID)
                            {
                                rn2 = search_var(funcname,p->opn2.id);
                                if(rn2 >= 0)
                                {
                                    rn1 = search_var(funcname,p->opn1.id);
                                    if(rn1 >= 0)
                                    {
                                        addnode = create_addnode(R_res,REG,R_res,REG,rn1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,rn2,R13,R_res,regindex);
                                    }
                                    else if(rn1 == -1)
                                    {
                                        R_op1 = alloc_myreg();
                                        vartable_index = vartable_select(vartbl,p->opn1.id);
                                        ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,rn2,R13,R_res,regindex);
                                    }
                                    else if(rn1 == -2)
                                    {
                                        R_op1 = alloc_myreg();
                                        ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                        armlink_insert(newnode,ldrnode);
                                        ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                        armlink_insert(newnode,ldrnode);

                                        addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,rn2,R13,R_res,regindex);
                                    }
                                    else
                                    {
                                        printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                    }
                                }
                                else if(rn2 == -1)
                                {
                                    R_op2 = alloc_myreg();
                                    vartable_index = vartable_select(vartbl,p->opn2.id);
                                    ldrnode = create_ldrnode(R_op2,NULL,R13,vartbl->table[vartable_index].index);
                                    armlink_insert(newnode,ldrnode);

                                    rn1 = search_var(funcname,p->opn1.id);
                                    if(rn1 >= 0)
                                    {
                                        addnode = create_addnode(R_res,REG,R_res,REG,rn1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);
                                    }
                                    else if(rn1 == -1)
                                    {
                                        R_op1 = alloc_myreg();                                       

                                        if(R_op1 != -1)
                                        {
                                            vartable_index = vartable_select(vartbl,p->opn1.id);
                                            ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);
                                        }
                                        else
                                        {
                                            reg[0] = get_other_reg(rn0,rn2);
                                            
                                            snode = mul_reg_node(STMFD,R13,reg,1);
                                            armlink_insert(newnode,snode);

                                            vartable_index = vartable_select(vartbl,p->opn1.id);
                                            ldrnode = create_ldrnode(reg[0],NULL,R13,vartbl->table[vartable_index].index);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);

                                            ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                            q->next = ldmnode;
                                            ldmnode->pre = q;
                                            q = ldmnode;

                                        }
                                    }
                                    else if(rn1 == -2)
                                    {
                                        R_op1 = alloc_myreg();                                       

                                        if(R_op1 != -1)
                                        {
                                            ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                            armlink_insert(newnode,ldrnode);
                                            ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);
                                        }
                                        else
                                        {
                                            reg[0] = get_other_reg(rn0,rn2);
                                            
                                            snode = mul_reg_node(STMFD,R13,reg,1);
                                            armlink_insert(newnode,snode);

                                            ldrnode = create_ldrnode(reg[0],p->opn1.id,0,0);
                                            armlink_insert(newnode,ldrnode);
                                            ldrnode = create_ldrnode(reg[0],NULL,reg[0],0);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);

                                            ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                            q->next = ldmnode;
                                            ldmnode->pre = q;
                                            q = ldmnode;

                                        }
                                    }
                                    else
                                    {
                                        printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                    }
                                }
                                else if(rn2 == -2)
                                {
                                    R_op2 = alloc_myreg();
                                    ldrnode = create_ldrnode(R_op2,p->opn2.id,0,0);
                                    armlink_insert(newnode,ldrnode);
                                    ldrnode = create_ldrnode(R_op2,NULL,R_op2,0);
                                    armlink_insert(newnode,ldrnode);

                                    rn1 = search_var(funcname,p->opn1.id);
                                    if(rn1 >= 0)
                                    {
                                        addnode = create_addnode(R_res,REG,R_res,REG,rn1);     //计算出相当于SP的偏移
                                        armlink_insert(newnode,addnode);

                                        init_strnode(newnode,R_op2,R13,R_res,regindex);
                                    }
                                    else if(rn1 == -1)
                                    {
                                        R_op1 = alloc_myreg();
                                        

                                        if(R_op1 != -1)
                                        {
                                            vartable_index = vartable_select(vartbl,p->opn1.id);
                                            ldrnode = create_ldrnode(R_op1,NULL,R13,vartbl->table[vartable_index].index);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);
                                        }
                                        else
                                        {
                                            reg[0] = get_other_reg(rn0,rn2);
                                            
                                            snode = mul_reg_node(STMFD,R13,reg,1);
                                            armlink_insert(newnode,snode);

                                            vartable_index = vartable_select(vartbl,p->opn1.id);
                                            ldrnode = create_ldrnode(reg[0],NULL,R13,vartbl->table[vartable_index].index);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);

                                            ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                            q->next = ldmnode;
                                            ldmnode->pre = q;
                                            q = ldmnode;

                                        }
                                    }
                                    else if(rn1 == -2)
                                    {
                                        R_op1 = alloc_myreg();

                                        if(R_op1 != -1)
                                        {
                                            ldrnode = create_ldrnode(R_op1,p->opn1.id,0,0);
                                            armlink_insert(newnode,ldrnode);
                                            ldrnode = create_ldrnode(R_op1,NULL,R_op1,0);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,R_op1);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);
                                        }
                                        else
                                        {
                                            reg[0] = get_other_reg(rn0,rn2);
                                            
                                            snode = mul_reg_node(STMFD,R13,reg,1);
                                            armlink_insert(newnode,snode);

                                            ldrnode = create_ldrnode(reg[0],p->opn1.id,0,0);
                                            armlink_insert(newnode,ldrnode);
                                            ldrnode = create_ldrnode(reg[0],NULL,reg[0],0);
                                            armlink_insert(newnode,ldrnode);

                                            addnode = create_addnode(R_res,REG,R_res,REG,reg[0]);     //计算出相当于SP的偏移
                                            armlink_insert(newnode,addnode);

                                            init_strnode(newnode,R_op2,R13,R_res,regindex);

                                            ldmnode = mul_reg_node(LDMFD,R13,reg,1);
                                            q->next = ldmnode;
                                            ldmnode->pre = q;
                                            q = ldmnode;

                                        }
                                    }
                                    else
                                    {
                                        printf("%d  ALLOC REG for p->opn1 ERROR!\n",p->UID);
                                    }
                                }
                                else
                                {
                                    printf("%d  ALLOC REG for p->opn2 ERROR!\n",p->UID);
                                }
                            }
                            else if(p->opn2.kind == FLOAT_LITERAL)
                            {

                            }
                            else 
                            {
                                printf("%d  p->opn2.kind ERROR!\n",p->UID);
                            }
                        }
                        else
                        {
                            printf("%d  p->opn1.kind ERROR!\n",p->UID);
                        }
                    }
                    else
                    {
                       printf("%d  %s REG ALLOC ERROR!\n",p->UID,p->result.id);
                    }
                    init_myreg();
                    break;

                case ARRAY_EXP:
                    init_myreg();
                    break;

                case GOTO:
                    newnode->op = B;
                    newnode->result.type = STRING;
                    strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                    // printf("B %s", newnode->result.str_id);
                    break;

                case JLE: // <=
                    if ((p->opn1.kind == LITERAL) && (p->opn2.kind == LITERAL))
                    {
                        if (p->opn1.const_int <= p->opn2.const_int)
                        {
                            newnode->op = B;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else
                        {
                            // 无效跳转
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == LITERAL))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = LE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == LITERAL) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn2.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = GT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -1)
                        {
                            // 右操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将栈中变量取到 R_op2 中
                            vartable_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -2)
                        {
                            // 右操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将全局变量值取到 R_op2 中
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {
                            // 左操作数正常分配寄存器

                            R_op1 = rn0;

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    init_myreg();
                    break;

                case JLT: // <
                    if ((p->opn1.kind == LITERAL) && (p->opn2.kind == LITERAL))
                    {
                        if (p->opn1.const_int < p->opn2.const_int)
                        {
                            newnode->op = B;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                            // printf("B %s", newnode->result.str_id);
                        }
                        else
                        {
                            // 无效跳转
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == LITERAL))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = LT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == LITERAL) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn2.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = GE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -1)
                        {
                            // 右操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将栈中变量取到 R_op2 中
                            vartable_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -2)
                        {
                            // 右操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将全局变量值取到 R_op2 中
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {
                            // 左操作数正常分配寄存器

                            R_op1 = rn0;

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    init_myreg();
                    break;

                case JGE: // >=
                    if ((p->opn1.kind == LITERAL) && (p->opn2.kind == LITERAL))
                    {
                        if (p->opn1.const_int >= p->opn2.const_int)
                        {
                            newnode->op = B;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                            // printf("B %s", newnode->result.str_id);
                        }
                        else
                        {
                            // 无效跳转
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == LITERAL))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = GE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == LITERAL) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn2.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = LT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -1)
                        {
                            // 右操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将栈中变量取到 R_op2 中
                            vartable_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -2)
                        {
                            // 右操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将全局变量值取到 R_op2 中
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {
                            // 左操作数正常分配寄存器

                            R_op1 = rn0;

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    init_myreg();
                    break;

                case EQ: // ==
                    if ((p->opn1.kind == LITERAL) && (p->opn2.kind == LITERAL))
                    {
                        if (p->opn1.const_int == p->opn2.const_int)
                        {
                            newnode->op = B;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                            // printf("B %s", newnode->result.str_id);
                        }
                        else
                        {
                            // 无效跳转
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == LITERAL))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = EQU;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = EQU;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = EQU;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == LITERAL) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn2.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = EQU;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -1)
                        {
                            // 右操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将栈中变量取到 R_op2 中
                            vartable_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = EQU;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -2)
                        {
                            // 右操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将全局变量值取到 R_op2 中
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = EQU;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = EQU;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = EQU;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {
                            // 左操作数正常分配寄存器

                            R_op1 = rn0;

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = EQU;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    init_myreg();
                    break;

                case NEQ: // !=
                    if ((p->opn1.kind == LITERAL) && (p->opn2.kind == LITERAL))
                    {
                        if (p->opn1.const_int != p->opn2.const_int)
                        {
                            newnode->op = B;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                            // printf("B %s", newnode->result.str_id);
                        }
                        else
                        {
                            // 无效跳转
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == LITERAL))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = NE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = NE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = NE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == LITERAL) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn2.id)) >= 0)
                        {

                            newnode->op = CMP;

                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = NE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -1)
                        {
                            // 右操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将栈中变量取到 R_op2 中
                            vartable_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = NE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn2.id)) == -2)
                        {
                            // 右操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将全局变量值取到 R_op2 中
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = NE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    else if ((p->opn1.kind == ID) && (p->opn2.kind == ID))
                    {
                        if ((rn0 = search_var(funcname, p->opn1.id)) == -1)
                        {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = NE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) == -2)
                        {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = NE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                        else if ((rn0 = search_var(funcname, p->opn1.id)) >= 0)
                        {
                            // 左操作数正常分配寄存器

                            R_op1 = rn0;

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1)
                            {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) == -2)
                            {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);
                            }
                            else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0)
                            {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;
                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = NE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    init_myreg();
                    break;

                case JGT: // >
                    if ((p->opn1.kind == LITERAL) && (p->opn2.kind == LITERAL)) {
                        if (p->opn1.const_int > p->opn2.const_int) {
                            newnode->op = B;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                            //printf("B %s", newnode->result.str_id);
                        } else {
                            // 无效跳转
                        }
                    }  else if ((p->opn1.kind == ID) && (p->opn2.kind == LITERAL)) {
                        if ((rn0 = search_var(funcname, p->opn1.id)) >= 0) {
                            
                            newnode->op = CMP;
                            
                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = GT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));

                        } else if ((rn0 = search_var(funcname, p->opn1.id)) == -1) {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));

                        } else if ((rn0 = search_var(funcname, p->opn1.id)) == -2) {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode,ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode,ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn2.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                            
                        }
                    } else if ((p->opn1.kind == LITERAL) && (p->opn2.kind == ID)) {
                        if ((rn0 = search_var(funcname, p->opn2.id)) >= 0) {
                            
                            newnode->op = CMP;
                            
                            newnode->oper1.type = REG;
                            newnode->oper1.value = rn0;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            newnode->op = B;
                            newnode->flag = LE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));

                        } else if ((rn0 = search_var(funcname, p->opn2.id)) == -1) {
                            // 右操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将栈中变量取到 R_op2 中
                            vartable_index = vartable_select(vartbl, p->opn2.id);
                            ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));

                        } else if ((rn0 = search_var(funcname, p->opn2.id)) == -2) {
                            // 右操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op2 = alloc_myreg();

                            // 将全局变量值取到 R_op2 中
                            ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                            armlink_insert(newnode, ldrnode);

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op2;

                            newnode->oper2.type = IMME;
                            newnode->oper2.value = p->opn1.const_int;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = LE;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));

                        }
                    } else if ((p->opn1.kind == ID) && (p->opn2.kind == ID)) {
                        if ((rn0 = search_var(funcname, p->opn1.id)) == -1) {
                            // 左操作数为变量且溢出

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将栈中变量取到 R_op1 中
                            vartable_index = vartable_select(vartbl, p->opn1.id);
                            ldrnode = create_ldrnode(R_op1, NULL, R13, vartbl->table[vartable_index].index);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1) {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            
                            } else if ((rn1 = search_var(funcname, p->opn2.id)) == -2) {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);

                            } else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0) {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;

                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));

                        } else if ((rn0 = search_var(funcname, p->opn1.id)) == -2) {
                            // 左操作数为全局变量

                            // 必然可分配到 R11、R12 之一
                            R_op1 = alloc_myreg();

                            // 将全局变量值取到 R_op1 中
                            ldrnode = create_ldrnode(R_op1, p->opn1.id, 0, 0);
                            armlink_insert(newnode, ldrnode);
                            ldrnode = create_ldrnode(R_op1, NULL, R_op1, 0);
                            armlink_insert(newnode, ldrnode);

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1) {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            
                            } else if ((rn1 = search_var(funcname, p->opn2.id)) == -2) {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);

                            } else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0) {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;

                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));

                        } else if ((rn0 = search_var(funcname, p->opn1.id)) >= 0 ) {
                            // 左操作数正常分配寄存器

                            R_op1 = rn0;

                            // 将右操作数取到 R_op2 中
                            if ((rn1 = search_var(funcname, p->opn2.id)) == -1) {
                                // 右操作数为变量且溢出

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将栈中变量取到 R_op2 中
                                vartable_index = vartable_select(vartbl, p->opn2.id);
                                ldrnode = create_ldrnode(R_op2, NULL, R13, vartbl->table[vartable_index].index);
                                armlink_insert(newnode, ldrnode);
                            
                            } else if ((rn1 = search_var(funcname, p->opn2.id)) == -2) {
                                // 右操作数为全局变量

                                // 必然可分配到 R11、R12 之一
                                R_op2 = alloc_myreg();

                                // 将全局变量值取到 R_op2 中
                                ldrnode = create_ldrnode(R_op2, p->opn2.id, 0, 0);
                                armlink_insert(newnode, ldrnode);
                                ldrnode = create_ldrnode(R_op2, NULL, R_op2, 0);
                                armlink_insert(newnode, ldrnode);

                            } else if ((rn1 = search_var(funcname, p->opn2.id)) >= 0) {
                                // 右操作数正常分配寄存器

                                // 便于后续统一操作
                                R_op2 = rn1;

                            }

                            // CMP 指令
                            newnode->op = CMP;
                            newnode->oper1.type = REG;
                            newnode->oper1.value = R_op1;

                            newnode->oper2.type = REG;
                            newnode->oper2.value = R_op2;

                            // 新建节点
                            newnode = initnewnode();
                            q->next = newnode;
                            newnode->pre = q;
                            q = newnode;

                            // B 指令
                            newnode->op = B;
                            newnode->flag = GT;
                            newnode->result.type = STRING;
                            strcpy(newnode->result.str_id, uid_to_label(p->result.const_int));
                        }
                    }
                    init_myreg();
                    break;

                default:
                    break;
                }
                p = p->next;
            }
        }
        cur_blocks = cur_blocks->next;
    }

    armcode *vnode = search_global_var();
    if (vnode != NULL)
    {
        q->next = vnode;
        vnode->pre = q;
        q = vnode;
    }

    return first;
}

armcode *mul_reg_node(armop opt, int stkreg, int reg[], int regnum)
{
    // armcode * snode = (armcode*)malloc(sizeof(struct armcode_));
    armcode *snode = initnewnode();
    snode->op = opt;
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

armcode *gvar_node_list(int func_gvar_num, char **gvartable)
{
    armcode *p = initnewnode();
    armcode *m = p;
    for (int i = 0; i < func_gvar_num; i++)
    {
        armcode *gvarnode = gvar_node(gvartable, i, func_index);
        m->next = gvarnode;
        gvarnode->pre = m;
        m = gvarnode->next;
    }

    func_index++;

    return p->next;
}

armcode *gvar_node(char **gvartable, int i, int func_index)
{
    char label[20] = {0};
    armcode *lblnode = initnewnode();
    lblnode->op = ARMLABEL;
    lblnode->result.type = STRING;
    sprintf(label, ".LCPI%d_%s", func_index, gvartable[i]);
    strcpy(lblnode->result.str_id, label);

    armcode *nnode = initnewnode();
    nnode->op = GVAR_LABEL;
    nnode->result.type = STRING;
    strcpy(nnode->result.str_id, ".long");
    nnode->oper1.type = STRING;
    char str[33] = {0};
    sprintf(str, "%s", gvartable[i]);
    // strcpy(nnode->oper1.str_id,gvartable[i]);
    strcpy(nnode->oper1.str_id, str);

    lblnode->next = nnode;
    nnode->pre = lblnode;

    return lblnode;
}

armcode *create_ldrnode(int Rn, char *gvarname, int Rm, int index)
{
    char tmp[33] = {0};
    armcode *lnode = initnewnode();
    lnode->op = LDR;
    lnode->result.value = Rn;

    if (gvarname == NULL)
    {
        lnode->oper1.type = MEM;
        lnode->oper1.value = Rm;
        lnode->oper1.index = index;
    }
    else
    {
        lnode->oper1.type = STRING;
        sprintf(tmp,".LCPI%d_%s",func_index,gvarname);
        strcpy(lnode->oper1.str_id,tmp);
    }

    return lnode;
}

armcode *create_strnode(int Rn, int Rm, int index)
{
    armcode *strnode = initnewnode();
    strnode->op = STR;
    strnode->result.value = Rn;
    strnode->oper1.type = MEM;
    strnode->oper1.value = Rm;
    strnode->oper1.index = index;

    return strnode;
}

armcode *create_movnode(int R_res, optype type, int value)
{
    armcode *mnode = initnewnode();
    mnode->op = MOV;
    mnode->result.value = R_res;

    mnode->oper1.type = type;
    mnode->oper1.value = value;

    return mnode;
}

armcode * create_addnode(int R_res,optype type_op1,int op1,int type_op2,int op2)
{
    armcode * anode = initnewnode();
    anode->op = ADD;
    anode->result.value = R_res;
    anode->oper1.type = type_op1;
    anode->oper1.value = op1;
    anode->oper2.type = type_op2;
    anode->oper2.value = op2;

    return anode;
}

void init_strnode(armcode * snode,int R_res,int Rm,int index,int indexkind)
{
    snode->op = STR;
    snode->result.type = REG;
    snode->result.value = R_res;
    snode->oper1.type = MEM;
    snode->oper1.value = Rm;
    snode->oper1.index = index;
    snode->oper1.kind = indexkind;
}

void init_myreg()
{
    R11R12 = 3;
}

/**
 * @brief 分配自由分配的寄存器，如果
 *
 * @return int
 */
int alloc_myreg()
{
    // if(R11R12 == 7)
    // {
    //     R11R12 = R11R12 >> 1;
    //     return R10;
    // }
    if (R11R12 == 3)
    {
        R11R12 = R11R12 >> 1;
        return R11;
    }
    else if (R11R12 == 1)
    {
        R11R12 = R11R12 >> 1;
        return R12;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief 一共16个寄存器，找到当前语句没用的寄存器
 *
 * @param i 有没有可能是-1表示不用
 * @param j
 * @return 寄存器号
 */
int get_other_reg(int i, int j)
{
    //最简单写法就遍历呗，但是这样应该会
    static int index = 0;
    for (index %= 10; index < 10; index = (index + 1) % 10)
    {
        if (index != i && index != j)
        {
            return index++;
        }
    }
}
