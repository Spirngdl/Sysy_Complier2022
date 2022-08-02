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
            if (sym.flag == ARRAY) //可能是数组
            {
            }
            else if(sym.flag == VAR) //全局变量
            {
                // char *alias = sym.alias;//别名，三地址代码中所有变量名都是别名
                // int type = sym.type; //可能是TOK_INT TOK_FLOAT

            }
        }
    }
}