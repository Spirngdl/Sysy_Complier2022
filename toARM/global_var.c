#include "../include/def.h"

armcode *search_global_var()
{
    int i;
    armcode *q = initnewnode();
    armcode *p = q;
    for (int i = symbolTable.index - 1; i >= 0; i--)
    {
        armcode *newnode = initnewnode();
        if (symbolTable.symbols[i].flag != FUNCTION && symbolTable.symbols[i].level == 0) //如果不是函数且层级为0，即是全局变量
        {
            //有可能是数组
            struct symbol sym = symbolTable.symbols[i];

            newnode->op = ARMLABEL;
            newnode->result.type = STRING;
            strcpy(newnode->result.str_id, sym.alias);
            p->next = newnode;
            newnode->pre = p;
            p = newnode;

            if (sym.flag == ARRAY) //可能是数组
            {
                int arysize = sym.length[0];
                for (int i = 1; i < sym.array_dimension; i++)
                {
                    arysize *= sym.length[i];
                }

                armcode * vnode = traverse_List(sym.value,arysize);
                p->next = vnode;
                vnode->pre = p;
                p = vnode;

            }
            else if (sym.flag == VAR ||sym.flag == CONST_VAR) //全局变量
            {
                // char *alias = sym.alias;//别名，三地址代码中所有变量名都是别名
                // int type = sym.type; //可能是TOK_INT TOK_FLOAT

                // newnode->op = ARMLABEL;
                // newnode->result.type = STRING;
                // strcpy(newnode->result.str_id, sym.alias);
                // p->next = newnode;
                // newnode->pre = p;
                // p = newnode;

                armcode *vnode = initnewnode();
                vnode->result.type = STRING;
                strcpy(vnode->result.str_id, ".long");
                vnode->oper1.type = IMME;
                if (sym.type == TOK_INT)
                {
                    vnode->oper1.value = sym.const_value;
                    vnode->op = GVAR_INT;
                }
                else if (sym.type == TOK_FLOAT)
                {
                    vnode->oper1.value = ToIeee754(sym.const_value);
                    vnode->op = GVAR_FLOAT;
                }

                p->next = vnode;
                vnode->pre = p;
                p = vnode;
            }
        }
    }

    return q->next;
}

armcode *traverse_List(List *value_list,int arysize)
{

    if (value_list == NULL)
    {
        armcode * anode = initnewnode();
        anode->op = GVAR_INT;
        anode->result.type = STRING;
        strcpy(anode->result.str_id,".zero");
        anode->oper1.type = IMME;
        anode->oper1.value = arysize*4;

        return anode;
    }
    else
    {
        void *element = NULL;
        ListFirst(value_list, false); //重置iterator
        armcode * anode = initnewnode();
        armcode * q = anode;
        while (ListNext(value_list, &element))
        {
            ArrayValue *value = (ArrayValue *)element;
            armcode * m = initnewnode();
            m->result.type = STRING;
            strcpy(m->result.str_id,".long");
            m->oper1.type = IMME;
            if (value->kind == LITERAL) //整型常量
            {
                m->oper1.value = value->v_int;
                m->op = GVAR_INT;
            }
            else if (value->kind == FLOAT_LITERAL) //浮点型常量
            {
                m->oper1.value = ToIeee754(value->v_float);
                m->op = GVAR_FLOAT;
            }
            q->next = m;
            m->pre = q;
            q = m;
            // else if (value->kind == ID) //可以显式的写出来
            // {
            // }
            // else if (value->kind == NONE) //没法存在符号表里，比如说数组，所以我会显式的写出来
            // {
            // }
        }

        return anode->next;
    }
}
