#include "include/def.h"

// 全局变量不参与寄存器分配，每次使用时后端部分选用一个空的寄存器，使用完成后保存于内存
// 寄存器分配以函数为单位进行，每个函数存在一个独立的分配方案，互不干扰
// 涉及到函数调用时，将所有的寄存器压栈以清空，通过子函数分配方案中的寄存器传参，运行完毕之后通过R0返回值，再将栈还原到寄存器

// 判断字符串是否在一个字符串数组中，不存在则返回-1，否则返回在数组中存在的位置i
/*int str_exist_strs (char* str, char* strTotal[], int size) {
    int i = 0;
    for (i = 0; i < size; i++) {
        if ((strTotal[i] != NULL) && (strcmp(str, strTotal[i]) == 0)) {
            return i;
        }
    }
    return -1;
}*/

int var_cnt = 0;

int next_reg (int curr_reg, const int reg[]) {
    int i = 0;
    curr_reg++;
    for (i = 0; (i < MAX_REG) && (curr_reg < MAX_REG); i++) {
        if (reg[curr_reg] == 0) return curr_reg;
        curr_reg++;
    }
    return curr_reg;
}

// 获取一个可用的寄存器号，并将相应位置 1
// 失败则返回 -1
int get_one_reg (int reg_list[]) {
    int i = 0;
    for (i = 0; i < MAX_REG; i++) {
        if (reg_list[i] == 0) {
            reg_list[i] = 1;
            return i;
        }
    }
    return -1;
}

// 获取IN、OUT中的变量总数
int get_var_num (char*** in, char*** out, int num_block) {
    int cnt = 0;
    int symbol = 0;
    int i = 0, j = 0;
    // 暂且认为最大变量数为 MAX_VAR * 10 
    char* temp[MAX_VAR * 10] = {NULL};
    for (i = 0; i < num_block; i++) {
        for (j = 0; j < MAX_VAR; j++) {
            if ((out[i][j] != NULL) && (strcmp(out[i][j], "") != 0)) {
                symbol = str_exist_strs(out[i][j], temp, MAX_VAR); // symbol为-1则不存在
                if (symbol == -1) {
                    temp[cnt] = out[i][j];
                    cnt++;
                }
            }
            if ((in[i][j] != NULL) && (strcmp(in[i][j], "") != 0)) {
                symbol = str_exist_strs(in[i][j], temp, MAX_VAR); // symbol为-1则不存在
                if (symbol == -1) {
                    temp[cnt] = in[i][j];
                    cnt++;
                }
                
            }
        }
    }
    return cnt;
}

// 对于变量序列中的name、order、reg、rmv_flag进行逐一赋值
void get_var_list (struct var_of_RIG* list[], char*** in, char*** out, int num_block) {
    /*printf("\n已进入变量序列生成中!\n");*/
    int i = 0, j = 0;
    int symbol = 0;
    int cnt = 0;
    char* temp[MAX_VAR * 10] = {NULL};
    for (i = 0; i < num_block; i++) {
        for (j = 0; j < MAX_VAR; j++) {
            if ((out[i][j] != NULL) && (strcmp(out[i][j], "") != 0)) {
                symbol = str_exist_strs(out[i][j], temp, MAX_VAR); // symbol为-1则不存在
                if (symbol == -1) {
                    temp[cnt] = out[i][j];
                    list[cnt]->name = temp[cnt];
                    list[cnt]->order = cnt;
                    list[cnt]->rmv_flag = -1;
                    list[cnt]->reg = -1;
                    cnt++;
                }
            }
            if ((in[i][j] != NULL) && (strcmp(in[i][j], "") != 0)) {
                symbol = str_exist_strs(in[i][j], temp, MAX_VAR); // symbol为-1则不存在
                if (symbol == -1) {
                    temp[cnt] = in[i][j];
                    list[cnt]->name = temp[cnt];
                    list[cnt]->order = cnt;
                    list[cnt]->rmv_flag = -1;
                    list[cnt]->reg = -1;
                    cnt++;
                }
                
            }
        }
    }
}

// 根据一个基本块的IN或OUT集合，确定RIG中变量的边，进一步得到需要着色的图
void one_in_out_RIG (struct var_of_RIG* list[], int** RIG, char* strs[], int num_var) {
    int i = 0, j = 0;
    int temp[MAX_VAR];
    for (i = 0; i < MAX_VAR; i++) {
        temp[i] = -1;
    }
    for (i = 0; i < MAX_VAR; i++) {
        for (j = 0; j < num_var; j++) {
            if (strcmp(strs[i], list[j]->name) == 0) {
                temp[list[j]->order] = 1;
            }
        }
    }
    for (i = 0; i < num_var; i++) {
        if (temp[i] == 1) {
            for (j = i; j < num_var; j++) {
                if (temp[j] == 1) {
                    RIG[i][j] = 1;
                    RIG[j][i] = 1;
                }
            }
        }
    }
    for (i = 0; i < num_var; i++) {
        RIG[i][i] = 0;
    }
}

// 删去一个边少于num_reg的节点，标记删去的节点，并在RIG中将其原有的边改为-1
void remove_one_point (struct var_of_RIG* list[], int** RIG, int* rmv_cnt, int num_reg, int num_var) {
    /*printf("\n 开始迭代算法!\n");*/
    int i = 0, j = 0;
    int num_edge = 0;
    int flag = 0;
    (*rmv_cnt)++;

    /*printf("\n\n\n");
    for (i = 0; i < num_var; i++) {
        printf(" ");
        for (j = 0; j < num_var; j++) {
            printf("%d ", RIG[i][j]);
        }
        printf("\n");
    }*/

    // 开始寻找这样一个节点，若成功找到则置flag为1，否则flag为0
    for (i = 0; i < num_var; i++) {
        for (j = 0; j < num_var; j++) {
            if (RIG[i][j] == 1) {
                num_edge++;
            }
        }
        if ((num_edge < num_reg) && (list[i]->rmv_flag == -1)) {
            flag = 1; // 成功找到这样一个节点
            /*printf("\n GET IT! %d\n", num_edge);*/
            break;
        }
        num_edge = 0;
    }
    if (flag == 1) {
        for (j = 0; j < num_var; j++) {
            if (RIG[i][j] == 1) {
                RIG[i][j] = -1;
                RIG[j][i] = -1;
            }
        }
        for (j = 0; j < num_var; j++) {
            if (list[j]->order == i) {
                list[j]->rmv_flag = *rmv_cnt;
                /*printf("\n 删去变量 %s\n", list[j]->name);*/
            } 
        }
    } else if (flag == 0) {
        // 变量溢出，只能删去一个边多于num_reg的节点（可进一步改为删去边最多的节点）
        for (i = 0; i < num_var; i++) {
            if (list[i]->rmv_flag == -1) {
                list[i]->rmv_flag = *rmv_cnt;
                break;
            }
        }
        for (j = 0; j < num_var; j++) {
            if (RIG[list[i]->order][j] == 1) {
                RIG[list[i]->order][j] = -1;
                RIG[j][list[i]->order] = -1;
                /*printf("\n Remove变量 %s\n", list[j]->name);*/
            }
        }
    }
}

// 将一个被删去的节点重新添加，并为其分配相应寄存器号，其中order为该节点在list序列中的位置
void add_one_point (struct var_of_RIG* list[], int** RIG, int num_reg, int num_var, int order, const int reg[]) {
    int i = 0, cnt = 0;
    int flag = 0;

    int temp_reg = 0;
    for (i = 0; i < MAX_REG; i++) {
        if (reg[temp_reg] == 0) break;
        temp_reg++;
    }
    
    int temp[MAX_VAR] = {0}; // 存放复原节点的编号
    // 恢复标记，复原节点的边
    list[order]->rmv_flag = -1;
    for (i = 0; i < num_var; i++) {
        if (RIG[list[order]->order][i] == -1) {
            RIG[list[order]->order][i] = 1;
            RIG[i][list[order]->order] = 1;
            temp[cnt] = i;
            cnt++;
        }
    }
    while ((temp_reg < MAX_REG) && (flag == 0)) {
        flag = 1;
        for (i = 0; i < cnt; i++) {
            if (list[temp[i]]->reg == temp_reg) {
                
                temp_reg = next_reg(temp_reg, reg);
                
                flag = 0;
                break;
            }
        }
    }
    if (flag == 1) {
        list[order]->reg = temp_reg;
    } else if (flag == 0) {
        list[order]->reg = -2;
    }
}

// 完成寄存器分配方案，并尽可能减少变量溢出
void input_color (struct var_of_RIG* list[], int** RIG, int num_var, int num_reg, int reg_list[]) {
    int i = 0, j = 0;
    int rmv_cnt = -1;
    int rmv_num = 0; // 被删去的变量总数
    int num_var_copy = num_var;
    
    int reg[MAX_REG];
    for (i = 0; i < MAX_REG; i++) {
        reg[i] = reg_list[i];
    }

    // 在图中变量大于寄存器总数时，删去一个边少于num_reg的节点，标记删去的节点
    while (num_var_copy > num_reg) {
        remove_one_point(list, RIG, &rmv_cnt, num_reg, num_var);
        num_var_copy--;
        rmv_num++;
    }
    
    // 此时，如无意外，已得到一个变量小于等于寄存器总数的RIG，可进行着色
    int use_reg = -1;
    for (i = 0; i < num_var; i++) {
        if (list[i]->rmv_flag == -1) {
            use_reg = get_one_reg(reg_list);
            list[i]->reg = use_reg;
        }
    }

    // 将被删去的节点依次重新添加，并为其分配寄存器号
    for (i = 0; i < rmv_num; i++) {
        for (j = 0; j < num_var; j++) {
            if (list[j]->rmv_flag == rmv_cnt) {
                add_one_point(list, RIG, num_reg, num_var, j, reg);
                rmv_cnt--;
                break;
            }
        }
    }
}

// 函数所需条件：各基本块的IN、OUT集合（即节点关系），基本块总数（方便遍历），寄存器总数（即颜色总数），所有变量（即着色节点）
void graph_coloring (char*** in, char*** out, int num_block, int num_reg, char* fun_add, int reg_list[]) {
    int i = 0, j = 0;
    int num_var = 0;
    num_var = get_var_num(in, out, num_block); // 变量总数
    /*printf("\nIN、OUT集合中共有 %d 个变量\n", num_var);*/
    // 初始化变量序列
    struct var_of_RIG** var_list = (struct var_of_RIG**)malloc(sizeof(struct var_of_RIG*) * num_var);
    for (i = 0; i < num_var; i++) {
        var_list[i] = (struct var_of_RIG*)malloc(sizeof(struct var_of_RIG));
    }
    int** RIG = (int**)malloc(sizeof(int*) * num_var); 
    for (i = 0; i < num_var; i++) {
        RIG[i] = (int*)malloc(sizeof(int) * num_var);
    }
    for (i = 0; i < num_var; i++) {
        for (j = 0; j < num_var; j++) {
            RIG[i][j] = 0;
        }
    }
    get_var_list(var_list, in, out, num_block);
    /*for (i = 0; i < num_var; i++) {
        printf("\n %s\t%d\tR%d\t%d", var_list[i]->name, var_list[i]->order, var_list[i]->reg, var_list[i]->rmv_flag);
    }
    printf("\n\n\n");*/
    // 获取RIG的邻接矩阵
    for (i = 0; i < num_block; i++) {
        one_in_out_RIG(var_list, RIG, in[i], num_var);
        one_in_out_RIG(var_list, RIG, out[i], num_var);
    }
    /*for (i = 0; i < num_var; i++) {
        printf(" ");
        for (j = 0; j < num_var; j++) {
            printf("%d ", RIG[i][j]);
        }
        printf("\n");
    }*/
    // 获取一个着色方案
    input_color(var_list, RIG, num_var, num_reg, reg_list);
    //printf("Register Allocation:");
    /*if (num_var == 0) {
        printf("\tno var in IN or OUT\n");
        return;
    }*/
    // 对于全局的变量序列进行赋值，以便ARM生成时调用（此时仅 基本块间变量 存在）
    for (i = 0; i < num_var; i++) {
        strcpy(vars[var_cnt].name, var_list[i]->name);
        strcpy(vars[var_cnt].fun_name, fun_add);
        if (var_list[i]->reg == -2) {
            vars[var_cnt].reg = -1;
        } else {
            vars[var_cnt].reg = var_list[i]->reg;
        }
        var_cnt++;
    }
    // 打印 基本块间变量 寄存器分配方案
    /*for (i = 0; i < num_var; i++) {
        if (var_list[i]->reg == -2) {
            printf("\t%s\tSpilling\n", var_list[i]->name);
        } else {
            printf("\t%s\tR%d\n", var_list[i]->name, var_list[i]->reg);
        }
    }*/
    //printf("\n");
}

void reg_param (char* fun_add, char* param[], int param_num) {
    int i = 0;
    for (i = 0; i < param_num; i++) {
        strcpy(vars[var_cnt].name, param[i]);
        strcpy(vars[var_cnt].fun_name, fun_add);
        vars[var_cnt].reg = i;
        var_cnt++;
    }
}

// 内部调用
// 根据变量别名返回寄存器号，-1为变量溢出，-2为查找失败
// 全局变量不在 vars[MAX_VARS] 数组中，即会查找失败
int _search_var (char* fun_name, char* name) {
    int i = 0;
    for (i = 0; i < var_cnt; i++) {
        if(strcmp(vars[i].name, name) == 0 && strcmp(vars[i].fun_name, fun_name) == 0) {
            return vars[i].reg;
        }
    }
    return -2;
}

// 外部调用
// 根据变量别名返回寄存器号，-1为变量溢出
// 全局变量不在 vars[MAX_VARS] 数组中，将返回 -2
// 无用赋值不属于活跃变量，不分配其寄存器，将返回 -3
int search_var (char* fun_name, char* name) {
    int i = 0;
    for (i = 0; i < var_cnt; i++) {
        if(strcmp(vars[i].name, name) == 0 && strcmp(vars[i].fun_name, fun_name) == 0) {
            return vars[i].reg;
        }
    }
    if (search_alias(name) != -1) {
        return -2;
    } else {
        return -3;
    }
}

// 输入函数名、寄存器数组
// 函数将为所用寄存器的相应位赋 1，其余为 0
// 寄存器默认为 16 个，其中 R11、R12 不分配，为 0
int one_fun_reg(char* fun_name, int reg[]) {
    int i = 0, num = 0;
    for (i = 0; i < 16; i++) {
        reg[i] = -1;
    }
    for (i = 0; i < var_cnt; i++) {
        if ((strcmp(fun_name, vars[i].fun_name) == 0) && (vars[i].reg >= 0)) {
            reg[num] = vars[i].reg;
            num++;
        }
    }
    return num;
}

int search_fun_spilling (char* fun_name) {
    int i = 0, num = 0;
    for (i = 0; i < var_cnt; i++) {
        if ((strcmp(fun_name, vars[i].fun_name) == 0) && (vars[i].reg == -1)) {
            num++;
        }
    }
    return num;
}

// 打印所有变量及其寄存器号
void print_vars (void) {
    int i = 0;
    printf("All Vars:\n");
    for (i = 0; i < var_cnt; i++) {
        if (vars[i].reg == -1) {
            printf("\t%s\t\t%s\t\tSpilling\n",vars[i].fun_name, vars[i].name);
        } else {
            printf("\t%s\t\t%s\t\tR%d\n",vars[i].fun_name, vars[i].name, vars[i].reg);
        }
    }
    printf("\n");
}
