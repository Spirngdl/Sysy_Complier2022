#include "include/def.h"
// 收集错误信息
void semantic_error(int line, char *msg1, char *msg2)
{
    printf("第%d行,%s %s\n", line, msg1, msg2);
}
//填写临时变量到符号表，返回临时变量在符号表中的位置
int temp_add(char *name, int level, int type, int flag)
{
    strcpy(symbolTable.symbols[symbolTable.index].name, "");
    strcpy(symbolTable.symbols[symbolTable.index].alias, name);
    symbolTable.symbols[symbolTable.index].level = level;
    symbolTable.symbols[symbolTable.index].type = type;
    symbolTable.symbols[symbolTable.index].flag = flag;
    // symbolTable.symbols[symbolTable.index].offset = offset;
    return symbolTable.index++; //返回的是临时变量在符号表中的位置序号
}

//合并多个中间代码的双向循环链表，首尾相连
struct codenode *merge(int num, ...)
{
    struct codenode *h1, *h2, *t1, *t2;
    va_list ap;                         //指向参数的指针
    va_start(ap, num);                  //宏初始化va_list变量，使其指向第一个可变参数的地址
    h1 = va_arg(ap, struct codenode *); //返回可变参数，va_arg的第二个参数是要返回的参数的类型,如果多个可变参数，依次调用va_arg获取各个参数
    while (--num > 0)
    {
        h2 = va_arg(ap, struct codenode *);
        if (h1 == NULL)
            h1 = h2;
        else if (h2)
        {
            t1 = h1->prior;
            t2 = h2->prior;
            t1->next = h2;
            t2->next = h1;
            h1->prior = t2;
            h2->prior = t1;
        }
    }
    va_end(ap); //使用va_end宏结束可变参数的获取
    return h1;
}

//生成一条TAC代码的结点组成的双向循环链表，返回头指针
struct codenode *genIR(int op, struct opn opn1, struct opn opn2, struct opn result)
{
    struct codenode *h = (struct codenode *)malloc(sizeof(struct codenode));
    h->op = op;
    h->opn1 = opn1;
    h->opn2 = opn2;
    h->result = result;
    h->next = h->prior = h;
    h->in = h->out = 0;
    return h;
}
//生成一条标号语句，返回头指针
struct codenode *genLabel(char *label)
{
    struct codenode *h = (struct codenode *)malloc(sizeof(struct codenode));
    h->op = LABEL;
    strcpy(h->result.id, label);
    h->next = h->prior = h;
    h->in = h->out = 0;
    return h;
}

//生成GOTO语句，返回头指针
struct codenode *genGoto(char *label)
{
    struct codenode *h = (struct codenode *)malloc(sizeof(struct codenode));
    h->op = GOTO;
    strcpy(h->result.id, label);
    h->next = h->prior = h;
    return h;
}
//生成待回填指令
struct codenode *genback(int kind)
{
    struct codenode *h = (struct codenode *)malloc(sizeof(struct codenode));
    h->op = BACK;
    h->result.kind = kind;
    h->next = h->prior = h;
    return h;
}
void print_IR(struct codenode *head)
{
    char opnstr1[32], opnstr2[32], resultstr[32];
    struct codenode *h = head;
    while(h != NULL)
    {
        printf("%d: ", h->UID);
        if (h->opn1.kind == INT)
            sprintf(opnstr1, "#%d", h->opn1.const_int);
        if (h->opn1.kind == ID)
            sprintf(opnstr1, "%s", h->opn1.id);
        if (h->opn2.kind == INT)
            sprintf(opnstr2, "#%d", h->opn2.const_int);
        if (h->opn2.kind == ID)
            sprintf(opnstr2, "%s", h->opn2.id);
        sprintf(resultstr, "%s", h->result.id);
        switch (h->op)
        {
        case TOK_ASSIGN:
            printf("  %s := %s\n", resultstr, opnstr1);
            break;
        case ARRAY_ASSIGN:
            printf("  %s[%s] := %s\n", resultstr, opnstr1, opnstr2);
            break;
        case TOK_ADD:
        case TOK_MUL:
        case TOK_SUB:
        case TOK_DIV:
            printf("  %s := %s %c %s\n", resultstr, opnstr1, h->op == TOK_ADD ? '+' : h->op == TOK_SUB  ? '-'
                                                                                  : h->op == TOK_MUL    ? '*'
                                                                                  : h->op == TOK_MODULO ? '%'
                                                                                  : h->op == TOK_NOT    ? '!'
                                                                                                        : '\\',
                   opnstr2);
            break;
        case FUNCTION:
            printf("FUNCTION %s :\n", h->result.id);
            break;
        case PARAM:
            printf("  PARAM %s\n", h->result.id);
            break;
        case LABEL:
            printf("LABEL %s :\n", h->result.id);
            break;
        case GOTO:
            printf("  GOTO %d\n", h->result.const_int);
            break;
        case JLE:
            printf("  IF %s <= %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case JLT:
            printf("  IF %s < %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case JGE:
            printf("  IF %s >= %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case JGT:
            printf("  IF %s > %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case EQ:
            printf("  IF %s == %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case NEQ:
            printf("  IF %s != %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case ARG:
            printf("  ARG %s\n", h->result.id);
            break;
        case CALL:
            printf("  %s := CALL %s\n", resultstr, opnstr1);
            break;
        case TOK_RETURN:
            if (h->result.kind)
                printf("  RETURN %s\n", resultstr);
            else
                printf("  RETURN\n");
            break;
        case END:
            printf("END\n");
            break;
        default:
            printf("not define\n");
            break;
        }
        h = h->next;
    }
}
/* inner code generation */
char *str_catch(char *s1, char *s2)
{
    static char result[10];
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *newAlias() //
{
    static int no = 1;
    char s[10];
    snprintf(s, 10, "%d", no++);
    // itoa(no++, s, 10);
    return str_catch("var", s);
}

char *newLabel()
{
    static int no = 1;
    char s[10];
    snprintf(s, 10, "%d", no++);
    // itoa(no++, s, 10);
    return str_catch("label", s);
}

char *newTemp()
{
    static int no = 1;
    char s[10];
    snprintf(s, 10, "%d", no++);
    return str_catch("temp", s);
}
