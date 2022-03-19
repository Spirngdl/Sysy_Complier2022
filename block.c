#include "def.h"

struct Block *newblock() //创建新块
{
    struct Block *block = (struct Block *)malloc(sizeof(struct Block));

    block->tac_list = NULL;
    return block;
}
void fillblock(struct Block *node) //添加块
{
    blocks.block[blocks.index] = node;
    blocks.index++;
}

struct Block *divide_block(struct codenode *head)
{
    struct Block *hblock = newblock();
    struct codenode *hcode = head;
    int id = 0;
    hcode->in = 1; //第一句是in
    while (hcode)
    {
        if (hcode->op == GOTO || hcode->op == JLE || hcode->op == JLT || hcode->op == JGE || hcode->op == JGT || hcode->op == EQ || hcode->op == NEQ)
        {
            hcode->out = 1;      // 条件跳转语句是out
            hcode->next->in = 1; //下一句为in
            struct codenode *temp = head;
            while (temp)
            {
                if (temp->UID == hcode->result.const_int) //跳转目标语句
                {
                    temp->in = 1;
                    temp->prior->out = 1;
                }
                temp = temp->next;
            }
        }
        hcode = hcode->next;
    }
    hblock->id = id++; //基本块id
    hblock->tac_list = head;
    fillblock(hblock);
    hcode = head;
    while (hcode)
    {
        if (hcode->out)
        {
            struct Block *temp = newblock();
            temp->id = id++;
            temp->tac_list = hcode->next; //遇到新的in创建新块
            fillblock(temp);
            hcode->next = NULL; //断开
            hcode = temp->tac_list;
        }
        else
            hcode = hcode->next;
    }
}
