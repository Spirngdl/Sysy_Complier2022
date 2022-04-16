#include "include/def.h"

int searchSymbolTable(char *name)
{
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--)
    {
        if (symbolTable.symbols[i].flag == FUNCTION)
        {
            if (symbolTable.symbols[i - 1].level == 1) //上一个函数的形参
            {
                for (i = i - 1; i >= 0; i--)
                {
                    if (symbolTable.symbols[i].flag == FUNCTION) //跳出上个函数的形参
                        break;
                }
            }
        }
        if (0 == strcmp(symbolTable.symbols[i].name, name))
            return i;
    }
    return -1;
}
int search_alias(char *alias)
{
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--)
    {
        if (strcmp(symbolTable.symbols[i].alias, alias) == 0)
        {
            return i;
        }
    }
    return -1;
}
// 首先根据name查符号表，不能重复定义 重复定义返回-1
int fillSymbolTable(char *name, char *alias, int level, int type, int flag)
{
    int i;
    /*符号查重，考虑外部变量声明前有函数定义，
    其形参名还在符号表中，这时的外部变量与前函数的形参重名是允许的*/
    for (i = symbolTable.index - 1; symbolTable.symbols[i].level == level || (level == 0 && i >= 0); i--)
    {
        if (level == 0 && symbolTable.symbols[i].level == 1)
            continue; //外部变量和形参不必比较重名
        if (!strcmp(symbolTable.symbols[i].name, name))
            return -1;
    }
    //填写符号表内容
    strcpy(symbolTable.symbols[symbolTable.index].name, name);
    strcpy(symbolTable.symbols[symbolTable.index].alias, alias);
    symbolTable.symbols[symbolTable.index].level = level;
    symbolTable.symbols[symbolTable.index].type = type;
    symbolTable.symbols[symbolTable.index].flag = flag;
    return symbolTable.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}
// 首先根据name查符号表，不能重复定义 重复定义返回-1
int fillast(char *name, char flag)
{
    //填写符号表内容
    strcpy(astsymbol.symbols[astsymbol.index].name, name);
    astsymbol.symbols[astsymbol.index].flag = flag;
    return astsymbol.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}
