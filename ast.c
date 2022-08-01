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
#include <math.h>
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
/**
 * @brief 处理加减乘除运算节点，如果可以计算则直接计算出结果。但是要考虑隐式转换
 *
 * @param kind
 * @param left
 * @param right
 * @param pos
 * @return struct node*
 */
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
 * @brief 创建const节点，提前把值存到符号表中
 *
 * @param kind
 * @param first
 * @param pos
 * @return struct node*
 */
struct node *mkconstnode(int kind, struct node *first, int pos, char *name)
{
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = kind;
    T->ptr[0] = first;
    T->ptr[1] = NULL;
    T->ptr[2] = NULL;
    strcpy(T->type_id, name);
    T->pos = pos;
    T->code = NULL;
    float value = first->type_float;
    int place = fillSymbolTable(name, newAlias(), 0, TOK_INT, VAR); //先临时把值放进符号表
    symbolTable.symbols[place].const_value = value;                 //暂时用array_dimension存值
    return T;
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
float const_exp(struct node *T)
{
    float left = 0, right = 0;
    if (T == NULL)
        return 0;
    if (T->kind == LITERAL)
        return T->type_int;
    else if (T->kind == FLOAT_LITERAL)
        return T->type_float;
    if (T->kind == ID) //如果是变量，只能是CONST 去符号表中取
    {
        int place = searchSymbolTable(T->type_id);
        if (place == -1)
        {
            printf("error const_exp\n");
            return 0;
        }
        return symbolTable.symbols[place].const_value;
    }
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
        return (int)left % (int)right;
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
/**
 * @brief 实现16进制的字符串转10进制float
 *
 * @param str 0xf.ffpf
 * @return float
 */
float hex_atof(char *str)
{
    float result = 0;
    int len = strlen(str);
    int index = 0;
    int p = 0;
    while (str[index] == ' ')
    {
        index++;
    }
    index += 2;     //忽略0x
    int inter[30];  //整数部分
    int floter[30]; //小数部分
    int pter[10];
    int i = 0, j = 0, k = 0;
    //整数部分
    for (; index < len; index++)
    {
        if (str[index] == '.')
        {
            index++;
            break;
        }
        if (str[index] <= '9')
            inter[i] = str[index] - '0';
        else if (str[index] <= 'F')
            inter[i] = str[index] - 'A';
        else if (str[index] <= 'f')
            inter[i] = str[index] - 'a';
        i++;
    }
    //小数部分
    for (; index < len; index++)
    {
        if (str[index] == 'p' || str[index] == 'P')
        {
            index++;
            break;
        }
        if (str[index] <= '9')
            floter[j] = str[index] - '0';
        else if (str[index] <= 'F')
            floter[j] = str[index] - 'A';
        else if (str[index] <= 'f')
            floter[j] = str[index] - 'a';
        j++;
    }
    //
    for (; index < len; index++)
    {
        pter[k++] = str[index] - '0';
    }
    int ad = 1;
    i -= 1, j--, k--;
    for (int c = i; c >= 0; c--)
    {
        result += inter[c] * ad;
        ad *= 16;
    }
    ad = 16;
    for (int c = 0; c <= j; c++)
    {
        result += (float)((float)floter[c] / ad);
        ad *= 16;
    }
    ad = 1;
    for (int c = k; c >= 0; c--)
    {
        p += pter[c] * ad;
        ad *= 10;
    }
    ad = pow(2, p);
    result *= ad;
    return result;
}
void add_function(char *name, int type, int paramnum)
{
}
/**
 * @brief 手动添加运行库函数
 *
 */
void add_functions()
{
    // int getint()
    fillFunctionTable("getint", TOK_INT, 0);
    // int getch()
    fillFunctionTable("getch", TOK_INT, 0);
    // float getfloat()
    fillFunctionTable("getfloat", TOK_FLOAT, 0);
    // int getarray(int [])
    fillSymbolTable("arr", "", 1, TOK_INT, PARAM_ARRAY);
    fillFunctionTable("getarray", TOK_INT, 1);
    // int getfarray(float[])
    fillSymbolTable("arr", "", 1, TOK_FLOAT, PARAM_ARRAY);
    fillFunctionTable("getfarray", TOK_INT, 1);
    // void putint(int)
    fillSymbolTable("i", "", 1, TOK_INT, PARAM);
    fillFunctionTable("putint", TOK_VOID, 1);
    // void putch(int)
    fillSymbolTable("i", "", 1, TOK_INT, PARAM);
    fillFunctionTable("putch", TOK_VOID, 1);
    // void putfloat(float)
    fillSymbolTable("i", "", 1, TOK_FLOAT, PARAM);
    fillFunctionTable("putfloat", TOK_VOID, 1);
    // void putarray(int ,int[])
    fillSymbolTable("i", "", 1, TOK_INT, PARAM);
    fillSymbolTable("arr", "", 1, TOK_INT, PARAM_ARRAY);
    fillFunctionTable("putarray", TOK_VOID, 2);
    // void putfarray(int ,float[])
    fillSymbolTable("i", "", 1, TOK_INT, PARAM);
    fillSymbolTable("ARRAY", "", 1, TOK_FLOAT, PARAM_ARRAY);
    fillFunctionTable("putfarray", TOK_VOID, 2);
    // void putf() 这个比较特殊
    // void starttime()
    fillFunctionTable("starttime", TOK_VOID, 0);
    // void stoptime()
    fillFunctionTable("stoptime", TOK_VOID, 0);
}