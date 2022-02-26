#include "def.h"

struct Block *newblock()
{
    struct Block *block = (struct Block *)malloc(sizeof(struct Block));
    block->next = NULL;
    block->pre = NULL;
    block->tac_list = NULL;
    return block;
}
struct Block *merge_block(struct Block *head, struct Block *newnode)
{
    head->next = newnode->next;
    newnode->pre = head;
}

struct Block *divide_block(struct codenode *head)
{
    struct Block *hblock = newblock();
    struct codenode *hcode = head;
    hcode->in = 1;
    while (hcode)
    {
        if (hcode->op == GOTO)
        {
            hcode->out = 1;
            hcode->next->in = 1;
            struct codenode *temp = head;
            while (temp)
            {
                if (temp->op == LABEL)
                    if (!strcmp(hcode->result.id, temp->result.id))
                    {
                        temp->in = 1;
                        temp->prior->out = 1;
                    }
                temp = temp->next;
            }
        }
        hcode = hcode->next;
    }
    hblock->tac_list = head;
    struct Block *tblcok = hblock;
    hcode = head;
    while (hcode)
    {
        if (hcode->out)
        {
            struct Block *temp = newblock();
            temp->tac_list = hcode->next;
            hcode->next = NULL;
            tblcok->next = temp;
            temp->pre = tblcok;
            tblcok = temp;
            hcode = temp->tac_list;
        }
        else
            hcode = hcode->next;    
    }
}