#include "../include/def.h"
#include <stdio.h>

int vartable_insert(vartable* vartable,char *varname,indexkind kind,int index)    //成功返回下标，-1失败
{
    char * tmp;
    int mnt = vartable->nodecmt;
    if(varname == NULL)
    {
        printf("varname can`t be null\n");
        return -1;
    }
    if((tmp=strcpy(vartable->table[mnt].name,varname))==NULL)
    {
        printf("strcpy varname %s error\n",varname);
        return -1;
    }
    vartable->table[mnt].kind = kind;
    vartable->table[mnt].index = index;

    vartable->nodecmt +=1; 
    return mnt;
}

int vartable_select(vartable* vartable,char *varname)                              //成功返回下标，-1失败
{
    int tmp;
    for(tmp =0;tmp<vartable->nodecmt;tmp++)
    {
        if(strcmp(vartable->table[tmp].name,varname) == 0)
            break;
    }
    if(tmp == vartable->nodecmt)
    {
        return -1;
    }
    return tmp;
}

/**
 * @brief 若varname是一个寄存器变量，则将表中变量的寄存器号修改为对应的index
 *         若varname在栈中，则将该变量相对于栈的偏移量加上栈指针的改变量index
 * 
 * @param vartable 
 * @param varname 
 * @param kind 
 * @param index 
 * @return 成功返回table中下标，-1失败
 */
int vartable_update(vartable* vartable,char *varname,indexkind kind,int index)   
{
    int tmp;
    if((tmp=vartable_select(vartable,varname)) == -1)
    {
        return -1;
    }
    if(vartable->table[tmp].kind == regindex)
    {
        vartable->table[tmp].index = index;
    }
    if(vartable->table[tmp].kind == memindex)
    {
        vartable->table[tmp].index += index;
    }
    return tmp;
}

/**
 * @brief 修改整个表中，栈中变量的偏移量
 * 
 * @param vartable 
 * @param varname 
 * @param stkamt 
 * @return 返回被改变的节点数
 */
int vartable_update_all(vartable* vartable,int stkamt)
{
    int count =0;
    for(int i=0;i<vartable->nodecmt;i++)
    {
          if(vartable->table[i].kind == memindex)
        {
            vartable->table[i].index += stkamt;
            count +=1;
        }   
    }
    return count;
}             


vartable * vartable_create()
{
    vartable *vartbl = (vartable*)malloc(sizeof(struct _vartable));
    memset(vartbl,0,sizeof(struct _vartable));

    return vartbl;
}