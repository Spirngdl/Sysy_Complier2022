#include "include/def.h"
struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos)
{
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = kind;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    return T;
}
struct node *mkparray(int kind, char *name, struct node *len, int pos)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->kind = kind;
    strcpy(temp->type_id, name);
    int rtn = fillast(name, TOK_INT, kind);
    temp->place = rtn;
    temp->pos = pos;
    astsymbol.symbols[rtn].array_dimension = 0;
    memset(astsymbol.symbols[rtn].length,0,sizeof(astsymbol.symbols[rtn].length));
    if (kind == PARAM_ARRAY)
    {
        astsymbol.symbols[rtn].length[0] = 0;
        astsymbol.symbols[rtn].array_dimension = 1;
    }
    //处理维度
    while (len != NULL) //多维
    {
        astsymbol.symbols[rtn].length[astsymbol.symbols[rtn].array_dimension] = len->type_int;
        astsymbol.symbols[rtn].array_dimension += 1;
        len = len->ptr[0];
    }
    return temp;
}
//对抽象语法树的先根遍历
// void display(struct node *T, int indent)
// {
//     int i = 1;
//     struct node *T0;
//     struct node *temp;
//     int array_len[20] = {0};
//     if (T)
//     {
//         switch (T->kind)
//         {
//         case COMPUNIT_LIST:
//             display(T->ptr[0], indent); //显示该外部定义列表中的第一个
//             display(T->ptr[1], indent); //显示该外部定义列表中的其它外部定义
//             break;
//         case CONSTDEF:
//             printf("%*cCONSTDEF: \n", indent, ' ');
//             display(T->ptr[0], indent + 3); //显示变量类型
//             // printf("%*cID: \n", indent + 3, ' ');
//             // display(T->ptr[0], indent + 6); //显示变量列表
//             break;
//         case TYPE:
//             printf("%*cTYPE: %s\n", indent, ' ', T->type_id);
//             break;
//         case CONSTDECL_LIST:
//             display(T->ptr[0], indent); //依次显示外部变量名，
//             display(T->ptr[1], indent); //后续还有相同的，仅显示语法树此处理代码可以和类似代码合并
//             break;
//         case VAR_DEF:
//             printf("%*cVARDEF: \n", indent, ' ');
//             display(T->ptr[0], indent + 3); //显示变量类型
//             printf("%*cID: \n", indent + 3, ' ');
//             display(T->ptr[0], indent + 6); //显示变量列表
//             break;
//         case VAR_DECL_LIST:
//             display(T->ptr[0], indent); //依次显示外部变量名，
//             display(T->ptr[1], indent); //后续还有相同的，仅显示语法树此处理代码可以和类似代码合并
//             break;
//         case FUNC_DEF:
//             printf("%*cFUNCDEF: \n", indent, ' ');
//             display(T->ptr[0], indent + 3); //显示函数返回类型
//             display(T->ptr[1], indent + 3); //显示函数名和参数
//             display(T->ptr[2], indent + 3); //显示函数体
//             break;
//         case FUNC_DEC:
//             printf("%*cID:%s\n", indent, ' ', T->type_id);
//             if (T->ptr[0])
//             {
//                 printf("%*cfuncfparams: \n", indent, ' ');
//                 display(T->ptr[0], indent + 3); //显示函数参数列表
//             }
//             else
//                 printf("%*c无参函数\n", indent + 3, ' ');
//             break;
//         case ARRAY_DEC:
//             i = 0;
//             temp = T->ptr[0]; // temp : array_dec or ID
//             while (temp->kind == ARRAY_DEC)
//             {
//                 array_len[i] = temp->ptr[1]->type_int;
//                 i++;
//                 temp = temp->ptr[0];
//             }
//             printf("%*c%s%s\n", indent, ' ', "ID: ", temp->type_id);
//             printf("%*c%s%d\n", indent, ' ', "length: ", i);
//             for (int j = i - 1; j >= 0; j--)
//                 printf("%d ", array_len[j]);
//             printf("\n");
//             break;
//         case INITVAL_LIST:
//             display(T->ptr[0], indent); //依次显示全部参数类型和名称，
//             display(T->ptr[1], indent);
//         case INITVAL:
//             printf("%*cinitval: \n", indent, ' ');
//             if (T->ptr[0])
//             {
//                 display(T->ptr[0], indent);
//             }
//             break;
//         case EXP_ELE:
//             break;
//         case PARAM_LIST:
//             display(T->ptr[0], indent); //依次显示全部参数类型和名称，
//             display(T->ptr[1], indent);
//             break;
//         case PARAM_ARRAY:
//             printf("%*ctype: %s,ID: %s\n", indent, ' ', "int array", T->ptr[1]->type_id);
//             i = 0;
//             temp = T->ptr[2]; // temp : array_dec
//             while (temp->kind == ARRAY_DEC)
//             {
//                 array_len[i] = temp->ptr[1]->type_int;
//                 i++;
//                 temp = temp->ptr[0];
//             }
//             printf("%*c%s%d\n", indent, ' ', "length: ", i + 1);
//             printf("%*c", indent, ' ');
//             for (int j = 0; j < i; j++)
//             {
//                 printf("%d ", array_len[j]);
//             }
//             printf("\n");
//             break;
//         case PARAM_DEC:
//             if (T->ptr[0]->type == INT)
//                 printf("%*ctype: %s,ID: %s\n", indent, ' ', "int", T->ptr[1]->type_id);
//             break;
//         case BLOCK_LIST:
//             printf("Block");
//             display(T->ptr[0], indent);
//             display(T->ptr[1], indent);
//             break;
//         case EXP_STMT:
//             printf("%*c表达式语句: \n", indent, ' ');
//             display(T->ptr[0], indent + 3);
//             break;
//         case TOK_RETURN:
//             printf("%*cRETURNSTMT: \n", indent, ' ');
//             display(T->ptr[0], indent + 3);
//             break;
//         case COMP_STM:
//             printf("%*c复合语句：\n", indent, ' ');
//             printf("%*c复合语句的变量定义：\n", indent + 3, ' ');
//             display(T->ptr[0], indent + 6); //显示定义部分
//             printf("%*c复合语句的语句部分：\n", indent + 3, ' ');
//             display(T->ptr[1], indent + 6); //显示语句部分
//             break;
//         case STM_LIST:
//             display(T->ptr[0], indent); //显示第一条语句
//             display(T->ptr[1], indent); //显示剩下语句
//             break;
//         case WHILE:
//             printf("%*cWHILESTMT: \n", indent, ' ');
//             printf("%*cCondition: \n", indent + 3, ' ');
//             display(T->ptr[0], indent + 6); //显示循环条件
//             printf("%*cBlock: \n", indent + 3, ' ');
//             display(T->ptr[1], indent + 6); //显示循环体
//             break;
//         case TOK_BREAK:
//             printf("%*cBREAKSTMT: \n", indent, ' ');
//             break;
//         case TOK_CONTINUE:
//             printf("%*cCONTINUE: \n", indent, ' ');
//             break;
//         case IF_THEN:
//             printf("%*cIFSTMT: \n", indent, ' ');
//             printf("%*cCondition: \n", indent + 3, ' ');
//             display(T->ptr[0], indent + 6); //显示条件
//             printf("%*cBlock: \n", indent + 3, ' ');
//             display(T->ptr[1], indent + 6); //显示if子句
//             break;
//         case IF_THEN_ELSE:
//             printf("%*cIF_THEN_ELSE): \n", indent, ' ');
//             printf("%*cCondition: \n", indent + 3, ' ');
//             display(T->ptr[0], indent + 6); //显示条件
//             printf("%*cIF子句：\n", indent + 3, ' ');
//             display(T->ptr[1], indent + 6); //显示if子句
//             printf("%*cELSE子句：\n", indent + 3, ' ');
//             display(T->ptr[2], indent + 6); //显示else子句
//             break;
//         case DEF_LIST:
//             display(T->ptr[0], indent); //显示该局部变量定义列表中的第一个
//             display(T->ptr[1], indent); //显示其它局部变量定义
//             break;
//         // case DEC_LIST:
//         //     printf("%*cVAR_NAME：\n", indent, ' ');
//         //     T0 = T;
//         //     while (T0)
//         //     {
//         //         if (T0->ptr[0]->kind == ID)
//         //             printf("%*c %s\n", indent + 3, ' ', T0->ptr[0]->type_id);
//         //         else if (T0->ptr[0]->kind == ASSIGNOP)
//         //         {
//         //             printf("%*c %s ASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
//         //             //显示初始化表达式
//         //             display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
//         //         }
//         //         else if (T0->ptr[0]->kind == PLUSASSIGNOP)
//         //         {
//         //             printf("%*c %s PLUSASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
//         //             //显示初始化表达式
//         //             display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
//         //         }
//         //         else if (T0->ptr[0]->kind == MINUSASSIGNOP)
//         //         {
//         //             printf("%*c %s MINUSASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
//         //             //显示初始化表达式
//         //             display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
//         //         }
//         //         else if (T0->ptr[0]->kind == STARASSIGNOP)
//         //         {
//         //             printf("%*c %s STARASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
//         //             //显示初始化表达式
//         //             display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
//         //         }
//         //         else if (T0->ptr[0]->kind == DIVASSIGNOP)
//         //         {
//         //             printf("%*c %s DIVASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
//         //             //显示初始化表达式
//         //             display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
//         //         }
//         //         T0 = T0->ptr[1];
//         //     }
//         //     break;
//         case EXP_ARRAY:
//             display(T->ptr[0], indent);//ID
//             display(T->ptr[1], indent);//exp
//             break;
//         case ID:
//             printf("%*cID: %s\n", indent, ' ', T->type_id);
//             break;
//         case INT:
//             printf("%*cINT: %d\n", indent, ' ', T->type_int);
//             break;
//         case UNARYEXP:
//         case TOK_ASSIGN:
//         case MINUSASSIGNOP:
//         case PLUSASSIGNOP:
//         case STARASSIGNOP:
//         case DIVASSIGNOP:
//         case AND:
//         case OR:
//         case RELOP:
//         case PLUS:
//         case MINUS:
//         case STAR:
//         case DIV:
//             printf("%*c%s\n", indent, ' ', T->type_id);
//             display(T->ptr[0], indent + 3);
//             display(T->ptr[1], indent + 3);
//             break;
//         case MMINUS:
//         case PPLUS:
//             printf("%*c%s\n", indent, ' ', T->type_id);
//             display(T->ptr[0], indent + 3);
//             break;
//         case NOT:
//         case UMINUS:
//             printf("%*c%s\n", indent, ' ', T->type_id);
//             display(T->ptr[0], indent + 3);
//             break;
//         case FUNC_CALL:
//             printf("%*cFunc_call: \n", indent, ' ');
//             printf("%*cID: %s\n", indent + 3, ' ', T->type_id);
//             display(T->ptr[0], indent + 3);
//             break;
//         case ARGS:
//             i = 1;
//             while (T)
//             { // ARGS表示实际参数表达式序列结点，其第一棵子树为其一个实际参数表达式，第二棵子树为剩下的。
//                 struct node *T0 = T->ptr[0];
//                 printf("%*c第%d个实际参数表达式：\n", indent, ' ', i++);
//                 display(T0, indent + 3);
//                 T = T->ptr[1];
//             }
//             //                    printf("%*c第%d个实际参数表达式：\n",indent,' ',i);
//             //                  display(T,indent+3);
//             printf("\n");
//             break;
//         }
//     }
// }