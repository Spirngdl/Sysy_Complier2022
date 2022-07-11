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

#endif