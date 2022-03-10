#include "def.h"
int LEV = 0;   //层号
int func_size; //函数的活动记录大小
// 收集错误信息
void semantic_error(int line, char *msg1, char *msg2)
{
    printf("第%d行,%s %s\n", line, msg1, msg2);
}
//填写临时变量到符号表，返回临时变量在符号表中的位置
int temp_add(char *name, int level, int type, char flag, int offset)
{
    strcpy(symbolTable.symbols[symbolTable.index].name, "");
    strcpy(symbolTable.symbols[symbolTable.index].alias, name);
    symbolTable.symbols[symbolTable.index].level = level;
    symbolTable.symbols[symbolTable.index].type = type;
    symbolTable.symbols[symbolTable.index].flag = flag;
    symbolTable.symbols[symbolTable.index].offset = offset;
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
    h->in = h->out = 0;
}
void print_IR(struct codenode *head)
{
    char opnstr1[32], opnstr2[32], resultstr[32];
    struct codenode *h = head;
    do
    {
        // printf("%d: ",h->UID);
        if (h->opn1.kind == INT)
            sprintf(opnstr1, "#%d", h->opn1.const_int);
        if (h->opn1.kind == ID)
            sprintf(opnstr1, "%s", h->opn1.id);
        if (h->opn2.kind == INT)
            sprintf(opnstr2, "#%d", h->opn2.const_int);
        if (h->opn2.kind == ID)
            sprintf(opnstr2, "%s", h->opn2.id);
        if (h->opn1.kind == NONE)
            sprintf(opnstr1, "%s", " ");
        if (h->opn2.kind == NONE)
            sprintf(opnstr2, "%s", " ");
        sprintf(resultstr, "%s", h->result.id);
        switch (h->op)
        {
        case TOK_ASSIGN:
            printf("  %s := %s\n", resultstr, opnstr1);
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
            printf("  GOTO %s\n", h->result.id);
            break;
        case JLE:
            printf("  IF %s <= %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case JLT:
            printf("  IF %s < %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case JGE:
            printf("  IF %s >= %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case JGT:
            printf("  IF %s > %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case EQ:
            printf("  IF %s == %s GOTO %s\n", opnstr1, opnstr2, resultstr);
            break;
        case NEQ:
            printf("  IF %s != %s GOTO %s\n", opnstr1, opnstr2, resultstr);
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
        }
        h = h->next;
    } while (h != head);
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

//打印函数
void DisplaySymbolTable(struct node *T)
{
    symbolTable.index = 0;
    // fillSymbolTable("read", "", 0, INT, 'F', 4);
    // symbolTable.symbols[0].paramnum = 0; //read的形参个数
    // fillSymbolTable("x", "", 1, INT, 'P', 12);
    // fillSymbolTable("write", "", 0, INT, 'F', 4);
    // symbolTable.symbols[2].paramnum = 1;
    symbol_scope_TX.TX[0] = 0; //外部变量在符号表中的起始序号为0
    symbol_scope_TX.top = 1;
    T->offset = 0; // 外部变量在数据区的偏移量
    semantic_Analysis(T);
    // make_uid(T->code);
    // change_label(T->code);
    print_IR(T->code);
}

//对抽象语法树的先根遍历,按display的控制结构修改完成符号表管理和语义检查和TAC生成（语句部分）
void semantic_Analysis(struct node *T)
{
    if (T)
    {
        switch (T->kind)
        {
        case COMPUNIT_LIST:
            compunit_list(T);
            break;
        case CONSTDEF:
            const_var_def(T);
        case VAR_DECL_LIST: //处理一个局部变量定义,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
            var_decl_list(T);
            break;
        case VAR_DECL:
            var_decl(T);
        // case EXT_VAR_DEF: //处理外部说明,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
        //     ext_var_def(T);
        //     break;
        case FUNC_DEF:
            func_def(T);
            break;
        case FUNC_DEC: //根据返回类型，函数名填写符号表
            func_dec(T);
            break;
        case PARAM_LIST: //处理函数形式参数列表
            param_list(T);
            break;
        case PARAM_DEC:
            param_dec(T);
            break;
        case BLOCK:
            block(T);
            break;
        case BLOCK_LIST:
            block_list(T);
            break;
        // case COMP_STM:
        //     comp_stm(T);
        //     break;
        // case DEF_LIST:
        //     def_list(T);
        //     break;
        // case STM_LIST:
        //     stmt_list(T);
        //     break;
        case IF_THEN:
            if_then(T);
            break; //控制语句都还没有处理offset和width属性
        case IF_THEN_ELSE:
            if_then_else(T);
            break;
        case TOK_WHILE:
            while_dec(T);
            break;
            // case EXP_STMT:
            //     exp_stmt(T);
            break;
        case TOK_RETURN:
            return_dec(T);
            break;
        case TOK_BREAK:
            break_stmt(T);
            break;
        case ID:
        case INT:
        case TOK_ASSIGN:
        case TOK_AND:
        case TOK_OR:
        case RELOP:
        case TOK_ADD:
        case TOK_SUB:
        case TOK_MUL:
        case TOK_DIV:
        case TOK_MODULO:

        case FUNC_CALL:
        case EXP_ARRAY:
            Exp(T); //处理基本表达式
            break;
        }
    }
}

void compunit_list(struct node *T)
{
    if (!T->ptr[0])
        return;

    // 语义分析之前先设置偏移地址
    T->ptr[0]->offset = T->offset;
    semantic_Analysis(T->ptr[0]); //访问外部定义列表中的第一个
    // 之后合并 code
    T->code = T->ptr[0]->code;

    // 可为空
    if (T->ptr[1])
    {
        T->ptr[1]->offset = T->ptr[0]->offset + T->ptr[0]->width;
        semantic_Analysis(T->ptr[1]); //访问该外部定义列表中的其它外部定义
        T->code = merge(2, T->code, T->ptr[1]->code);
    }
}
void var_decl_list(struct node *T)
{
    if (!T->ptr[0])
        return;
    //设置偏移地址
    T->ptr[0]->offset = T->offset;
    semantic_Analysis(T->ptr[0]); //访问外部定义列表中的第一个
    //之后合并code
    T->code = T->ptr[0]->code;
    //可为空
    if (T->ptr[0])
    {
        T->ptr[1]->offset = T->ptr[0]->offset + T->ptr[0]->width;
        semantic_Analysis(T->ptr[1]); //访问list中的其他
        T->code = merge(2, T->code, T->ptr[1]->code);
    }
}
void var_decl(struct node *T) //变量声明 -- kind -- NAME --TYPE
{
    int rtn, num, width;
    struct opn opn1, opn2, opn3, result;
    struct node *T0;
    T->code = NULL;
    if (T->array_dimension == 0) //非数组
    {
        if (T->type == TOK_INT)
            T->width = 4;

        rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, 'V', T->width + T->offset); //填符号表
        if (rtn == -1)
            semantic_error(T->pos, T->type_id, "变量重复定义");
        else
            T->place = rtn;    //用place记录在符号表中的位置
        if (T->ptr[0] != NULL) //赋值
        {
            T->ptr[0]->offset = T->offset + T->width;
            Exp(T->ptr[0]);
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias); //赋上别名
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->place].alias);                              //赋上别名
            T->code = merge(3, T->code, T->ptr[0]->code, genIR(TOK_ASSIGN, opn1, opn2, result)); //合并三地址代码
        }
    }
    else if (T->array_dimension > 0) //数组了
    {
        if (T->type == TOK_INT)
        {
            int widths = 0;
            for (int i = 0; i < T->array_dimension; i++)
            {
                widths = widths + 4 * T->length[i];
            }
            T->width = widths;
        }
        rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, 'V', T->width + T->offset); //填符号表
        if (rtn == -1)
            semantic_error(T->pos, T->type_id, "变量重复定义");
        else
            T->place = rtn;    //用place记录在符号表中的位置
        if (T->ptr[0] != NULL) //数组赋值
        {
            //假设只有一维
            T->ptr[0]->offset = T->offset + T->width;
            T0 = T->ptr[0];
            for (int i = 0; i < T->length[0]; i++)
            {
                struct opn op1, op2, res; // res[op2] = op1 op2 = 4 * i
                Exp(T0->ptr[0]);
                op1.kind = ID;
                strcpy(op1.id, symbolTable.symbols[T0->ptr[0]->place].alias); //附上别名

                struct opn ops1, ops2;                                     //用于生成语句 op2 = 4 * i
                int place = temp_add(newTemp(), LEV, INT, 'V', T->offset); //生成临时变量
                op2.kind = ID;
                strcpy(op2.id, symbolTable.symbols[place].alias);
                ops1.kind = INT;
                ops1.const_int = i;
                ops2.kind = INT;
                ops2.const_int = 4;
                struct codenode *addr = genIR(TOK_MUL, ops1, ops2, op2); //生成三地址代码

                T->code = merge(3, T->code, T0->ptr[0]->code, addr, genIR(ARRAY_ASSIGN, op1, op2, res)); //合并三地址代码
                T0 = T0->ptr[1];
            }
        }
    }
}
void const_var_def(struct node *T)
{
    int rtn, num, width;
    struct node *T0;
    struct opn opn1, opn2, result;
    T->code = NULL;
    T->ptr[1]->type = CONST_INT;
    T0 = T->ptr[1]; // T0为变量名列表子树根指针，对ID、ASSIGNOP类结点在登记到符号表，作为局部变量
    num = 0;
    T0->offset = T->offset;
    T->width = 0;
    while (T0) //处理const_list上的结点
    {
        num++;
        T0->ptr[0]->type = T0->type; //类型属性向下传递
        if (T0->ptr[1])
            T0->ptr[1]->type = T0->type;
        T0->ptr[0]->offset = T0->offset; //偏移量向下传递
        if (T0->ptr[1])
            T0->ptr[1]->offset = T0->offset + width;
        if (T0->ptr[0]->kind == ID)
        {
            rtn = fillSymbolTable(T0->ptr[0]->type_id, newAlias(), LEV, T0->ptr[0]->type, 'V', T->offset + T->width); //此处偏移量未计算，暂时为0
            if (rtn == -1)
                semantic_error(T0->ptr[0]->pos, T0->ptr[0]->type_id, "变量重复定义");
            else
                T0->ptr[0]->place = rtn;
            T->width += width;
        }
        else if (T0->ptr[0]->kind == TOK_ASSIGN)
        {
            if (T0->ptr[0]->ptr[0]->kind == ID)
            {
                rtn = fillSymbolTable(T0->ptr[0]->ptr[0]->type_id, newAlias(), LEV, T0->ptr[0]->type, 'V', T->offset + T->width); //此处偏移量未计算，暂时为0
                if (rtn == -1)
                    semantic_error(T0->ptr[0]->ptr[0]->pos, T0->ptr[0]->ptr[0]->type_id, "变量重复定义");
                else
                {
                    T0->ptr[0]->place = rtn;
                    T0->ptr[0]->ptr[1]->offset = T->offset + T->width + width;
                    Exp(T0->ptr[0]->ptr[1]);
                    opn1.kind = ID;
                    strcpy(opn1.id, symbolTable.symbols[T0->ptr[0]->ptr[1]->place].alias);
                    result.kind = ID;
                    strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
                    T->code = merge(3, T->code, T0->ptr[0]->ptr[1]->code, genIR(TOK_ASSIGN, opn1, opn2, result));
                }
                T->width += width + T0->ptr[0]->ptr[1]->width;
            }
        }
        T0 = T0->ptr[1];
    }
    // T->ptr[0]->width = 4;
    // T->ptr[0]->offset = T->offset; //这个变量的偏移量向下传递
    // const_var_list(T->ptr[0]);     //处理变量说明中的标识符序列
    // T->width = (T->ptr[0]->width) * T->ptr[0]->num;
    // T->code = NULL;
}
void var_def(struct node *T)
{
    int rtn, num, width;
    struct node *T0;
    struct opn opn1, opn2, opn3, result;
    T->code = NULL;
    T->ptr[1]->type = INT;
    T0 = T->ptr[1]; // T0为变量名列表子树根指针，对ID、ASSIGNOP类结点在登记到符号表，作为局部变量
    num = 0;
    T0->offset = T->offset;
    T->width = 0;
    while (T0) //处理var_list上的结点
    {
        num++;
        T0->ptr[0]->type = T0->type; //类型属性向下传递
        if (T0->ptr[1])
            T0->ptr[1]->type = T0->type;
        T0->ptr[0]->offset = T0->offset; //偏移量向下传递
        if (T0->ptr[1])
            T0->ptr[1]->offset = T0->offset + width;
        if (T0->ptr[0]->kind == ID)
        {
            rtn = fillSymbolTable(T0->ptr[0]->type_id, newAlias(), LEV, T0->ptr[0]->type, 'V', T->offset + T->width); //此处偏移量未计算，暂时为0
            if (rtn == -1)
                semantic_error(T0->ptr[0]->pos, T0->ptr[0]->type_id, "变量重复定义");
            else
                T0->ptr[0]->place = rtn;
            T->width += width;
        }
        else if (T0->ptr[0]->kind == TOK_ASSIGN)
        {
            if (T0->ptr[0]->ptr[0]->kind == ID)
            {
                rtn = fillSymbolTable(T0->ptr[0]->ptr[0]->type_id, newAlias(), LEV, T0->ptr[0]->type, 'V', T->offset + T->width); //此处偏移量未计算，暂时为0
                if (rtn == -1)
                    semantic_error(T0->ptr[0]->ptr[0]->pos, T0->ptr[0]->ptr[0]->type_id, "变量重复定义");
                else
                {
                    T0->ptr[0]->place = rtn;
                    T0->ptr[0]->ptr[1]->offset = T->offset + T->width + width;
                    Exp(T0->ptr[0]->ptr[1]);
                    opn1.kind = ID;
                    strcpy(opn1.id, symbolTable.symbols[T0->ptr[0]->ptr[1]->place].alias);
                    result.kind = ID;
                    strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
                    T->code = merge(3, T->code, T0->ptr[0]->ptr[1]->code, genIR(TOK_ASSIGN, opn1, opn2, result));
                }
                T->width += width + T0->ptr[0]->ptr[1]->width;
            }
        }
        T0 = T0->ptr[1];
    }
    // T->ptr[0]->width = 4;
    // T->ptr[0]->offset = T->offset; //这个变量的偏移量向下传递
    // const_var_list(T->ptr[0]);     //处理变量说明中的标识符序列
    // T->width = (T->ptr[0]->width) * T->ptr[0]->num;
    // T->code = NULL;
}
//处理基本表达式
void Exp(struct node *T)
{
    if (T)
    {
        switch (T->kind)
        {
        case ID:
            id_exp(T); //右值引用
            break;
        case INT:
            int_exp(T);
            break;
        case TOK_ASSIGN:
            assignop_exp(T);
            break;
        case FUNC_CALL: //根据T->type_id查出函数的定义，如果语言中增加了实验教材的read，write需要单独处理一下
            func_call_exp(T);
            break;
        case ARGS:
            args_exp(T);
            break;
        case TOK_ADD:
        case TOK_SUB:
        case TOK_MUL:
        case TOK_DIV:
        case TOK_MODULO:
            op_exp(T);
            break;
        case TOK_AND:
        case TOK_OR:
        case RELOP:
            relop_exp(T);
            break;
        case UNARYEXP:
            unaryexp(T);
            break;
        default:
            break;
        }
    }
}
void id_exp(struct node *T)
{
    int rtn;

    rtn = searchSymbolTable(T->type_id);
    if (rtn == -1)
        semantic_error(T->pos, T->type_id, "变量未声明定义就引用，语义错误");
    if (symbolTable.symbols[rtn].flag == 'F')
        semantic_error(T->pos, T->type_id, "是函数名，不是普通变量，语义错误");
    else
    {
        T->place = rtn; //结点保存变量在符号表中的位置
        T->code = NULL; //标识符不需要生成TAC
        T->type = symbolTable.symbols[rtn].type;
        T->offset = symbolTable.symbols[rtn].offset;
        T->width = 0; //未再使用新单元
    }
}
void int_exp(struct node *T)
{
    struct opn opn1, opn2, result;
    T->place = temp_add(newTemp(), LEV, T->type, 'T', T->offset); //为整常量生成一个临时变量
    T->type = INT;
    opn1.kind = INT;
    opn1.const_int = T->type_int;
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[T->place].alias);
    result.offset = symbolTable.symbols[T->place].offset;
    T->code = genIR(TOK_ASSIGN, opn1, opn2, result);
    T->width = 4;
}
//算术运算，+-*/
void op_exp(struct node *T)
{
    struct opn opn1, opn2, result;
    T->ptr[0]->offset = T->offset;
    Exp(T->ptr[0]);
    T->ptr[1]->offset = T->offset + T->ptr[0]->width;
    Exp(T->ptr[1]);
    //判断T->ptr[0]，T->ptr[1]类型是否正确，可能根据运算符生成不同形式的代码，给T的type赋值
    //下面的类型属性计算，没有考虑错误处理情况
    T->type = INT, T->width = T->ptr[0]->width + T->ptr[1]->width + 2;
    T->place = temp_add(newTemp(), LEV, T->type, 'T', T->offset + T->ptr[0]->width + T->ptr[1]->width);

    opn1.kind = ID;
    strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
    opn1.type = T->ptr[0]->type;
    opn1.offset = symbolTable.symbols[T->ptr[0]->place].offset;

    opn2.kind = ID;
    strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
    opn2.type = T->ptr[1]->type;
    opn2.offset = symbolTable.symbols[T->ptr[1]->place].offset;

    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[T->place].alias);
    result.type = T->type;
    result.offset = symbolTable.symbols[T->place].offset;
    T->code = merge(3, T->ptr[0]->code, T->ptr[1]->code, genIR(T->kind, opn1, opn2, result));
    T->width = T->ptr[0]->width + T->ptr[1]->width + 4; // INT
}
//单
void unaryexp(struct node *T)
{
    struct opn opn1, opn2, result;
    T->ptr[1]->offset = T->offset + T->ptr[0]->width;
    Exp(T->ptr[1]);
    //判断T->ptr[0]，T->ptr[1]类型是否正确，可能根据运算符生成不同形式的代码，给T的type赋值
    //下面的类型属性计算，没有考虑错误处理情况
    T->type = INT, T->width = T->ptr[1]->width + 2;
    T->place = temp_add(newTemp(), LEV, T->type, 'T', T->offset + T->ptr[1]->width);

    opn1.kind = ID;
    strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias);
    opn1.type = T->ptr[1]->type;
    opn1.offset = symbolTable.symbols[T->ptr[1]->place].offset;

    opn2.kind = NONE;
    // opn2.kind = ID;
    // strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
    // opn2.type = T->ptr[1]->type;
    // opn2.offset = symbolTable.symbols[T->ptr[1]->place].offset;

    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[T->place].alias);
    result.type = T->type;
    result.offset = symbolTable.symbols[T->place].offset;
    T->code = merge(2, T->ptr[1]->code, genIR(T->ptr[0]->kind, opn1, opn2, result));
    T->width = T->ptr[1]->width + 4; // INT
}
//赋值
void assignop_exp(struct node *T)
{
    struct opn opn1, opn2, result;
    if (T->ptr[0]->kind != ID)
    {
        semantic_error(T->pos, "", "赋值语句没有左值，语义错误");
    }
    else
    {
        Exp(T->ptr[0]); //处理左值，例中仅为变量
        T->ptr[1]->offset = T->offset;
        Exp(T->ptr[1]);
        T->type = T->ptr[0]->type;
        T->width = T->ptr[1]->width;
        T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code);

        opn1.kind = ID;
        strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias); //右值一定是个变量或临时变量
        opn1.offset = symbolTable.symbols[T->ptr[1]->place].offset;
        result.kind = ID;
        strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
        result.offset = symbolTable.symbols[T->ptr[0]->place].offset;
        T->code = merge(2, T->code, genIR(TOK_ASSIGN, opn1, opn2, result));
    }
}
//条件
void relop_exp(struct node *T)
{
    T->type = INT;
    T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
    Exp(T->ptr[0]);
    Exp(T->ptr[1]);
}
//布尔表达式
void boolExp(struct node *T)
{
    struct opn opn1, opn2, result;
    int op;
    int rtn;
    if (T)
    {
        switch (T->kind)
        {
        case INT:
            if (T->type_id != 0)
                T->code = genGoto(T->Etrue);
            else
                T->code = genGoto(T->Efalse);
            break;
        case ID: //查符号表，获得符号表中的位置，类型送type
            rtn = searchSymbolTable(T->type_id);
            if (rtn == -1)
                semantic_error(T->pos, T->type_id, "函数未定义，语义错误");
            if (symbolTable.symbols[rtn].flag == 'F')
                semantic_error(T->pos, T->type_id, "不是函数名，不能进行函数调用，语义错误");
            else
            {
                opn1.kind = ID;
                strcpy(opn1.id, symbolTable.symbols[rtn].alias);
                opn1.offset = symbolTable.symbols[rtn].offset;
                opn2.kind = INT;
                opn2.const_int = 0;
                result.kind = ID;
                strcpy(result.id, T->Etrue);
                T->code = genIR(NEQ, opn1, opn2, result);
                T->code = merge(2, T->code, genGoto(T->Efalse));
            }
            T->width = 0;
            break;
        case RELOP: //处理关系运算表达式,2个操作数都按基本表达式处理
            T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
            Exp(T->ptr[0]);
            T->width = T->ptr[0]->width;
            Exp(T->ptr[1]);
            if (T->width < T->ptr[1]->width)
                T->width = T->ptr[1]->width;
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            opn1.offset = symbolTable.symbols[T->ptr[0]->place].offset;
            opn2.kind = ID;
            strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
            opn2.offset = symbolTable.symbols[T->ptr[1]->place].offset;
            result.kind = ID;
            strcpy(result.id, T->Etrue);
            if (strcmp(T->type_id, "<") == 0)
                op = JLT;
            else if (strcmp(T->type_id, "<=") == 0)
                op = JLE;
            else if (strcmp(T->type_id, ">") == 0)
                op = JGT;
            else if (strcmp(T->type_id, ">=") == 0)
                op = JGE;
            else if (strcmp(T->type_id, "==") == 0)
                op = EQ;
            else if (strcmp(T->type_id, "!=") == 0)
                op = NEQ;
            T->code = genIR(op, opn1, opn2, result);
            T->code = merge(4, T->ptr[0]->code, T->ptr[1]->code, T->code, genGoto(T->Efalse));
            break;
        case TOK_AND:
        case TOK_OR:
            if (T->kind == TOK_AND)
            {
                strcpy(T->ptr[0]->Etrue, newLabel());
                strcpy(T->ptr[0]->Efalse, T->Efalse);
            }
            else
            {
                strcpy(T->ptr[0]->Etrue, T->Etrue);
                strcpy(T->ptr[0]->Efalse, newLabel());
            }
            strcpy(T->ptr[1]->Etrue, T->Etrue);
            strcpy(T->ptr[1]->Efalse, T->Efalse);
            T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
            boolExp(T->ptr[0]);
            T->width = T->ptr[0]->width;
            boolExp(T->ptr[1]);
            if (T->width < T->ptr[1]->width)
                T->width = T->ptr[1]->width;
            if (T->kind == TOK_AND)
                T->code = merge(3, T->ptr[0]->code, genLabel(T->ptr[0]->Etrue), T->ptr[1]->code);
            else
                T->code = merge(3, T->ptr[0]->code, genLabel(T->ptr[0]->Efalse), T->ptr[1]->code);
            break;

        case UNARYEXP:
            if (T->ptr[0]->kind == TOK_NOT)
            {
                strcpy(T->ptr[1]->Etrue, T->Efalse);
                strcpy(T->ptr[1]->Efalse, T->Etrue);
            }
            else
            {
                strcpy(T->ptr[1]->Etrue, T->Etrue);
                strcpy(T->ptr[1]->Efalse, T->Efalse);
            }
            boolExp(T->ptr[1]);
            T->width = T->ptr[1]->width;
            T->code = T->ptr[1]->code;
            break;
        default:
            break;
        }
    }
}
//函数调用
void func_call_exp(struct node *T)
{
    int rtn, width;
    struct node *T0;
    struct opn opn1, opn2, result;
    rtn = searchSymbolTable(T->type_id);
    if (rtn == -1)
    {
        semantic_error(T->pos, T->type_id, "函数未定义，语义错误");
        return;
    }
    if (symbolTable.symbols[rtn].flag != 'F')
    {
        semantic_error(T->pos, T->type_id, "不是函数名，不能进行函数调用，语义错误");
        return;
    }
    T->type = symbolTable.symbols[rtn].type;
    width = 4;     //存放函数返回值的单数字字节数
    if (T->ptr[0]) //处理参数
    {
        T->ptr[0]->offset = T->offset;
        Exp(T->ptr[0]); //处理所有实参表达式求值，及类型
        T->width = T->ptr[0]->width + width;
        T->code = T->ptr[0]->code;
    }
    else
    {
        T->width = width;
        T->code = NULL;
    }
    match_param(rtn, T->ptr[0]);
    //处理参数列表的中间代码
    T0 = T->ptr[0];
    while (T0)
    {
        result.kind = ID;
        strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
        result.offset = symbolTable.symbols[T0->ptr[0]->place].offset;
        T->code = merge(2, T->code, genIR(ARG, opn1, opn2, result));
        T0 = T0->ptr[1];
    }
    T->place = temp_add(newTemp(), LEV, T->type, 'T', T->offset + T->width - width);
    opn1.kind = ID;
    strcpy(opn1.id, T->type_id); //保存函数名
    opn1.offset = rtn;           //这里offset用以保存函数定义入口,在目标代码生成时，能获取相应信息
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[T->place].alias);
    result.offset = symbolTable.symbols[T->place].offset;
    T->code = merge(2, T->code, genIR(CALL, opn1, opn2, result)); //生成函数调用中间代码
}
int match_param(int i, struct node *T)
{
    int j, num = symbolTable.symbols[i].paramnum;
    int type1, type2;
    if (num == 0 && T == NULL)
        return 1;
    for (j = 1; j < num; j++)
    {
        type1 = symbolTable.symbols[i + j].type; //形参类型
        type2 = T->ptr[0]->type;
        if (type1 != type2)
        {
            semantic_error(T->pos, "", "参数类型不匹配");
            return 0;
        }
        T = T->ptr[1];
    }
    if (T->ptr[1])
    { // num个参数已经匹配完，还有实参表达式
        semantic_error(T->pos, "", "函数调用参数太多");
        return 0;
    }
    return 1;
}
//参数列表
void args_exp(struct node *T)
{
    T->ptr[0]->offset = T->offset;
    Exp(T->ptr[0]);
    T->width = T->ptr[0]->width;
    T->code = T->ptr[0]->code;
    if (T->ptr[1])
    {
        T->ptr[1]->offset = T->offset + T->ptr[0]->width;
        Exp(T->ptr[1]);
        T->width += T->ptr[1]->width;
        T->code = merge(2, T->code, T->ptr[1]->code);
    }
}
void const_var_list(struct node *T)
{
    if (!T->ptr[0])
        return;
    // 语义分析之前先设置偏移地址
    T->ptr[0]->offset = T->offset;
    semantic_Analysis(T->ptr[0]); //访问定义列表中的第一个
    // 之后合并 code
    T->code = T->ptr[0]->code;

    // 可为空
    if (T->ptr[1])
    {
        T->ptr[1]->offset = T->ptr[0]->offset + T->ptr[0]->width;
        semantic_Analysis(T->ptr[1]); //访问该定义列表中的其它定义
        T->code = merge(2, T->code, T->ptr[1]->code);
    }
}

void func_def(struct node *T) // kind type name ---params -- block
{
    // if (!strcmp(T->ptr[0]->type_id, "int"))
    // {
    //     T->ptr[1]->type = INT;
    // }
    // if (!strcmp(T->ptr[0]->type_id, "void"))
    // {
    //     T->ptr[1]->type = TOK_VOID;
    // }

    //填写函数定义信息到符号表
    T->width = 0;   //函数的宽度设置为0，不会对外部变量的地址分配产生影响
    T->offset = DX; //设置局部变量在活动记录中的偏移量初值
    //处理函数名
    int rtn;
    struct opn opn1, opn2, result;
    rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, 'F', 0);
    if (rtn == -1)
    {
        semantic_error(T->pos, T->type_id, "函数名重复使用，可能是函数重复定义，语义错误");
        return;
    }
    else
        T->place = rtn;
    result.kind = ID;
    strcpy(result.id, T->type_id);
    result.offset = rtn;
    T->code = genIR(FUNCTION, opn1, opn2, result);
    T->offset = DX;
    if (T->ptr[0])
    {
        //判断是否有参数
        T->ptr[0]->offset = T->offset;
        semantic_Analysis(T->ptr[0]); //处理函数参数列表
        T->width = T->ptr[0]->width;
        symbolTable.symbols[rtn].paramnum = T->ptr[0]->num;
        T->code = merge(2, T->code, T->ptr[0]->code);
    }
    else
        symbolTable.symbols[rtn].paramnum = 0, T->width = 0;
    semantic_Analysis(T->ptr[1]);  //处理函数名和参数结点部分，这里不考虑用寄存器传递参数
    T->offset += T->ptr[1]->width; //用形参单元宽度修改函数局部变量的起始偏移量
    T->ptr[2]->offset = T->offset;
    strcpy(T->ptr[2]->Snext, newLabel()); //函数体语句执行结束后的位置属性
    semantic_Analysis(T->ptr[2]);         //处理函数体结点
    //计算活动记录大小,这里offset属性存放的是活动记录大小，不是偏移
    symbolTable.symbols[T->ptr[1]->place].offset = T->offset + T->ptr[2]->width;
    T->code = merge(3, T->ptr[1]->code, T->ptr[2]->code, genLabel(T->ptr[2]->Snext)); //函数体的代码作为函数的代码
}

void func_dec(struct node *T)
{
    int rtn;
    struct opn opn1, opn2, result;
    rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, 'F', 0); //函数不在数据区中分配单元，偏移量为0
    if (rtn == -1)
    {
        semantic_error(T->pos, T->type_id, "函数名重复使用，可能是函数重复定义，语义错误");
        return;
    }
    else
        T->place = rtn;

    result.kind = ID;
    strcpy(result.id, T->type_id);
    result.offset = rtn;
    T->code = genIR(FUNCTION, opn1, opn2, result); //生成中间代码：FUNCTION 函数名
    T->offset = DX;                                //设置形式参数在活动记录中的偏移量初值
    if (T->ptr[0])
    {
        //判断是否有参数
        T->ptr[0]->offset = T->offset;
        semantic_Analysis(T->ptr[0]); //处理函数参数列表
        T->width = T->ptr[0]->width;
        symbolTable.symbols[rtn].paramnum = T->ptr[0]->num;
        T->code = merge(2, T->code, T->ptr[0]->code); //连接函数名和参数代码序列
    }
    else
        symbolTable.symbols[rtn].paramnum = 0, T->width = 0;
}
void param_list(struct node *T)
{
    T->ptr[0]->offset = T->offset;
    semantic_Analysis(T->ptr[0]);
    if (T->ptr[1])
    {
        T->ptr[1]->offset = T->offset + T->ptr[0]->width;
        semantic_Analysis(T->ptr[1]);
        T->num = T->ptr[0]->num + T->ptr[1]->num;             //统计参数个数
        T->width = T->ptr[0]->width + T->ptr[1]->width;       //累加参数单元宽度
        T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code); //连接参数代码
    }
    else
    {
        T->num = T->ptr[0]->num;
        T->width = T->ptr[0]->width;
        T->code = T->ptr[0]->code;
    }
}
void param_dec(struct node *T)
{
    int rtn;
    struct opn opn1, opn2, result;
    rtn = fillSymbolTable(T->ptr[1]->type_id, newAlias(), 1, T->ptr[0]->type, 'P', T->offset);
    if (rtn == -1)
        semantic_error(T->ptr[1]->pos, T->ptr[1]->type_id, "参数名重复定义,语义错误");
    else
        T->ptr[1]->place = rtn;
    T->num = 1;                                //参数个数计算的初始值
    T->width = T->ptr[0]->type == INT ? 4 : 8; //参数宽度
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[rtn].alias);
    result.offset = T->offset;
    T->code = genIR(PARAM, opn1, opn2, result); //生成：FUNCTION 函数名
}
void block(struct node *T)
{
    LEV++; //设置层号加1，并且保存该层局部变量在符号表中的起始位置在symbol_scope_TX
    symbol_scope_TX.TX[symbol_scope_TX.top++] = symbolTable.index;
    T->width = 0;
    T->code = NULL;
    T->ptr[0]->offset = T->offset;            //传递偏移
    strcpy(T->ptr[0]->Snext, T->Snext);       //传递next
    strcpy(T->ptr[0]->Sbreak, T->Sbreak);     // while语句的break
    strcpy(T->ptr[0]->Scontinu, T->Scontinu); // continue
    semantic_Analysis(T->ptr[0]);
    T->code = T->ptr[0]->code;
    // print_symbol();                                                  //c在退出一个符合语句前显示的符号表
    LEV--;                                                         //出复合语句，层号减1
    symbolTable.index = symbol_scope_TX.TX[--symbol_scope_TX.top]; //删除该作用域中的符号
}
void block_list(struct node *T)
{
    T->width = 0;
    T->code = NULL;
    if (T->ptr[0])
    { //为跳转语句生成标号
        if (T->ptr[0]->kind == IF_THEN || T->ptr[0]->kind == IF_THEN_ELSE || T->ptr[0]->kind == TOK_WHILE)
        {
            strcpy(T->ptr[0]->Snext, newLabel());
        }
        strcpy(T->ptr[0]->Snext, T->Snext);       // S.next属性向下传递
        strcpy(T->ptr[0]->Sbreak, T->Sbreak);     // while语句的break
        strcpy(T->ptr[0]->Scontinu, T->Scontinu); // continue
        T->ptr[0]->offset = T->offset;
        semantic_Analysis(T->ptr[0]); //
        T->width += T->ptr[0]->width;
        T->code = T->ptr[0]->code;
    }
    if (T->ptr[1])
    {
        // 2条以上语句连接,S.next属性向下传递
        strcpy(T->ptr[1]->Snext, T->Snext);
        strcpy(T->ptr[1]->Sbreak, T->Sbreak);     // while语句的break
        strcpy(T->ptr[1]->Scontinu, T->Scontinu); // continue
        T->ptr[1]->offset = T->offset + T->width;
        semantic_Analysis(T->ptr[1]); //处理复合语句的语句序列
        //序列中第1条为if，if else,while,(break,continu)?时，第2条前需要标号
        if (T->ptr[0]->kind == IF_THEN || T->ptr[0]->kind == IF_THEN_ELSE || T->ptr[0]->kind == TOK_WHILE)
        {
            T->code = merge(3, T->code, genLabel(T->ptr[0]->Snext), T->ptr[1]->code);
        }
        else
            T->code = merge(2, T->code, T->ptr[1]->code);

        T->width += T->ptr[1]->width;
    }
}

void if_then(struct node *T)
{
    strcpy(T->ptr[0]->Etrue, newLabel()); //设置条件语句真假转移位置
    strcpy(T->ptr[0]->Efalse, T->Snext);
    strcpy(T->ptr[0]->Sbreak, T->Sbreak);     // while语句的break
    strcpy(T->ptr[0]->Scontinu, T->Scontinu); // continue
    T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
    boolExp(T->ptr[0]);
    T->width = T->ptr[0]->width;
    strcpy(T->ptr[1]->Snext, T->Snext);
    strcpy(T->ptr[1]->Sbreak, T->Sbreak);     // while语句的break
    strcpy(T->ptr[1]->Scontinu, T->Scontinu); // continue
    semantic_Analysis(T->ptr[1]);             // if子句
    if (T->width < T->ptr[1]->width)
        T->width = T->ptr[1]->width;
    T->code = merge(3, T->ptr[0]->code, genLabel(T->ptr[0]->Etrue), T->ptr[1]->code);
}

void if_then_else(struct node *T)
{
    strcpy(T->ptr[0]->Etrue, newLabel()); //设置条件语句真假转移位置
    strcpy(T->ptr[0]->Efalse, newLabel());
    T->ptr[0]->offset = T->ptr[1]->offset = T->ptr[2]->offset = T->offset;
    boolExp(T->ptr[0]); //条件，要单独按短路代码处理
    T->width = T->ptr[0]->width;
    strcpy(T->ptr[1]->Snext, T->Snext);
    strcpy(T->ptr[1]->Sbreak, T->Sbreak);     // while语句的break
    strcpy(T->ptr[1]->Scontinu, T->Scontinu); // continue
    semantic_Analysis(T->ptr[1]);             // if子句
    if (T->width < T->ptr[1]->width)
        T->width = T->ptr[1]->width;
    strcpy(T->ptr[2]->Snext, T->Snext);
    strcpy(T->ptr[2]->Sbreak, T->Sbreak);     // while语句的break
    strcpy(T->ptr[2]->Scontinu, T->Scontinu); // continue
    semantic_Analysis(T->ptr[2]);             // else子句
    if (T->width < T->ptr[2]->width)
        T->width = T->ptr[2]->width;
    T->code = merge(6, T->ptr[0]->code, genLabel(T->ptr[0]->Etrue), T->ptr[1]->code,
                    genGoto(T->Snext), genLabel(T->ptr[0]->Efalse), T->ptr[2]->code);
}

void while_dec(struct node *T)
{
    strcpy(T->ptr[0]->Etrue, newLabel()); //子结点继承属性的计算
    strcpy(T->ptr[0]->Efalse, T->Snext);
    T->ptr[0]->offset = T->ptr[1]->offset = T->offset;
    boolExp(T->ptr[0]); //循环条件，要单独按短路代码处理
    T->width = T->ptr[0]->width;
    strcpy(T->ptr[1]->Snext, newLabel());
    strcpy(T->ptr[1]->Sbreak, T->Snext);
    strcat(T->ptr[1]->Scontinu, T->ptr[1]->Snext);
    semantic_Analysis(T->ptr[1]); //循环体
    if (T->width < T->ptr[1]->width)
        T->width = T->ptr[1]->width;
    T->code = merge(5, genLabel(T->ptr[1]->Snext), T->ptr[0]->code, genLabel(T->ptr[0]->Etrue), T->ptr[1]->code, genGoto(T->ptr[1]->Snext));
}

void return_dec(struct node *T)
{
    int num;
    struct opn opn1, opn2, result;
    if (T->ptr[0])
    {
        T->ptr[0]->offset = T->offset;
        Exp(T->ptr[0]);
        num = symbolTable.index;
        do
            num--;
        while (symbolTable.symbols[num].flag != 'F');
        if (T->ptr[0]->type != symbolTable.symbols[num].type)
        {
            semantic_error(T->pos, "返回值类型错误，语义错误", "");
            T->width = 0;
            T->code = NULL;
            return;
        }
        T->width = T->ptr[0]->width;
        result.kind = ID;
        strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
        result.offset = symbolTable.symbols[T->ptr[0]->place].offset;
        T->code = merge(2, T->ptr[0]->code, genIR(TOK_RETURN, opn1, opn2, result));
    }
    else
    {
        T->width = 0;
        result.kind = 0;
        T->code = genIR(TOK_RETURN, opn1, opn2, result);
    }
}

void break_stmt(struct node *T)
{
    T->code = genGoto(T->Sbreak);
}

void continue_stmt(struct node *T)
{
    T->code = genGoto(T->Scontinu);
}

void make_uid(struct codenode *head)
{
    int uid = 100;
    struct codenode *temp = head;
    do
    {
        temp->UID = uid;
        if (temp->op != LABEL)
        {
            uid++;
        }
        temp = temp->next;
    } while (temp != head);
}
void change_label(struct codenode *head)
{
    struct codenode *hcode = head;
    do
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
                        sprintf(hcode->result.id, "%d", temp->UID);
                        break;
                    }
                }
                temp = temp->next;
            }
        }
        hcode = hcode->next;
    } while (hcode != head);
    hcode = head;
    do
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
    } while (hcode != head);
}