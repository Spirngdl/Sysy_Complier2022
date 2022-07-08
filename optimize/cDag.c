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
        if (dagnode->v_num == op && dagnode->left == l && dagnode->right == r && dagnode->tri == t) //通过语句op和子节点
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
//判断带有附加标识符symbol的结点是否表示一个字面常量
bool isLiteralNode(DAG *dag, char *symbol)
{
    DAGnode *n = findnode_symbol(dag, symbol);
    if (n == NULL)
        return false;
    if (n->left != -1 && n->right == -1 && n->tri == -1)
    {
        void *node;
        dag->nodes->get_at(dag->nodes, n->left, &node);
        if (((DAGnode *)node)->kind == LITERAL)
            return true;
    }
    if (n->right != -1 && n->left == -1 && n->tri == -1)
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
            vector[sum++] = i;
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
    if (T->op == GOTO || T->op == JLE || T->op == JLT || T->op == JGE || T->op == JGT || T->op == EQ || T->op == NEQ)
    {
        dag->jumperRec = T;
        T->prior = NULL;
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
    else if (T->op == TOK_ADD || T->op == TOK_SUB || T->op == TOK_DIV || T->op == TOK_MUL || T->op == TOK_MODULO)
    {
        return readquad2(dag, T);
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
                addSymbol(n1, T->result.id);
                // TODO: 添加节点
                //  nodes.emplace_back(n1);
                dag->nodes->push_back(dag->nodes, n2);
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
    if (T->opn1.kind == LITERAL && T->opn2.kind == LITERAL) //应该不存在
    {
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
            n1->v_num = T->op;
            // TODO: 判断T.kind FAR 是数组相关
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
int readquad3(DAG *dag, struct codenode *T)
{
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

    void *elem;
    struct opn result, opn1, opn2;
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
    //对于非叶子结点v_num中保存OP
    struct codenode *e = genIR(n->v_num, opn1, opn2, result);

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
        // TODO: 姑且先认为叶结点和symlist为空的结点无用
        if (isLeaf(cur_dagnode) || cur_dagnode->symList->size(cur_dagnode->symList) == 0 /*|| isFutileSet(node)*/) //记录各结点是否被访问过，叶子和无用的赋值初始化就认为是访问过的，即不生成代码
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
                int self = findNode(dag, node->ID);
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
                dag->functionrec->next = result;
                dag->functionrec->prior = result->prior;
                result->prior = dag->functionrec;
                result = dag->functionrec;
            }
            if (dag->endfunction != NULL)
            {
                result->prior->next = dag->endfunction;
                dag->endfunction->prior = result->prior;
                dag->endfunction->next = result;
                result->prior = dag->endfunction;
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