#include "include/def.h"
struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos)
{
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = kind;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    return T;
}
struct node *mkparray(int kind, char *name, struct node *len, int pos)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->kind = kind;
    strcpy(temp->type_id, name);
    int rtn = fillast(name, kind);
    temp->place = rtn;
    temp->pos = pos;
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
    if (left->kind == LITERAL && right->kind == LITERAL)
    {
        int lval = left->type_int, rval = right->type_int;
        T->kind = LITERAL;
        switch (kind)
        {
        case TOK_ADD:
            T->type_int = lval + rval;
            break;
        case TOK_SUB:
            T->type_int = lval - rval;
            break;
        case TOK_DIV:
            T->type_int = lval / rval;
            break;
        case TOK_MUL:
            T->type_int = lval * rval;
            break;
        case TOK_MODULO:
            T->type_int = lval % rval;
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
    ListPushBack(node, (void *)(__intptr_t)i);
}
int find_initvalue_arr(int symbol_index, int offset)
{
    void *element;
    if (ListGetAt(symbolTable.symbols[symbol_index].value, offset, &element))
    {
        return (int)(__intptr_t)element;
    }
    else
        return 0;
}