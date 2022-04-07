#include "include/def.h"

Block *newblock() //创建新块
{
    Block *block = (Block *)malloc(sizeof(Block));
    block->num_children = 0;
    block->tac_list = NULL;
    return block;
}
void newBlocks(char *name)
{
    Blocks *temp = (Blocks *)malloc(sizeof(Blocks));
    strcpy(temp->name, name);
    temp->count = 0;
    temp->next = temp->pre = NULL;
    if (current_block == NULL)
    {
        current_block = temp;
    }
    else
    {
        current_block->next = temp;
        temp->pre = current_block;
        current_block = temp;
    }
}
void fillblock(Block *node) //添加块
{
    current_block->block[current_block->count] = node;
    current_block->count++;
}

void divide_block(struct codenode *head)
{
    Block *hblock = newblock();
    struct codenode *hcode = head;
    int id = 0;
    hcode->in = 1; //第一句是in
    // hcode->prior = 1;//最后一句是out
    while (hcode) //判断in和out
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
                    temp->prior->out = 1; //上一句为out
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
    while (hcode) //开始分块
    {
        if (hcode->out) //遇到out,可能是跳转，条件跳转，目标语句的上一句
        {
            if (hcode->op == JLE || hcode->op == JLT || hcode->op == JGE || hcode->op == JGT || hcode->op == EQ || hcode->op == NEQ) //遇到条件跳转语句，下一句必定是goto
            {
                hcode = hcode->next; //把下一句加进来
            }
            Block *temp = newblock();
            temp->tac_list = hcode->next; //遇到新的in创建新块
            temp->id = id++;
            fillblock(temp);
            hcode->next = NULL; //断开
            hcode = temp->tac_list;
        }
        else
            hcode = hcode->next; //
    }
}
void merge_block(Block *fa, Block *ch)
{
    if (ch != NULL)
    {
        fa->children[fa->num_children] = ch;
        fa->num_children++;
    }
}
Block *find_target(int target)
{
    for (int i = 0; i < current_block->count; i++)
    {
        if (current_block->block[i]->tac_list->UID == target)
        {
            return current_block->block[i];
        }
    }
    return NULL;
}
void link_block() //到最后一句，找跳转目标
{
    int i = 0;
    Block *temp;
    struct codenode *head;
    Block *tar;
    while (i < current_block->count) //遍历每个基本块
    {
        temp = current_block->block[i];
        head = temp->tac_list;
        i++;
        while (head->next)
        {
            head = head->next;
        }
        if (head->prior->op == JLE || head->prior->op == JLT || head->prior->op == JGE || head->prior->op == JGT || head->prior->op == EQ || head->prior->op == NEQ) //如果前一句是条件跳转
        {
            tar = find_target(head->prior->result.const_int);
            merge_block(temp, tar);
            tar = find_target(head->result.const_int);
            merge_block(temp, tar);
        }
        else if (head->op == GOTO)
        {
            tar = find_target(head->result.const_int);
            merge_block(temp, tar);
        }
        else //某个跳转语句目标语句的上一句
        {
            if (i != current_block->count - 1) //不是最后一个块
            {
                tar = current_block->block[i]; //直接连接下一个块
                merge_block(temp, tar);
            }
        }
    }
}
void basic_block(struct codenode *head)
{
    current_block = NULL;
    Block *globel = (Block *)malloc(sizeof(Block));
    struct codenode *code = head;
    while (code)
    {
        if (code->op == FUNCTION) //遇到函数
        {
            struct codenode *hcode = code;
            while (code->op != END)
            {
                code = code->next;
            }
            struct codenode *lcode = code;
            code = code->next;
            lcode->next = NULL;          //断开
            newBlocks(hcode->result.id); //为该函数创建新块
            divide_block(hcode);         //划分基本块
            link_block();                //链接
            continue;
        }
        else //全局变量
        {
            struct codenode *hcode = code;
            while (code != NULL && code->next->op != FUNCTION)
            {
                code = code->next;
            }
            struct codenode *lcode = code;
            if (code != NULL)
            {
                code = code->next;
                lcode->next = NULL;
            }
            globel->tac_list = hcode;
            newBlocks("globel");              //为全局变量创建个块
            current_block->block[0] = globel; //把三地址代码存进去
            continue;
        }
    }
    while (current_block->pre) //回到头指针
    {
        current_block = current_block->pre;
    }
}