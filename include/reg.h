#define MAX_REG 11    // 最大寄存器数（R4、R5 翻译自用）
#define MAX_VARS 1024 // 最大变量数

struct var_of_RIG
{
    char *name;   // 变量名
    int order;    // 节点序号
    int reg;      // 寄存器号，初始为-1，若溢出则为-2
    int rmv_flag; // 删去时所作标记，未删去则为-1
};

struct var
{
    char name[32];     // 变量名
    char fun_name[32]; // 函数名
    int reg;           // 寄存器号，溢出为-1
};

struct var vars[MAX_VARS];

// 获取IN、OUT中的变量总数
int get_var_num(char ***in, char ***out, int num_block);

// 对于变量序列中的name、order、reg、rmv_flag进行逐一赋值
void get_var_list(struct var_of_RIG *list[], char ***in, char ***out, int num_block);

// 根据一个基本块的IN或OUT集合，确定RIG中变量的边，进一步得到需要着色的图
void one_in_out_RIG(struct var_of_RIG *list[], int **RIG, char *strs[], int num_var);

// 删去一个边少于num_reg的节点，标记删去的节点，并在RIG中将其原有的边改为-1
void remove_one_point(struct var_of_RIG *list[], int **RIG, int *rmv_cnt, int num_reg, int num_var);

// 将一个被删去的节点重新添加，并为其分配相应寄存器号，其中order为该节点在list序列中的位置
void add_one_point(struct var_of_RIG *list[], int **RIG, int num_reg, int num_var, int order, const int reg[]);

// 完成寄存器分配方案，并尽可能减少变量溢出
void input_color(struct var_of_RIG *list[], int **RIG, int num_var, int num_reg, int reg_list[]);

// 函数所需条件：各基本块的IN、OUT集合（即节点关系），基本块总数（方便遍历），寄存器总数（即颜色总数），所有变量（即着色节点）
void graph_coloring(char ***in, char ***out, int num_block, int num_reg, char *fun_add, int reg_list[]);

// 内部调用
// 根据变量别名返回寄存器号，-1为变量溢出，-2为查找失败
// 全局变量不在 vars[MAX_VARS] 数组中，即会查找失败
int _search_var(char *fun_name, char *name);

//外部调用
int search_var(char *fun_name, char *name);
// 输入函数名、寄存器数组
// 函数将为所用寄存器的相应位赋 1，其余为 0
// 寄存器默认为 16 个，其中 R11、R12 不分配，为 0
int one_fun_reg(char *fun_name, int reg[]);

// 打印所有变量及其寄存器号
void print_vars(void);

void reg_param(char *fun_add, char *param[], int param_num);

int search_fun_spilling (char* fun_name);