#include "../include/def.h"
// 根据变量别名去查询符号表中的位置i（此时，符号表仅存全局变量）
/*int search_alias(char *alias)
{
    int i;
    for (i = symbolTable.index - 1; i >= 0; i--)
    {
        if (strcmp(symbolTable.symbols[i].alias, alias) == 0)
        {
            return i;
        }
    }
    return -1;
}*/

int gobal_cnt = 0;

// 获取一个基本块中三地址代码的数量
int codenode_num (struct codenode* TAC) {
    int cnt = 0;
    struct codenode* p = TAC;
    while (p != NULL) {
        cnt++;
        p = p->next;
    }
    return cnt;
}

// 判断id是否已在use、def集合中存在，size为字符串数组的大小
int id_exist_strs (char* str, char* total1[], char* total2[], int size) {
    int i = 0;
    for (i = 0; i < size; i++) {
        if ((total1[i] != NULL) && (strcmp(str, total1[i]) == 0)) {
            return 1;
        }
        if ((total2[i] != NULL) && (strcmp(str, total2[i]) == 0)) {
            return 1;
        }
    }
    return 0;
}

int exist_gobal (char* name, char* fun_name, const int num) {
    int i = 0;
    for (i = 0; i < num; i++) {
        if (strcmp(name, gobals[i].name) == 0 && strcmp(fun_name, gobals[i].fun_name) == 0) return 1;
    }
    return 0;
}

// 获取一个基本块中的use、def集合
void one_block_use_def (char* use[], char* def[], int size, Block block, char* fun_name, char* param[], int param_num) {
    struct codenode* p = block.tac_list;
    int use_num = 0, def_num = 0;
    while (p != NULL) {
        //printf("\n*%d*", p->op);
        
        if (p->op != FUNCTION && p->op != CALL) {
            if (p->result.kind == ID && search_alias(p->result.id) != -1 && exist_gobal(p->result.id, fun_name, gobal_cnt) == 0) {
                strcpy(gobals[gobal_cnt].name, p->result.id);
                strcpy(gobals[gobal_cnt].fun_name, fun_name);
                gobal_cnt++;
            }
            if (p->opn1.kind == ID && search_alias(p->opn1.id) != -1 && exist_gobal(p->opn1.id, fun_name, gobal_cnt) == 0) {
                strcpy(gobals[gobal_cnt].name, p->opn1.id);
                strcpy(gobals[gobal_cnt].fun_name, fun_name);
                gobal_cnt++;
            }
            if (p->opn2.kind == ID && search_alias(p->opn2.id) != -1 && exist_gobal(p->opn2.id, fun_name, gobal_cnt) == 0) {
                strcpy(gobals[gobal_cnt].name, p->opn2.id);
                strcpy(gobals[gobal_cnt].fun_name, fun_name);
                gobal_cnt++;
            }
        }
        if (p->op == CALL) {
            if (p->result.kind == ID && search_alias(p->result.id) != -1 && exist_gobal(p->result.id, fun_name, gobal_cnt) == 0) {
                strcpy(gobals[gobal_cnt].name, p->result.id);
                strcpy(gobals[gobal_cnt].fun_name, fun_name);
                gobal_cnt++;
            }
        }
            
        // LDR语句
        if (p->op == TOK_LDR) {
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, param, param, param_num) && !id_exist_strs(p->result.id, use, def, size) && search_alias(p->result.id) == -1 && _search_var(fun_name, p->result.id) == -2) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // return返回语句
        if (p->op == TOK_RETURN) {
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, param, param, param_num) && !id_exist_strs(p->result.id, use, def, size) && search_alias(p->result.id) == -1 && _search_var(fun_name, p->result.id) == -2) {
                use[use_num] = p->result.id;
                use_num++;
                }
            }     
        }    
        // if判断语句
        if (p->op == JLT || p->op == JLE || p->op == JGT || p->op == JGE || p->op == EQ || p->op == NEQ ) {
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, param, param, param_num) && !id_exist_strs(p->opn1.id, use, def, size) && search_alias(p->opn1.id) == -1 && _search_var(fun_name, p->opn1.id) == -2) { 
                use[use_num] = p->opn1.id;
                use_num++;
                }
            }
            if (p->opn2.kind == ID) {
                if (!id_exist_strs(p->opn2.id, param, param, param_num) && !id_exist_strs(p->opn2.id, use, def, size) && search_alias(p->opn2.id) == -1 && _search_var(fun_name, p->opn2.id) == -2) {
                use[use_num] = p->opn2.id;
                use_num++;
                }
            } 
        }
        // 赋值语句
        if (p->op == TOK_ASSIGN) {
            //printf("\n*%d*", p->op);
            //printf("\n%d, %d", p->result.kind, p->opn1.kind);
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, param, param, param_num) && !id_exist_strs(p->opn1.id, use, def, size) && search_alias(p->opn1.id) == -1 && _search_var(fun_name, p->opn1.id) == -2) {
                    use[use_num] = p->opn1.id;
                    use_num++;
                }
            }
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, param, param, param_num) && !id_exist_strs(p->result.id, use, def, size) && search_alias(p->result.id) == -1 && _search_var(fun_name, p->result.id) == -2) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // 加减乘除语句
        if (p->op == TOK_ADD || p->op == TOK_MUL || p->op == TOK_SUB || p->op == TOK_DIV || p->op == TOK_MODULO) {
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, param, param, param_num) && !id_exist_strs(p->opn1.id, use, def, size) && search_alias(p->opn1.id) == -1 && _search_var(fun_name, p->opn1.id) == -2) {
                    use[use_num] = p->opn1.id;
                    use_num++;
                }    
            }
            if (p->opn2.kind == ID) {
                if (!id_exist_strs(p->opn2.id, param, param, param_num) && !id_exist_strs(p->opn2.id, use, def, size) && search_alias(p->opn2.id) == -1 && _search_var(fun_name, p->opn2.id) == -2) {
                    use[use_num] = p->opn2.id;
                    use_num++;
                }
            }
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, param, param, param_num) && !id_exist_strs(p->result.id, use, def, size) && search_alias(p->result.id) == -1 && _search_var(fun_name, p->result.id) == -2) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // 数组声明语句
        if (p->op == ARRAY_DEF) {
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, param, param, param_num) && !id_exist_strs(p->result.id, use, def, size) && search_alias(p->result.id) == -1 && _search_var(fun_name, p->result.id) == -2) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // 数组赋值语句（数组下标为 use，数组名为 use）
        if (p->op == ARRAY_ASSIGN) {
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, param, param, param_num) && !id_exist_strs(p->opn1.id, use, def, size) && search_alias(p->opn1.id) == -1 && _search_var(fun_name, p->opn1.id) == -2) {
                    use[use_num] = p->opn1.id;
                    use_num++;
                }    
            }
            if (p->opn2.kind == ID) {
                if (!id_exist_strs(p->opn2.id, param, param, param_num) && !id_exist_strs(p->opn2.id, use, def, size) && search_alias(p->opn2.id) == -1 && _search_var(fun_name, p->opn2.id) == -2) {
                    use[use_num] = p->opn2.id;
                    use_num++;
                }
            }
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, param, param, param_num) && !id_exist_strs(p->result.id, use, def, size) && search_alias(p->result.id) == -1 && _search_var(fun_name, p->result.id) == -2) {
                    use[use_num] = p->result.id;
                    use_num++;
                }
            }
        }
        // 数组引用语句（数组下标为 use，数组名为 use）
        if (p->op == ARRAY_EXP) {
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, param, param, param_num) && !id_exist_strs(p->opn1.id, use, def, size) && search_alias(p->opn1.id) == -1 && _search_var(fun_name, p->opn1.id) == -2) {
                    use[use_num] = p->opn1.id;
                    use_num++;
                }    
            }
            if (p->opn2.kind == ID) {
                if (!id_exist_strs(p->opn2.id, param, param, param_num) && !id_exist_strs(p->opn2.id, use, def, size) && search_alias(p->opn2.id) == -1 && _search_var(fun_name, p->opn2.id) == -2) {
                    use[use_num] = p->opn2.id;
                    use_num++;
                }
            }
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, param, param, param_num) && !id_exist_strs(p->result.id, use, def, size) && search_alias(p->result.id) == -1 && _search_var(fun_name, p->result.id) == -2) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // 函数调用语句
        if (p->op == CALL) {
            /*if (p->opn1.kind == ID) {
                nop; // 函数名不是变量，不作处理
            }*/
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, param, param, param_num) && !id_exist_strs(p->result.id, use, def, size) && search_alias(p->result.id) == -1 && _search_var(fun_name, p->result.id) == -2) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // goto跳转语句
        /*if (p->op == GOTO) {
            nop;
        }*/
        // 函数声明语句
        /*if (p->op == FUNCTION) {
            nop;
        }*/
        // 形参语句
        /*if (p->op == PARAM) {
            nop;
        }*/
        // 实参语句
        if (p->op == ARG) {
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, param, param, param_num) && !id_exist_strs(p->result.id, use, def, size) && search_alias(p->result.id) == -1 && _search_var(fun_name, p->result.id) == -2) {
                    use[use_num] = p->result.id;
                    use_num++;
                }
            }
        }
        // 结束语句
        /*if (p->op == END) {
            nop;
        }*/

        p = p->next;
    }
}

// 判断字符串是否在一个字符串数组中，不存在则返回-1，否则返回在数组中存在的位置i
int str_exist_strs (char* str, char* strTotal[], int size) {
    int i = 0;
    for (i = 0; i < size; i++) {
        if ((strTotal[i] != NULL) && (strcmp(str, strTotal[i]) == 0)) {
            return i;
        }
    }
    return -1;
}

// 完成in[n]的计算，若in[n]的内容发生改变，则赋值flag为1，否则不修改flag
void one_block_in (char* use[], char* def[], char* in[], char* out[], int size, int* flag) {
    char** temp = (char**)malloc(sizeof(char*) * size);
    int i = 0, symbol = 0, cnt = 0;
    for (i = 0; i < size; i++) {
        temp[i] = NULL;
    }
    for (i = 0; i < size; i++) {
        // 确保传入的字符串不为NULL
        if (out[i] == NULL) continue;
        symbol = str_exist_strs(out[i], def, size); // symbol为-1则不存在
        if (symbol != -1) {
            continue;
        }
        temp[cnt] = out[i];
        cnt++;
    }
    for (i = 0; i < size; i++) {
        if (use[i] == NULL) continue;
        symbol = str_exist_strs(use[i], temp, size);
        if (symbol != -1) {
            continue;
        }
        temp[cnt] = use[i];
        cnt++; 
    }
    // 此时，对于新的in[n]已完成赋值，即temp字符串数组
    for (i = 0; i < size; i++) {
        if (temp[i] == NULL) continue;
        symbol = str_exist_strs(temp[i], in, size);
        if (symbol == -1) {
            *flag = 1;
            break;
        }
    }
    for (i = 0; i < size; i++) {
        in[i] = temp[i];
    }
}

// 完成out[n]的计算，其中succ_num表征传入的基本块后继结点个数
void one_block_out (char* out[], char* in1[], char* in2[], int size, int succ_num) {
    char** temp = (char**)malloc(sizeof(char*) * size);
    int i = 0, symbol = 0, cnt = 0;
    for (i = 0; i < size; i++) {
        temp[i] = in1[i];
        if(temp[i] != NULL) cnt++;
    }
    if (succ_num == 2) {
        for (i = 0; i < size; i++) {
        if (in2[i] == NULL) continue;
        symbol = str_exist_strs(in2[i], temp, size);
        if (symbol != -1) {
            continue;
        }
        temp[cnt] = in2[i];
        //printf("\ttemp %d: %s", cnt, temp[cnt]);
        cnt++; 
        }
    }
    for (i = 0; i < size; i++) {
        out[i] = temp[i];
        //if(out[i] != NULL && succ_num == 2) printf("\t%s", out[i]);
    }
}

// 根据基本块IN、OUT集合，确定空闲的寄存器，并对reg数组进行相应赋值
void available_reg (int reg[], char* in[], char* out[], char* fun_add) {
    int i = 0;
    int temp = -2;
    for (i = 0; i < MAX_VAR; i++) {
        if (strcmp(in[i], "") != 0) {
            temp = _search_var(fun_add, in[i]);
            if (temp >= 0) reg[temp] = 1;
            temp = -2;
        }
        if (strcmp(out[i], "") != 0) {
            temp = _search_var(fun_add, out[i]);
            if (temp >= 0) reg[temp] = 1;
            temp = -2;
        }
    }
}

// 将一个由TAC组成的基本块，转换为基本块结构的集合，每一基本块有且仅有一条TAC指令
void divide_TACs_to_blocks (Block* p_source, Blocks* p_dest, int num_source) {
    int i = 0;
    
    struct codenode* temp;
    temp = p_source->tac_list;

    for (i = 0; i < num_source; i++) {
        p_dest->block[i]->tac_list = temp;
        p_dest->count++;
        temp = temp->next;
        //printf("%d\n", i);
    }
}

// 传入一个基本块，分别计算每个语句的USE、DEF、IN、OUT，并通过图着色算法为其分配寄存器
// 需注意哪些寄存器已被块间变量占用（通过该基本块的IN、OUT集合计算）
void one_block_tac_liveness (Block* cntr, char* block_in[], char* block_out[], char* fun_add, char* param[], int param_num) {
    int i = 0, j = 0;
    int reg[MAX_REG] = {0};      // 寄存器数组，空闲为0，占用为1
    available_reg(reg, block_in, block_out, fun_add); // 对reg数组进行处理，得到空闲寄存器集合
    /*for (i = 0; i < MAX_REG; i++) {
        if (reg[i] == 0) printf("\n\tR%d\tfree", i);
        if (reg[i] == 1) printf("\n\tR%d\tbusy", i);
    }
    printf("\n");*/
    int num_tac = 0; // 三地址代码的数量
    num_tac = codenode_num(cntr->tac_list);
    //printf("\tnum_tac = %d\n", num_tac);
    // 该基本块的USE、DEF、IN、OUT集合(可能含有基本块间的变量，需注意！)
    char*** use = (char***)malloc(sizeof(char**) * num_tac);
    char*** def = (char***)malloc(sizeof(char**) * num_tac);
    char*** in = (char***)malloc(sizeof(char**) * num_tac);
    char*** out = (char***)malloc(sizeof(char**) * num_tac);
    for (i = 0; i < num_tac; i++) {
        use[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        def[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        in[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        out[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        for (j = 0; j < MAX_VAR; j++) {
            use[i][j] = NULL;
            def[i][j] = NULL;
            in[i][j] = NULL;
            out[i][j] = NULL;
        }
    }
    // 处理基本块的结构，使之可兼容块间函数
    Blocks* temp_blocks = (Blocks*)malloc(sizeof(Blocks));
    temp_blocks->count = 0;
    for (i = 0; i < num_tac; i++) {
        temp_blocks->block[i] = (Block*)malloc(sizeof(Block));
    }
    divide_TACs_to_blocks(cntr, temp_blocks, num_tac);
    for (i = 0; i < num_tac; i++) {
        one_block_use_def(use[i], def[i], MAX_VAR, *(temp_blocks->block[i]), fun_add, param, param_num);
    }
    int flag = 1;
    while (flag == 1) {
        flag = 0;
        for (i = num_tac - 1; i >= 0; i--) {
            if ( i < num_tac - 1) {
                one_block_out(out[i], in[i + 1], NULL, MAX_VAR, 1);
            }
            one_block_in(use[i], def[i], in[i], out[i], MAX_VAR, &flag);
        }
    }
    /*for (i = 0; i < num_tac; i++) {
        for (j = 0; j < MAX_VAR; j++){
            printf("\n\t%s\t%s", in[i][j], out[i][j]);
        }
    }*/
    for (i = 0; i < num_tac; i++) {
        for (j = 0; j < MAX_VAR; j++) {
            if (in[i][j] == NULL) {
                in[i][j] = (char*)malloc(sizeof(char) * 32);
                strcpy(in[i][j], "");
            }
            if (out[i][j] == NULL) {
                out[i][j] = (char*)malloc(sizeof(char) * 32);
                strcpy(out[i][j], "");
            }
        }
    }

    int reg_cnt = 0;
    for (i = 0; i < MAX_REG; i++) {
        if (reg[i] == 0) reg_cnt++;
    }
    //printf("\n%d\n", reg_cnt);

    // 此时分配寄存器已考虑块间变量占用的问题
    graph_coloring(in, out, num_tac, reg_cnt, fun_add, reg);
    
    for (i = 0; i < num_tac; i++) {
        free(use[i]);
        free(def[i]);
        free(in[i]);
        free(out[i]);
    }
    free(def);
    free(use);
    free(in);
    free(out);
}

void get_param (char* param[], int* param_num, Block block) {
    struct codenode* p = block.tac_list;
    while (p != NULL) {
        if (p->op == PARAM) {
            param[*param_num] = p->result.id;
            (*param_num)++;
        }
        p = p->next;
    }
}

// 根据算法进行liveness分析
// 对于每一个基本块而言，通过对于use、def的操作，得到其in、out，对所有基本块同样处理
// in->live-in at n、out->live-out at n、def->结点所定义、use->结点所使用
void all_block_liveness (Block* cntr[], int num_block, char* fun_add) {
    int flag = 1;
    int i = 0, j = 0;
    int in1 = -1, in2 = -1;
    char*** use = (char***)malloc(sizeof(char**) * num_block);
    char*** def = (char***)malloc(sizeof(char**) * num_block);
    char*** in = (char***)malloc(sizeof(char**) * num_block);
    char*** out = (char***)malloc(sizeof(char**) * num_block);
    // 初始化每个基本块的use、def、in、out集合
    for (i = 0; i < num_block; i++) {
        use[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        def[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        in[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        out[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        for (j = 0; j < MAX_VAR; j++) {
            use[i][j] = NULL;
            def[i][j] = NULL;
            in[i][j] = NULL;
            out[i][j] = NULL;
        }
    }

    char* param[100] = {""};
    int param_num = 0;

    get_param(param, &param_num, *cntr[0]);
    
    reg_param(fun_add, param, param_num);

    /*for (i = 0; i < param_num; i++) {
        printf("\n\t%s\tPARAM %d: %s", fun_add, i, param[i]);
    }
    printf("\n");*/

    // 分析得到每个基本块的use、def集合
    for (i = 0; i < num_block; i++) {
        one_block_use_def(use[i], def[i], MAX_VAR, *cntr[i], fun_add, param, param_num);
    }
    // 分析得到每个基本块的in、out集合
    while (flag == 1) {
        flag = 0;
        // 执行liveness算法
        for (i = num_block - 1; i >= 0; i--) {
            /*if (cntr[i]->num_child = 0) {
              do nothing;
            }*/
            if (cntr[i]->num_children == 1) {
                in1 = cntr[i]->children[0]->id;
                // printf("\nin1 = %d\n", in1);
                one_block_out(out[i], in[in1], NULL, MAX_VAR, 1);
            }
            if (cntr[i]->num_children == 2) {
                in1 = cntr[i]->children[0]->id;
                in2 = cntr[i]->children[1]->id;
                // printf("\nin1 = %d, in2 = %d\n", in1, in2);
                one_block_out(out[i], in[in1], in[in2], MAX_VAR, 2);
            }
            one_block_in(use[i], def[i], in[i], out[i], MAX_VAR, &flag);
        }
    }

    // 对于use、def、in、out集合进行打印输出
    /*for (i = 0; i < num_block; i++) {
        printf("\nBlock%d:\n", i);
        printf("\tUSE\t\t");
        for (j = 0; j < MAX_VAR; j++) {
            if (use[i][j] != NULL) {
                printf("%s\t", use[i][j]);
            }
        }
        printf("\n");
        printf("\tDEF\t\t");
        for (j = 0; j < MAX_VAR; j++) {
            if (def[i][j] != NULL) {
                printf("%s\t", def[i][j]);
            }
        }
        printf("\n");
        printf("\tIN\t\t");
        for (j = 0; j < MAX_VAR; j++) {
            if (in[i][j] != NULL) {
                printf("%s\t", in[i][j]);
            }
        }
        printf("\n");
        printf("\tOUT\t\t");
        for (j = 0; j < MAX_VAR; j++) {
            if (out[i][j] != NULL) {
                printf("%s\t", out[i][j]);
            }
        }
        printf("\n");
    }*/

    for (i = 0; i < num_block; i++) {
        for (j = 0; j < MAX_VAR; j++) {
            if (in[i][j] == NULL) {
                in[i][j] = (char*)malloc(sizeof(char) * 32);
                strcpy(in[i][j], "");
            }
            if (out[i][j] == NULL) {
                out[i][j] = (char*)malloc(sizeof(char) * 32);
                strcpy(out[i][j], "");
            }
        }
    }
    //printf("\n");
    int reg[MAX_REG] = {0};

    for (i = 0; i < param_num; i++) {
        reg[i] = 1;
    }
    
    int reg_cnt = 0;
    for (i = 0; i < MAX_REG; i++) {
        if (reg[i] == 0) reg_cnt++;
    }
    
    graph_coloring(in, out, num_block, reg_cnt, fun_add, reg);
    // 基本块内变量寄存器
    // 传入一个函数的基本块以及in、out，分别对每个基本块进行处理，每次循环得到一个基本块的寄存器分配情况
    // 结果存于vars[MAX_VARS]中
    for (i = 0; i < num_block; i++) {
        one_block_tac_liveness(cntr[i], in[i], out[i], fun_add, param, param_num);
    }
    
    for (i = 0; i < num_block; i++) {
        free(use[i]);
        free(def[i]);
        free(in[i]);
        free(out[i]);
    }
    free(def);
    free(use);
    free(in);
    free(out);
}

// 遍历所有的函数，对每一个函数进行liveness分析与寄存器分配
void all_fun_reg (Blocks* head_fun) {
    // 每次循环处理一个函数
    while (head_fun != NULL) {
        //printf("\nFUNCTION %s:", head_fun->name);
        all_block_liveness(head_fun->block, head_fun->count, head_fun->block[0]->tac_list->result.id);
        head_fun = head_fun->next;
    }
    printf("\n");
}

int search_func_gvar (char* funcname, char** gvartable) {
    int i = 0, cnt = 0;
    for (i = 0; i < gobal_cnt; i++) {
        if (strcmp(funcname, gobals[i].fun_name) == 0) {
            gvartable[cnt] = gobals[i].name;
            cnt++;
        }
    }
    return cnt;
}

void _one_block_use_def (char* use[], char* def[], int size, Block block) {
    struct codenode* p = block.tac_list;
    int use_num = 0, def_num = 0;
    while (p != NULL) {

        // LDR语句
        if (p->op == TOK_LDR) {
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, use, def, size)) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // return返回语句
        if (p->op == TOK_RETURN) {
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, use, def, size)) {
                use[use_num] = p->result.id;
                use_num++;
                }
            }     
        }    
        // if判断语句
        if (p->op == JLT || p->op == JLE || p->op == JGT || p->op == JGE || p->op == EQ || p->op == NEQ ) {
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, use, def, size)) { 
                use[use_num] = p->opn1.id;
                use_num++;
                }
            }
            if (p->opn2.kind == ID) {
                if (!id_exist_strs(p->opn2.id, use, def, size)) {
                use[use_num] = p->opn2.id;
                use_num++;
                }
            } 
        }
        // 赋值语句
        if (p->op == TOK_ASSIGN) {
            //printf("\n*%d*", p->op);
            //printf("\n%d, %d", p->result.kind, p->opn1.kind);
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, use, def, size)) {
                    use[use_num] = p->opn1.id;
                    use_num++;
                }
            }
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, use, def, size)) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // 加减乘除语句
        if (p->op == TOK_ADD || p->op == TOK_MUL || p->op == TOK_SUB || p->op == TOK_DIV || p->op == TOK_MODULO) {
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, use, def, size)) {
                    use[use_num] = p->opn1.id;
                    use_num++;
                }    
            }
            if (p->opn2.kind == ID) {
                if (!id_exist_strs(p->opn2.id, use, def, size)) {
                    use[use_num] = p->opn2.id;
                    use_num++;
                }
            }
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, use, def, size)) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // 数组声明语句
        if (p->op == ARRAY_DEF) {
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, use, def, size)) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // 数组赋值语句（数组下标为 use，数组名不做处理）
        if (p->op == ARRAY_ASSIGN) {
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, use, def, size)) {
                    use[use_num] = p->opn1.id;
                    use_num++;
                }    
            }
            if (p->opn2.kind == ID) {
                if (!id_exist_strs(p->opn2.id, use, def, size)) {
                    use[use_num] = p->opn2.id;
                    use_num++;
                }
            }
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, use, def, size)) {
                    use[use_num] = p->result.id;
                    use_num++;
                }
            }
        }
        // 数组引用语句（数组下标为 use，数组名不做处理）
        if (p->op == ARRAY_EXP) {
            if (p->opn1.kind == ID) {
                if (!id_exist_strs(p->opn1.id, use, def, size)) {
                    use[use_num] = p->opn1.id;
                    use_num++;
                }    
            }
            if (p->opn2.kind == ID) {
                if (!id_exist_strs(p->opn2.id, use, def, size)) {
                    use[use_num] = p->opn2.id;
                    use_num++;
                }
            }
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, use, def, size)) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // 函数调用语句
        if (p->op == CALL) {
            /*if (p->opn1.kind == ID) {
                nop; // 函数名不是变量，不作处理
            }*/
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, use, def, size)) {
                    def[def_num] = p->result.id;
                    def_num++;
                }
            }
        }
        // goto跳转语句
        /*if (p->op == GOTO) {
            nop;
        }*/
        // 函数声明语句
        /*if (p->op == FUNCTION) {
            nop;
        }*/
        // 形参语句
        /*if (p->op == PARAM) {
            nop;
        }*/
        // 实参语句
        if (p->op == ARG) {
            if (p->result.kind == ID) {
                if (!id_exist_strs(p->result.id, use, def, size)) {
                    use[use_num] = p->result.id;
                    use_num++;
                }
            }
        }
        // 结束语句
        /*if (p->op == END) {
            nop;
        }*/

        p = p->next;
    }
}

// block_index -> 基本块序号，char** out -> 存入该基本块OUT活跃变量
// 返回值为数组中的变量个数
int search_out (char* fun_name, int block_index, char* _out[]) {
    int _cnt = 0;
    int _flag = 0;
    Blocks* p = head_block;
    while (p != NULL) {
        if (strcmp(fun_name, p->block[0]->tac_list->result.id) == 0) {
            _flag = 1;
            break;
        }
        p = p->next;
    }
    if (_flag == 0) {
        return -1;
    }

    int num_block = p->count;
    int flag = 1;
    int i = 0, j = 0;
    int in1 = -1, in2 = -1;
    char*** use = (char***)malloc(sizeof(char**) * num_block);
    char*** def = (char***)malloc(sizeof(char**) * num_block);
    char*** in = (char***)malloc(sizeof(char**) * num_block);
    char*** out = (char***)malloc(sizeof(char**) * num_block);
    // 初始化每个基本块的use、def、in、out集合
    for (i = 0; i < num_block; i++) {
        use[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        def[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        in[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        out[i] = (char**)malloc(sizeof(char*) * MAX_VAR);
        for (j = 0; j < MAX_VAR; j++) {
            use[i][j] = NULL;
            def[i][j] = NULL;
            in[i][j] = NULL;
            out[i][j] = NULL;
        }
    }
    // 分析得到每个基本块的use、def集合
    for (i = 0; i < num_block; i++) {
        _one_block_use_def(use[i], def[i], MAX_VAR, *(p->block[i]));
    }
    // 分析得到每个基本块的in、out集合
    while (flag == 1) {
        flag = 0;
        // 执行liveness算法
        for (i = num_block - 1; i >= 0; i--) {
            if (p->block[i]->num_children == 1) {
                in1 = p->block[i]->children[0]->id;
                //printf("\nblock %d: in1 = %d", i, in1);
                one_block_out(out[i], in[in1], NULL, MAX_VAR, 1);
            }
            if (p->block[i]->num_children == 2) {
                in1 = p->block[i]->children[0]->id;
                in2 = p->block[i]->children[1]->id;
                //printf("\nblock %d: in1 = %d, in2 = %d", i, in1, in2);
                one_block_out(out[i], in[in1], in[in2], MAX_VAR, 2);
            }
            one_block_in(use[i], def[i], in[i], out[i], MAX_VAR, &flag);
        }
    }

    // 对于use、def、in、out集合进行打印输出
    /*for (i = 0; i < num_block; i++) {
        printf("\nBlock%d:\n", i);
        printf("\tUSE\t\t");
        for (j = 0; j < MAX_VAR; j++) {
            if (use[i][j] != NULL) {
                printf("%s\t", use[i][j]);
            }
        }
        printf("\n");
        printf("\tDEF\t\t");
        for (j = 0; j < MAX_VAR; j++) {
            if (def[i][j] != NULL) {
                printf("%s\t", def[i][j]);
            }
        }
        printf("\n");
        printf("\tIN\t\t");
        for (j = 0; j < MAX_VAR; j++) {
            if (in[i][j] != NULL) {
                printf("%s\t", in[i][j]);
            }
        }
        printf("\n");
        printf("\tOUT\t\t");
        for (j = 0; j < MAX_VAR; j++) {
            if (out[i][j] != NULL) {
                printf("%s\t", out[i][j]);
            }
        }
        printf("\n");
    }*/

    for (i = 0; i < num_block; i++) {
        for (j = 0; j < MAX_VAR; j++) {
            if (in[i][j] == NULL) {
                in[i][j] = (char*)malloc(sizeof(char) * 32);
                strcpy(in[i][j], "");
            }
            if (out[i][j] == NULL) {
                out[i][j] = (char*)malloc(sizeof(char) * 32);
                strcpy(out[i][j], "");
            }
        }
    }

    /*for (i = 0; i < MAX_VAR; i++) {
        printf("\t%s", out[block_index][i]);
    }*/

    for (i = 0; i < MAX_VAR; i++) {
        if (strcmp(out[block_index][i], "") != 0) {
            _out[_cnt] = out[block_index][i];
            _cnt++;
        }
    }

    for (i = 0; i < num_block; i++) {
        free(use[i]);
        free(def[i]);
        free(in[i]);
        free(out[i]);
    }
    free(def);
    free(use);
    free(in);
    free(out);

    return _cnt;
}