#include "../include/def.h"
struct codenode *node_cur;
int DAG_ID = 0;
DAGnode *deb = NULL;
DAGnode *create_dagnode()
{
    DAGnode *newnode = (DAGnode *)malloc(sizeof(DAGnode));
    if (newnode == NULL)
    {
        printf("error\n");
        return NULL;
    }
    newnode->symList = ListInit();
    newnode->left = newnode->right = newnode->tri = NULL;
    newnode->isvisited = 0;
    newnode->isKilled = false;
    newnode->kind = -1;
    newnode->ID = DAG_ID++;
    newnode->arrOptSerial = newnode->callOptSerial = 100;
    return newnode;
}
DAG *creat_dag()
{
    DAG *newnode = (DAG *)malloc(sizeof(DAG));
    newnode->arrOptSerial = 0;
    newnode->callOptSerial = 0;
    newnode->haltRec = newnode->jumperRec = NULL;
    newnode->functionrec = newnode->endfunction = NULL;
    newnode->nodes = ListInit();
    return newnode;
}
bool isLeaf(DAGnode *node)
{
    return (node->left == NULL && node->right == NULL && node->tri == NULL);
}
void addSymbol(DAGnode *node, char *target)
{
    void *element;
    List *list = node->symList;
    ListFirst(list, false);
    // char *name = (char *)malloc(sizeof(char) * 10);
    // strcpy(name, target);
    while (list->next(list, &element))
    {
        if (strcmp(target, (char *)element) == 0) //已经存在了
        {
            return;
        }
    }
    list->push_back(list, target);
}
void removeSymbol_dagnode(DAGnode *node, char *target)
{
    if (node == NULL)
    {
        return;
    }
    void *element;
    int index = 0;
    List *list = node->symList;
    ListFirst(list, false);

    while (list->next(list, &element)) //查找
    {

        if (strcmp(target, (char *)element) == 0)
        {
            break;
        }
        index++;
    }
    if (index > list->size(list))
    {
        printf("error > size\n");
        return;
    }
    list->remove(list, index);
}

// 返回结点 target 在 DAG 中的索引
// TODO: 暂且先不管，可以考虑加个ID属性
int findNode(DAG *dag, int DAG_ID)
{
    List *nodelist = dag->nodes;
    ListFirst(nodelist, false);
    void *node;
    int i = 0;
    while (nodelist->next(nodelist, &node))
    {
        if (DAG_ID == ((DAGnode *)node)->ID)
            return i;
        i++;
    }
    return -1;
}
// 通过 Symbol 查找结点
DAGnode *findnode_symbol(DAG *dag, char *target)
{
    List *nodelist = dag->nodes;
    ListFirst(nodelist, false);
    void *node;
    while (nodelist->next(nodelist, &node))
    {
        DAGnode *debug_node = (DAGnode *)node;
        List *list = debug_node->symList;
        void *element;
        ListFirst(list, false);
        while (list->next(list, &element))
        {
            if (strcmp(target, (char *)element) == 0)
            {
                return (DAGnode *)node;
            }
        }
    }
    return NULL;
}
// 通过 value 及子结点来查找结点
DAGnode *findNode_value(DAG *dag, char *value, int l, int r, int t)
{
    List *nodelist = dag->nodes;
    ListFirst(nodelist, false);
    void *node;
    while (nodelist->next(nodelist, &node))
    {
        DAGnode *dagnode = (DAGnode *)node;
        int left = dagnode->left == NULL ? -1 : dagnode->left->ID;
        int right = dagnode->right == NULL ? -1 : dagnode->right->ID;
        int tri = dagnode->tri == NULL ? -1 : dagnode->tri->ID;

        if (strcmp(dagnode->v_id, value) == 0 && left == l && right == r && tri == t)
        {
            return dagnode;
        }
    }
    return NULL;
}
//查找是否有相同的数组赋值，数组名，数组下标
DAGnode *findNode_ArrayAssign(DAG *dag, int kind, int l, int r)
{
    List *nodelist = dag->nodes;
    ListFirst(nodelist, false);
    void *node;
    while (nodelist->next(nodelist, &node))
    {
        DAGnode *dagnode = (DAGnode *)node;
        if (dagnode->kind == kind)
        {
            int left = dagnode->left == NULL ? -1 : dagnode->left->ID;
            int right = dagnode->right == NULL ? -1 : dagnode->right->ID;
            if (left == l && right == r)
                return dagnode;
        }
    }
    return NULL;
}
//通过语句类型和子节点
DAGnode *findNode_OP_value(DAG *dag, int op, int l, int r, int t)
{
    List *nodelist = dag->nodes;
    ListFirst(nodelist, false);
    void *node;
    while (nodelist->next(nodelist, &node))
    {
        DAGnode *dagnode = (DAGnode *)node;
        int left = dagnode->left == NULL ? -1 : dagnode->left->ID;
        int right = dagnode->right == NULL ? -1 : dagnode->right->ID;
        int tri = dagnode->tri == NULL ? -1 : dagnode->tri->ID;
        if (dagnode->kind == op && left == l && right == r && tri == t) //通过语句op和子节点
            return dagnode;
    }
    return NULL;
}
/**
 * @brief 通过浮点常量查找
 *
 * @param dag
 * @param value
 * @param l
 * @param r
 * @param t
 * @return DAGnode*
 */
DAGnode *find_value_float(DAG *dag, float value, int l, int r, int t)
{
    List *nodelist = dag->nodes;
    ListFirst(nodelist, false);
    void *node;
    while (nodelist->next(nodelist, &node))
    {
        DAGnode *dagnode = (DAGnode *)node;
        if (dagnode->v_float == value) //如果值相等
        {
            int left = dagnode->left == NULL ? -1 : dagnode->left->ID;
            int right = dagnode->right == NULL ? -1 : dagnode->right->ID;
            int tri = dagnode->tri == NULL ? -1 : dagnode->tri->ID;
            if (left == l && right == r && tri == t) //通过语句op和子节点
                return dagnode;
        }
    }
    return NULL;
}
//通过Literal查找
DAGnode *find_value_num(DAG *dag, float value, int l, int r, int t, int kind)
{
    List *nodelist = dag->nodes;
    ListFirst(nodelist, false);
    void *node;
    while (nodelist->next(nodelist, &node))
    {
        DAGnode *dagnode = (DAGnode *)node;
        // if (dagnode->v_num == value)
        if (dagnode->kind == kind)
        {
            if ((kind == LITERAL && dagnode->v_num == (int)value) || (kind == FLOAT_LITERAL && dagnode->v_float == value))
            {
                int left = dagnode->left == NULL ? -1 : dagnode->left->ID;
                int right = dagnode->right == NULL ? -1 : dagnode->right->ID;
                int tri = dagnode->tri == NULL ? -1 : dagnode->tri->ID;
                if (left == l && right == r && tri == t) //通过语句op和子节点
                    return dagnode;
            }
        }
    }
    return NULL;
}
//判断带有附加标识符symbol的结点是否表示一个字面常量 如果是,顺便把值上移
bool isLiteralNode(DAG *dag, char *symbol)
{
    DAGnode *n = findnode_symbol(dag, symbol);
    if (n == NULL || n->kind == CALL) //姑且认为函数调用不能表示
        return false;
    if (n->kind == LITERAL || n->kind == FLOAT_LITERAL)
        return true;
    if (n->left != NULL && n->right == NULL && n->tri == NULL) //如果只存在左子节点，且左子节点为LITERAL 或者FLOAT_LITERAL
    {
        if (n->left->kind == LITERAL)
        {
            // n->kind = LITERAL;
            // n->v_num = n->left->v_num;
            // n->left = NULL;
            return true;
        }
        else if (n->left->kind == TOK_ASSIGN)
        {
            void *elem;
            ListGetFront(n->left->symList, &elem);
            return isLiteralNode(dag, (char *)elem);
        }
        else if (n->left->kind == FLOAT_LITERAL)
        {
            // n->kind = FLOAT_LITERAL;
            // n->v_float = n->left->v_float;
            // n->left = NULL;
            return true;
        }
        // void *node;
        // dag->nodes->get_at(dag->nodes, n->left, &node);
        // if (((DAGnode *)node)->kind == LITERAL)
        // {
        //     //都查了，干脆直接把值搞上来
        //     n->v_num = ((DAGnode *)node)->v_num;
        //     return true;
        // }
    }
    if (n->right != NULL && n->left == NULL && n->tri == NULL) //如果只存在右子节点，且右子节点为LITERAL 到底什么情况下会只存在右子节点
    {
        if (n->right->kind == LITERAL)
            return true;
        // void *node;
        // dag->nodes->get_at(dag->nodes, n->right, &node);
        // if (((DAGnode *)node)->kind == LITERAL)
        //     return true;
    }
    return false;
}
//取得一个字面常量结点所表示的常量值
float getLiteral(DAG *dag, DAGnode *n)
{
    switch (n->kind)
    {
    case LITERAL:
        return n->v_num;
        break;
    case FLOAT_LITERAL:
        return n->v_float;
        break;
    default:
        break;
    }
    if (n->left)
        return getLiteral(dag, n->left);
    else
        return getLiteral(dag, n->right);
}
/**
 * @brief 判断节点n是否有活跃变量节点
 *
 * @param n
 * @param out_count
 * @param outActive
 * @return true
 * @return false
 */
bool isActivenNode(DAGnode *n, int out_count, char *outActive[])
{
    if (n == NULL)
        return false;
    if (isLeaf(n))
        return false;
    if (n->kind == ARRAY_ASSIGN || n->kind == ARRAY_DEF)
        return true;
    if (n->kind == TOK_RETURN)
        return true;
    //如果symlist中存在outactivate
    void *element;
    ListFirst(n->symList, false);
    while (ListNext(n->symList, &element))
    {
        char *name = (char *)element;
        for (int i = 0; i < out_count; i++)
        {
            if (strcmp(name, outActive[i]) == 0)
                return true;
        }
    }
    return false;
}
//删除DAG上所有等于target的附加标识符
void removeSymbol(DAG *dag, char *symbol)
{
    List *nodelist = dag->nodes;
    ListFirst(nodelist, false);
    void *node;
    while (nodelist->next(nodelist, &node))
    {
        removeSymbol_dagnode((DAGnode *)node, symbol);
    }
}

//查找DAG上所有依赖于n的结点 返回数组长度
int findnode_depend_on(DAG *dag, DAGnode *n, int vector[])
{
    int sum = 0, i = 0;
    int index = n->ID; //通过ID查找
    List *list = dag->nodes;
    ListFirst(list, false);
    void *elem;
    while (list->next(list, &elem))
    {
        DAGnode *node = (DAGnode *)elem;
        int left = node->left == NULL ? -1 : node->left->ID;
        int right = node->right == NULL ? -1 : node->right->ID;
        int tri = node->tri == NULL ? -1 : node->tri->ID;
        if (left == index || right == index || tri == index)
        {
            vector[sum] = node->ID;
            sum++;
        }
        i++;
    }
    return sum;
}

int readquad(DAG *dag, struct codenode **T)
{
    if (T == NULL)
    {
        return 0;
    }
    switch ((*T)->op)
    {
    case GOTO: //跳转指令不处理
        if (dag->jumperRec == NULL)
        {
            (*T)->prior = *T;
            dag->jumperRec = *T;
        }
        else
        {
            dag->jumperRec->next = *T;
            dag->jumperRec->prior = *T;
            (*T)->prior = dag->jumperRec;
        }
        return 0;
        break;
    case FUNCTION: //遇到FUNCTION
        dag->functionrec = *T;
        while ((*T)->next->op == PARAM)
        {
            (*T) = (*T)->next;
        }

        return 0;
        break;
    case END:
        dag->endfunction = *T;
        // T->prior = NULL;
        return 0;
        break;
    case TOK_ASSIGN:
        return readquad0(dag, *T);
        break;
    case TOK_RETURN:
    case ARRAY_DEF:
        return readquad1(dag, *T);
        break;
    case TOK_ADD:
    case TOK_SUB:
    case TOK_DIV:
    case TOK_MUL:
    case TOK_MODULO:
    case EXP_ARRAY:

        return readquad2(dag, *T);
        break;
    case JLE:
    case JLT:
    case JGE:
    case JGT:
    case EQ:
    case NEQ:
        return readquad2_1(dag, *T);
        break;
    case ARRAY_EXP:
        return readquad2_2(dag, *T);
        break;
    case ARRAY_ASSIGN: //数组赋值
        return readquad3(dag, *T);
        break;
    case ARG:
    case CALL:
        return readquad4(dag, T);
        break;
    default:
        break;
    }
    return 0;
}
//赋值
int readquad0(DAG *dag, struct codenode *T) // 0型暂时不考虑数组
{
    DAGnode *n1 = NULL, *n2 = NULL;
    //先看result，如果reult出现过，
    DAGnode *n = findnode_symbol(dag, T->result.id);
    if (n != NULL && !n->isKilled) // result出现过，删除
    {
        removeSymbol(dag, T->result.id);
    }
    if (T->opn1.kind == ID)
    {
        n2 = findnode_symbol(dag, T->opn1.id);
        if (n2 != NULL && !n2->isKilled) // opn1作为内部变量（可能活跃）出现过
        {
            addSymbol(n2, T->result.id);
        }
        else // opn1没有作为内部变量出现过，还可能作为外部变量（叶子）出现过
        {
            n2 = findNode_value(dag, T->opn1.id, -1, -1, -1);
            if (n2 == NULL) //不存在n2，创建
            {
                n2 = create_dagnode();
                n2->kind = ID;
                strcpy(n2->v_id, T->opn1.id);
                dag->nodes->push_back(dag->nodes, n2);
            }
        }
    }
    else if (T->opn1.kind == LITERAL) //如果是常数赋值
    {
        n2 = find_value_num(dag, T->opn1.const_int, -1, -1, -1, LITERAL); //查找是否已经有该值出现
        if (n2 == NULL)                                                   //如果没有
        {
            n2 = create_dagnode();
            n2->kind = LITERAL;            //先赋个kind  其实DAG结点的kind。。应该挺杂的
            n2->v_num = T->opn1.const_int; //怎么之前忘记把值附上了
            dag->nodes->push_back(dag->nodes, n2);
        }
    }
    else if (T->opn1.kind == FLOAT_LITERAL)
    {
        n2 = find_value_float(dag, T->opn1.const_float, -1, -1, -1); //查找是否已经有该值出现
        if (n2 == NULL)                                              //如果没有
        {
            n2 = create_dagnode();
            n2->kind = FLOAT_LITERAL;      //先赋个kind  其实DAG结点的kind。。应该挺杂的
            n2->v_num = T->opn1.const_int; //怎么之前忘记把值附上了
            dag->nodes->push_back(dag->nodes, n2);
        }
    }
    //查看是否有result = opn1这样的赋值，如果有则直接附上a1
    n1 = findNode_OP_value(dag, T->op, n2->ID, -1, -1);
    if (n1 != NULL && !n1->isKilled)
    {
        addSymbol(n1, T->result.id);
    }
    else //如果不存在
    {
        n1 = create_dagnode();
        n1->left = n2;
        // n1->v_num = T->op;
        n1->kind = T->op; //用kind来存储op比较好点应该
        addSymbol(n1, T->result.id);
        dag->nodes->push_back(dag->nodes, n1);
    }
    return 0;
}
/**
 * @brief 处理return 还有ARRAY_DEF
 * @return int
 */
int readquad1(DAG *dag, struct codenode *T)
{
    DAGnode *n1 = NULL, *n = NULL, *n2 = NULL;
    if (T->result.kind == ID)
    {
        n1 = findnode_symbol(dag, T->result.id);
        if (isLiteralNode(dag, T->result.id))
        {
        }
        else
        {
            if (n1 == NULL)
            {
                n1 = findNode_value(dag, T->result.id, -1, -1, -1);
                if (n1 == NULL)
                {
                    n1 = create_dagnode();
                    n1->kind = ID;
                    strcpy(n1->v_id, T->result.id);
                    ListPushBack(dag->nodes, n1);
                }
            }
        }
    }
    else if (T->result.kind == LITERAL)
    {
        n1 = find_value_num(dag, T->result.const_int, -1, -1, -1, LITERAL);
        if (n1 == NULL)
        {
            n1 = create_dagnode();
            n1->kind = LITERAL;
            n1->v_num = T->result.const_int;
            ListPushBack(dag->nodes, n1);
        }
    }
    else if (T->result.kind == FLOAT_LITERAL)
    {
        n1 = find_value_num(dag, T->result.const_float, -1, -1, -1, FLOAT_LITERAL);
        if (n1 == NULL)
        {
            n1 = create_dagnode();
            n1->kind = FLOAT_LITERAL;
            n1->v_float = T->result.const_float;
            ListPushBack(dag->nodes, n1);
        }
    }
    if (T->op == ARRAY_DEF)
    {
        n2 = find_value_num(dag, T->opn1.const_int, -1, -1, -1, LITERAL);
        if (n2 == NULL)
        {
            n2 = create_dagnode();
            n2->kind = LITERAL;
            n2->v_num = T->opn1.const_int;
            ListPushBack(dag->nodes, n2);
        }
    }
    n = create_dagnode();
    n->kind = T->op;
    n->left = n1;
    n->right = n2;
    ListPushBack(dag->nodes, n);
    return 0;
}
//(op, a1, a2, a3) 条件跳转也算read2
//要做隐式强制转换，好麻烦
int readquad2(DAG *dag, struct codenode *T)
{
    bool n2Literal = false, n3Literal = false;
    DAGnode *n1 = NULL, *n2 = NULL, *n3 = NULL;
    float val = 0, val1 = 0, val2 = 0;

    if (T->opn1.kind == LITERAL)
    {
        n2Literal = true;
        val1 = T->opn1.const_int; //直接把值赋上来
    }
    else if (T->opn1.kind == FLOAT_LITERAL)
    {
        n2Literal = true;
        val1 = T->opn1.const_float;
    }
    else if (T->opn1.kind == ID)
        if (isLiteralNode(dag, T->opn1.id))
        {
            n2Literal = true;
            n2 = findnode_symbol(dag, T->opn1.id);
            // if (n2->kind == LITERAL)
            //     val1 = n2->v_num;
            // else if (n2->kind == FLOAT_LITERAL)
            //     val1 = n2->v_float;
            val1 = getLiteral(dag, n2);
        }

    if (T->opn2.kind == LITERAL)
    {
        n3Literal = true;
        val2 = T->opn2.const_int;
    }
    else if (T->opn2.kind == FLOAT_LITERAL)
    {
        n3Literal = true;
        val2 = T->opn2.const_float;
    }
    else if (T->opn2.kind == ID)
        if (isLiteralNode(dag, T->opn2.id))
        {
            n3Literal = true;
            n3 = findnode_symbol(dag, T->opn2.id);
            // if (n3->kind == LITERAL)
            //     val2 = n3->v_num;
            // else if (n3->kind == FLOAT_LITERAL)
            //     val2 = n3->v_float;
            val2 = getLiteral(dag, n3);
        }

    if (n2Literal && n3Literal) //可以常量传播
    {
        if (T->op == TOK_ADD)
            val = val1 + val2;
        else if (T->op == TOK_SUB)
            val = val1 - val2;
        else if (T->op == TOK_MUL)
            val = val1 * val2;
        else if (T->op == TOK_DIV)
            val = val1 / val2;
        else if (T->op == TOK_MODULO)
            val = (int)val1 % (int)val2;
        int type = search_all_alias(T->result.id);
        //查看是否存在
        DAGnode *n = find_value_num(dag, val, -1, -1, -1, type);
        if (n == NULL)
        {
            n = create_dagnode();

            if (type == TOK_INT)
            {
                n->kind = LITERAL;
                n->v_num = val;
            }
            else if (type == TOK_FLOAT)
            {
                n->kind = FLOAT_LITERAL;
                n->v_float = val;
            }
            dag->nodes->push_back(dag->nodes, n);
        }
        else
            n1 = findNode_OP_value(dag, TOK_ASSIGN, n->ID, -1, -1);
        if (n1 != NULL && !n1->isKilled)
        {
            removeSymbol(dag, T->result.id);
            addSymbol(n1, T->result.id);
        }
        else
        {
            n1 = create_dagnode();
            n1->left = n;
            n1->kind = TOK_ASSIGN;
            removeSymbol(dag, T->result.id);
            addSymbol(n1, T->result.id);
            dag->nodes->push_back(dag->nodes, n1);
        }
    }
    else //存在变量
    {
        // opn1
        if (T->opn1.kind == LITERAL)
        {
            n2 = find_value_num(dag, T->opn1.const_int, -1, -1, -1, LITERAL);
            if (n2 == NULL)
            {
                n2 = create_dagnode();
                n2->kind = LITERAL;
                n2->v_num = T->opn1.const_int;
                ListPushBack(dag->nodes, n2);
            }
        }
        else if (T->opn1.kind == FLOAT_LITERAL)
        {
            n2 = find_value_num(dag, T->opn1.const_float, -1, -1, -1, FLOAT_LITERAL);
            if (n2 == NULL)
            {
                n2 = create_dagnode();
                n2->kind = FLOAT_LITERAL;
                n2->v_float = T->opn1.const_float;
                ListPushBack(dag->nodes, n2);
            }
        }
        else if (T->opn1.kind == ID)
        {
            if (n2Literal)
            {
                n2 = findnode_symbol(dag, T->opn1.id); //查找symbolist
                if (n2->left)
                    n2 = n2->left;
                else
                    n2 = n2->right;
            }
            else
            {
                n2 = findnode_symbol(dag, T->opn1.id);
                if (n2 == NULL)
                {
                    n2 = findNode_value(dag, T->opn1.id, -1, -1, -1); //查找value
                    if (n2 == NULL)
                    {
                        n2 = create_dagnode();
                        n2->kind = ID;
                        strcpy(n2->v_id, T->opn1.id);
                        ListPushBack(dag->nodes, n2);
                    }
                }
            }
        }
        // opn2
        if (T->opn2.kind == LITERAL)
        {
            n3 = find_value_num(dag, T->opn2.const_int, -1, -1, -1, LITERAL);
            if (n3 == NULL)
            {
                n3 = create_dagnode();
                n3->kind = LITERAL;
                n3->v_num = T->opn2.const_int;
                ListPushBack(dag->nodes, n3);
            }
        }
        else if (T->opn2.kind == FLOAT_LITERAL)
        {
            n3 = find_value_num(dag, T->opn2.const_float, -1, -1, -1, FLOAT_LITERAL);
            if (n3 == NULL)
            {
                n3 = create_dagnode();
                n3->kind = FLOAT_LITERAL;
                n3->v_float = T->opn2.const_float;
                ListPushBack(dag->nodes, n3);
            }
        }
        else if (T->opn2.kind == ID)
        {
            if (n3Literal)
            {
                n3 = findnode_symbol(dag, T->opn2.id); //查找symbolist
                if (n3->left)
                    n3 = n3->left;
                else
                    n3 = n3->right;
            }
            else
            {
                n3 = findnode_symbol(dag, T->opn2.id);
                if (n3 == NULL)
                {
                    n3 = findNode_value(dag, T->opn2.id, -1, -1, -1); //查找value
                    if (n3 == NULL)
                    {
                        n3 = create_dagnode();
                        n3->kind = ID;
                        strcpy(n3->v_id, T->opn2.id);
                        ListPushBack(dag->nodes, n3);
                    }
                }
            }
        }
        // result
        // if (T->op >= JLT) //如果是条件跳转
        // {
        //     n1 = create_dagnode();
        //     n1->left = n2, n1->right = n3;
        //     n1->kind = T->op;
        //     n1->v_num = T->result.const_int;
        //     ListPushBack(dag->nodes, n1);
        //     return 0;
        // }
        n1 = findNode_OP_value(dag, T->op, n2->ID, n3->ID, -1);

        if (n1 != NULL && !n1->isKilled)
        {
            removeSymbol(dag, T->result.id); //出现新的赋值，把原来位置的全删了
            addSymbol(n1, T->result.id);
        }
        else
        {
            n1 = create_dagnode();
            n1->left = n2, n1->right = n3;
            n1->kind = T->op;
            // TODO: 判断T.kind FAR 是数组相关
            if (T->op == ARRAY_EXP)
                n1->arrOptSerial = (dag->arrOptSerial)++;
            //         if (E.op == "FAR")
            // n1->arrOptSerial = (this->arrOptSerial)++;
            removeSymbol(dag, T->result.id); //出现新的赋值，把原来位置的全删了
            addSymbol(n1, T->result.id);
            // TODO：
            dag->nodes->push_back(dag->nodes, n1);
        }
    }
    return 0;
}

//数组引用
int readquad2_2(DAG *dag, struct codenode *T)
{
    DAGnode *n1 = NULL, *n2 = NULL, *n3 = NULL;
    // T.opn1 数组名，只能是ID
    n1 = findNode_value(dag, T->opn1.id, -1, -1, -1);
    if (n1 == NULL)
    {
        n1 = create_dagnode();
        n1->kind = ID;
        strcpy(n1->v_id, T->result.id);
        ListPushBack(dag->nodes, n1);
    }
    //再处理下标
    if (T->opn2.kind == LITERAL)
    {
        n2 = find_value_num(dag, T->opn2.const_int, -1, -1, -1, LITERAL);
        if (n2 == NULL)
        {
            n2 = create_dagnode();
            n2->kind = LITERAL;
            n2->v_num = T->opn2.const_int;
            ListPushBack(dag->nodes, n2);
        }
    }
    else if (T->opn2.kind == ID)
    {
        n2 = findnode_symbol(dag, T->opn2.id);
        if (n2 == NULL)
        {
            n2 = findNode_value(dag, T->opn2.id, -1, -1, -1);
            if (n2 == NULL)
            {
                n2 = create_dagnode();
                n2->kind = ID;
                strcpy(n2->v_id, T->opn2.id);
                ListPushBack(dag->nodes, n2);
            }
        }
    }
    //判断当前基本块内是否有该数组的赋值
    n3 = findNode_ArrayAssign(dag, ARRAY_ASSIGN, n1->ID, n2->ID);
    if (n3 != NULL) //如果找到了
    {

        DAGnode *n;
        //查看是否有result = opn1这样的赋值，如果有则直接附上a1
        n = findNode_OP_value(dag, TOK_ASSIGN, n3->tri->ID, -1, -1);
        if (n != NULL && !n->isKilled)
        {
            addSymbol(n, T->result.id);
        }
        else
        {
            n = create_dagnode();
            n->kind = TOK_ASSIGN;
            n->left = n3->tri;
            addSymbol(n, T->result.id);
            ListPushBack(dag->nodes, n);
        }
    }
    else
    {
        n3 = create_dagnode();
        n3->kind = T->op;
        n3->left = n1;
        n3->right = n2;
        n3->arrOptSerial = (dag->arrOptSerial)++;
        addSymbol(n3, T->result.id);
        ListPushBack(dag->nodes, n3);
    }
    return 0;
}
// (TAR, a1, a2, a3)  a1[a2] = a3 先放着
// TODO:
/**
 * @brief result[opn1] = opn2;
 *
 * @param dag
 * @param T
 * @return int
 */
int readquad3(DAG *dag, struct codenode *T)
{
    DAGnode *n1 = NULL, *n2 = NULL, *n3 = NULL, *n = NULL;
    //先找数组名
    n1 = findNode_value(dag, T->result.id, -1, -1, -1);
    if (n1 == NULL)
    {
        n1 = create_dagnode();
        n1->kind = ID;
        strcpy(n1->v_id, T->result.id);
        ListPushBack(dag->nodes, n1);
    }
    //再处理下标
    if (T->opn1.kind == LITERAL)
    {
        n2 = find_value_num(dag, T->opn1.const_int, -1, -1, -1, LITERAL);
        if (n2 == NULL)
        {
            n2 = create_dagnode();
            n2->kind = LITERAL;
            n2->v_num = T->opn1.const_int;
            ListPushBack(dag->nodes, n2);
        }
    }
    else if (T->opn1.kind == ID)
    {
        n2 = findnode_symbol(dag, T->opn1.id);
        if (n2 == NULL)
        {
            n2 = findNode_value(dag, T->opn1.id, -1, -1, -1);
            if (n2 == NULL)
            {
                n2 = create_dagnode();
                n2->kind = ID;
                strcpy(n2->v_id, T->opn1.id);
                ListPushBack(dag->nodes, n2);
            }
        }
    }
    //然后是右值
    if (T->opn2.kind == LITERAL)
    {
        n3 = find_value_num(dag, T->opn2.const_int, -1, -1, -1, LITERAL);
        if (n3 == NULL)
        {
            n3 = create_dagnode();
            n3->kind = LITERAL;
            n3->v_num = T->opn2.const_int;
            ListPushBack(dag->nodes, n3);
        }
    }
    else if (T->opn2.kind == FLOAT_LITERAL)
    {
        n3 = find_value_num(dag, T->opn2.const_float, -1, -1, -1, FLOAT_LITERAL);
        if (n3 == NULL)
        {
            n3 = create_dagnode();
            n3->kind = FLOAT_LITERAL;
            n3->v_float = T->opn2.const_float;
            ListPushBack(dag->nodes, n3);
        }
    }
    else if (T->opn2.kind == ID)
    {
        n3 = findnode_symbol(dag, T->opn2.id);
        if (n3 == NULL)
        {
            n3 = findNode_value(dag, T->opn2.id, -1, -1, -1);
            if (n3 == NULL)
            {
                n3 = create_dagnode();
                n3->kind = ID;
                strcpy(n3->v_id, T->opn2.id);
                ListPushBack(dag->nodes, n3);
            }
        }
    }

    //最后是result
    n = findNode_ArrayAssign(dag, ARRAY_ASSIGN, n1->ID, n2->ID);
    if (n == NULL)
    {
        n = create_dagnode();
        n->left = n1, n->right = n2;
        n->kind = T->op;
    }
    n->tri = n3; //赋上新值
    n->arrOptSerial = (dag->arrOptSerial++);

    ListPushBack(dag->nodes, n);
    int index = n1->ID;
    void *element = NULL;
    ListFirst(dag->nodes, false);
    while (ListNext(dag->nodes, &element))
    {
        DAGnode *node = (DAGnode *)element;
        if (node->left != NULL && node->left->ID == index)
            node->isKilled = true;
    }
    return 0;
}
/**
 * @brief 处理函数调用，计划是一个CALL节点，symlist为左值，right为函数名，left为ARG列表
 *        ARG节点中左节点对应相应的值，可能是常量也可能是ID，然后右节点为next
 *
 * @param dag
 * @param T
 * @return int
 */
int readquad4(DAG *dag, struct codenode **T)
{
    //拿到的第一个是ARG
    DAGnode *n1 = NULL, *cur = NULL, *head = NULL, *n = NULL;
    struct codenode *tcode = *T;
    //先处理参数 可能没有参数
    while (tcode->op != CALL)
    {
        n = create_dagnode();
        n->kind = ARG;
        if (tcode->result.kind == ID) //如果实参是变量
        {
            n1 = findNode_value(dag, tcode->result.id, -1, -1, -1);
            if (n1 == NULL)
            {
                n1 = findnode_symbol(dag, tcode->result.id);
                if (n1 == NULL)
                {
                    n1 = create_dagnode();
                    n1->kind = ID;
                    strcpy(n1->v_id, tcode->result.id);
                    ListPushBack(dag->nodes, n1);
                }
            }
        }
        else if (tcode->result.kind == LITERAL)
        {
            n1 = find_value_num(dag, tcode->opn1.const_int, -1, -1, -1, LITERAL);
            if (n1 == NULL)
            {
                n1 = create_dagnode();
                n1->kind = LITERAL;
                n1->v_num = tcode->opn1.const_int;
                ListPushBack(dag->nodes, n1);
            }
        }
        else if (tcode->opn1.kind == FLOAT_LITERAL)
        {
            n1 = find_value_num(dag, tcode->opn1.const_float, -1, -1, -1, FLOAT_LITERAL);
            if (n1 == NULL)
            {
                n1 = create_dagnode();
                n1->kind = FLOAT_LITERAL;
                n1->v_float = tcode->opn1.const_float;
                ListPushBack(dag->nodes, n1);
            }
        }
        n->left = n1;
        if (cur != NULL)
        {
            n->right = cur;
        }
        ListPushBack(dag->nodes, n);
        cur = n;
        tcode = tcode->next;
    }
    //然后是CALL
    n = create_dagnode();
    n->kind = CALL;
    n->left = cur;
    //还原顺序
    n->callOptSerial = (dag->callOptSerial)++;
    //先处理左值
    if (tcode->result.kind == ID) //如果有
    {
        n1 = findnode_symbol(dag, tcode->result.id);
        if (n1 != NULL && !n1->isKilled) //如果result出现过，删除
        {
            removeSymbol(dag, tcode->result.id);
        }
        addSymbol(n, tcode->result.id);
    }
    //处理函数名
    n1 = findNode_value(dag, tcode->opn1.id, -1, -1, -1);
    if (n1 == NULL)
    {
        n1 = create_dagnode();
        n1->kind = ID;
        strcpy(n1->v_id, tcode->opn1.id);
        ListPushBack(dag->nodes, n1);
    }
    n->right = n1;
    ListPushBack(dag->nodes, n);
    *T = tcode;
    return 0;
}

/**
 * @brief 处理条件跳转
 *
 * @param dag
 * @param T
 * @return int
 */
int readquad2_1(DAG *dag, struct codenode *T)
{
    DAGnode *n1 = NULL, *n2 = NULL, *n3 = NULL;
    // opn1
    if (T->opn1.kind == LITERAL)
    {
        n2 = find_value_num(dag, T->opn1.const_int, -1, -1, -1, LITERAL);
        if (n2 == NULL)
        {
            n2 = create_dagnode();
            n2->kind = LITERAL;
            n2->v_num = T->opn1.const_int;
            ListPushBack(dag->nodes, n2);
        }
    }
    else if (T->opn1.kind == FLOAT_LITERAL)
    {
        n2 = find_value_num(dag, T->opn1.const_float, -1, -1, -1, FLOAT_LITERAL);
        if (n2 == NULL)
        {
            n2 = create_dagnode();
            n2->kind = FLOAT_LITERAL;
            n2->v_float = T->opn1.const_float;
            ListPushBack(dag->nodes, n2);
        }
    }
    else if (T->opn1.kind == ID)
    {
        n2 = findnode_symbol(dag, T->opn1.id); //查找symbolist

        if (n2 == NULL)
        {
            n2 = findNode_value(dag, T->opn1.id, -1, -1, -1); //查找value
            if (n2 == NULL)
            {
                n2 = create_dagnode();
                n2->kind = ID;
                strcpy(n2->v_id, T->opn1.id);
                ListPushBack(dag->nodes, n2);
            }
        }
    }
    // opn2
    if (T->opn2.kind == LITERAL)
    {
        n3 = find_value_num(dag, T->opn2.const_int, -1, -1, -1, LITERAL);
        if (n3 == NULL)
        {
            n3 = create_dagnode();
            n3->kind = LITERAL;
            n3->v_num = T->opn2.const_int;
            ListPushBack(dag->nodes, n3);
        }
    }
    else if (T->opn2.kind == FLOAT_LITERAL)
    {
        n3 = find_value_num(dag, T->opn2.const_float, -1, -1, -1, FLOAT_LITERAL);
        if (n3 == NULL)
        {
            n3 = create_dagnode();
            n3->kind = FLOAT_LITERAL;
            n3->v_float = T->opn1.const_float;
            ListPushBack(dag->nodes, n3);
        }
    }
    else if (T->opn2.kind == ID)
    {
        n3 = findnode_symbol(dag, T->opn2.id);

        if (n3 == NULL)
        {
            n3 = findNode_value(dag, T->opn2.id, -1, -1, -1);
            if (n3 == NULL)
            {
                n3 = create_dagnode();
                n3->kind = ID;
                strcpy(n3->v_id, T->opn2.id);
                ListPushBack(dag->nodes, n3);
            }
        }
    }
    n1 = create_dagnode();
    n1->left = n2, n1->right = n3;
    n1->kind = T->op;
    n1->v_num = T->result.const_int;
    ListPushBack(dag->nodes, n1);
    return 0;
}
// 判断结点 n 是否是入度为 0 的结点
bool isRoot(DAG *dag, DAGnode *n)
{
    if (n == NULL)
        return false;
    void *elem;
    dag->nodes->first(dag->nodes, false);
    while (dag->nodes->next(dag->nodes, &elem)) //找有没有节点的子节点是n，如果有返回false
    {
        DAGnode *node = (DAGnode *)elem;
        if (node->left != NULL)
        {
            if (node->left->ID == n->ID)
                return false;
        }
        if (node->right != NULL)
        {
            if (node->right->ID == n->ID)
                return false;
        }
        if (node->tri != NULL)
        {
            if (node->tri->ID == n->ID)
                return false;
        }
    }
    return true;
}
struct codenode *to_code(DAG *dag, DAGnode *n, int out_count, char *outActive[])
{
    if (n == NULL)
        return NULL;
    if (n->kind == ARRAY_ASSIGN) // result[opn1] = opn2
    {
        struct opn result, opn1, opn2;
        void *elem;
        //下标
        // ListGetAt(dag->nodes, n->right, &elem);
        DAGnode *left_node = n->right; //
        if (isLeaf(left_node))
        {
            if (left_node->kind == LITERAL)
            {
                opn1.kind = LITERAL;
                opn1.const_int = left_node->v_num;
            }
            else if (left_node->kind == FLOAT_LITERAL)
            {
                opn1.kind = FLOAT_LITERAL;
                opn1.const_float = left_node->v_float;
            }
            else if (left_node->kind == ID)
            {
                opn1.kind = ID;
                strcpy(opn1.id, left_node->v_id);
            }
        }
        else if (isFutileASSIGN(dag, left_node, out_count, outActive)) //如果是无用赋值，那就直接拿赋值的
        {
            if (left_node->left != NULL)
            {
                if (left_node->left->kind == LITERAL)
                {
                    opn1.kind = LITERAL;
                    opn1.const_int = left_node->left->v_num;
                }
                else if (left_node->left->kind == FLOAT_LITERAL)
                {
                    opn1.kind = FLOAT_LITERAL;
                    opn1.const_float = left_node->left->v_float;
                }
                else if (left_node->left->kind == ID)
                {
                    opn1.kind = ID;
                    strcpy(opn1.id, left_node->left->v_id);
                }
            }
        }
        else //是一个有用的赋值，那就只能拿symlist了
        {
            opn1.kind = ID;
            void *sym;
            ListGetFront(left_node->symList, &sym);
            strcpy(opn1.id, (char *)sym);
        }
        //右值
        DAGnode *right_node = n->tri; //
        if (isLeaf(right_node))       //是否是叶子节点
        {
            if (right_node->kind == LITERAL)
            {
                opn2.kind = LITERAL;
                opn2.const_int = right_node->v_num;
            }
            else if (right_node->kind == FLOAT_LITERAL)
            {
                opn2.kind = FLOAT_LITERAL;
                opn2.const_float = right_node->v_float;
            }
            else if (right_node->kind == ID)
            {
                opn2.kind = ID;
                strcpy(opn2.id, right_node->v_id);
            }
        }
        else if (isFutileASSIGN(dag, right_node, out_count, outActive)) //如果是无用赋值，那就直接拿赋值的
        {
            DAGnode *assign_node = right_node->left;
            if (assign_node->kind == LITERAL)
            {
                opn2.kind = LITERAL;
                opn2.const_int = assign_node->v_num;
            }
            else if (assign_node->kind == FLOAT_LITERAL)
            {
                opn2.kind = FLOAT_LITERAL;
                opn2.const_float = assign_node->v_float;
            }
            else if (assign_node->kind == ID)
            {
                opn2.kind = ID;
                strcpy(opn2.id, assign_node->v_id);
            }
        }
        else //是一个有用的赋值，那就只能拿symlist了
        {
            opn2.kind = ID;
            void *sym;
            ListGetFront(right_node->symList, &sym);
            strcpy(opn2.id, (char *)sym);
        }
        //函数名了
        DAGnode *array_name = n->left;
        // if (isLeaf(array_name))
        // {

        // }
        result.kind = ID;
        strcpy(result.id, array_name->v_id);
        struct codenode *e = genIR(ARRAY_ASSIGN, opn1, opn2, result);
        return e;
    }
    if (n->kind == ARG)
    {
        void *elem;
        struct opn result, opn1, opn2;
        if (isLeaf(n->left))
        {
            //叶子节点也有可能是LITERAL或者ID
            opn1.kind = n->left->kind;
            if (opn1.kind == ID)
            {
                strcpy(opn1.id, n->left->v_id);
            }
            else if (opn1.kind == LITERAL)
            {
                opn1.const_int = n->left->v_num;
            }
            else if (opn1.kind == FLOAT_LITERAL)
            {
                opn1.const_float = n->left->v_float;
            }
        }
        else if (isFutileASSIGN(dag, n->left, out_count, outActive)) // TODO: 检验活跃变量
        {
            DAGnode *temp = n->left->left;
            opn1.kind = temp->kind;
            if (temp->kind == LITERAL)
                opn1.const_int = temp->v_num;
            else if (temp->kind == FLOAT_LITERAL)
                opn1.const_float = temp->v_float;
            else if (temp->kind == ID)
                strcpy(opn1.id, temp->v_id);
        }
        else //不是叶子节点
        {
            opn1.kind = ID;
            ListGetFront(n->left->symList, &elem);
            strcpy(opn1.id, (char *)elem);
        }
        result.kind = opn2.kind = NONE;
        struct codenode *e = genIR(n->kind, result, opn2, opn1);
        return e;
    }
    if (n->kind == CALL)
    {
        void *elem;
        struct opn result, opn1, opn2;
        result.kind = opn1.kind = opn2.kind = NONE;
        //先搞左值
        if (ListSize(n->symList) != 0) //如果有
        {
            ListGetFront(n->symList, &elem);
            result.kind = ID;
            strcpy(result.id, (char *)elem);
        }
        //函数名
        opn1.kind = ID;
        strcpy(opn1.id, n->right->v_id);
        struct codenode *e = genIR(n->kind, opn1, opn2, result);
        return e;
    }
    if (n->kind == TOK_RETURN)
    {
        struct opn result, opn1, opn2;
        result.kind = opn1.kind = opn2.kind = NONE;
        void *elem;
        if (n->left != NULL) //左子树不为空
        {
            if (isLeaf(n->left))
            {
                //叶子节点也有可能是LITERAL或者ID
                result.kind = n->left->kind;
                if (result.kind == ID)
                {
                    strcpy(result.id, n->left->v_id);
                }
                else if (result.kind == LITERAL)
                {
                    result.const_int = n->left->v_num;
                }
                else if (result.kind == FLOAT_LITERAL)
                {
                    result.const_float = n->left->v_float;
                }
            }
            else if (isFutileASSIGN(dag, n->left, out_count, outActive)) // TODO: 检验活跃变量
            {
                DAGnode *temp = n->left->left;
                result.kind = temp->kind;
                if (temp->kind == LITERAL)
                    result.const_int = temp->v_num;
                else if (temp->kind == FLOAT_LITERAL)
                    result.const_float = temp->v_float;
                else if (temp->kind == ID)
                    strcpy(result.id, temp->v_id);
            }
            else //不是叶子节点
            {
                result.kind = ID;
                ListGetFront(n->left->symList, &elem);
                strcpy(result.id, (char *)elem);
            }
        }
        struct codenode *e = genIR(n->kind, opn1, opn2, result);
        return e;
    }
    if (n->kind == ARRAY_DEF)
    {
        struct opn result, opn1, opn2;
        opn2.kind = NONE;
        result.kind = ID;
        strcpy(result.id, n->left->v_id);
        opn1.kind = LITERAL;
        opn1.const_int = n->right->v_num;
        struct codenode *e = genIR(n->kind, opn1, opn2, result);
        return e;
    }
    void *elem;
    struct opn result, opn1, opn2;
    opn1.kind = opn2.kind = NONE;
    //先搞result
    if (n->kind >= JLT) //如果是条件跳转
    {
        result.kind = LITERAL;
        result.const_int = n->v_num;
    }
    else
    {
        result.kind = ID; //默认result为ID
        n->symList->get_front(n->symList, &elem);
        char *result_id = (char *)elem;
        strcpy(result.id, (char *)elem);
    }
    if (n->left != NULL) //左子树不为空
    {
        if (isLeaf(n->left))
        {
            //叶子节点也有可能是LITERAL或者ID
            opn1.kind = n->left->kind;
            if (opn1.kind == ID)
            {
                strcpy(opn1.id, n->left->v_id);
            }
            else if (opn1.kind == LITERAL)
            {
                opn1.const_int = n->left->v_num;
            }
            else if (opn1.kind == FLOAT_LITERAL)
            {
                opn1.const_float = n->left->v_float;
            }
        }
        else if (isFutileASSIGN(dag, n->left, out_count, outActive)) // TODO: 检验活跃变量
        {
            DAGnode *temp = n->left->left;
            opn1.kind = temp->kind;
            if (temp->kind == LITERAL)
                opn1.const_int = temp->v_num;
            else if (temp->kind == FLOAT_LITERAL)
                opn1.const_float = temp->v_float;
            else if (temp->kind == ID)
                strcpy(opn1.id, temp->v_id);
        }
        else //不是叶子节点
        {
            opn1.kind = ID;
            ListGetFront(n->left->symList, &elem);
            strcpy(opn1.id, (char *)elem);
        }
    }
    if (n->right != NULL) //右子树不为空
    {
        if (isLeaf(n->right))
        {
            //叶子节点也有可能是LITERAL或者ID
            opn2.kind = n->right->kind;
            if (opn2.kind == ID)
            {
                strcpy(opn2.id, n->right->v_id);
            }
            else if (opn2.kind == LITERAL)
            {
                opn2.const_int = n->right->v_num;
            }
            else if (opn2.kind == FLOAT_LITERAL)
            {
                opn2.const_float = n->right->v_float;
            }
        }
        else if (isFutileASSIGN(dag, n->right, out_count, outActive)) // TODO: 检验活跃变量
        {
            DAGnode *temp = n->right->left;
            opn2.kind = temp->kind;
            if (temp->kind == LITERAL)
                opn2.const_int = temp->v_num;
            else if (temp->kind == FLOAT_LITERAL)
                opn2.const_float = temp->v_float;
            else if (temp->kind == ID)
                strcpy(opn2.id, temp->v_id);
        }
        // else if (isFutileSet(n->right, )) // TODO: 检验活跃变量
        // {
        //     //如果是直接赋值它的lieft
        //     // e.a2 = nodes[nodes[n->left]->left]->value;
        // }
        else //不是叶子节点
        {
            opn2.kind = ID;
            ListGetFront(n->right->symList, &elem);
            strcpy(opn2.id, (char *)elem);
        }
    }
    //对于非叶子结点v_num中保存OP 可能用kind保存比较好
    struct codenode *e = genIR(n->kind, opn1, opn2, result);

    opn1.kind = ID;
    opn2.kind = NONE;
    strcpy(opn1.id, result.id);
    if (n->symList->size(n->symList) != 1) //如果有额外的
    {
        n->symList->first(n->symList, false);
        while (n->symList->next(n->symList, &elem))
        {
            if (strcmp(result.id, (char *)elem) == 0)
                continue;
            strcpy(result.id, (char *)elem);
            e = merge(2, e, genIR(TOK_ASSIGN, opn1, opn2, result)); //合并双向链表
        }
    }

    return e;
}
/**
 * @brief 暂时用来判断是否无用赋值
 *
 * @param dag
 * @param n
 * @return true 是无用的
 * @return false
 */
bool isFutileASSIGN(DAG *dag, DAGnode *n, int out_count, char *outActive[])
{
    //如果一个语句是赋值语句，且子节点是叶子节点，那么如果它的symlist为空时，判断为无用。symlist 中没有活跃变量
    if (n->kind == TOK_ASSIGN)
    {
        if (isLeaf(n->left))
        {
            void *sym;
            ListFirst(n->symList, false);
            while (ListNext(n->symList, &sym))
            {
                for (int i = 0; i < out_count; i++)
                {
                    if (strcmp((char *)sym, outActive[i]) == 0) //有一个活跃变量
                        return false;
                }
            }
            return true;
        }
    }
    return false;
}
//生成优化后的代码
/**
 * @brief 生成优化后的代码
 *
 * @param dag
 * @param out_count
 * @param outActive
 * @return struct codenode*
 */
struct codenode *genOptimizedCode(DAG *dag, int out_count, char *outActive[])
{
    struct codenode *result = NULL;
    DAGnode *head_dagnode = NULL;
    DAGnode *cur_dagnode = NULL;
    // std::vector<QuadExp> result;
    bool changed = true;
    List *root_list = ListInit();
    void *elem = NULL;
    dag->nodes->first(dag->nodes, false);
    while (dag->nodes->next(dag->nodes, &elem))
    {
        if (head_dagnode == NULL)
        {
            head_dagnode = (DAGnode *)elem;
            cur_dagnode = head_dagnode;
            cur_dagnode->next = NULL;
        }
        else
        {

            cur_dagnode->next = (DAGnode *)elem;
            cur_dagnode = cur_dagnode->next;
            cur_dagnode->next = NULL;
        }
    }
    cur_dagnode = head_dagnode;
    // TODO: 删除不活跃的根节点需要活跃变量分析，可能得寄存器分配的数据
    while (changed)
    {
        changed = false;
        cur_dagnode = head_dagnode;
        while (cur_dagnode)
        {
            DAGnode *tnode = cur_dagnode;
            if (isRoot(dag, tnode) && tnode->isvisited != 1 && !isActivenNode(tnode, out_count, outActive)) //如果是根节点，且不活跃
            {
                // TODO: 从nodelist中删除
                tnode->isvisited = 1;
                changed = true;
                //  ListRemove(dag->nodes,)
                //  dagnode = NULL; // TODO: 暂时懒得改，修改这玩意是不是太麻烦了
                //  changed = true;
            }
            cur_dagnode = cur_dagnode->next;
        }
    }
    //清楚不活跃的标识符，为标识符为空的节点新增一个 si 标识符
    dag->nodes->first(dag->nodes, false);
    while (dag->nodes->next(dag->nodes, &elem))
    {
        DAGnode *temp_node = (DAGnode *)elem;
        // if(ARRAY)
        // continue;
        if (temp_node->kind == ARRAY_ASSIGN)
            continue;
        // TODO: 判断sym是否在活跃变量内，如果不是，删除

        //为不是叶节点的节点添加临时变量
        if (!isLeaf(temp_node) && temp_node->symList->size(temp_node->symList) == 0)
        {
            // char temp_name[33];
            // strcpy(temp_name,newTemp());
            char *temp_name = (char *)malloc(sizeof(char) * 10);
            strcpy(temp_name, newTemp());
            addSymbol(temp_node, temp_name);
            // temp_name = NULL;
        }
    }
    size_t symSerial = 0;
    // TODO: 啦啦啦
    //  for (auto &&node : nodes)
    //  {
    //      if (node == nullptr)
    //          continue;
    //      if (node->value == "TAR")
    //          continue;
    //     for (auto it = node->symList.begin(); it < node->symList.end();)
    //     {
    //         if (!contain(outActive, *it))
    //             it = node->symList.erase(it);
    //         else
    //             ++it;
    //     }
    //     if (!node->isLeaf() && node->symList.empty())
    //         node->symList.emplace_back(std::string{"S" + std::to_string(symSerial++)});
    // }
    // DFS自下而上生成代码
    //查找根节点  好蠢的操作
    cur_dagnode = head_dagnode;
    while (cur_dagnode != NULL)
    {
        bool isroot = isRoot(dag, cur_dagnode);
        if (isroot)
            root_list->push_back(root_list, cur_dagnode);
        // TODO: 姑且先认为叶结点和赋值语句下symlist为空的结点无用
        if (isLeaf(cur_dagnode) || isFutileASSIGN(dag, cur_dagnode, out_count, outActive) /*|| isFutileSet(node)*/) //记录各结点是否被访问过，叶子和无用的赋值初始化就认为是访问过的，即不生成代码
        {
            cur_dagnode->isvisited = 1;
        }
        cur_dagnode = cur_dagnode->next;
    }

    //依次从每个根节点dfs
    root_list->first(root_list, false);
    while (root_list->next(root_list, &elem))
    {
        DAGnode *node = (DAGnode *)elem; //根节点
        List *stk = ListInit();          //待访问的栈
        stk->push_back(stk, node);
        while (stk->size(stk) != 0) //递归遍历根节点
        {
            stk->get_front(stk, &elem); //取头
            stk->pop_front(stk);
            DAGnode *cur = (DAGnode *)elem;
            if (cur->isvisited == 1) //如果该节点被访问过，则
                continue;
            //如果对某一个要生成代码的结点，图中有它的同名叶结点
            //则必须先解决依赖于这些叶结点的结点
            List *samename = ListInit();
            List *depending = ListInit();
            int flag = 0; //判断是不是要回炉重造
            void *sym_elem;
            cur->symList->first(cur->symList, false);
            while (cur->symList->next(cur->symList, &sym_elem))
            {
                DAGnode *sameleaves = findNode_value(dag, (char *)sym_elem, -1, -1, -1);
                if (sameleaves != NULL)
                    samename->push_back(samename, sameleaves);
            }
            samename->first(samename, false);
            while (samename->next(samename, &sym_elem))
            {
                DAGnode *node = (DAGnode *)sym_elem;
                int leaf[500];
                int self = findNode(dag, cur->ID);
                int num = findnode_depend_on(dag, node, leaf);
                for (int i = 0; i < num; i++)
                {
                    if (leaf[i] == self)
                        continue;
                    dag->nodes->get_at(dag->nodes, leaf[i], &sym_elem);
                    if (((DAGnode *)sym_elem)->isvisited == 0)
                    {
                        if (flag == 0)
                            stk->push_back(stk, cur); //回去吧，你
                        stk->push_back(stk, (DAGnode *)sym_elem);
                        flag = 1;
                    }
                }
            }
            if (flag)
                continue;
            // TODO: 处理数组的，之后再写
            //根据还原顺序做
            if (cur->kind == ARRAY_ASSIGN || cur->kind == ARRAY_EXP)
            {
                List *prefArrOpt = ListInit();
                cur_dagnode = head_dagnode;
                while (cur_dagnode)
                {
                    if ((cur_dagnode->kind == ARRAY_ASSIGN || cur_dagnode->kind == ARRAY_EXP) && cur_dagnode->arrOptSerial < cur->arrOptSerial && cur_dagnode->isvisited == 0)
                        ListPushBack(prefArrOpt, cur_dagnode);
                    cur_dagnode = cur_dagnode->next;
                }
                if (ListSize(prefArrOpt) != 0)
                {
                    ListPushBack(stk, cur);
                    void *temp = NULL;
                    ListFirst(prefArrOpt, false);
                    while (ListNext(prefArrOpt, &temp))
                    {
                        ListPushBack(stk, (DAGnode *)temp);
                    }
                    continue;
                }
            }
            //函数调用的还原顺序
            if (cur->kind == CALL)
            {
                List *prefCallOpt = ListInit();
                cur_dagnode = head_dagnode;
                while (cur_dagnode)
                {
                    if (cur_dagnode->kind == CALL && cur_dagnode->isvisited == 0 && cur_dagnode->callOptSerial < cur->callOptSerial)
                        ListPushBack(prefCallOpt, cur_dagnode);
                    cur_dagnode = cur_dagnode->next;
                }
                if (ListSize(prefCallOpt) != 0)
                {
                    ListPushBack(stk, cur);
                    void *temp = NULL;
                    ListFirst(prefCallOpt, false);
                    while (ListNext(prefCallOpt, &temp))
                    {
                        ListPushBack(stk, (DAGnode *)temp);
                    }
                    continue;
                }
            }
            if (cur->left != NULL)
            {
                if (cur->left->isvisited == 0)
                {
                    if (flag == 0)
                    {
                        stk->push_front(stk, cur);
                    }
                    stk->push_front(stk, cur->left);
                    flag = 1;
                }
            }
            if (cur->right != NULL)
            {
                if (cur->right->isvisited == 0)
                {
                    if (flag == 0)
                    {
                        stk->push_front(stk, cur);
                    }
                    stk->push_front(stk, cur->right);
                    flag = 1;
                }
            }
            if (cur->tri != NULL)
            {
                if (cur->tri->isvisited == 0)
                {
                    if (flag == 0)
                    {
                        stk->push_front(stk, cur);
                    }
                    stk->push_front(stk, cur->tri);
                    flag = 1;
                }
            }
            if (flag == 0) //如果子节点全杀了
            {
                cur->isvisited = 1;
                struct codenode *gencode = to_code(dag, cur, out_count, outActive);
                result = merge(2, result, gencode);
            }
        }
    }
    // result->prior->next = NULL;
    return result;
}
/**
 * @brief DAG优化，
 *
 * @param blocks
 */
void dag_optimize(Blocks *blocks)
{
    Blocks *cur_blocks = blocks;
    struct codenode *result = NULL;
    while (cur_blocks != NULL) //遍历所有基本块
    {
        for (int i = 0; i < cur_blocks->count; i++)
        {
            char *outActive[1000];
            int out_count = search_out(cur_blocks->name, cur_blocks->block[i]->id, outActive);
            //  out_count = ii(outlive);//获取活跃变量
            struct codenode *tcode = cur_blocks->block[i]->tac_list;
            DAG *dag = creat_dag();
            DAG_ID = 0;
            while (tcode != NULL)
            {
                readquad(dag, &tcode);
                tcode = tcode->next;
            }

            result = genOptimizedCode(dag, out_count, outActive);
            if (dag->functionrec != NULL)
            {
                // dag->functionrec->next = result;
                // dag->functionrec->prior = result->prior;
                // result->prior = dag->functionrec;
                // result = dag->functionrec;
                struct codenode *tail = dag->functionrec;
                // if (tail->next->op == PARAM)
                //     tail = tail->next;
                while (tail->next->op == PARAM)
                {
                    tail = tail->next;
                }
                tail->next = dag->functionrec;
                dag->functionrec->prior = tail;
                result = merge(2, dag->functionrec, result);
            }
            if (dag->jumperRec != NULL)
            {
                if (dag->jumperRec->prior != dag->jumperRec)
                {
                    dag->jumperRec->next->next = dag->jumperRec;
                }
                else
                {
                    dag->jumperRec->next = dag->jumperRec;
                }
                result = merge(2, result, dag->jumperRec);
            }
            if (dag->endfunction != NULL)
            {
                // result->prior->next = dag->endfunction;
                // dag->endfunction->prior = result->prior;
                // dag->endfunction->next = result;
                // result->prior = dag->endfunction;
                dag->endfunction->prior = dag->endfunction->next = dag->endfunction;
                result = merge(2, result, dag->endfunction);
            }
            if (result->next != NULL)
            {
                result->prior->next = NULL;
            }
            cur_blocks->block[i]->tac_list = result;
        }
        cur_blocks = cur_blocks->next;
    }
}