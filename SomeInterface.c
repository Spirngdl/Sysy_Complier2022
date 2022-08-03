/**
 * @file SomeInterface.c
 * @author your name (you@domain.com)
 * @brief 提供对外的接口
 * @version 0.1
 * @date 2022-08-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "./include/def.h"

/**
 * @brief 返回函数形式参数个数
 *
 * @param fun_name
 * @return int
 */
int search_func(char *fun_name)
{
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--)
    {
        if (symbolTable.symbols[i].flag == FUNCTION)
            if (strcmp(symbolTable.symbols[i].name, fun_name) == 0)
                return symbolTable.symbols[i].paramnum;
    }
}

/**
 * @brief 获取当前函数内所有数组的大小
 *
 * @param func_name 函数名
 * @return int
 */
int get_array_infunc(char *func_name)
{
    int result = 0;
    for (int i = arrayTalbe.index - 1; i >= 0; i--)
    {
        if (strcmp(func_name, arrayTalbe.symbols[i].func_name) == 0)
        {
            int width = 1;
            for (int j = arrayTalbe.symbols[i].array_demension - 1; j >= 0; j--)
            {
                width *= arrayTalbe.symbols[i].length[j];
            }
            result += width;
        }
    }
    return result;
}

/**
 * @brief 通过uid查询label
 *
 * @param uid
 * @return
 */
char *search_uid(int uid)
{
    int i;
    for (i = LabelTable.count - 1; i >= 0; i--)
    {
        if (LabelTable.table[i].uid == uid)
        {
            return LabelTable.table[i].label;
        }
    }
    return NULL;
}
/**
 * @brief 遍历所有三地址代码，查找超过四个的参数的第一次出现的位置
 *
 * @param head
 */
void insert_param(Blocks *head)
{
}
/**
 * @brief 获取所有全局变量
 *
 */
void search_global_var()
{
    int i;
    for (int i = symbolTable.index - 1; i >= 0; i--)
    {
        if (symbolTable.symbols[i].flag != FUNCTION && symbolTable.symbols[i].level == 0) //如果不是函数且层级为0，即是全局变量
        {
            //有可能是数组
            struct symbol sym = symbolTable.symbols[i];
            if (sym.flag == ARRAY || sym.flag == CONST_ARRAY) //可能是数组
            {
            }
            else if (sym.flag == VAR || sym.flag == CONST_VAR) //全局变量 全局CONST变量
            {
                // char *alias = sym.alias;//别名，三地址代码中所有变量名都是别名
                // int type = sym.type; //可能是TOK_INT TOK_FLOAT
            }
        }
    }
}
/**
 * @brief 遍历数组的初值链表,是按顺序存的
 *
 * @param value_list
 */
void traverse_List(List *value_list)
{
    void *element = NULL;
    ListFirst(value_list, false); //重置iterator
    while (ListNext(value_list, &element))
    {
        ArrayValue *value = (ArrayValue *)element;
        if (value->kind == LITERAL) //整型常量
        {
        }
        else if (value->kind == FLOAT_LITERAL) //浮点型常量
        {
        }
        else if (value->kind == ID) //可以显式的写出来
        {
        }
        else if (value->kind == NONE) //没法存在符号表里，比如说数组，所以我会显式的写出来
        {
        }
    }
}
/**
 * @brief 通过别名查找符号表，一般是在后端调用了，因为三地址代码中存储的变量的ID就是别名
 *
 * @param alias 别名
 * @return 全局变量返回i 其他返回-1
 */
int search_alias(char *alias)
{
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--)
    {
        if (strcmp(symbolTable.symbols[i].alias, alias) == 0) //找到了，可能是全局变量或者形式参数
        {
            if (symbolTable.symbols[i].level == 0) //是全局变量
                return i;
            else
                return -1;
        }
    }
    return -1;
}
/**
 * @brief 通过指针变化将浮点数转换为IEEE754
 *
 * @param i
 * @return int
 */
int ToIeee754(float i)
{
    return *(int *)&i;
}