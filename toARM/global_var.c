#include "../include/def.h"

armcode * search_global_var()
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

                armcode * newnode = initnewnode();

            }
        }
    }
}