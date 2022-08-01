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