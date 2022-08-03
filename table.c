#include "include/def.h"
// #define DU
int searchSymbolTable(char *name)
{
    int i;

    for (i = symbolTable.index - 1; i >= 0; i--)
    {
        if (0 == strcmp(symbolTable.symbols[i].name, name))
            return i;
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
    }
    return -1;
}
int searchFuncTable(char *name)
{
    int i = 0;
    for (i = symbolTable.index - 1; i >= 0; i--)
    {
        if (symbolTable.symbols[i].flag == FUNCTION)
            if (strcmp(symbolTable.symbols[i].name, name) == 0)
                return i;
    }
    return -1;
}
/**
 * @brief 在总的别名符号表里查找，主要找type
 *
 * @param alias
 * @return int
 */
int search_all_alias(char *alias)
{
    int i = 0;
    for (i = AliasTable.index - 1; i >= 0; i--)
    {
        if (strcmp(AliasTable.table[i].name, alias) == 0)
            return AliasTable.table[i].type;
    }
    return -1;
}
/**
 * @brief 填充所有别名符号表
 *
 * @param alias
 * @param type
 */
void AliasTableADD(char *alias, int type)
{
    AliasTable.table[AliasTable.index].type = type;
    strcpy(AliasTable.table[AliasTable.index].name, alias);
    AliasTable.index++;
}
/**
 * @brief  首先根据name查符号表，不能重复定义 重复定义返回-1
 *
 * @param name 名字
 * @param alias 别名
 * @param level 层级，全局变量和函数为0
 * @param type  TOK_INT TOK_FLOAT TOK_VOID
 * @param flag  FUNCTION
 * @return int 符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
 */
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
#ifdef DU
    strcpy(symbolTable.symbols[symbolTable.index].alias, name);
#else
    strcpy(symbolTable.symbols[symbolTable.index].alias, alias);
#endif
    symbolTable.symbols[symbolTable.index].level = level;
    symbolTable.symbols[symbolTable.index].type = type;
    symbolTable.symbols[symbolTable.index].flag = flag;
    AliasTableADD(alias, type);
    return symbolTable.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}
//首先根据name查符号表，不能重复定义 重复定义返回-1
int fillArrayTable(char *name, char *alias, int level, int type)
{
    //应该不用查重 暂时只做填表功能
    strcpy(arrayTalbe.symbols[arrayTalbe.index].name, name);
    arrayTalbe.symbols[arrayTalbe.index].level = level;
    arrayTalbe.symbols[arrayTalbe.index].type = type;
    arrayTalbe.symbols[arrayTalbe.index].value = NULL;
    return arrayTalbe.index++;
}
//添加函数
int fillFunctionTable(char *name, int type, int paramnum)
{
    int index = symbolTable.index;
    strcpy(symbolTable.symbols[index].name, name);
    symbolTable.symbols[index].level = 0;
    symbolTable.symbols[index].type = type;
    symbolTable.symbols[index].flag = FUNCTION;
    symbolTable.symbols[index].paramnum = paramnum;
    symbolTable.index++;
}
// 首先根据name查符号表，不能重复定义 重复定义返回-1
int fillast(char *name, char flag)
{
    //填写符号表内容
    strcpy(astsymbol.symbols[astsymbol.index].name, name);
    astsymbol.symbols[astsymbol.index].flag = flag;
    return astsymbol.index++; //返回的是符号在符号表中的位置序号，中间代码生成时可用序号取到符号别名
}
/**
 * @brief 添加label和uid
 *
 * @param name
 * @param uid
 * @return int
 */
int filllabel(char *name, int uid)
{
    strcpy(LabelTable.table[LabelTable.count].label, name);
    LabelTable.table[LabelTable.count].uid = uid;
    return LabelTable.count++;
}
