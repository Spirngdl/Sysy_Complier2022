/**
 * @file ast.c
 * @author your name (you@domain.com)
 * @brief 定义对抽象语法树生成相关的函数。
 * @version 0.1
 * @date 2022-07-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "include/def.h"
struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos)
{
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = kind;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    T->code = NULL;
    return T;
}
struct node *mkparray(int kind, char *name, struct node *len, int pos)
{
    struct node *temp = mknode(kind, NULL, NULL, NULL, pos);

    strcpy(temp->type_id, name);
    int rtn = fillast(name, kind);
    temp->place = rtn;

    astsymbol.symbols[rtn].array_dimension = 0;
    memset(astsymbol.symbols[rtn].length, 0, sizeof(astsymbol.symbols[rtn].length));
    if (kind == PARAM_ARRAY)
    {
        astsymbol.symbols[rtn].length[0] = 0;
        astsymbol.symbols[rtn].array_dimension = 1;
    }
    //处理维度
    while (len != NULL) //多维
    {
        astsymbol.symbols[rtn].length[astsymbol.symbols[rtn].array_dimension] = len->type_int;
        astsymbol.symbols[rtn].array_dimension += 1;
        len = len->ptr[0];
    }
    return temp;
}
struct node *mkopnode(int kind, struct node *left, struct node *right, int pos)
{
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->pos = pos;
    if ((left->kind == LITERAL || left->kind == FLOAT_LITERAL) && (right->kind == LITERAL || right->kind == FLOAT_LITERAL))
    {
        double lval, rval;
        int lint, rint;
        T->kind = LITERAL;
        if (left->kind == LITERAL)
            lval = left->type_int;
        else
        {
            lval = left->type_float;
            T->kind = FLOAT_LITERAL;
        }
        if (right->kind == LITERAL)
            rval = right->type_int;
        else
        {
            rval = right->type_float;
            T->kind = FLOAT_LITERAL;
        }
        switch (kind)
        {
        case TOK_ADD:
            if (T->kind == LITERAL)
                T->type_int = lval + rval;
            else
                T->type_float = lval + rval;
            break;
        case TOK_SUB:
            if (T->kind == LITERAL)
                T->type_int = lval - rval;
            else
                T->type_float = lval - rval;
            break;
        case TOK_DIV:
            if (T->kind == LITERAL)
                T->type_int = lval / rval;
            else
                T->type_float = lval / rval;
            break;
        case TOK_MUL:
            if (T->kind == LITERAL)
                T->type_int = lval * rval;
            else
                T->type_float = lval * rval;
            break;
        case TOK_MODULO:
            lint = lval;
            rint = rval;
            if (T->kind == LITERAL)
                T->type_int = lint % rint;
            else
                T->type_float = lint % rint;
            break;
        default:
            break;
        }
        return T;
    }
    else
    {
        T->kind = kind;
        T->ptr[0] = left;
        T->ptr[1] = right;
        T->ptr[2] = NULL;
        T->code = NULL;
        return T;
    }
}
/**
 * @brief 用于单目运算节点，如果是LITERAL，直接生成
 * 
 * @param kind 
 * @param first 
 * @param second 
 * @param pos 
 * @return struct node* 
 */
struct node *mkunarynode(int kind, struct node *first, struct node *second, int pos)
{
    struct node *T = mknode(kind, NULL, NULL, NULL, pos);
    if (second->kind == LITERAL)
    {
        T->kind = LITERAL;
        if (first->kind == TOK_ADD)
        {
            T->type_int = second->type_int;
        }
        else if (first->kind == TOK_SUB)
        {
            T->type_int = -(second->type_int);
        }
        else if (first->kind == TOK_NOT)
        {
            T->type_int = second->type_int == 0 ? 1 : 0;
        }
        return T;
    }
    else if (second->kind == FLOAT_LITERAL)
    {
        T->kind == FLOAT_LITERAL;
        if (first->kind == TOK_ADD)
        {
            T->type_float = second->type_float;
        }
        else if (first->kind == TOK_SUB)
        {
            T->type_float = second->type_float;
        }
        else if (first->kind == TOK_NOT)
        {
            T->type_float = second->type_float == 0 ? 1 : 0;
        }
        return T;
    }
    else //
    {
        T->ptr[0] = first;
        T->ptr[1] = second;
        return T;
    }
}
int const_exp(struct node *T)
{
    int left = 0, right = 0;
    if (T == NULL)
        return 0;
    if (T->kind == LITERAL)
        return T->type_int;
    left = const_exp(T->ptr[0]);
    right = const_exp(T->ptr[1]);
    switch (T->kind)
    {
    case TOK_ADD:
        return left + right;
        break;
    case TOK_SUB:
        return left - right;
        break;
    case TOK_MUL:
        return left * right;
        break;
    case TOK_DIV:
        return left / right;
        break;
    case TOK_MODULO:
        return left % right;
        break;
    default:
        break;
    }
}

void push_initarray(struct node *T, struct node *newnode)
{
    struct node *temp = T;
    while (temp->ptr[1] != NULL)
    {
        temp = temp->ptr[1];
    }
    temp->ptr[1] = newnode;
}
void push_initvalue(int i, List *node)
{
    ListPushBack(node, (void *)(long long)i);
    return;
}
int find_initvalue_arr(int symbol_index, int offset)
{
    void *element;
    if (ListGetAt(symbolTable.symbols[symbol_index].value, offset, &element))
    {
        return (int)(long long)element;
    }
    else
        return 0;
}