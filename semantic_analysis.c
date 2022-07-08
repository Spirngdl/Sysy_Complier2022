#include "include/def.h"
// #define DD
int LEV = 0;   //层号
int func_size; //函数的活动记录大小

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
        case VAR_DECL_LIST: //处理一个局部变量定义,将第一个孩子(TYPE结点)中的类型送到第二个孩子的类型域
            var_decl_list(T);
            break;
        case VAR_DECL:
            var_decl(T);
            break;
        case ARRAY_DEC:
            array_decl(T);
            break;
        case FUNC_DEF:
            func_def(T);
            break;
        case PARAM_DEC:
            param_dec(T);
            break;
        case PARAM_LIST:
            param_list(T);
            break;
        case BLOCK:
            block(T);
            break;
        case BLOCK_LIST:
            block_list(T);
            break;
        case IF_THEN:
            if_stmt(T);
            break; //控制语句都还没有处理offset和width属性
        case IF_THEN_ELSE:
            if_else_stmt(T);
            break;
        case TOK_WHILE:
            while_stmt(T);
            break;
        case TOK_RETURN:
            return_dec(T);
            break;
        case TOK_BREAK:
            break_stmt(T);
            break;
        case TOK_CONTINUE:
            continue_stmt;
            break;
        case ID:
        case LITERAL:
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
        default:
            printf("not define\n");
            break;
        }
    }
}

void compunit_list(struct node *T)
{
    if (!T->ptr[0])
        return;
    semantic_Analysis(T->ptr[0]); //访问外部定义列表中的第一个
    // 之后合并 code
    T->code = T->ptr[0]->code;
    // 可为空
    if (T->ptr[1])
    {
        semantic_Analysis(T->ptr[1]); //访问该外部定义列表中的其它外部定义
        T->code = merge(2, T->code, T->ptr[1]->code);
    }
}
void var_decl_list(struct node *T)
{
    if (T->ptr[0] == NULL)
        return;
    T->ptr[0]->type = T->type;    //传递type TOK_INT || TOK_FLOAT
    semantic_Analysis(T->ptr[0]); //访问定义列表中的第一个
    //之后合并code
    T->code = T->ptr[0]->code;
    //可为空
    if (T->ptr[1])
    {
        T->ptr[1]->type = T->type;
        semantic_Analysis(T->ptr[1]); //访问list中的其他
        T->code = merge(2, T->code, T->ptr[1]->code);
    }
}
void array_decl(struct node *T)
{
    int rtn, num;
    int array_dimension = astsymbol.symbols[T->place].array_dimension;
    int length[10];
    memcpy(length, astsymbol.symbols[T->place].length, array_dimension * sizeof(int)); //数组复制
    rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, ARRAY);                //符号表
    if (rtn == -1)
        semantic_error(T->pos, T->type_id, "变量重复定义");
    else
    {
        List *value_list = ListInit();
        T->place = rtn; //用place记录在符号表中的位置
        symbolTable.symbols[rtn].array_dimension = array_dimension;
        memcpy(symbolTable.symbols[T->place].length, length, array_dimension * sizeof(int)); //数组复制

        if (T->ptr[0] != NULL) //数组初始化
        {
            int temp_width[10], width = 1;
            int array_offset = 0;
            int brace_num = 1;
            struct node *initarray = T->ptr[0];
            for (int i = array_dimension - 1; i >= 0; i--)
            {
                width *= length[i];
                temp_width[i] = width;
            }
            if (LEV == 0) //全局变量
            {
                initarray = initarray->ptr[0];
                arrayinit_bracker(value_list, initarray, brace_num, &array_offset, temp_width, array_dimension);
                symbolTable.symbols[rtn].value = value_list;
            }
            else //局部变量
            {
            }
        }
    }
}
//暂时对错误检查还有欠缺
void arrayinit_bracker(List *value_list, struct node *T, int brace_num, int *array_offset, int width[], int dimension)
{
    if (T == NULL)
        return;
    if (T->kind == INITARRAY) //表示一个大括号
    {
        brace_num++;
        arrayinit_bracker(value_list, T->ptr[0], brace_num, array_offset, width, dimension);
        brace_num--;
        arrayinit_bracker(value_list, T->ptr[1], brace_num, array_offset, width, dimension);
    }
    else
    {
        struct node *initarray = T;
        int temp_width = width[brace_num - 1];
        while (*array_offset >= temp_width)
        {
            temp_width += width[brace_num - 1];
        }
        int final_offset = temp_width;
        while (initarray != NULL && initarray->kind != INITARRAY)
        {
            int const_value = const_exp(initarray);
            push_initvalue(const_value, value_list);
            (*array_offset)++;
            initarray = initarray->ptr[1];
        }
        if (initarray == NULL)
        {
            if (*array_offset > final_offset)
            {
                semantic_error(T->pos, "初始值设定项值太多", ".");
            }
            else
            {
                if (brace_num > 1) //大括号数量大于1
                {
                    for (int i = 0; i < final_offset - *array_offset; i++) //补0
                    {
                        push_initvalue(0, value_list);
                    }
                    (*array_offset) = final_offset;
                }
            }
        }
        else if (initarray->kind == INITARRAY)
        {
            if (brace_num + 1 > dimension)
                semantic_error(T->pos, "多余大括号", ".");
            else
                arrayinit_bracker(value_list, initarray, brace_num, array_offset, width, dimension);
        }
    }
}
int array_index(struct node *T, int i, int offset) //生成数组下标
{
    struct opn opn1, opn2, opn3, result;
    int place = temp_add(newTemp(), LEV, TOK_INT, TEMP_VAR);
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[place].alias);
    opn1.kind = INT;
    opn1.const_int = i;
    opn2.kind = INT;
    opn2.const_int = 4;
    T->code = merge(2, T->code, genIR(TOK_MUL, opn1, opn2, result)); //生成三地址代码
    if (offset)                                                      //存在偏移，即是多维数组
    {
        opn3.kind = INT;
        opn3.const_int = offset;
        place = temp_add(newTemp(), LEV, TOK_INT, TEMP_VAR);
        opn1.kind = ID;
        strcpy(opn1.id, symbolTable.symbols[place].alias);
        T->code = merge(2, T->code, genIR(TOK_ADD, opn3, result, opn1)); //三地址代码
    }
    return place; //返回符号表中的位置
}
void var_decl(struct node *T) //变量声明 -- kind -- NAME --TYPE
{
    int rtn, num;
    struct opn opn1, opn2, opn3, result;
    T->code = NULL;

    rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, VAR); //填符号表
    if (rtn == -1)
        semantic_error(T->pos, T->type_id, "变量重复定义");
    else
        T->place = rtn;    //用place记录在符号表中的位置
    if (T->ptr[0] != NULL) //赋值
    {
        if (T->type == TOK_INT)
        {
            if (T->ptr[0]->kind == LITERAL)
            {
                opn1.kind = LITERAL;
                opn1.const_int = T->ptr[0]->type_int;
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T->place].alias); //赋上别名
#ifdef DD
                if (symbolTable.symbols[T->place].flag != TEMP_VAR)
                {
                    strcpy(result.id, symbolTable.symbols[T->place].name);
                    strcpy(result.alice, symbolTable.symbols[T->place].alias);
                }
#endif
                T->code = merge(2, T->code, genIR(TOK_ASSIGN, opn1, opn2, result)); //合并三地址代码
            }
            else if (T->ptr[0]->kind == FLOAT_LITERAL) //给整型赋值浮点，直接转
            {
                opn1.kind = LITERAL;
                opn1.const_int = T->ptr[0]->type_float; //转换
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T->place].alias); //赋上别名
#ifdef DD
                if (symbolTable.symbols[T->place].flag != TEMP_VAR)
                {
                    strcpy(result.id, symbolTable.symbols[T->place].name);
                    strcpy(result.alice, symbolTable.symbols[T->place].alias);
                }
#endif
                T->code = merge(2, T->code, genIR(TOK_ASSIGN, opn1, opn2, result)); //合并三地址代码
            }
            else if (T->ptr[0]->kind == ID) //是变量赋初始值
            {
                opn1.kind = ID;
                Exp(T->ptr[0]);
                strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias); //赋上别名
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T->place].alias);             //赋上别名
                T->code = merge(2, T->code, genIR(TOK_ASSIGN, opn1, opn2, result)); //合并三地址代码
            }
            else
            {
                Exp(T->ptr[0]);
                struct codenode *lassign = T->ptr[0]->code->prior; //右值语句的最后一句
                strcpy(lassign->result.id, symbolTable.symbols[T->place].alias);
#ifdef DD
                if (symbolTable.symbols[T->place].flag != TEMP_VAR)
                {
                    strcpy(result.id, symbolTable.symbols[T->place].name);
                    strcpy(result.alice, symbolTable.symbols[T->place].alias);
                }
#endif
                T->code = merge(2, T->code, T->ptr[0]->code);
            }
        }
        else if (T->type == TOK_FLOAT)
        {
            if (T->ptr[0]->kind == LITERAL) //浮点数赋值整型。
            {
                opn1.kind = FLOAT_LITERAL;
                opn1.const_float = T->ptr[0]->type_int; //转换
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T->place].alias); //赋上别名
#ifdef DD
                if (symbolTable.symbols[T->place].flag != TEMP_VAR)
                {
                    strcpy(result.id, symbolTable.symbols[T->place].name);
                    strcpy(result.alice, symbolTable.symbols[T->place].alias);
                }
#endif
                T->code = merge(2, T->code, genIR(TOK_ASSIGN, opn1, opn2, result)); //合并三地址代码
            }
            else if (T->ptr[0]->kind == FLOAT_LITERAL) //
            {
                opn1.kind = FLOAT_LITERAL;
                opn1.const_float = T->ptr[0]->type_float;
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[T->place].alias); //赋上别名
#ifdef DD
                if (symbolTable.symbols[T->place].flag != TEMP_VAR)
                {
                    strcpy(result.id, symbolTable.symbols[T->place].name);
                    strcpy(result.alice, symbolTable.symbols[T->place].alias);
                }
#endif
                T->code = merge(2, T->code, genIR(TOK_ASSIGN, opn1, opn2, result)); //合并三地址代码
            }
            else
            {
                Exp(T->ptr[0]);
                struct codenode *lassign = T->ptr[0]->code->prior; //右值语句的最后一句
                strcpy(lassign->result.id, symbolTable.symbols[T->place].alias);
#ifdef DD
                if (symbolTable.symbols[T->place].flag != TEMP_VAR)
                {
                    strcpy(result.id, symbolTable.symbols[T->place].name);
                    strcpy(result.alice, symbolTable.symbols[T->place].alias);
                }
#endif
                T->code = merge(2, T->code, T->ptr[0]->code);
            }
        }
    }
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
        case LITERAL:
        case FLOAT_LITERAL:
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
        case TOK_ADD: //二元表达式
        case TOK_SUB:
        case TOK_MUL:
        case TOK_DIV:
        case TOK_MODULO:
        case TOK_AND:
        case TOK_OR:
        case TOK_LESS:
        case TOK_GREAT:
        case TOK_LESSEQ:
        case TOK_GREATEQ:
        case TOK_EQ:
        case TOK_NOTEQ:
            op_exp(T);
            break;
        case UNARYEXP:
            unaryexp(T);
            break;
        case EXP_ARRAY:
            rval_array(T);
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
    }
}
void int_exp(struct node *T)
{
    struct opn opn1, opn2, result;
    T->place = temp_add(newTemp(), LEV, T->type, TEMP_VAR); //为整常量生成一个临时变量
    T->type = T->kind;
    opn1.kind = T->kind;
    opn1.kind = T->kind;
    if (T->kind == LITERAL)
        opn1.const_int = T->type_int;
    else if (T->kind == FLOAT_LITERAL)
        opn1.const_float = T->type_float;
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[T->place].alias);
    T->code = genIR(TOK_ASSIGN, opn1, opn2, result);
}
//算术运算，+-*/
void op_exp(struct node *T)
{
    struct opn opn1, opn2, result;
    //下面的类型属性计算，没有考虑错误处理情况
    // TODO: 处理float
    T->type = TOK_INT;
    T->place = temp_add(newTemp(), LEV, T->type, TEMP_VAR); //声明临时变量
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[T->place].alias);
    //常量
    if (T->ptr[0]->kind == LITERAL) //左子树为整数常数
    {
        T->ptr[0]->code = NULL;
        opn1.kind = LITERAL;
        opn1.const_int = T->ptr[0]->type_int;
    }
    else if (T->ptr[0]->kind == FLOAT_LITERAL) //右子树为浮点数
    {
        T->ptr[0]->code = NULL;
        opn1.kind = FLOAT_LITERAL;
        opn1.const_float = T->ptr[0]->type_float;
    }
    else
    {
        Exp(T->ptr[0]);
        opn1.kind = ID;
        strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
#ifdef DD
        if (symbolTable.symbols[T->ptr[0]->place].flag != TEMP_VAR)
        {
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].name);
            strcpy(opn1.alice, symbolTable.symbols[T->ptr[0]->place].alias);
        }
#endif
    }
    if (T->ptr[1]->kind == LITERAL)
    {
        T->ptr[1]->code = NULL;
        opn2.kind = LITERAL;
        opn2.const_int = T->ptr[1]->type_int;
    }
    else if (T->ptr[1]->kind == FLOAT_LITERAL)
    {
        T->ptr[1]->code = NULL;
        opn2.kind = FLOAT_LITERAL;
        opn2.const_float = T->ptr[1]->type_float;
    }
    else
    {
        Exp(T->ptr[1]);
        opn2.kind = ID;
        strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
#ifdef DD
        if (symbolTable.symbols[T->ptr[1]->place].flag != TEMP_VAR)
        {
            strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].name);
            strcpy(opn2.alice, symbolTable.symbols[T->ptr[1]->place].alias);
        }
#endif
    }
    T->code = merge(3, T->ptr[0]->code, T->ptr[1]->code, genIR(T->kind, opn1, opn2, result));
}
//单
void unaryexp(struct node *T)
{
    struct opn opn1, opn2, result;
    Exp(T->ptr[1]);
    //判断T->ptr[0]，T->ptr[1]类型是否正确，可能根据运算符生成不同形式的代码，给T的type赋值
    //下面的类型属性计算，没有考虑错误处理情况
    T->type = TOK_INT;
    T->place = temp_add(newTemp(), LEV, T->type, TEMP_VAR);
    opn1.kind = ID;
    strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias);
#ifdef DD
    if (symbolTable.symbols[T->ptr[1]->place].flag != TEMP_VAR)
    {
        strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].name);
        strcpy(opn1.alice, symbolTable.symbols[T->ptr[1]->place].alias);
    }
#endif
    opn2.kind = NONE;
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[T->place].alias);
    T->code = merge(2, T->ptr[1]->code, genIR(T->ptr[0]->kind, opn1, opn2, result));
}
//赋值
void assignop_exp(struct node *T)
{
    struct opn opn1, opn2, result;
    if (T->ptr[0]->kind == ID)
    {
        Exp(T->ptr[0]);                 //处理左值，例中仅为变量
        if (T->ptr[1]->kind == LITERAL) //
        {
            opn1.kind = LITERAL;
            opn1.const_int = T->ptr[1]->type_int;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
#ifdef DD
            if (symbolTable.symbols[T->ptr[0]->place].flag != TEMP_VAR)
            {
                strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].name);
                strcpy(result.alice, symbolTable.symbols[T->ptr[0]->place].alias);
            }
#endif
            T->code = merge(2, T->ptr[0]->code, genIR(TOK_ASSIGN, opn1, opn2, result));
        }
        else if (T->ptr[1]->kind == ID)
        {
            T->type = T->ptr[1]->type;
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
#ifdef DD
            if (symbolTable.symbols[T->ptr[0]->place].flag != TEMP_VAR)
            {
                strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].name);
                strcpy(result.alice, symbolTable.symbols[T->ptr[0]->place].alias);
            }
#endif
            Exp(T->ptr[1]);
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias);
#ifdef DD
            if (symbolTable.symbols[T->ptr[1]->place].flag != TEMP_VAR)
            {
                strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].name);
                strcpy(opn1.alice, symbolTable.symbols[T->ptr[1]->place].alias);
            }
#endif
            T->code = merge(2, T->ptr[0]->code, genIR(TOK_ASSIGN, opn1, opn2, result));
        }
        else
        {
            Exp(T->ptr[1]); //处理右值
            T->type = T->ptr[1]->type;
            opn1.kind = ID;
            struct codenode *lassign = T->ptr[1]->code->prior; //右值语句的最后一句
            strcpy(lassign->result.id, symbolTable.symbols[T->ptr[0]->place].alias);
#ifdef DD
            if (symbolTable.symbols[T->ptr[0]->place].flag != TEMP_VAR)
            {
                strcpy(lassign->result.id, symbolTable.symbols[T->ptr[0]->place].name);
                strcpy(lassign->result.alice, symbolTable.symbols[T->ptr[0]->place].alias);
            }
#endif
            // strcpy(opn1.id, symbolTable.symbols[T->ptr[1]->place].alias); //右值一定是个变量或临时变量
            T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code);
        }
    }
    else if (T->ptr[0]->kind == EXP_ARRAY) //数组作为左值
    {
        exp_array(T->ptr[0]); //处理左值
        Exp(T->ptr[1]);
        T->type = T->ptr[0]->type;

        if (symbolTable.symbols[T->ptr[0]->ptr[0]->place].type == LITERAL)
        {
            opn1.kind = LITERAL;
            opn1.const_int = symbolTable.symbols[T->ptr[0]->ptr[0]->place].array_dimension;
        }
        else
        {
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->ptr[0]->place].alias); // index
        }
        opn2.kind = ID;
        strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias); //右值一定是个变量或临时变量

        result.kind = ID;
        strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);

        T->code = merge(3, T->ptr[0]->code, T->ptr[1]->code, genIR(ARRAY_ASSIGN, opn1, opn2, result));
    }
    else
    {
        semantic_error(T->pos, "", "赋值语句没有左值，语义错误");
    }
}
void rval_array(struct node *T) //数组作为右值
{
    exp_array(T); //先进这边处理，下面基本只是凑 temp = arr[t];
    struct opn opn1, opn2, result;
    int place = temp_add(newTemp(), LEV, TOK_INT, TEMP_VAR);
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[place].alias);
    opn1.kind = ID;
    strcpy(opn1.id, symbolTable.symbols[T->place].alias); //数组名
    if (symbolTable.symbols[T->ptr[0]->place].type == LITERAL)
    {
        opn2.kind = LITERAL;
        opn2.const_int = symbolTable.symbols[T->ptr[0]->place].array_dimension;
    }
    else /*if (symbolTable.symbols[T->ptr[0]->place].type == ID)*/
    {
        opn2.kind = ID;
        strcpy(opn2.id, symbolTable.symbols[T->ptr[0]->place].alias); // index
    }
    T->code = merge(2, T->code, genIR(ARRAY_EXP, opn1, opn2, result));
    T->place = place;
}
/**
 * @brief dd
 *
 * @param T d
 */
void exp_array(struct node *T) //数组处理比较核心的地方
{
    struct opn opn1, opn2, result;
    int rtn = searchSymbolTable(T->type_id); //先处理数组ID
    if (rtn == -1)
        semantic_error(T->pos, T->type_id, "变量未声明定义就引用，语义错误");
    if (symbolTable.symbols[rtn].flag == 'F')
        semantic_error(T->pos, T->type_id, "是函数名，不是普通变量，语义错误");
    else
    {
        T->place = rtn; //结点保存变量在符号表中的位置
        T->code = NULL; //标识符不需要生成TAC
        T->type = symbolTable.symbols[rtn].type;
    }
    //处理下标
    int width[10];
    width[symbolTable.symbols[rtn].array_dimension - 1] = 1;
    for (int i = symbolTable.symbols[rtn].array_dimension - 1; i > 0; i--)
    {
        width[i - 1] = width[i] * symbolTable.symbols[rtn].length[i];
    }
    int place = exp_index(T->ptr[0], 0, width); //处理下标
    T->code = merge(2, T->code, T->ptr[0]->code);
    T->ptr[0]->place = place;
}
// place 传递符号表位置 index 第几维
int exp_index(struct node *T, int index, int width[]) //处理数组引用的下标
{
    struct opn opn1, opn2, result;
    int place;
    int offset = width[index];

    if (T->ptr[0]->kind == LITERAL)
    {
        place = temp_add(newTemp(), LEV, LITERAL, TEMP_VAR);
        if (T->ptr[1] != NULL) // 有下一维.
        {
            int temp_place = exp_index(T->ptr[1], index + 1, width);
            if (symbolTable.symbols[temp_place].type = LITERAL)
            {
                result.const_int = offset * T->ptr[0]->type_int + symbolTable.symbols[temp_place].array_dimension;
                symbolTable.symbols[place].array_dimension = result.const_int;
                symbolTable.symbols[place].type = LITERAL;
            }
            else
            {
                opn1.kind = LITERAL;
                opn1.const_int = T->ptr[0]->type_int * offset;
                opn2.kind = ID;
                strcpy(opn2.id, symbolTable.symbols[temp_place].alias);
                result.kind = ID;
                strcpy(result.id, symbolTable.symbols[place].alias);
                symbolTable.symbols[place].type = ID;
                T->code = merge(2, T->code, genIR(TOK_ADD, opn1, opn2, result));
            }
        }
        else //无下一维
        {
            symbolTable.symbols[place].type = LITERAL;
            symbolTable.symbols[place].array_dimension = T->ptr[0]->type_int * offset;
        }
    }
    else //是变量
    {
        Exp(T->ptr[0]); //处理第一个得到i1

        // i1 * offset
        place = mul_exp(T->ptr[0], symbolTable.symbols[T->ptr[0]->place].alias, offset);
        T->code = merge(2, T->code, T->ptr[0]->code);
        if (T->ptr[1] != NULL) //处理下一个
        {
            int temp_place = exp_index(T->ptr[1], index + 1, width);
            if (symbolTable.symbols[temp_place].type == LITERAL)
            {
                opn2.kind = LITERAL;
                opn2.const_int = symbolTable.symbols[temp_place].array_dimension;
            }
            else
            {
                opn2.kind = ID;
                strcpy(opn2.id, symbolTable.symbols[temp_place].alias);
            }
            opn1.kind = ID;
            strcpy(opn1.id, symbolTable.symbols[place].alias);

            place = temp_add(newTemp(), LEV, TOK_INT, TEMP_VAR);
            strcpy(result.id, symbolTable.symbols[place].alias);

            T->code = merge(3, T->code, T->ptr[1]->code, genIR(TOK_ADD, opn1, opn2, result));
        }
    }

    return place;
}
int mul_exp(struct node *T, char *i, int offset)
{
    struct opn opn1, opn2, result;
    opn1.kind = ID;
    strcpy(opn1.id, i);
    opn2.kind = LITERAL;
    opn2.const_int = offset;

    int place = temp_add(newTemp(), LEV, TOK_INT, TEMP_VAR);
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[place].alias);
    T->code = merge(2, T->code, genIR(TOK_MUL, opn1, opn2, result));
    return place;
}
//布尔表达式
void boolExp(struct node *T, char *Etrue, char *Efalse) //二代
{
    struct opn opn1, opn2, result;
    int op;
    int rtn;
    char mi_label[20];
    if (T)
    {
        switch (T->kind)
        {
        case LITERAL: //常数
            if (T->type_int != 0)
                T->code = genGoto(Etrue);
            else
                T->code = genGoto(Efalse);
            break;
        case ID: //查符号表，获得符号表中的位置，类型送type
            rtn = searchSymbolTable(T->type_id);
            if (rtn == -1)
                semantic_error(T->pos, T->type_id, "变量未定义，语义错误");
            if (symbolTable.symbols[rtn].flag == 'F')
                semantic_error(T->pos, T->type_id, "不是函数名，不能进行函数调用");
            else
            {
                opn1.kind = ID;
                strcpy(opn1.id, symbolTable.symbols[rtn].alias); //传递别名

                opn2.kind = LITERAL;
                opn2.const_int = 0;
                result.kind = ID;
                strcpy(result.id, Etrue);
                T->code = genIR(NEQ, opn1, opn2, result); //生成三地址代码
                T->code = merge(2, T->code, genGoto(Efalse));
            }

            break;
        case TOK_LESS:
        case TOK_GREAT:
        case TOK_LESSEQ:
        case TOK_GREATEQ:
        case TOK_EQ:
        case TOK_NOTEQ: //处理关系运算表达式,2个操作数都按基本表达式处理
            if (T->ptr[0]->kind == LITERAL)
            {
                opn1.kind = LITERAL;
                opn1.const_int = T->ptr[0]->type_int;
            }
            else
            {
                Exp(T->ptr[0]); //处理左
                opn1.kind = ID;
                strcpy(opn1.id, symbolTable.symbols[T->ptr[0]->place].alias);
            }
            if (T->ptr[1]->kind == LITERAL)
            {
                T->ptr[1]->code = NULL;
                opn2.kind = LITERAL;
                opn2.const_int = T->ptr[1]->type_int;
            }
            else
            {
                Exp(T->ptr[1]); //处理右
                opn2.kind = ID;
                strcpy(opn2.id, symbolTable.symbols[T->ptr[1]->place].alias);
            }
            result.kind = ID;
            strcpy(result.id, Etrue); // B.true
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
            T->code = genIR(op, opn1, opn2, result);                                        // if ... GOTO B.true
            T->code = merge(4, T->ptr[0]->code, T->ptr[1]->code, T->code, genGoto(Efalse)); // GOTO B.false
            break;
        case TOK_AND:
            strcpy(mi_label, newLabel());
            boolExp(T->ptr[0], mi_label, Efalse); //处理左边的表达式
            boolExp(T->ptr[1], Etrue, Efalse);    //处理右边表达式
            T->code = merge(3, T->ptr[0]->code, genLabel(mi_label), T->ptr[1]->code);
        case TOK_OR:
            strcpy(mi_label, newLabel());
            boolExp(T->ptr[0], Etrue, mi_label);
            boolExp(T->ptr[1], Etrue, Efalse);
            T->code = merge(3, T->ptr[0]->code, genLabel(mi_label), T->ptr[1]->code);
        case UNARYEXP:
            if (T->ptr[0]->kind == TOK_NOT)
                boolExp(T->ptr[1], Efalse, Etrue);
            else
                boolExp(T->ptr[1], Etrue, Efalse);
            T->code = T->ptr[1]->code;
        default:
            break;
        }
    }
}
//函数调用
void func_call_exp(struct node *T)
{
    int rtn;
    struct node *T0;
    struct opn opn1, opn2, result;
    rtn = searchSymbolTable(T->type_id);
    if (rtn == -1)
    {
        semantic_error(T->pos, T->type_id, "函数未定义，语义错误");
        return;
    }
    if (symbolTable.symbols[rtn].flag != FUNCTION)
    {
        semantic_error(T->pos, T->type_id, "不是函数名，不能进行函数调用，语义错误");
        return;
    }
    T->type = symbolTable.symbols[rtn].type;
    if (T->ptr[0]) //参数列表
    {
        args_exp(T->ptr[0]); //处理所有实参表达式求值，及类型
        T->code = T->ptr[0]->code;
    }
    else
    {
        T->code = NULL;
    }
    match_param(rtn, T->ptr[0]);
    //处理参数列表的中间代码
    T0 = T->ptr[0];
    while (T0)
    {
        result.kind = ID;
        strcpy(result.id, symbolTable.symbols[T0->ptr[0]->place].alias);
        T->code = merge(2, T->code, genIR(ARG, opn1, opn2, result));
        T0 = T0->ptr[1];
    }
    T->place = temp_add(newTemp(), LEV, T->type, TEMP_VAR);
    opn1.kind = ID;
    strcpy(opn1.id, T->type_id); //保存函数名
    //暂时不知道啥用opn1.offset = rtn;           //这里offset用以保存函数定义入口,在目标代码生成时，能获取相应信息
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[T->place].alias);
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
    Exp(T->ptr[0]);
    T->code = T->ptr[0]->code;
    if (T->ptr[1])
    {
        args_exp(T->ptr[1]);
        T->code = merge(2, T->code, T->ptr[1]->code);
    }
}

void func_def(struct node *T) // kind type name ---params -- block
{

    //填写函数定义信息到符号表
    //处理函数名
    int rtn;
    struct opn opn1, opn2, result;
    rtn = fillSymbolTable(T->type_id, newAlias(), LEV, T->type, FUNCTION);
    if (rtn == -1)
    {
        semantic_error(T->pos, T->type_id, "函数名重复使用，可能是函数重复定义，语义错误");
        return;
    }
    else
        T->place = rtn;
    result.kind = ID;
    strcpy(result.id, T->type_id);
    T->code = genIR(FUNCTION, opn1, opn2, result);
    if (T->ptr[0]) //判断是否有参数
    {
        T->ptr[0]->type_int = 0;      //统计函数参数个数
        semantic_Analysis(T->ptr[0]); //处理函数参数列表
        symbolTable.symbols[rtn].paramnum = T->ptr[0]->type_int;
        T->code = merge(2, T->code, T->ptr[0]->code);
    }
    else
        symbolTable.symbols[rtn].paramnum = 0;

    semantic_Analysis(T->ptr[1]); //处理函数体结点

    T->code = merge(3, T->code, T->ptr[1]->code, genIR(END, opn1, opn1, opn1)); //函数体的代码作为函数的代码
}

void param_list(struct node *T) // PARAM_LIST
{
    semantic_Analysis(T->ptr[0]); //第一个参数
    if (T->ptr[1])
    {
        semantic_Analysis(T->ptr[1]);
        T->type_int = T->ptr[0]->type_int + T->ptr[1]->type_int; //统计参数个数
        T->code = merge(2, T->ptr[0]->code, T->ptr[1]->code);    //连接参数代码
    }
    else
    {
        T->type_int = T->ptr[0]->type_int;
        T->code = T->ptr[0]->code;
    }
}
void param_dec(struct node *T) //函数形参声明
{
    int rtn;
    struct opn opn1, opn2, result;
    rtn = fillSymbolTable(T->type_id, newAlias(), 1, T->type, PARAM); //函数参数的层数设为1
    if (rtn == -1)
        semantic_error(T->pos, T->type_id, "参数名重复定义,语义错误");
    else
        T->place = rtn;
    T->type_int = 1; //参数个数计算的初始值
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[rtn].alias);
    T->code = genIR(PARAM, opn1, opn2, result); //生成：PARAM 参数名
}
void param_array(struct node *T)
{
    int rtn, num;
    struct opn opn1, opn2, result;
    int array_dimension = astsymbol.symbols[T->place].array_dimension;
    int length[10];
    length[0] = 0;
    if (array_dimension > 1)
    {
        memcpy(length, astsymbol.symbols[T->place].length, array_dimension * sizeof(int)); //数组复制
    }
    rtn = fillSymbolTable(T->type_id, newAlias(), 1, T->type, PARAM_ARRAY); //符号表
    if (rtn == -1)
        semantic_error(T->pos, T->type_id, "变量重复定义");
    else
    {
        T->place = rtn; //用place记录在符号表中的位置
        symbolTable.symbols[rtn].array_dimension = array_dimension;
        memcpy(astsymbol.symbols[T->place].length, length, array_dimension * sizeof(int)); //数组复制
    }
    T->type_int = 1;
    result.kind = ID;
    strcpy(result.id, symbolTable.symbols[rtn].alias);
    T->code = genIR(PARAM, opn1, opn2, result); //生成：PARAM 参数名
}
void block(struct node *T)
{
    LEV++; //设置层号加1，并且保存该层局部变量在符号表中的起始位置在symbol_scope_TX
    symbol_scope_TX.TX[symbol_scope_TX.top++] = symbolTable.index;
    T->code = NULL;
    semantic_Analysis(T->ptr[0]);
    T->code = T->ptr[0]->code;
    // print_symbol();                                                  //c在退出一个符合语句前显示的符号表
    LEV--;                                                         //出复合语句，层号减1
    symbolTable.index = symbol_scope_TX.TX[--symbol_scope_TX.top]; //删除该作用域中的符号
}
void block_list(struct node *T)
{
    T->code = NULL;
    if (T->ptr[0])
    {
        semantic_Analysis(T->ptr[0]); //处理第一个语句
        T->code = T->ptr[0]->code;
    }
    if (T->ptr[1])
    {
        semantic_Analysis(T->ptr[1]); //处理复合语句的语句序列
        T->code = merge(2, T->code, T->ptr[1]->code);
    }
}
void if_stmt(struct node *T)
{
    //两个子节点，一个判断语句，一个代码块
    char Etrue[20];
    char Efalse[20];
    strcpy(Etrue, newLabel());
    strcpy(Efalse, newLabel());
    boolExp(T->ptr[0], Etrue, Efalse); //处理判断语句
    semantic_Analysis(T->ptr[1]);      //处理代码块
    T->code = merge(4, T->ptr[0]->code, genLabel(Etrue), T->ptr[1]->code, genLabel(Efalse));
}

void if_else_stmt(struct node *T)
{
    char Etrue[20];
    char Efalse[20];
    char Enext[20];
    strcpy(Etrue, newLabel());
    strcpy(Efalse, newLabel());
    strcpy(Enext, newLabel());
    boolExp(T->ptr[0], Etrue, Efalse); //处理if处判断，为false则跳else
    semantic_Analysis(T->ptr[1]);      // if的代码块
    if (T->ptr[2]->kind == IF_THEN)    //遇到else if情况传递Enext
        else_if_stmt(T->ptr[2], Enext);
    else if (T->ptr[2]->kind == IF_THEN_ELSE)
        else_if_else_stmt(T->ptr[2], Enext);
    else
        semantic_Analysis(T->ptr[2]); //开始else
    T->code = merge(7, T->ptr[0]->code, genLabel(Etrue), T->ptr[1]->code, genGoto(Enext), genLabel(Efalse), T->ptr[2]->code, genLabel(Enext));
}
void else_if_stmt(struct node *T, char *Efalse) // else if的情况且无else
{
    //两个子节点，一个判断语句，一个代码块
    char Etrue[20];
    strcpy(Etrue, newLabel());
    boolExp(T->ptr[0], Etrue, Efalse); //处理判断语句
    semantic_Analysis(T->ptr[1]);      //处理代码块
    T->code = merge(4, T->ptr[0]->code, genLabel(Etrue), T->ptr[1]->code, genLabel(Efalse));
}
void else_if_else_stmt(struct node *T, char *Enext) // else if else情况跟if else情况相同但是Enext由上级传递
{
    char Etrue[20];
    char Efalse[20];
    strcpy(Etrue, newLabel());
    strcpy(Efalse, newLabel());

    boolExp(T->ptr[0], Etrue, Efalse); //处理if处判断，为false则跳else

    semantic_Analysis(T->ptr[1]); // if的代码块

    if (T->ptr[2]->kind == IF_THEN) //遇到else if情况传递Enext
        else_if_stmt(T->ptr[2], Enext);
    else if (T->ptr[2]->kind == IF_THEN_ELSE)
        else_if_else_stmt(T->ptr[2], Enext);
    else
        semantic_Analysis(T->ptr[2]); //开始else

    T->code = merge(5, T->ptr[0]->code, genLabel(Etrue), T->ptr[1]->code, genGoto(Enext), genLabel(Efalse), T->ptr[2]);
}

void while_stmt(struct node *T)
{
    char Etrue[20];
    char Efalse[20];
    char Enext[20];
    strcpy(Etrue, newLabel());
    strcpy(Efalse, newLabel());
    strcpy(Enext, newLabel());

    boolExp(T->ptr[0], Etrue, Efalse);

    semantic_Analysis(T->ptr[1]); //处理循环体

    T->code = merge(6, genLabel(Enext), T->ptr[0]->code, genLabel(Etrue), T->ptr[1]->code, genGoto(Enext), genLabel(Efalse)); //合并代码
    //开始回填
    struct codenode *h = T->code;
    do
    {
        if (h->op == BACK)
        {
            h->op = GOTO;
            if (h->result.kind == TOK_BREAK)
            {
                strcpy(h->result.id, Efalse);
            }
            else if (h->result.kind == TOK_CONTINUE)
            {
                strcpy(h->result.id, Enext);
            }
        }
        h = h->next;
    } while (h != T->code);
}

void return_dec(struct node *T) //
{
    int num;
    struct opn opn1, opn2, result;
    if (T->ptr[0]) //有返回值
    {
        num = symbolTable.index;
        do
            num--;
        while (symbolTable.symbols[num].flag != FUNCTION); //遇到第一个即本函数
        if (T->ptr[0]->kind == LITERAL)                    //如果返回值为常数
        {
            if (symbolTable.symbols[num].type != TOK_INT)
            {
                semantic_error(T->pos, "返回值类型错误，语义错误", "");
                T->code = NULL;
                return;
            }
            else
            {
                result.const_int = T->ptr[0]->type_int;
                result.kind = LITERAL;
            }
        }
        else
        {
            Exp(T->ptr[0]);
            if (T->ptr[0]->type != symbolTable.symbols[num].type) //类型检查
            {
                semantic_error(T->pos, "返回值类型错误，语义错误", "");
                T->code = NULL;
                return;
            }
            result.kind = ID;
            strcpy(result.id, symbolTable.symbols[T->ptr[0]->place].alias);
        }
        T->code = merge(2, T->ptr[0]->code, genIR(TOK_RETURN, opn1, opn2, result));
    }
    else
    {
        result.kind = 0;
        T->code = genIR(TOK_RETURN, opn1, opn2, result);
    }
}

void break_stmt(struct node *T)
{
    T->code = genback(TOK_BREAK);
}

void continue_stmt(struct node *T)
{
    T->code = genback(TOK_CONTINUE);
}
