/**
 * @file Optimize.h
 * @author your name (you@domain.com)
 * @brief 优化
 * @version 0.1
 * @date 2022-07-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef OPTIMIZE_H_
#define OPTIMIZE_H_
#include "def.h"
#include "../Common/hash_set.h"
//常量传播
/**
 * @brief 通过循环，以基本块为基本单位进行优化
 *
 * @param head_fun
 */
void all_fun(Blocks *head_fun);
/**
 * @brief 寻找块内常量，逐个进行优化处理
 *
 * @param cntr
 */
void one_block(Block *cntr);
/**
 * @brief 对某一数组变量进行块内常量传播
 *
 * @param const_array
 */
void one_const_array(struct codenode *const_array);
/**
 * @brief 对某一普通变量进行块内常量传播
 *
 * @param const_var
 */
void one_const_var(struct codenode *const_var);

//循环优化
//简易符号表
typedef struct Var_Ass_
{
    char name[32]; //变量名
    int ass_uid;   //赋值的语句的UID
    bool is_multi; //是否有多个定值语句

} var_assign;
//表
typedef struct var_ass_table_
{
    var_assign table[1000];
    int count;
} var_table;
int search_var_loop(var_table *table, char *name);
void add_var_loop(var_table *table, char *name, int uid);

// TODO: 先找循环
//获取该基本块的前驱
//在划分基本块时实现
//获取该基本块的必经节点

void get_dom_node(Blocks *fun_block, HashSet *dom_set[]);
//获取回边
int get_back_edge(Blocks *fun_block, HashSet *dom_set[], int back_edge[][2]);
//找回边对应的自然循环
List *get_loop(Blocks *fun_block, int back_edge[][2], int count);
//找只有一个基本块的循环

//对单个循环体查找循环不变量
void find_invariants(Blocks *fun_block, List *loop, HashSet *dom_set[]);

#endif