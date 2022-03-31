//基本块
typedef struct _Blocks Blocks;
typedef struct _Block Block;
struct _Blocks //分函数的基本块
{
    Block *block[100];
    int count;
    char name[20];
    Blocks *next;
    Blocks *pre;
};
struct _Block //最底层的基本块
{
    int id;
    struct codenode *tac_list; //中间代码集合
    int num_children;          //统计孩子结点个数
    Block *children[2];
};

Blocks *current_block; //在分块时做为尾结点，结束时变头结点

void divide_block(struct codenode *head); //判断该段代码的in和out
Block *newblock();                        //创建新块
void merge_block(Block *fa, Block *ch);   //连接
Block *find_target(int target);           //找子块
void link_block();                        //根据跳转连接
void basic_block(struct codenode *head);  //接口，划分基本块
void newBlocks(char *name);               //生成新的分函数的基本块
