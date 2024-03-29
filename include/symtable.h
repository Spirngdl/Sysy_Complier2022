/**
 * @file symtable.h
 * @author your name (you@domain.com)
 * @brief 符号表头文件
 * @version 0.1
 * @date 2022-07-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SYMTABLE_H_
#define SYMTABLE_H_
#include "../Common/list.h"
#define MAXLENGTH 20000 //定义符号表的大小
struct symbol
{                   //这里只列出了一个符号表项的部分属性，没考虑属性间的互斥
    char name[33];  //变量或函数名
    int level;      //层号，外部变量名或函数名层号为0，形参名为1，每到1个复合语句层号加1，退出减1
    int kind;       //变量名，函数名，数组名
    int type;       //变量类型或函数返回值类型数
    int paramnum;   //形式参数个数
    char alias[20]; //别名，为解决嵌套层次使用，使得每一个数据名称唯一
    int flag;       //符号标记，函数：'F'  变量：'V'   参数：'P'  临时变量：'T'
    // int offset;          //外部变量和局部变量在其静态数据区或活动记录中的偏移量
    //或函数活动记录大小，目标代码生成时使用
    int length[15];      //数组每一维的长度
    int array_dimension; //数组维度
    List *value;         //保存全局变量的值，用来数组初始化 如果没有完全没有初始化，置为NULL
    float const_value;   //暂存临时的值
};

//考虑为数组专门开个符号表
typedef struct ArraySymbol_
{
    char name[33];       //数组名
    int level;           //层号
    int type;            //类型 int 或float
    char alias[10];      // 别名
    int length[15];      //数组每一维的长度
    int array_demension; //数组维度
    List *value;         //保存数组的值 如果没有初值，设置为NULL
    char func_name[32];  //所在函数的函数名
} arraysymbol;

// UID-标号
struct LabelUid
{
    int uid;
    char label[32];
};
//数组初始化的值
typedef struct ArrayValue_ ArrayValue;
struct ArrayValue_
{
    int kind; //可能是 LITERAL FLOAT_LITERAL ID NONE
    union
    {
        int v_int;
        float v_float;
        char var_name[32];
    };
};
//处理label和uid的对应关系
struct LabelUidTable
{
    struct LabelUid table[10000];
    int count;
} LabelTable;
//临时的符号表
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
//用来给数组的。
struct arraysymtalbe
{
    arraysymbol symbols[MAXLENGTH];
    int index;
} arrayTalbe;

struct symbol_scope_begin
{ /*当前作用域的符号在符号表的起始位置序号,这是一个栈结构，/每到达一个复合语句，将符号表的index值进栈，离开复合语句时，取其退栈值修改符号表的index值，完成删除该复合语句中的所有变量和临时变量*/
    int TX[30];
    int top;
} symbol_scope_TX;

//做一个记录所有变量的符号表
struct aliassym
{
    char name[33];         //别名
    enum yytokentype type; // TOK_INT TOK_FLOAT
    int value;             //主要是const
};
struct _aliastable
{
    struct aliassym table[MAXLENGTH];
    int index;
} AliasTable;
typedef struct _uidnode uidnode;
struct _uidnode
{
    char Label[32];
    int uid;
};
List *uidTable;
// FUNCITON
int searchSymbolTable(char *name);
int searchFuncTable(char *name); //用来查找函数的 目前只在函数调用时候查找一下

int search_alias(char *alias);   //通过别名查找符号表，一般是在后端调用了，因为三地址代码中存储的变量的ID就是别名
int search_func(char *fun_name); //查找函数名，返回参数个数
int search_all_alias(char *alias);
int fillast(char *name, char flag); //用来处理临时符号表
int fillSymbolTable(char *name, char *alias, int level, int type, int flag);
void AliasTableADD(char *alias, int type);
int fillSymbolTable_(char *name, char *alias, int level, int type, char flag, int offset);
int fillArrayTable(char *name, char *alias, int level, int type);
int fillFunctionTable(char *name, int type, int paramnum);
int get_array_infunc(char *func_name);
int filllabel(char *name, int uid);
// char *search_uid(int uid);
bool isLibraryfunctions(char *func_name);
char*uid_to_label(int uid);
#endif