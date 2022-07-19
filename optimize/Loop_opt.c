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

#include "../include/Optimize.h"

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
 * @brief 在循环不变运算中，查看符号表中是否有
 *
 * @param table
 * @param name
 * @return int 失败返回-1
 */
int search_var_loop(var_table *table, char *name)
{
    int count = table->count;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(table->table[i].name, name))
        {
            return i;
        }
    }
    return -1;
}
//二分查找
int binarySearch(int a[], int n, int key)
{
    int low = 0;
    int high = n - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        int midVal = a[mid];
        if (midVal < key)
            low = mid + 1;
        else if (midVal > key)
            high = mid - 1;
        else
            return mid;
    }
    return -1;
}
/**
 * @brief 添加变量
 *
 * @param table
 * @param name
 * @param uid
 */
void add_var_loop(var_table *table, char *name, int uid)
{
    int i = search_var_loop(table, name);
    if (i == -1) //如果此前没有定值
    {
        strcpy(table->table[table->count].name, name);
        table->table[table->count].ass_uid = uid;
        table->table[table->count++].is_multi = false;
    }
    else //已经被定过值了
    {
        table->table[i].is_multi = true;
    }
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
        for (int j = 0; j < count; j++)
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
        for (int j = 0; j < count_next; j++)
        {
            next_list[j] = fun_block->block[i]->children[j]->id;
        }
        // next_list[0] = fun_block->block[i]->children[0]->id;
        // if (count_next == 2)
        //     next_list[1] = fun_block->block[i]->children[1]->id;
        //遍历dom集合 出问题了 麻了
        for (int j = 0; j < count_next; j++)
        {
            if (HashSetFind(dom_set[i], (void *)(intptr_t)next_list[j])) //如果该块的必经节点集有块在子节点中，则为回边
            {
                back_edge[count_edge][0] = next_list[j];
                back_edge[count_edge++][1] = i;
            }
        }
        // dom_set[i]->first(dom_set[i]);
        // while ((elem = dom_set[i]->next(dom_set[i])) != NULL)
        // {
        //     int *temp = (int *)elem;
        //     for (int j = 0; j < count_next; j++) //如果该块的必经节点集有块在子节点中，则为回边
        //     {
        //         if (*temp == next_list[j])
        //         {
        //             //排序一下
        //             if (*temp <= next_list[j])
        //             {
        //                 back_edge[count_edge][0] = *temp;
        //                 back_edge[count_edge][1] = next_list[j];
        //             }
        //             else
        //             {
        //                 back_edge[count_edge][0] = next_list[j];
        //                 back_edge[count_edge][1] = *temp;
        //             }
        //             count_edge++;
        //         }
        //     }
        // }
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
/**
 * @brief 检查loop中对x的使用是否都只能有本句话的定值到达。防止的是这种情况：
 * x=3;while(){y=x;...x=2;}
 * 这种情况你把x=2提外面了，那第一次的y=x就不对了
 * 所以check就从入口开始按图遍历loop，碰到x的def就返回，碰到x的use则返回false
 * @param Block 入口节点，可能是要新建的那个头节点
 * @return int
 */
int check3(Blocks *fun_block, List *loop, Block *enter, char *name)
{
    //从入口开始遍历，按图遍历
    HashSet *vis = HashSetInit(); // visited
    int stack[100];
    int top = 0;
    stack[top++] = enter->id;
    while (top != 0)
    {
        int x = stack[top--];
        if (HashSetFind(vis, (void *)(intptr_t)x) == true) //如果该块访问过
            continue;
        struct codenode *tcode = fun_block->block[x]->tac_list;
        while (tcode != NULL)
        {
            if (tcode->op == TOK_ASSIGN || tcode->op == TOK_ADD || tcode->op == TOK_SUB || tcode->op == TOK_MUL || tcode->op == TOK_DIV)
            {
                if (strcmp(name, tcode->result.id)) //遇到第一条def，是不是可以返回true了 不行，只能保证这个块可以了，要全部遍历完
                    break;
                if (tcode->opn1.kind == ID)
                {
                    if (strcmp(name, tcode->opn1.id)) //遇到use
                        return false;
                }
                if (tcode->opn2.kind == ID)
                {
                    if (strcmp(name, tcode->opn2.id))
                        return false;
                }
            }
        }
        HashSetAdd(vis, (void *)(intptr_t)x); //添加visited集合
        for (int i = 0; i < fun_block->block[x]->num_children; i++)
        {
            int y = fun_block->block[x]->children[i]->id;
            if (HashSetFind(vis, (void *)(intptr_t)y)) //如果访问过
                continue;
            stack[top++] = y;
        }
    }
    return true;
}
/**
 * @brief 查找循环不变运算
 * TODO: 要求每个操作数不能是全局变量（？？为啥）
 * @param fun_block
 * @param loop 单个循环体
 */
void find_invariants(Blocks *fun_block, List *loop, HashSet *dom_set[], int flag[], int base_id[])
{
    //先定义一个标记数组
    // int flag[1000];

    // memset(flag, 0, sizeof(flag));
    //整个简易符号表 其中记录了变量名和定值点的UID
    var_table *table = (var_table *)malloc(sizeof(var_table));
    table->count = 0;
    int blocks[100], count = 0, changed = 1, base_count = 0;
    // int base_id[100] = {0};
    struct codenode *hcode = NULL;
    void *elem;
    loop->first(loop, false);
    while (loop->next(loop, &elem))
    {
        blocks[count++] = (int)(intptr_t)elem;
    }
    //排序一下吧 count = size
    qsort(blocks, count, sizeof(int), comp);
    //先初步遍历一遍块 ,顺变记录一下各个变量的定值语句在哪
    for (int i = 0; i < count; i++)
    {
        //获取三地址代码
        hcode = fun_block->block[blocks[i]]->tac_list;
        base_id[i] = hcode->UID + base_count;
        //如果每个运算为常数 则标记
        while (hcode)
        {
            switch (hcode->op)
            {
            //如果是赋值
            case TOK_ASSIGN:
                if (hcode->opn1.kind == LITERAL || hcode->opn1.kind == FLOAT_LITERAL)
                {
                    flag[hcode->UID - base_id[i]] = 1; //标记
                }
                add_var_loop(table, hcode->result.id, hcode->UID - base_id[i]); //记录该变量的定值 相对UID
                break;
            //五个运算
            case TOK_ADD:
            case TOK_SUB:
            case TOK_MUL:
            case TOK_DIV:
            case TOK_MODULO:
                if ((hcode->opn1.kind == LITERAL || hcode->opn1.kind == FLOAT_LITERAL) && (hcode->opn2.kind == LITERAL || hcode->opn2.kind == FLOAT_LITERAL))
                {
                    flag[hcode->UID - base_id[i]] = 1;
                }
                add_var_loop(table, hcode->result.id, hcode->UID); //记录该变量的定值相对UID
                break;
                //两个数组
                // case ARRAY_ASSIGN: //右值
                // case ARRAY_EXP:    //下标
                //     if (hcode->opn2.kind == LITERAL || hcode->opn2.kind == FLOAT_LITERAL)
                //     {
                //         flag[hcode->UID - base_id[i]] = 1;
                //     }
                //     add_var_loop(table, hcode->result.id, hcode->UID); //记录该变量的定值相对UID
                break;
            default:
                break;
            }
            base_count++;
            hcode = hcode->next;
        }
    }
    //疯狂遍历，找到循环不变表达式
    while (changed)
    {
        changed = 0;
        for (int i = 0; i < count; i++)
        {
            hcode = fun_block->block[blocks[i]]->tac_list;
            while (hcode)
            {
                /**
                 * 运算对象是常数，或者定值点是常数，或者只有一个到达定值点并且该点的代码已经被标记
                 * 就是找opn1 和 opn2 如果opn1和opn2的赋值语句都在外面就标记，就是循环内没有他们的赋值
                 * opn1 和 opn2 的赋值只有一个且已经被标记
                 * 我的实现
                 * 找个容器，存变量的定值在哪 整个简易符号表吗
                 * 赋值在外面的  标记这个变量
                 * 赋值被标记的 标记这个变量
                 * 如果两个变量都被标记 标记这个代码
                 */
                if (flag[hcode->UID - base_id[i]] == 0) //如果未被标记
                {
                    int flag1 = 0, flag2 = 0;
                    int ass_uid = 0; //定值语句的uid
                    int index = 0;
                    switch (hcode->op)
                    {
                    case TOK_ASSIGN:
                        flag2 = 1;
                        if (hcode->opn1.kind == ID)
                        {
                            //查找定值语句
                            index = search_var_loop(table, hcode->opn1.id);
                            if (index == -1) //没找到，表示定值语句在外头
                                flag1 = 1;
                            else //找到了
                            {
                                if (table->table[index].is_multi == 0) //没有多次定值
                                {
                                    ass_uid = table->table[index].ass_uid;
                                    if (flag[ass_uid] == 1) //定值语句被标记
                                    {
                                        flag1 = 1;
                                    }
                                }
                            }
                        }
                        else if (hcode->opn1.kind == LITERAL || hcode->opn1.kind == FLOAT_LITERAL)
                            flag1 = 1;
                        if (flag1 + flag2 == 2)
                        {
                            flag[hcode->UID - base_id[i]] = 1; //标记这个语句
                            changed = 1;
                        }
                        break;
                        //五个运算
                    case TOK_ADD:
                    case TOK_SUB:
                    case TOK_MUL:
                    case TOK_DIV:
                    case TOK_MODULO:
                        //判断第一个
                        if (hcode->opn1.kind == ID)
                        {
                            //查找定值语句
                            index = search_var_loop(table, hcode->opn1.id);
                            if (index == -1) //没找到，表示定值语句在外头
                                flag1 = 1;
                            else //找到了
                            {
                                if (table->table[index].is_multi == 0) //没有多次定值
                                {
                                    ass_uid = table->table[index].ass_uid;
                                    if (flag[ass_uid] == 1) //定值语句被标记
                                    {
                                        flag1 = 1;
                                    }
                                }
                            }
                        }
                        else if (hcode->opn1.kind == LITERAL || hcode->opn1.kind == FLOAT_LITERAL)
                            flag1 = 1;
                        //判断第二个
                        if (hcode->opn2.kind == ID)
                        {
                            //查找定值语句
                            index = search_var_loop(table, hcode->opn2.id);
                            if (index == -1) //没找到，表示定值语句在外头
                                flag2 = 1;
                            else //找到了
                            {
                                if (table->table[index].is_multi == 0) //没有多次定值
                                {
                                    ass_uid = table->table[index].ass_uid;
                                    if (flag[ass_uid] == 1) //定值语句被标记
                                    {
                                        flag1 = 1;
                                    }
                                }
                            }
                        }
                        else if (hcode->opn2.kind == LITERAL || hcode->opn2.kind == FLOAT_LITERAL)
                            flag1 = 1;
                        //两个变量都被标记
                        if (flag1 + flag2 == 2)
                        {
                            flag[hcode->UID - base_id[i]] = 1; //标记这个语句
                            changed = 1;
                        }
                        break;
                    // case ARRAY_ASSIGN: //右值
                    // case ARRAY_EXP:    //下标
                    //     flag1 = 1;
                    //     if (hcode->opn2.kind == ID)
                    //     {
                    //         index = search_var_loop(table, hcode->opn2.id);
                    //         if (index == -1) //没找到
                    //             flag2 = 1;
                    //         else
                    //         {
                    //             if (table->table[index].is_multi == 0) //没有多次定值
                    //             {
                    //                 ass_uid = table->table[index].ass_uid;
                    //                 if (flag[ass_uid] == 1) //定值语句被标记
                    //                 {
                    //                     flag2 = 1;
                    //                 }
                    //             }
                    //         }
                    //     }
                    //     else if (hcode->opn2.kind == LITERAL || hcode->opn2.kind == FLOAT_LITERAL)
                    //     {
                    //         flag2 = 1;
                    //     }
                    //     if (flag1 + flag2 == 2)
                    //     {
                    //         flag[hcode->UID - base_id[i]] = 1; //标记这个语句
                    //         changed = 1;
                    //     }
                    default:
                        break;
                    }
                }
                hcode = hcode->next;
            }
        }
    }
    //判断能否进行外提
    /**
     * 满足三点
     * 1.这条语句所在的节点是所有出口的必经节点（应该只有一个出口吧）
     * 要找出口节点，
     * 2.result在循环中没有其他定值语句 (这个应该在前面做比较好吧)
     *  因为有可能存在一个是赋常数，一个是常数引用
     * 3.循环中所有对result的引用，只有这条语句中对于result的定值能够到达 (这个到达，怎么判，) 必经节点集吗，也不是，
     *      因为有可能循环外有对result的定值，
     * 我的实现
     * 第一个就拿必经节点集，查找一下 怎么找出口节点
     * 第二个,在前面也算做了吧，看看ismulti
     * 第三个，要找对result的引用，检查loop中对x的使用是否都只能有本句话的定值到达。防止的是这种情况：
     * x=3;while(){y=x;...x=2;}
     * 这种情况你把x=2提外面了，那第一次的y=x就不对了
     * 所以check就从入口开始按图遍历loop，碰到x的def就返回，碰到x的use则返回false
     * 还有一个机会就是result在离开循环后不再活跃，并且还是满足没有其他定值，所有对result的引用能够到达
     */
    //查找出口节点
    // 遍历所有块，看他们的子节点在不在循环内，有不在的就是出口节点
    int out[10], out_count = 0;
    for (int i = 0; i < count; i++)
    {
        int index = blocks[i];
        for (int j = 0; j < fun_block->block[index]->num_children; j++)
        {
            int children_id = fun_block->block[index]->children[j]->id;
            if (binarySearch(blocks, count, children_id) == -1) //如果有不在里面的
            {
                out[out_count++] = index;
                break;
            }
        }
    }
    //拿到所有出口节点的必经节点的交集
    HashSet *out_dom = NULL;
    if (out_count == 1)
    {
        out_dom = dom_set[out[0]];
    }
    else if (out_count == 2)
    {
        out_dom = HashSetIntersect(dom_set[out[0]], dom_set[out[1]]);
    }
    else
        for (int i = 1; i < out_count; i++)
        {
            out_dom = dom_set[out[0]];
            out_dom = HashSetIntersect(out_dom, dom_set[out[i]]);
        }

    //遍历所有被标记语句
    for (int i = 0; i < count; i++)
    {
        //获取三地址代码
        hcode = fun_block->block[blocks[i]]->tac_list;
        while (hcode)
        {
            if (flag[hcode->UID - base_id[i]] == 1) // 如果该语句被标记
            {
                int var_index = search_var_loop(table, hcode->result.id);
                //先看在不在出口节点的必经节点
                if (!HashSetFind(out_dom, (void *)(intptr_t)i))
                {                                      //如果不在
                    flag[hcode->UID - base_id[i]] = 0; //先改标记
                    // break;                             //为啥要break来着 我是想跳出这个if来着
                }
                //看下有没有重复定值
                else if (table->table[var_index].is_multi == 1)
                { //如果重复定值
                    flag[hcode->UID - base_id[i]] = 0;
                }
                //找对result的引用
                else if (check3(fun_block, loop, fun_block->block[blocks[0]], hcode->result.id) == false)
                {
                    flag[hcode->UID - base_id[i]] = 0;
                }
            }
            hcode = hcode->next;
        }
    }
}
//判断能否外提
void check_invariants(Blocks *fun_block, List *loop)
{
    /**
     * 满足三点
     * 1.这条语句所在的节点是所有出口的必经节点（应该只有一个出口吧）
     * 要找出口节点，
     * 2.result在循环中没有其他定值语句 (这个应该在前面做比较好吧)
     *  因为有可能存在一个是赋常数，一个是常数引用
     * 3.循环中所有对result的引用，只有这条语句中对于result的定值能够到达 (这个到达，怎么判，) 必经节点集吗，也不是，
     *      因为有可能循环外有对result的定值，
     * 我的实现
     * 第一个就拿必经节点集，查找一下 怎么找出口节点
     * 第二个,在前面也算做了吧，看看ismulti
     * 第三个，要找对result的引用，
     *
     */
    //查找出口节点
    // 遍历所有块，看他们的子节点在不在循环内，有不在的就是出口节点
    int out[10];
}

//进行外提
//标记数组，循环，fun_block
void Extrapolation(Blocks *fun_block, int flag[], int base_id[], List *loop, int enter)
{
    Block *new_block = newblock(); //新块
    struct codenode *cur_code = NULL;
    int block_loop[100], count = 0;
    void *elem;
    loop->first(loop, false);
    while (loop->next(loop, &elem))
    {
        block_loop[count++] = (int)(intptr_t)elem;
    }
    //遍历所有语句
    for (int i = 0; i < count; i++)
    {
        struct codenode *tcode = fun_block->block[block_loop[i]]->tac_list;
        while (tcode != NULL)
        {
            if (flag[tcode->UID - base_id[i]] == 1)
            {
                //如果是被标记的语句,进行外提
                if (tcode->prior)
                    if (tcode->prior->next)
                        tcode->prior->next = tcode->next;
                if (tcode->next)
                    tcode->next->prior = tcode->prior;
                tcode->next = tcode->prior = NULL;
                new_block->tac_list = merge(2, new_block->tac_list, tcode);
            }
            tcode = tcode->next;
        }
    }
    //插入新块
    if (new_block->tac_list != NULL)
    {
        Block *enter_block = fun_block->block[block_loop[enter]];
        for (int i = 0; i < enter_block->num_pre; i++)
        {
            Block *pre = fun_block->block[enter_block->pre_list[i]];
            new_block->pre_list[new_block->num_pre++] = pre->id;
            if (pre->num_children == 1)
            {
                pre->children[0] = new_block;
            }
            else if (pre->num_children == 2)
            {
                if (pre->children[0]->id == enter_block->id)
                {
                    pre->children[0] = new_block;
                }
                else
                {
                    pre->children[1] = new_block;
                }
            }
        }
        new_block->children[new_block->num_children++] = enter_block;
        enter_block->num_pre = 1;
        enter_block->pre_list[0] = new_block->id;
    }
}
//对所有块
/**
 * @brief 进行循环不变式外提
 *
 * @param blocks
 */
void invariant_Extrapolation(Blocks *blocks)
{
    Blocks *cur_blocks = blocks;
    while (cur_blocks)
    {
        int back_edge[100][2];
        HashSet *dom_set[100];                                          //所有块的必经节点集
        get_dom_node(cur_blocks, dom_set);                              //获取必经节点集
        int count_edge = get_back_edge(cur_blocks, dom_set, back_edge); //获取回边
        List *loops = get_loop(cur_blocks, back_edge, count_edge);
        void *elem = NULL;
        int index = 0;
        loops->first(loops, false);
        while (loops->next(loops, &elem))
        {
            List *loop = (List *)elem;
            int flag[1000];
            memset(flag, 0, sizeof(flag));
            int base_id[100];
            find_invariants(cur_blocks, loop, dom_set, flag, base_id);
            Extrapolation(cur_blocks, flag, base_id, loop, back_edge[index][0]);
            index++;
        }
        cur_blocks = cur_blocks->next;
    }
}