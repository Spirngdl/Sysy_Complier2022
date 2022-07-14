/**
 * @file Loop_opt.c
 * @author your name (you@domain.com)
 * @brief 做循环优化
 * @version 0.1
 * @date 2022-07-12
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../include/def.h"
/**
 * @brief 查找List中是否有常数value
 *
 * @param list
 * @param vlaue
 * @return true
 * @return false
 */
bool find_literal(List *list, int vlaue)
{
    void *element;
    list->first(list, false);
    while (list->next(list, &element))
    {
        if (vlaue == *(int *)element)
            return true;
    }
    return false;
}
int comp(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}
/**
 * @brief 获取必经节点要对该函数内所有基本块遍历，然后拿到的是所有基本块的必经节点集合
 *
 * @param fun_block
 */
void get_dom_node(Blocks *fun_block, HashSet *dom_set[])
{
    int count = fun_block->count;
    int changed = 1;
    //初始化
    dom_set[0] = HashSetInit();
    HashSetAdd(dom_set[0], (void *)(intptr_t)0); // 0块集合初始化为{0}
    for (int i = 1; i < count; i++)
    {
        dom_set[i] = HashSetInit();
        for (int j = 0; i < count; j++)
        {
            HashSetAdd(dom_set[i], (void *)(intptr_t)j); //其余初始为{0,...,i}
        }
    }
    while (changed)
    {
        changed = 0;
        for (int i = 1; i < count; i++)
        {
            HashSet *inter = NULL;
            int num_pre = fun_block->block[i]->num_pre;
            if (num_pre == 1) //如果只有一个前驱，不用求交集 但是应该也不能直接把指针赋过来，需要复制
            {
                inter = HashSetInit();
                inter = HashSetUnion(inter, dom_set[fun_block->block[i]->pre_list[0]]); //暂时用并集操作进行复制
            }
            else if (num_pre == 2)
            {
                inter = HashSetIntersect(dom_set[fun_block->block[i]->pre_list[0]], dom_set[fun_block->block[i]->pre_list[1]]);
            }
            else
            {
                inter = dom_set[fun_block->block[i]->pre_list[0]];
                for (int j = 1; j < num_pre; j++)
                {
                    inter = HashSetIntersect(inter, dom_set[fun_block->block[i]->pre_list[j]]);
                }
            }
            //并上本体
            HashSetAdd(inter, (void *)(intptr_t)i);
            if (HashSetSize(dom_set[i]) != HashSetSize(inter)) //姑且考虑用大小来判断是否发生变化
                changed = 1;
            //改变必经节点集数组
            HashSetDeinit(dom_set[i]);
            dom_set[i] = inter;
        }
    }
}
/**
 * @brief 获取回边
 *
 * @param fun_block
 * @param dom_set
 * @param back_edge
 * @return int
 */
int get_back_edge(Blocks *fun_block, HashSet *dom_set[], int back_edge[][2])
{
    int count = fun_block->count;
    int count_edge = 0;
    int next_list[2], count_next = 0;
    void *elem = NULL;
    for (int i = 0; i < count; i++)
    {
        count_next = fun_block->block[i]->num_children;
        next_list[0] = fun_block->block[i]->children[0]->id;
        if (count_next == 2)
            next_list[1] = fun_block->block[i]->children[1]->id;
        //遍历dom集合
        dom_set[i]->first(dom_set[i]);
        while ((elem = dom_set[i]->next(dom_set[i])) != NULL)
        {
            int *temp = (int *)elem;
            for (int j = 0; j < count_next; j++) //如果该块的必经节点集有块在子节点中，则为回边
            {
                if (*temp == next_list[j])
                {
                    //排序一下
                    if (*temp <= next_list[j])
                    {
                        back_edge[count_edge][0] = *temp;
                        back_edge[count_edge][1] = next_list[j];
                    }
                    else
                    {
                        back_edge[count_edge][0] = next_list[j];
                        back_edge[count_edge][1] = *temp;
                    }
                    count_edge++;
                }
            }
        }
    }
    return count_edge; //返回回边数量
}

/**
 * @brief 寻找回边对应循环
 *        回边数量对应了循环的数量
 * @param fun_block
 * @param back_edge
 * @param count
 * @return List*
 */
List *get_loop(Blocks *fun_block, int back_edge[][2], int count)
{
    List *result = ListInit();
    for (int i = 0; i < count; i++) //遍历所有回边
    {
        List *loop = ListInit();
        if (back_edge[i][0] == back_edge[i][1]) //如果只有一个基本块
        {
            loop->push_back(loop, (void *)(intptr_t)back_edge[i][0]);
        }
        else
        {
            Block *block = fun_block->block[i];
            //设计一个栈
            int stack[20];
            int top = 0, m;
            stack[top++] = back_edge[i][1];
            //先把回边加进来
            loop->push_back(loop, (void *)(intptr_t)back_edge[i][0]);
            loop->push_back(loop, (void *)(intptr_t)back_edge[i][1]);
            while (top > 0)
            {
                m = stack[top--]; //出栈
                for (int j = 0; j < block->num_pre; j++)
                {
                    int p = block->pre_list[j];
                    if (!find_literal(loop, p)) //如果没找到
                    {
                        loop->push_back(loop, (void *)(intptr_t)p);
                        stack[top++] = p; //压栈
                    }
                }
            }
            result->push_back(result, (void *)loop);
        }
    }
    return result;
}

void find_invariants(Blocks *fun_block, List *loop)
{
    int flag[100]; //先定义一个标记数组
    memset(flag, 0, sizeof(flag));
    int blocks[100], count = 0, changed = 1;
    struct codenode *hcode = NULL;
    void *elem;
    loop->first(loop, false);
    while (loop->next(loop, &elem))
    {
        blocks[count++] = *(int *)elem;
    }
    //排序一下吧
    qsort(blocks, count, sizeof(int), comp);
    //先初步遍历一遍块
    for (int i = 0; i < count; i++)
    {
        //获取三地址代码
        hcode = fun_block->block[blocks[i]]->tac_list;
        int base_id = hcode->UID;
        //如果每个运算为常数 则标记
        while (hcode)
        {
            switch (hcode->op)
            {
            //如果是赋值
            case TOK_ASSIGN:
                if (hcode->opn1.kind == LITERAL || hcode->opn1.kind == FLOAT_LITERAL)
                {
                    flag[hcode->UID - base_id] = 1; //标记
                }
                break;
            //五个运算
            case TOK_ADD:
            case TOK_SUB:
            case TOK_MUL:
            case TOK_DIV:
            case TOK_MODULO:
                if ((hcode->opn1.kind == LITERAL || hcode->opn1.kind == FLOAT_LITERAL) && (hcode->opn2.kind == LITERAL || hcode->opn2.kind == FLOAT_LITERAL))
                {
                    flag[hcode->UID - base_id] = 1;
                }

                break;
            //两个数组
            case ARRAY_ASSIGN: //右值
            case ARRAY_EXP:    //下标
                if (hcode->opn2.kind == LITERAL || hcode->opn2.kind == FLOAT_LITERAL)
                {
                    flag[hcode->UID - base_id] = 1;
                }
                break;
            default:
                break;
            }
            hcode = hcode->next;
        }
    }
    while (changed)
    {
        for (int i = 0; i < count; i++)
        {
            hcode = fun_block->block[blocks[i]]->tac_list;
            while (hcode)
            {
                //运算对象是常数，或者定值点是常数，或者只有一个到达定值点并且该点的代码已经被标记
                //就是找opn1 和 opn2 如果opn1和opn2的赋值语句都在外面就标记，就是循环内没有他们的赋值
                //opn1 和 opn2 的赋值已经被标记
                //
                hcode = hcode->next;
            }
        }
    }
}