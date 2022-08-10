#include "include/def.h"
// 收集错误信息
void semantic_error(int line, char *msg1, char *msg2)
{
    printf("第%d行,%s %s\n", line, msg1, msg2);
}
//填写临时变量到符号表，返回临时变量在符号表中的位置
int temp_add(char *name, int level, int type, int flag)
{
    strcpy(symbolTable.symbols[symbolTable.index].name, name);
    strcpy(symbolTable.symbols[symbolTable.index].alias, name);
    symbolTable.symbols[symbolTable.index].level = level;
    symbolTable.symbols[symbolTable.index].type = type;
    symbolTable.symbols[symbolTable.index].flag = flag;
    // symbolTable.symbols[symbolTable.index].offset = offset;
    AliasTableADD(name, type);
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
        else if (h2 != NULL)
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

//生成一条TOK_LDR代码
struct codenode *genLDR(int val, struct codenode *t, int flag)
{
    struct codenode *h = (struct codenode *)malloc(sizeof(struct codenode));
    h->next = h->prior = NULL;
    h->in = h->out = 0;
    char *name = newTemp();
    h->op = TOK_LDR;
    h->opn1.kind = LITERAL;
    h->opn1.const_int = val;
    h->result.kind = ID;
    strcpy(h->result.id, name);
    //指针的操作还是在外面做？
    // if (t->prior != NULL)
    // {
    //     t->prior->next = h;
    // }
    // h->prior = t->prior;
    // h->next = t;
    // t->prior = h;
    if (flag == 1)
    {
        t->opn1.kind = ID;
        strcpy(t->opn1.id, name);
    }
    else if (flag == 2)
    {
        t->opn2.kind = ID;
        strcpy(t->opn2.id, name);
    }
    return h;
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
    h->result.kind = ID;
    h->opn1.kind = h->opn2.kind = NONE;
    strcpy(h->result.id, label);
    h->UID = 0;
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
    char str[128];
    // FILE *fp = fopen("./tac.txt", "w");
    while (h != NULL)
    {
        printf("%d: ", h->UID);
        if (h->opn1.kind == LITERAL)
            sprintf(opnstr1, "#%d", h->opn1.const_int);
        if (h->opn1.kind == ID)
            sprintf(opnstr1, "%s", h->opn1.id);
        if (h->opn1.kind == FLOAT_LITERAL)
        {
            sprintf(opnstr1, "#%f", h->opn1.const_float);
            // printf("\n......dd = %f", h->opn1.const_float);
        }
        if (h->opn2.kind == LITERAL)
        {
            sprintf(opnstr2, "#%d", h->opn2.const_int);
        }
        if (h->opn2.kind == ID)
        {
            sprintf(opnstr2, "%s", h->opn2.id);
        }
        if (h->opn2.kind == FLOAT_LITERAL)
        {
            sprintf(opnstr2, "#%f", h->opn2.const_float);
        }

        if (h->result.kind == LITERAL)
        {
            sprintf(resultstr, "#%d", h->result.const_int);
        }
        if (h->result.kind == FLOAT_LITERAL)
        {
            sprintf(resultstr, "#%f", h->result.const_float);
        }
        if (h->result.kind == ID)
        {
            sprintf(resultstr, "%s", h->result.id);
        }

        switch (h->op)
        {
        case TOK_ASSIGN:
            sprintf(str, "  %s := %s\n", resultstr, opnstr1);
            break;
        case TOK_LDR:
            sprintf(str, "  %s := %s\n", resultstr, opnstr1);
            break;
        case ARRAY_ASSIGN:
            sprintf(str, "  %s[%s] := %s\n", resultstr, opnstr1, opnstr2);
            break;
        case ARRAY_DEF:
            sprintf(str, "  %s[%s] \n", resultstr, opnstr1);
            break;
        case TOK_ADD:
        case TOK_MUL:
        case TOK_SUB:
        case TOK_DIV:
        case TOK_MODULO:
            sprintf(str, "  %s := %s %c %s\n", resultstr, opnstr1, h->op == TOK_ADD ? '+' : h->op == TOK_SUB  ? '-'
                                                                                        : h->op == TOK_MUL    ? '*'
                                                                                        : h->op == TOK_MODULO ? '%'
                                                                                        : h->op == TOK_NOT    ? '!'
                                                                                                              : '/',
                    opnstr2);
            break;
        case FUNCTION:
            sprintf(str, "FUNCTION %s :\n", h->result.id);
            break;
        case PARAM:
            sprintf(str, "  PARAM %s\n", h->result.id);
            break;
        case LABEL:
            sprintf(str, "LABEL %s :\n", h->result.id);
            break;
        case GOTO:
            sprintf(str, "  GOTO %d\n", h->result.const_int);
            break;
        case JLE:
            sprintf(str, "  IF %s <= %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case JLT:
            sprintf(str, "  IF %s < %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case JGE:
            sprintf(str, "  IF %s >= %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case JGT:
            sprintf(str, "  IF %s > %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case EQ:
            sprintf(str, "  IF %s == %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case NEQ:
            sprintf(str, "  IF %s != %s GOTO %d\n", opnstr1, opnstr2, h->result.const_int);
            break;
        case ARRAY_EXP:
            sprintf(str, "  %s := %s[%s]\n", resultstr, opnstr1, opnstr2);
            break;
        case ARG:
            sprintf(str, "  ARG %s\n", resultstr);
            break;
        case CALL:
            if (h->result.kind == NONE)
            {
                sprintf(str, "  CALL %s\n", opnstr1);
            }
            else
                sprintf(str, "  %s := CALL %s\n", resultstr, opnstr1);
            break;
        case TOK_RETURN:
            if (h->result.kind)
                sprintf(str, "  RETURN %s\n", resultstr);
            else
                sprintf(str, "  RETURN\n");
            break;

        case END:
            sprintf(str, "END\n");
            break;
        default:
            sprintf(str, "not define\n");
            break;
        }
        printf("%s", str);
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

void make_uid(struct codenode **head_ptr)
{
    int uid = 100;
    struct codenode *head = *head_ptr;
    struct codenode *temp = head;
    struct codenode *globel = NULL, *ge = NULL;
    struct codenode *function = NULL, *end = NULL;
    head->prior->next = NULL;
    head->prior = NULL;
    while (temp != NULL) //把所有全局变量声明移到最前面
    {
        struct codenode *hcode;
        struct codenode *lcode;
        if (temp->op == FUNCTION)
        {
            hcode = temp;
            while (temp != NULL && temp->op != END)
            {
                temp = temp->next;
            }
            lcode = temp;
            temp = temp->next;
            lcode->next = NULL;
            if (function == NULL)
            {
                function = hcode;
                end = lcode;
            }
            else
            {
                end->next = hcode;
                hcode->prior = end;
                end = lcode;
            }
            continue;
        }
        else //是全局变量
        {
            hcode = temp;
            while (temp != NULL && temp->next != NULL && temp->next->op != FUNCTION)
            {
                temp = temp->next;
            }
            lcode = temp;
            temp = temp->next;
            lcode->next = NULL;
            if (globel == NULL)
            {
                globel = hcode;
                ge = lcode;
            }
            else
            {
                ge->next = hcode;
                hcode->prior = ge;
                ge = lcode;
            }
            continue;
        }
    }
    if (globel) //如果有全局变量语句
    {
        *head_ptr = globel;
        ge->next = function;
    }
    else
    {
        *head_ptr = function;
    }
    temp = *head_ptr;
    while (temp)
    {
        temp->UID = uid;
        if (temp->op != LABEL)
        {
            uid++;
        }
        temp = temp->next;
    }
}
void change_label(struct codenode *head)
{
    struct codenode *hcode = head;
    while (hcode != NULL)
    {
        if (hcode->op == GOTO || hcode->op == JLE || hcode->op == JLT || hcode->op == JLT || hcode->op == JGE || hcode->op == JGT || hcode->op == EQ || hcode->op == NEQ)
        {
            struct codenode *temp = head;
            while (temp)
            {
                if (temp->op == LABEL)
                {
                    if (!strcmp(temp->result.id, hcode->result.id))
                    {
                        hcode->result.const_int = temp->UID;
                        break;
                    }
                }
                temp = temp->next;
            }
        }
        hcode = hcode->next;
    }
    hcode = head;
    while (hcode != NULL)
    {
        if (hcode->op == LABEL)
        {
            hcode->prior->next = hcode->next;
            hcode->next->prior = hcode->prior;
            struct codenode *delnode = hcode;
            hcode = hcode->next;
            free(delnode);
        }
        else
            hcode = hcode->next;
    }
}
/**
 * @brief 对产生的流图重新编写UID，主要是因为dag优化后，好像全乱了，但是考虑到流图的前后结点属性
 * 应该可以做到对GOTO 和IF GOTO 目标的重写。
 *
 * @param block 基本块头
 */
void make_uid_block(Blocks *block)
{
    int UID = 100;
    //首先遍历基本块 先把所有语句重写一下UID，然后再想怎么改写
    Blocks *cur_blocks = block;
    while (cur_blocks != NULL)
    {
        for (int i = 0; i < cur_blocks->count; i++)
        {
            struct codenode *tcode = cur_blocks->block[i]->tac_list;
            while (tcode != NULL)
            {
                tcode->UID = UID++;
                tcode = tcode->next;
            }
        }
        cur_blocks = cur_blocks->next;
    }
    //修改GOTO语句 感觉好影响效率
    cur_blocks = block;
    while (cur_blocks != NULL)
    {
        for (int i = 0; i < cur_blocks->count; i++)
        {
            struct codenode *tcode = cur_blocks->block[i]->tac_list;
            int num_chi = cur_blocks->block[i]->num_children;
            if (num_chi == 1) //子结点块只有一个，一般是只有GOTO或者没有GOTO
            {
                while (tcode != NULL)
                {
                    if (tcode->op == GOTO)
                    {
                        int target = cur_blocks->block[i]->children[0]->tac_list->UID; //拿到子节点块的第一句的UID
                        tcode->result.const_int = target;
                    }
                    tcode = tcode->next;
                }
            }
            else if (num_chi == 2) // 子节点块有两个，一般是遇到了IF
            {
                while (tcode)
                {
                    if (tcode->op >= GOTO) //条件跳转或者GOTO
                    {

                        tcode->result.const_int = cur_blocks->block[i]->children[0]->tac_list->UID; //条件跳转
                        tcode = tcode->next;
                        tcode->result.const_int = cur_blocks->block[i]->children[1]->tac_list->UID; // GOTO
                    }
                    tcode = tcode->next;
                }
            }
        }
        cur_blocks = cur_blocks->next;
    }
}
void test_array()
{
    List *var = symbolTable.symbols[0].value;
    void *element;
    var->first(var, false);
    while (var->next(var, &element))
    {
        int i = (int)(long long)element;
        printf("%d ", i);
    }
    printf("\n");
}
//用于判断合法立即数
int op_mask[16] = {0, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1};
int op_unmask[16] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};
int count_mask[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
/**
 * @brief 用于判断合法立即数
 *
 * @param imme
 * @return int 0表示合法，-1 表示不合法
 */
int check_imme(int imme)
{
    if(imme < 0)
        imme = -imme;
    int tmp;
    int count1 = 0;
    int first = 0, firstflag = 0;
    int last = 0, lastflag = 0;
    for (int i = 0; i < 8; i++)
    {
        // printf(" the %d time enter\n", i);
        tmp = (imme & (0x0000000f << 4 * i));
        tmp = tmp >> (i * 4);
        if (!firstflag && tmp)
        {
            first = i * 4 + op_mask[tmp];
            firstflag = 1;
            // printf("first 1 is %d\n", first);
        }
        count1 += count_mask[tmp];
        if (tmp)
        {
            last = i * 4 + op_unmask[tmp];
            // printf("%d      last is %d\n", i, last);
            int dif = last - first;
            if (dif == 7)
            {
                // the num is started from 1
                if ((first % 2) == 0)
                {
                    // printf("the first 1 at odd number!\n");
                    return -1;
                }
            }
            if (dif > 7 && dif < 25)
            {
                // printf("not fair!\n");
                return -1;
            }
            else if (dif >= 25)
            {
                int high8 = (imme & 0xff000000);
                imme = imme << 8;
                imme |= (high8 >> 24);
                firstflag = 0;
                i = -1;
                count1 = 0;
            }
        }
    }
    if (count1 > 8)
    {
        // printf("too much 1!\n");
        return -1;
    }
    return 0;
}
/**
 * @brief 遍历所有基本块，遍历所有三地址代码，查看使用的立即数是否合法
 *
 * @param blocks
 */
void check_immes(Blocks *blocks)
{
    Blocks *cur_blocks = blocks;
    struct codenode *result = NULL;
    char *temp_name = NULL;
    int val = 0;
    int base_id = 0;
    while (cur_blocks != NULL) //遍历所有基本块
    {
        for (int i = 0; i < cur_blocks->count; i++)
        {
            struct codenode *tcode = cur_blocks->block[i]->tac_list;
            base_id = tcode->UID;
            while (tcode) //遍历三地址代码
            {
                switch (tcode->op)
                {
                case TOK_ASSIGN:
                    if (tcode->opn1.kind == LITERAL)
                    {
                        val = tcode->opn1.const_int;
                        if (check_imme(val) == -1) //非法立即数
                        {
                            // genLDR(val, tcode, 1);
                            // if (tcode->UID == base_id) //该语句是当前基本块的头部
                            // {
                            //     cur_blocks->block[i]->tac_list = cur_blocks->block[i]->tac_list->prior;
                            // }
                            tcode->op = TOK_LDR;
                        }
                    }
                    break;
                case TOK_ADD:
                case TOK_SUB:
                case TOK_MUL:
                case TOK_DIV:
                case TOK_MODULO:
                case ARRAY_ASSIGN:
                case ARRAY_EXP:
                case JLE: //条件跳转要不要判断
                case JLT:
                case JGE:
                case JGT:
                case EQ:
                case NEQ:
                    if (tcode->opn1.kind == LITERAL)
                    {
                        val = tcode->opn1.const_int;
                        if (check_imme(val) == -1)
                        {
                            struct codenode *e = genLDR(val, tcode, 1);
                            e->next = tcode;
                            if (tcode->UID == base_id) //该语句是当前基本块的头部
                            {
                                cur_blocks->block[i]->tac_list = e;
                            }
                            else
                            {
                                tcode->prior->next = e;
                            }
                            tcode->prior = e;
                        }
                    }
                    if (tcode->opn2.kind == LITERAL)
                    {
                        val = tcode->opn2.const_int;
                        if (check_imme(val) == -1)
                        {
                            struct codenode *e = genLDR(val, tcode, 2);
                            e->next = tcode;
                            if (tcode->UID == base_id) //该语句是当前基本块的头部
                            {
                                cur_blocks->block[i]->tac_list = e;
                            }
                            else
                            {
                                tcode->prior->next = e;
                            }
                            tcode->prior = e;
                        }
                    }
                    break;
                default:
                    break;
                }
                tcode = tcode->next;
            }
        }
        cur_blocks = cur_blocks->next;
    }
}

int CompareKey(void *lhs, void *rhs)
{
    return strcmp((char *)lhs, (char *)rhs);
}

void CleanKey(void *key)
{
    free(key);
}
/**
 * @brief 拿到一个函数内，使用的变量个数
 *
 * @param head
 * @return int
 */
int get_count_var(Blocks *head)
{
    //用集合吧
    HashSet *var_set = HashSetInit();
    HashSetSetHash(var_set, HashKey);
    HashSetSetCompare(var_set, CompareKey);
    HashSetSetCleanKey(var_set, CleanKey);
    char *func_name = head->name;
    for (int i = 0; i < head->count; i++)
    {
        struct codenode *tcode = head->block[i]->tac_list;
        while (tcode)
        {
            if (tcode->result.kind == ID)
                HashSetAdd(var_set, strdup(tcode->result.id));
            if (tcode->opn1.kind == ID)
                HashSetAdd(var_set, strdup(tcode->opn1.id));
            if (tcode->opn2.kind == ID)
                HashSetAdd(var_set, strdup(tcode->opn2.id));
            tcode = tcode->next;
        }
    }
    HashSetRemove(var_set, func_name);
    //其实还得删掉全局变量的
    int size = HashSetSize(var_set);
    return size;
}