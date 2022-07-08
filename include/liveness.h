#define MAX_VAR 64 // 假设一个基本块中的变量最多有64个

// 获取一个基本块中三地址代码的数量
int codenode_num (struct codenode* TAC);

// 判断id是否已在use、def集合中存在，size为字符串数组的大小
int id_exist_strs (char* str, char* total1[], char* total2[], int size);

// 获取一个基本块中的use、def集合
void one_block_use_def (char* use[], char* def[], int size, Block block);

// 判断字符串是否在一个字符串数组中，不存在则返回-1，否则返回在数组中存在的位置i
int str_exist_strs (char* str, char* strTotal[], int size);

// 完成in[n]的计算，若in[n]的内容发生改变，则赋值flag为1，否则不修改flag
void one_block_in (char* use[], char* def[], char* in[], char* out[], int size, int* flag);

// 完成out[n]的计算，其中succ_num表征传入的基本块后继结点个数
void one_block_out (char* out[], char* in1[], char* in2[], int size, int succ_num);

// 根据基本块IN、OUT集合，确定空闲的寄存器，并对reg数组进行相应赋值
void available_reg (int reg[], char* in[], char* out[]);

// 将一个由TAC组成的基本块，转换为基本块结构的集合，每一基本块有且仅有一条TAC指令
void divide_TACs_to_blocks (Block* p_source, Blocks* p_dest, int num_source);

// 传入一个基本块，分别计算每个语句的USE、DEF、IN、OUT，并通过图着色算法为其分配寄存器
void one_block_tac_liveness (Block* cntr, char* in[], char* out[]);

// 根据算法进行liveness分析
void all_block_liveness (Block* cntr[], int num_block);

// 遍历所有的函数，对每一个函数进行liveness分析与寄存器分配
void all_fun_reg (Blocks* head_fun);