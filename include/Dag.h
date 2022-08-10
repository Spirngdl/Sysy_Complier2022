/**
 * @file Dag.h
 * @author your name (you@domain.com)
 * @brief dag用
 * @version 0.1
 * @date 2022-07-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DAG_H_
#define DAG_H_

#include "def.h"
typedef struct DAGNode_
{
    // vector<string> symList

    List *symList; //<char *>
    int ID;        //标识身份
    int kind;
    struct DAGNode_ *left, *right, *tri;
    // int left;
    // int right;
    // int tri;
    char value[4];
    union
    {
        char v_id[10];
        int v_num;
        float v_float;
    };
    int isvisited;
    // 对于形如 A [ I ] = X 的操作，其之后需要杀死依赖于 A 的所有结点
    bool isKilled;
    // 数组操作的还原顺序
    size_t arrOptSerial;
    //函数调用操作的还原顺序
    size_t callOptSerial;
    struct DAGNode_ *next; //迫不得已，好蠢的一个操作
} DAGnode;

typedef struct DAG_
{
    // vector<DagNode> nodes
    List *nodes; //<DAGnode*>

    // 强制跳转和停机语句不生成DAG，仅暂存
    struct codenode *jumperRec;
    struct codenode *haltRec;
    //函数声明语句
    struct codenode *functionrec;
    //函数结束
    struct codenode *endfunction;
    //数组还原顺序
    size_t arrOptSerial;
    //函数调用顺序
    size_t callOptSerial;
    //判断是否有ARG需要调一下三地址代码
    bool has_arg;
} DAG;

// DAGNODE
DAGnode *create_dagnode();                   // newnode
bool isLeaf(DAGnode *node);                  //判断是否为叶子结点
void addSymbol(DAGnode *node, char *target); //添加附加
bool isLeaf();
void removeSymbol_dagnode(DAGnode *node, char *target); //删除附加
// void removeSymbol(DAGnode *node, char *target); //删除附加

// DAG
int findNode(DAG *dag, int ID); //返回结点target在DAG中的索引
DAG *creat_dag();
DAGnode *findnode_symbol(DAG *dag, char *target);                              //通过symbol查找结点
DAGnode *findNode_value(DAG *dag, char *value, int l, int r, int t);           //通过value及子节点来查找结点
DAGnode *findNode_OP_value(DAG *dag, int op, int l, int r, int t);             //通过语句类型和子节点
DAGnode *find_value_num(DAG *dga, float value, int l, int r, int t, int kind); //通过Literal查找或 FLOAT_LITERAL
bool isLiteralNode(DAG *dag, char *symbol);                                    //判断带有附加标识符symbol的结点是否表示一个字面常量
DAGnode *findNode_ArrayAssign(DAG *dag, int kind, int l, int r);               //
float getLiteral(DAG *dag, DAGnode *n);                                        //取得一个字面常量结点所表示的常量值
void removeSymbol(DAG *dag, char *symbol);                                     //删除DAG上所有等于target的附加标识符
int findnode_depend_on(DAG *dag, DAGnode *n, int vector[]);                    //查找DAG上所有依赖于n的结点
int readquad(DAG *dag, struct codenode **T);                                   //读取三地址代码
int readquad0(DAG *dag, struct codenode *T);                                   // 0型
int readquad1(DAG *dag, struct codenode *T);                                   // 1型 暂时是return
int readquad2(DAG *dag, struct codenode *T);                                   // 2型
int readquad2_2(DAG *dag, struct codenode *T);                                 //主要处理数组引用
int readquad2_1(DAG *dag, struct codenode *T);                                 //
int readquad3(DAG *dag, struct codenode *T);                                   // 3型 数组赋值
int readquad4(DAG *dag, struct codenode **T);                                  // 4型我定义为函数调用
bool isRoot(DAG *dag, DAGnode *n);                                             // 判断结点 n 是否是入度为 0 的结点
bool isActivenNode(DAGnode *n, int out_count, char *outActive[]);              //判断结点 n 是否是有活跃变量的结点
// TODO: 由dag变回三地址
// std::vector<QuadExp> genCode(std::shared_ptr<DAGNode> n, const std::vector<std::string> &outActive); // 返回一个结点 n 生成的所有代码
//由dag结点变三地址
struct codenode *genOptimizedCode(DAG *dag, int out_count, char *outActive[]);
struct codenode *to_code(DAG *dag, DAGnode *n, int out_count, char *outActive[]); //暂时不考虑是不是活跃变量
bool isFutileSet(DAGnode *n, int active[]);                                       // 判断结点 n 是否代表一个无用赋值语句（形如 T = N ，其中 T 为非活跃变量）
bool isFutileASSIGN(DAG *dag, DAGnode *n, int out_count, char *outActive[]);      //暂时用来判断一个赋值语句是否无用
void dag_optimize(Blocks *blocks);                                                // dag优化接口

#endif