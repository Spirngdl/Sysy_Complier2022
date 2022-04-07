#define MAX_REG 5  // 最大寄存器数

struct var_of_RIG {
    char* name; // 变量名
    int order;  // 节点序号
    int reg;    // 寄存器号，初始为-1，若溢出则为-2
    int rmv_flag; // 删去时所作标记，未删去则为-1
};

int get_var_num (char*** in, char*** out, int num_block); // 获取IN、OUT中的变量总数
void get_var_list (struct var_of_RIG* list[], char*** in, char*** out, int num_block); // 对于变量序列中的name、order、reg、rmv_flag进行逐一赋值
void one_in_out_RIG (struct var_of_RIG* list[], int** RIG, char* strs[], int num_var); // 根据一个基本块的IN或OUT集合，确定RIG中变量的边，进一步得到需要着色的图
void remove_one_point (struct var_of_RIG* list[], int** RIG, int* rmv_cnt, int num_reg, int num_var); // 删去一个边少于num_reg的节点，标记删去的节点，并在RIG中将其原有的边改为-1
void add_one_point (struct var_of_RIG* list[], int** RIG, int num_reg, int num_var, int order); // 将一个被删去的节点重新添加，并为其分配相应寄存器号，其中order为该节点在list序列中的位置
void input_color (struct var_of_RIG* list[], int** RIG, int num_var, int num_reg); // 完成寄存器分配方案，并尽可能减少变量溢出
void graph_coloring (char*** in, char*** out, int num_block, int num_reg); // 函数所需条件：各基本块的IN、OUT集合（即节点关系），基本块总数（方便遍历），寄存器总数（即颜色总数），所有变量（即着色节点）
