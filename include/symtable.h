#ifndef SYMTABLE_H_
#define SYMTABLE_H_
#include "list.h"
#define MAXLENGTH 1000 //定义符号表的大小
struct symbol
{                   //这里只列出了一个符号表项的部分属性，没考虑属性间的互斥
    char name[33];  //变量或函数名
    int level;      //层号，外部变量名或函数名层号为0，形参名为1，每到1个复合语句层号加1，退出减1
    int kind;       //变量名，函数名，数组名
    int type;       //变量类型或函数返回值类型数
    int paramnum;   //形式参数个数
    char alias[10]; //别名，为解决嵌套层次使用，使得每一个数据名称唯一
    int flag;       //符号标记，函数：'F'  变量：'V'   参数：'P'  临时变量：'T'
    // int offset;          //外部变量和局部变量在其静态数据区或活动记录中的偏移量
                            //或函数活动记录大小，目标代码生成时使用
    int length[10];      //数组每一维的长度
    int array_dimension; //数组维度
    List *value;         //保存全局变量的值，用来数组初始化
};
struct astsymboltable
{
    struct symbol symbols[MAXLENGTH];
    int index;
} astsymbol;
//符号表，是一个顺序栈，index初值为0
struct symboltable
{
    struct symbol symbols[MAXLENGTH];
    int index;
} symbolTable;

struct symbol_scope_begin
{ /*当前作用域的符号在符号表的起始位置序号,这是一个栈结构，/每到达一个复合语句，将符号表的index值进栈，离开复合语句时，取其退栈值修改符号表的index值，完成删除该复合语句中的所有变量和临时变量*/
    int TX[30];
    int top;
} symbol_scope_TX;
int searchSymbolTable(char *name);
int search_alias(char *alias);
int fillast(char *name, char flag); //用来处理临时符号表
int fillSymbolTable(char *name, char *alias, int level, int type, int flag);
int fillSymbolTable_(char *name, char *alias, int level, int type, char flag, int offset);

#endif