/**
 * @file constant_propagation.c
 * @author your name (you@domain.com)
 * @brief 常量传播
 * @version 0.1
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../include/def.h"

// 常量传播优先于删除无用赋值，在传播的过程中会产生无用赋值
// 局部优化，仅处理 def、use 皆处于块内的变量
// 全局变量不作处理

// 对某一普通变量进行块内常量传播
void one_const_var (struct codenode* const_var) {
    struct codenode* p = const_var;
    while (p != NULL) {
        // 出现对于常量的再赋值，则跳出循环，A = 13 除外
        if (p->op == TOK_ASSIGN && p->result.id == const_var->result.id && p->opn1.const_int != const_var->opn1.const_int) {
            break;
        }
        if (p->opn1.id == const_var->result.id) {
            p->opn1.kind = LITERAL;
            p->opn1.const_int = const_var->opn1.const_int;
        }
        if (p->opn2.id == const_var->result.id) {
            p->opn2.kind = LITERAL;
            p->opn2.const_int = const_var->opn1.const_int;
        }
        // 若出现 B = 13 + 14，则进行计算
        if (p->opn1.kind == LITERAL && p->opn2.kind == LITERAL) {
            int temp = 0;
            switch (p->op) {
                case TOK_ADD:
                    temp = p->opn1.const_int + p->opn2.const_int;
                    p->op = TOK_ASSIGN;
                    p->opn1.const_int = temp;
                    break;
                case TOK_MUL:
                    temp = p->opn1.const_int * p->opn2.const_int;
                    p->op = TOK_ASSIGN;
                    p->opn1.const_int = temp;
                    break;
                case TOK_SUB:
                    temp = p->opn1.const_int - p->opn2.const_int;
                    p->op = TOK_ASSIGN;
                    p->opn1.const_int = temp;
                    break;
                case TOK_DIV:
                    temp = p->opn1.const_int / p->opn2.const_int;
                    p->op = TOK_ASSIGN;
                    p->opn1.const_int = temp;
            }
        }
        p = p->next;
    }
}

// 对某一数组变量进行块内常量传播
void one_const_array (struct codenode* const_array) {
    struct codenode* p = const_array;
    while (p != NULL) {
        if (p->op == ARRAY_ASSIGN && p->opn1.const_int == const_array->opn1.const_int && p->opn2.const_int != const_array->opn2.const_int) {
            break;
        }
        // 数组引用有且仅有ARRAY_EXP指令
        if (p->op == ARRAY_EXP && p->opn1.id == const_array->result.id && p->opn2.const_int == const_array->opn1.const_int) {
            p->op = TOK_ASSIGN;
            p->opn1.kind = LITERAL;
            p->opn1.const_int = const_array->opn2.const_int;
        }
        p = p->next;
    }
}

// 寻找块内常量，逐个进行优化处理
void one_block (Block* cntr) {
    int i = 0;
    struct codenode* p = cntr->tac_list;
    // 寻找诸如 A = 13 的表达式
    while (p != NULL) {
        // 赋值语句 + 左边为变量 + 右边为常数 + 非全局变量
        if (p->op == TOK_ASSIGN && p->result.kind == ID && p->opn1.kind == LITERAL && search_alias(p->result.id) == -1) {
            // 调用函数对于该常量进行优化处理
            one_const_var(p);
        }
        // 数组赋值
        if (p->op == ARRAY_ASSIGN && p->opn2.kind == LITERAL && search_alias(p->result.id) == -1) {
            one_const_array(p);
        }
        p = p->next;
    }
}

// 通过循环，以基本块为基本单位进行优化
void all_fun (Blocks* head_fun) {
    int i = 0;
    Blocks* p = head_fun;
    // 循环处理所有函数
    while (p != NULL) {
        // 循环处理函数内基本块
        for (i = 0; i < p->count; i++) {
            one_block(p->block[i]);
        }
        p = p->next;
    }
}