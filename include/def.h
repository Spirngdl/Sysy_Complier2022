/**
 * @file def.h
 * @author 尹相豪 (you@domain.com)
 * @brief 定义语法树节点结构，三地址代码结构，枚举语法树节点类型，枚举三地址代码类型。但是有些类型是枚举在calc_c.tab.h中。
 * @version 0.1
 * @date 2022-07-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DEF_H_
#define DEF_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "calc_c.tab.h"
#include "block.h"
#include "liveness.h"
#include "reg.h"
#include "symtable.h"
#include "../Common/list.h"
#include "Optimize.h"
#include "../Common/hash_set.h"
// #define DD
enum node_kind
{
    TOK_LDR = 2022,
    VAR_DECL,
    VAR_DECL_LIST,
    COMPUNIT_LIST,
    CONSTDEF,
    CONSTDECL_LIST,

    CONSTINITVAL_LIST,
    INITVAL_LIST,
    INITVAL,
    PARAM_ARRAY,
    BLOCK,
    BLOCK_LIST,
    UNARYEXP,
    RELOP,
    TYPE,
    WHILE,
    STMT,
    DECL,

    ID,
    INT,
    CONST_INT,
    LITERAL,
    FLOAT_LITERAL,
    ARRAY_ASSIGN,
    INITARRAY,
    BACK,
    VAR,
    FUN,
    ARRAY,
    TEMP_VAR,
    EXT_DEF_LIST,
    EXT_VAR_DEF,
    FUNC_DEF,
    FUNC_DEC,

    EXP_LIST,
    EXP_ELE,
    EXP_ARRAY,
    ARRAY_DEC,
    EXT_DEC_LIST,
    PARAM_LIST,
    PARAM_DEC,
    VAR_DEF,
    DEC_LIST,
    DEF_LIST,
    COMP_STM,
    STM_LIST,
    EXP_STMT,
    IF_THEN,
    IF_THEN_ELSE,
    FUNC_CALL,
    ARRAY_EXP,
    ARGS,
    FUNCTION,
    PARAM,
    ARG,
    CALL,
    LABEL,
    GOTO,
    JLT,
    JLE,
    JGT,
    JGE,
    EQ,
    NEQ,
    NONE,
    END
};

#define MAXLENGTH 1000     //定义符号表的大小
#define DX 3 * sizeof(int) //活动记录控制信息需要的单元数

struct opn
{
    int kind; //标识操作的类型
    union
    {
        int const_int;      //整常数值，立即数
        double const_float; //浮点数
        char id[33];        //变量或临时变量的别名或标号字符串
        char alice[33];
    };
};

struct codenode
{                                  //三地址TAC代码结点,采用双向循环链表存放中间语言代码
    int op;                        // TAC代码的运算符种类
    struct opn opn1, opn2, result; // 2个操作数和运算结果
    struct codenode *next, *prior;

    int in;  //划分基本块
    int out; //划分基本块
    int UID; //编号
};

struct node
{
    enum node_kind kind; //结点类型
    int type;
    union
    {
        char type_id[33];  //由标识符生成的叶结点
        int type_int;      //由整常数生成的叶结点
        double type_float; //由浮点数生成的叶结点
    };
    struct node *ptr[3]; //子树指针，由kind确定有多少棵子树
    int level;           //层号
    int place;           //表示结点对应的变量或运算结果符号表的位置序号

    struct codenode *code; //该结点中间代码链表头指针
    int pos;               //语法单位所在位置行号
};

/*generate AST*/
struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos);
struct node *mkopnode(int kind, struct node *left, struct node *right, int pos);
struct node *mkarrnode(int kind, struct node *first, int length, int pos);
struct node *mkparray(int kind, char *first, struct node *len, int pos);

int const_exp(struct node *T);
/*semantic analysis*/
void semantic_error(int line, char *msg1, char *msg2);

void Exp(struct node *T);
void boolExp(struct node *T, char *Etrue, char *Efalse);
void semantic_Analysis(struct node *T);
void Driver(struct node *T);
int temp_add(char *name, int level, int type, int flag);
/*inner code generation*/
char *str_catch(char *s1, char *s2);
char *newAlias();
char *newLabel();
char *newTemp();
struct codenode *genIR(int op, struct opn opn1, struct opn opn2, struct opn result);
struct codenode *genLabel(char *label);
struct codenode *genGoto(char *label);
struct codenode *genback(int kind);
struct codenode *merge(int num, ...);
void print_IR(struct codenode *head);

void id_exp(struct node *T);
void int_exp(struct node *T);
void exp_array(struct node *T);
void assignop_exp(struct node *T);
void relop_exp(struct node *T);
void args_exp(struct node *T);
void op_exp(struct node *T);
void func_call_exp(struct node *T);
void not_exp(struct node *T);

void func_def(struct node *T);
void param_list(struct node *T);
void param_dec(struct node *T);
void param_array(struct node *T);

void var_decl_list(struct node *T);

void var_decl(struct node *T);
void array_decl(struct node *T);
int array_index(struct node *T, int i, int offset);    //生成数组下标
int exp_index(struct node *T, int index, int width[]); //处理数组引用的下标
int mul_exp(struct node *T, char *i, int offset);      //生成乘法语句
void else_if_stmt(struct node *T, char *Efalse);       // else if的情况且无else
void if_else_stmt(struct node *T);
void else_if_else_stmt(struct node *T, char *Enext);
void if_stmt(struct node *T);
void while_stmt(struct node *T);

void if_then(struct node *T);
void if_then_else(struct node *T);
void while_dec(struct node *T);

void return_dec(struct node *T);

void compunit_list(struct node *T);
void const_def_list(struct node *T);
void const_var_def(struct node *T);
void break_stmt(struct node *T);
void continue_stmt(struct node *T);
void unaryexp(struct node *T);
void unaryexp(struct node *T);
void block(struct node *T);
void block_list(struct node *T);
int match_param(int i, struct node *T);

void make_uid(struct codenode **head);
void make_uid_block(Blocks *block);
void change_label(struct codenode *head);

void rval_array(struct node *T);

void push_initvalue(int i, List *node);
int find_initvalue_arr(int symbol_index, int offset);
void arrayinit_bracker(List *value_list, struct node *T, int brace_num, int *array_offset, int width[], int dimension);

void test_array();
void push_initarray(struct node *T, struct node *newnode);




int check_imme(int imme);
void check_immes(Blocks *blocks);
#endif