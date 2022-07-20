#include "../include/Dag.h"
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
    newnode->left = newnode->right = newnode->tri = -1;
    newnode->isvisited = 0;
    newnode->isKilled = false;
    newnode->kind = -1;
    newnode->ID = DAG_ID++;
    return newnode;
}
DAG *creat_dag()
{
    DAG *newnode = (DAG *)malloc(sizeof(DAG));
    newnode->arrOptSerial = 0;
    newnode->haltRec = newnode->jumperRec = NULL;
    newnode->functionrec = newnode->endfunction = NULL;
    newnode->nodes = ListInit();
    return newnode;
}
bool isLeaf(DAGnode *node)
{
    return (node->left == -1 && node->right == -1 && node->tri == -1);
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
        if (strcmp(target, (char *)element) == 0)
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
        if (debug_node->ID == 1)
            deb = debug_node;
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
        if (strcmp(dagnode->v_id, value) == 0 && dagnode->left == l && dagnode->right == r && dagnode->tri == t)
            return dagnode;
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
        if (dagnode->kind == op && dagnode->left == l && dagnode->right == r && dagnode->tri == t) //通过语句op和子节点
            return dagnode;
    }
    return NULL;
}
//通过Literal查找
DAGnode *find_value_num(DAG *dag, int value, int l, int r, int t)
{
    List *nodelist = dag->nodes;
    ListFirst(nodelist, false);
    void *node;
    while (nodelist->next(nodelist, &node))
    {
        DAGnode *dagnode = (DAGnode *)node;
        if (dagnode->v_num == value && dagnode->left == l && dagnode->right == r && dagnode->tri == t) //通过语句op和子节点
            return dagnode;
    }
    return NULL;
}
//判断带有附加标识符symbol的结点是否表示一个字面常量 如果是,顺便把值上移
bool isLiteralNode(DAG *dag, char *symbol)
{
    DAGnode *n = findnode_symbol(dag, symbol);
    if (n == NULL)
        return false;
    if (n->left != -1 && n->right == -1 && n->tri == -1) //如果只存在左子节点，且左子节点为LITERAL
    {
        void *node;
        dag->nodes->get_at(dag->nodes, n->left, &node);
        if (((DAGnode *)node)->kind == LITERAL)
        {
            //都查了，干脆直接把值搞上来
            n->v_num = ((DAGnode *)node)->v_num;
            return true;
        }
    }
    if (n->right != -1 && n->left == -1 && n->tri == -1) //如果只存在右子节点，且右子节点为LITERAL
    {
        void *node;
        dag->nodes->get_at(dag->nodes, n->right, &node);
        if (((DAGnode *)node)->kind == LITERAL)
            return true;
    }
    return false;
}
//取得一个字面常量结点所表示的常量值
int getLiteral(DAG *dag, char *symbol)
{
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
    int index = findNode(dag, n->ID); //通过ID查找
    List *list = dag->nodes;
    ListFirst(list, false);
    void *elem;
    while (list->next(list, &elem))
    {
        DAGnode *node = (DAGnode *)elem;
        if (node->left == index || node->right == index || node->tri == index)
        {
            vector[sum] = i;
            sum++;
        }
        i++;
    }
    return sum;
}

int readquad(DAG *dag, struct codenode *T)
{
    if (T == NULL)
    {
        return 0;
    }
    //跳转指令不处理
    if (T->op == GOTO || T->op == JLE || T->op == JLT || T->op == JGE || T->op == JGT || T->op == EQ || T->op == NEQ || T->op == TOK_RETURN)
    {
        if (dag->jumperRec == NULL)
        {
            T->prior = T;
            dag->jumperRec = T;
        }
        else
        {
            dag->jumperRec->next = T;
            dag->jumperRec->prior = T;
            T->prior = dag->jumperRec;
        }

        return 0;
    }
    if (T->op == FUNCTION)
    {
        dag->functionrec = T;
        // T->next = NULL;
        return 0;
    }
    if (T->op == END)
    {
        dag->endfunction = T;
        // T->prior = NULL;
        return 0;
    }
    if (T->op == TOK_ASSIGN)
    {
        return readquad0(dag, T);
    }
    else if (T->op == TOK_ADD || T->op == TOK_SUB || T->op == TOK_DIV || T->op == TOK_MUL || T->op == TOK_MODULO || T->op == EXP_ARRAY)
    {
        return readquad2(dag, T);
    }
    else if (T->op == ARRAY_ASSIGN) // 数组赋值
    {
        return readquad3(dag, T);
    }
}

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
                n2->left = -1, n2->right = -1, n2->tri = -1;
                n2->kind = ID;
                strcpy(n2->v_id, T->opn1.id);
                //暂存该结点，
                // TODO:
                // nodes.emplace_back(n2);
                // result.emplace_back(nodes.size() - 1);
                dag->nodes->push_back(dag->nodes, n2);
            }
            int indexn2 = findNode(dag, n2->ID);
            //查看是否有result = opn1这样的赋值，如果有则直接附上a1
            n1 = findNode_OP_value(dag, T->op, indexn2, -1, -1);
            if (n1 != NULL && !n1->isKilled)
            {
                addSymbol(n1, T->result.id);
            }
            else //如果不存在
            {
                n1 = create_dagnode();
                n1->left = indexn2, n1->right = -1, n1->tri = -1;
                n1->v_num = T->op;
                n1->kind = T->op; //用kind来存储op比较好点应该
                addSymbol(n1, T->result.id);
                // TODO: 添加节点
                //  nodes.emplace_back(n1);
                dag->nodes->push_back(dag->nodes, n1);
                //  result.emplace_back(nodes.size() - 1);
            }
        }
    }
    else if (T->opn1.kind == LITERAL) //如果是常数赋值
    {
        n2 = find_value_num(dag, T->opn1.const_int, -1, -1, -1); //查找是否已经有该值出现
        if (n2 == NULL)                                          //如果没有
        {
            n2 = create_dagnode();
            n2->left = -1, n2->right = -1, n2->tri = -1;
            n2->kind = LITERAL;            //先赋个kind  其实DAG结点的kind。。应该挺杂的
            n2->v_num = T->opn1.const_int; //怎么之前忘记把值附上了
            dag->nodes->push_back(dag->nodes, n2);
        }
        int indexn2 = findNode(dag, n2->ID);
        //查看是否有result = opn1这样的赋值，如果有则直接附上a1
        n1 = findNode_OP_value(dag, T->op, indexn2, -1, -1);
        if (n1 != NULL && !n1->isKilled)
        {
            addSymbol(n1, T->result.id);
        }
        else //如果不存在
        {
            n1 = create_dagnode();
            n1->left = indexn2, n1->right = -1, n1->tri = -1;
            n1->v_num = T->op;
            n1->kind = T->op;
            addSymbol(n1, T->result.id);
            // TODO: 添加节点
            //  nodes.emplace_back(n1);

            dag->nodes->push_back(dag->nodes, n1);
            //  result.emplace_back(nodes.size() - 1);
        }
    }
    // TODO: reutn ?
    return 0;
}

//(op, a1, a2, a3)
int readquad2(DAG *dag, struct codenode *T)
{
    bool n2Literal = false, n3Literal = false;
    DAGnode *n1 = NULL, *n2 = NULL, *n3 = NULL;
    int val = 0, val1 = 0, val2 = 0;
    if (T->opn1.kind == LITERAL)
    {
        n2Literal = true;
        val1 = T->opn1.const_int; //直接把值赋上来
    }
    else if (T->opn1.kind == ID)
        if (isLiteralNode(dag, T->opn1.id))
        {
            n2Literal = true;
            n2 = findnode_symbol(dag, T->opn1.id);
            val1 = n2->v_num;
        }

    if (T->opn2.kind == LITERAL)
    {
        n3Literal = true;
        val2 = T->opn2.const_int;
    }
    else if (T->opn2.kind == ID)
        if (isLiteralNode(dag, T->opn2.id))
        {
            n2Literal = true;
            n3 = findnode_symbol(dag, T->opn2.id);
            val2 = n3->v_num;
        }

    if (n2Literal && n3Literal)
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
            val = val1 % val2;
        //查看是否存在
        DAGnode *n = find_value_num(dag, val, -1, -1, -1);
        if (n == NULL)
        {
            n = create_dagnode();
            n->v_num = val;
            n->kind = LITERAL;
            n->left = n->right = n->tri = -1;
            dag->nodes->push_back(dag->nodes, n);
        }
        int indexn = findNode(dag, n->ID);
        n1 = findNode_OP_value(dag, TOK_ASSIGN, indexn, -1, -1);
        if (n1 != NULL && !n1->isKilled)
        {
            removeSymbol(dag, T->result.id);
            addSymbol(n1, T->result.id);
        }
        else
        {
            n1 = create_dagnode();
            n1->left = indexn, n1->right = -1, n1->tri = -1;
            n1->kind = n1->v_num = TOK_ASSIGN;
            removeSymbol(dag, T->result.id);
            addSymbol(n1, T->result.id);
            dag->nodes->push_back(dag->nodes, n1);
        }
    }
    else
    {
        if (T->opn1.kind == LITERAL)
        {
            n2 = find_value_num(dag, T->opn1.const_int, -1, -1, -1);
        }
        else if (T->opn1.kind == ID)
        {
            n2 = findNode_value(dag, T->opn1.id, -1, -1, -1); //查找value
            if (n2 == NULL)
                n2 = findnode_symbol(dag, T->opn1.id); //查找symbolist
        }
        if (n2 == NULL) //不存在该节点
        {
            n2 = create_dagnode(); //创建节点
            n2->left = n2->right = n2->tri = -1;
            if (T->opn1.kind == LITERAL)
            {
                n2->v_num = T->opn1.const_int;
                n2->kind = LITERAL;
            }
            else if (T->opn1.kind == ID)
            {
                strcpy(n2->v_id, T->opn1.id);
                n2->kind = ID;
            }
            // TODO: 添加结点
            dag->nodes->push_back(dag->nodes, n2);
            // nodes.emplace_back(n2);
            // result.emplace_back(nodes.size() - 1);
        }
        if (T->opn2.kind == LITERAL)

            n3 = find_value_num(dag, T->opn2.const_int, -1, -1, -1);
        else if (T->opn2.kind == ID)
        {
            n3 = findNode_value(dag, T->opn2.id, -1, -1, -1);
            if (n3 == NULL)
            {
                n3 = findnode_symbol(dag, T->opn2.id);
            }
        }
        if (n3 == NULL)
        {
            n3 = create_dagnode();
            n3->left = n3->right = n3->tri = -1;
            if (T->opn2.kind == LITERAL)
            {
                n3->v_num = T->opn2.const_int;
                n3->kind = LITERAL;
            }
            else if (T->opn2.kind == ID)
            {
                strcpy(n3->v_id, T->opn2.id);
                n3->kind = ID;
            }
            // TODO: 添加结点
            dag->nodes->push_back(dag->nodes, n3);
            // nodes.emplace_back(n3);
            // result.emplace_back(nodes.size() - 1);
        }
        int indexn2 = findNode(dag, n2->ID);
        int indexn3 = findNode(dag, n3->ID);
        n1 = findNode_OP_value(dag, T->op, indexn2, indexn3, -1);

        if (n1 != NULL && !n1->isKilled)
        {
            removeSymbol(dag, T->result.id); //出现新的赋值，把原来位置的全删了
            addSymbol(n1, T->result.id);
        }
        else
        {
            n1 = create_dagnode();
            n1->left = indexn2, n1->right = indexn3, n1->tri = -1;
            n1->kind = T->op;
            // TODO: 判断T.kind FAR 是数组相关
            if (T->op == EXP_ARRAY)
                n1->arrOptSerial = dag->arrOptSerial++;
            //         if (E.op == "FAR")
            // n1->arrOptSerial = (this->arrOptSerial)++;
            removeSymbol(dag, T->result.id); //出现新的赋值，把原来位置的全删了
            addSymbol(n1, T->result.id);
            // TODO：
            dag->nodes->push_back(dag->nodes, n1);
            // removeSymbol(E.a1);
            // n1->addSymbol(E.a1);
            // nodes.emplace_back(n1);
            // result.emplace_back(nodes.size() - 1);
        }
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
    //先找函数名
    n1 = findnode_symbol(dag, T->result.id);
    if (n1 == NULL)
    {
        n1 = findNode_value(dag, T->result.id, -1, -1, -1);
        if (n1 == NULL)
        {
            n1 = create_dagnode();
            n1->left = n1->right = n1->tri = -1;
            n1->kind = ID;
            strcpy(n1->v_id, T->result.id);
            ListPushBack(dag->nodes, n1);
        }
    }
    //再处理下标
    if (T->opn1.kind == LITERAL)
    {
        n2 = find_value_num(dag, T->opn1.const_int, -1, -1, -1);
        if (n2 == NULL)
        {
            n2 = create_dagnode();
            n2->left = n2->right = n2->tri = -1;
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
                n2->left = n2->right = n2->tri = -1;
                n2->kind = ID;
                strcpy(n2->v_id, T->opn1.id);
                ListPushBack(dag->nodes, n2);
            }
        }
    }
    //然后是右值
    if (T->opn2.kind == LITERAL)
    {
        n3 = find_value_num(dag, T->opn2.const_int, -1, -1, -1);
        if (n3 == NULL)
        {
            n3 = create_dagnode();
            n3->left = n3->right = n3->tri = -1;
            n3->kind = LITERAL;
            n3->v_num = T->opn2.const_int;
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
                n3->left = n3->right = n3->tri = -1;
                n3->kind = ID;
                strcpy(n3->v_id, T->opn2.id);
                ListPushBack(dag->nodes, n3);
            }
        }
    }
    n = create_dagnode();
    n->left = findNode(dag, n1->ID), n->right = findNode(dag, n2->ID), n->tri = findNode(dag, n3->ID);
    n->kind = ARRAY_ASSIGN;
    n->arrOptSerial = (dag->arrOptSerial++);

    ListPushBack(dag->nodes, n);
    int index = findNode(dag, n1->ID);
    void *element = NULL;
    ListFirst(dag->nodes, false);
    while (ListNext(dag->nodes, &element))
    {
        DAGnode *node = (DAGnode *)element;
        if (node->left == index)
            node->isKilled = true;
    }
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
        if (node->left != -1)
        {
            void *temp = NULL;
            int left = node->left;
            dag->nodes->get_at(dag->nodes, left, &temp);
            DAGnode *debug = (DAGnode *)temp;
            if (((DAGnode *)temp)->ID == n->ID)
                return false;
        }
        if (node->right != -1)
        {
            void *temp;
            dag->nodes->get_at(dag->nodes, node->right, &temp);
            if (((DAGnode *)temp)->ID == n->ID)
                return false;
        }
        if (node->tri != -1)
        {
            void *temp;
            dag->nodes->get_at(dag->nodes, node->tri, &temp);
            if (((DAGnode *)temp)->ID == n->ID)
                return false;
        }
    }
    return true;
}
struct codenode *to_code(DAG *dag, DAGnode *n)
{
    if (n == NULL)
        return NULL;
    if (n->kind == ARRAY_ASSIGN) // result[opn1] = opn2
    {
        struct opn result, opn1, opn2;
        void *elem;
        //下标
        ListGetAt(dag->nodes, n->right, &elem);
        DAGnode *left_node = (DAGnode *)elem; //
        if (isLeaf(left_node))
        {
            if (left_node->kind == LITERAL)
            {
                opn1.kind = LITERAL;
                opn1.const_int = left_node->v_num;
            }
            else if (left_node->kind == ID)
            {
                opn1.kind = ID;
                strcpy(opn1.id, left_node->v_id);
            }
        }
        else if (isFutileASSIGN(dag, left_node)) //如果是无用赋值，那就直接拿赋值的
        {
            ListGetAt(dag->nodes, left_node->left, &elem);
            DAGnode *assign_node = (DAGnode *)elem;
            if (assign_node->kind == LITERAL)
            {
                opn1.kind = LITERAL;
                opn1.const_int = assign_node->v_num;
            }
            else if (assign_node->kind == ID)
            {
                opn1.kind = ID;
                strcpy(opn1.id, assign_node->v_id);
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
        ListGetAt(dag->nodes, n->tri, &elem);
        DAGnode *right_node = (DAGnode *)elem; //
        if (isLeaf(right_node))                //是否是叶子节点
        {
            if (right_node->kind == LITERAL)
            {
                opn2.kind = LITERAL;
                opn2.const_int = right_node->v_num;
            }
            else if (right_node->kind == ID)
            {
                opn2.kind = ID;
                strcpy(opn2.id, right_node->v_id);
            }
        }
        else if (isFutileASSIGN(dag, right_node)) //如果是无用赋值，那就直接拿赋值的
        {
            ListGetAt(dag->nodes, right_node->left, &elem);
            DAGnode *assign_node = (DAGnode *)elem;
            if (assign_node->kind == LITERAL)
            {
                opn2.kind = LITERAL;
                opn2.const_int = assign_node->v_num;
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
        ListGetAt(dag->nodes, n->left, &elem);
        DAGnode *array_name = (DAGnode *)elem;
        // if (isLeaf(array_name))
        // {

        // }
        result.kind = ID;
        strcpy(result.id, array_name->v_id);
        struct codenode *e = genIR(ARRAY_ASSIGN, opn1, opn2, result);
        return e;
    }
    void *elem;
    struct opn result, opn1, opn2;
    opn1.kind = opn2.kind = NONE;
    result.kind = ID; //默认result为ID
    n->symList->get_front(n->symList, &elem);
    // n->symList->pop_front(n->symList);
    char *result_id = (char *)elem;
    strcpy(result.id, (char *)elem);
    if (n->left != -1) //左子树不为空
    {
        dag->nodes->get_at(dag->nodes, n->left, &elem);
        if (isLeaf((DAGnode *)elem))
        {
            //叶子节点也有可能是LITERAL或者ID
            opn1.kind = ((DAGnode *)elem)->kind;
            if (opn1.kind == ID)
            {
                strcpy(opn1.id, ((DAGnode *)elem)->v_id);
            }
            else if (opn1.kind == LITERAL)
            {
                opn1.const_int = ((DAGnode *)elem)->v_num;
            }
        }
        // else if (isFutileSet(n->left, )) // TODO: 检验活跃变量
        // {
        //     //如果是直接赋值它的lieft
        //     // e.a2 = nodes[nodes[n->left]->left]->value;
        // }
        else //不是叶子节点
        {
            opn1.kind = ID;
            DAGnode *node = (DAGnode *)elem;
            elem = NULL;
            node->symList->get_front(node->symList, &elem);
            char *opn1_id = (char *)elem;
            strcpy(opn1.id, (char *)elem);
        }
    }
    if (n->right != -1) //右子树不为空
    {
        dag->nodes->get_at(dag->nodes, n->right, &elem);
        if (isLeaf((DAGnode *)elem))
        {
            //叶子节点也有可能是LITERAL或者ID
            opn2.kind = ((DAGnode *)elem)->kind;
            if (opn2.kind == ID)
            {
                strcpy(opn2.id, ((DAGnode *)elem)->v_id);
            }
            else if (opn2.kind == LITERAL)
            {
                opn2.const_int = ((DAGnode *)elem)->v_num;
            }
        }
        // else if (isFutileSet(n->right, )) // TODO: 检验活跃变量
        // {
        //     //如果是直接赋值它的lieft
        //     // e.a2 = nodes[nodes[n->left]->left]->value;
        // }
        else //不是叶子节点
        {
            opn2.kind = ID;
            DAGnode *node = (DAGnode *)elem;
            node->symList->get_front(node->symList, &elem);
            strcpy(opn2.id, (char *)elem);
        }
    }
    //对于非叶子结点v_num中保存OP 可能用kind保存比较好
    struct codenode *e = genIR(n->kind, opn1, opn2, result);

    opn1.kind = ID;
    opn2.kind = 0;
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
bool isFutileASSIGN(DAG *dag, DAGnode *n)
{
    //如果一个语句是赋值语句，且子节点是叶子节点，那么如果它的symlist为空时，判断为无用。
    if (n->kind == TOK_ASSIGN)
    {
        int left = n->left;
        void *elem = NULL;
        dag->nodes->get_at(dag->nodes, left, &elem);
        if (isLeaf((DAGnode *)elem))
        {
            if (n->symList->size(n->symList) == 0)
            {
                return true;
            }
        }
    }
    return false;
}
//生成优化后的代码
struct codenode *genOptimizedCode(DAG *dag)
{
    struct codenode *result = NULL;
    DAGnode *head_dagnode = NULL;
    DAGnode *cur_dagnode = NULL;
    // std::vector<QuadExp> result;
    bool changed = true;
    List *root_list = ListInit();
    void *elem = NULL;
    // TODO: 删除不活跃的根节点需要活跃变量分析，可能得寄存器分配的数据
    // while (changed)
    // {
    //     changed = false;
    //     List *nodes = dag->nodes;
    //     void *elem;
    //     nodes->first(nodes, false);
    //     while (nodes->next(nodes, &elem))
    //     {
    //         DAGnode *dagnode = (DAGnode *)elem;
    //         if (isRoot(dag, dagnode) && isActivenNode(dagnode))
    //         {
    //             dagnode = NULL; // TODO: 暂时懒得改，修改这玩意是不是太麻烦了
    //             changed = true;
    //         }
    //     }
    // }

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
            char *temp_name = (char *)malloc(sizeof(char) * 6);
            strcpy(temp_name, newTemp());
            addSymbol(temp_node, temp_name);
            temp_name = NULL;
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
    while (cur_dagnode != NULL)
    {
        bool isr = isRoot(dag, cur_dagnode);
        if (isr)
            root_list->push_back(root_list, cur_dagnode);
        // TODO: 姑且先认为叶结点和赋值语句下symlist为空的结点无用
        if (isLeaf(cur_dagnode) || isFutileASSIGN(dag, cur_dagnode) /*|| isFutileSet(node)*/) //记录各结点是否被访问过，叶子和无用的赋值初始化就认为是访问过的，即不生成代码
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
            if (cur->kind == ARRAY_ASSIGN || cur->kind == EXP_ARRAY)
            {
                List *prefArrOpt = ListInit();
                cur_dagnode = head_dagnode;
                while (cur_dagnode)
                {
                    if ((cur_dagnode->kind == ARRAY_ASSIGN || cur_dagnode->kind == EXP_ARRAY) && cur_dagnode->arrOptSerial < cur->arrOptSerial && cur_dagnode->isvisited == 0)
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

            //  if (cur->value == "TAR" || cur->value == "FAR")
            //  {
            //      std::vector<std::shared_ptr<DAGNode>> prefArrOpt;
            //      for (size_t i = 0; i < nodes.size(); ++i)
            //          if (
            //              (nodes[i]->value == "FAR" || nodes[i]->value == "TAR") && nodes[i]->arrOptSerial < cur->arrOptSerial && visited[i] == false)
            //              prefArrOpt.emplace_back(nodes[i]);

            //     if (!prefArrOpt.empty())
            //     {
            //         stk.push(cur);
            //         for (auto &&n : prefArrOpt)
            //             stk.push(n);
            //         continue;
            //     }
            // }

            if (cur->left != -1)
            {
                dag->nodes->get_at(dag->nodes, cur->left, &sym_elem);
                if (((DAGnode *)sym_elem)->isvisited == 0)
                {
                    if (flag == 0)
                    {
                        stk->push_front(stk, cur);
                    }
                    stk->push_front(stk, (DAGnode *)sym_elem);
                    flag = 1;
                }
            }
            if (cur->right != -1)
            {
                dag->nodes->get_at(dag->nodes, cur->right, &sym_elem);
                if (((DAGnode *)sym_elem)->isvisited == 0)
                {
                    if (flag == 0)
                    {
                        stk->push_front(stk, cur);
                    }
                    stk->push_front(stk, (DAGnode *)sym_elem);
                    flag = 1;
                }
            }
            if (cur->tri != -1)
            {
                dag->nodes->get_at(dag->nodes, cur->tri, &sym_elem);
                if (((DAGnode *)sym_elem)->isvisited == 0)
                {
                    if (flag == 0)
                    {
                        stk->push_front(stk, cur);
                    }
                    stk->push_front(stk, (DAGnode *)sym_elem);
                    flag = 1;
                }
            }
            if (flag == 0) //如果子节点全杀了
            {
                cur->isvisited = 1;
                struct codenode *gencode = to_code(dag, cur);
                result = merge(2, result, gencode);
            }
        }
    }
    // result->prior->next = NULL;
    return result;
}

void dag_optimize(Blocks *blocks)
{
    Blocks *cur_blocks = blocks;
    struct codenode *result = NULL;
    while (cur_blocks != NULL) //遍历所有基本块
    {
        for (int i = 0; i < cur_blocks->count; i++)
        {
            struct codenode *tcode = cur_blocks->block[i]->tac_list;
            DAG *dag = creat_dag();
            while (tcode != NULL)
            {
                readquad(dag, tcode);
                tcode = tcode->next;
            }
            result = genOptimizedCode(dag);
            if (dag->functionrec != NULL)
            {
                // dag->functionrec->next = result;
                // dag->functionrec->prior = result->prior;
                // result->prior = dag->functionrec;
                // result = dag->functionrec;
                dag->functionrec->next = dag->functionrec->prior = dag->functionrec;
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