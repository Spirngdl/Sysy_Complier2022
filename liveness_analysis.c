#include "include/def.h"

// 根据变量别名去查询符号表中的位置i（从后往前遍历，先局部，后全局）
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

// 根据变量别名来判断其是否为全局变量，全局返回1，局部返回0，失败返回-1
int var_level (char* var_name) {
    int index = 0;
    index = search_alias(var_name);
    if (index == -1) {
        return -1;
    } else if () {
        
    } else if () {
        
    }
}

// 获取一个基本块中三地址代码的数量
int codenode_num (struct codenode TAC) {
    int cnt = 0;
    struct codenode* p = &TAC;
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

// 获取一个基本块中的use、def集合
void one_block_use_def (char* use[], char* def[], int size, Block block) {
    struct codenode* p = block.tac_list;
    int use_num = 0, def_num = 0;
    while (p != NULL) {
        //printf("\n*%d*", p->op);
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
        // 数组赋值语句（三地址皆为use）
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
        // 数组引用语句（合并后解除注释）
        /*if (p->op == ARRAY_EXP) {
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
        }*/
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
        // 形参、实参语句
        /*if (p->op == PARAM || p->op == ARGS) {
            nop;
        }*/
        // 结束语句
        /*if (p->op == END) {
            nop;
        }*/

        /*if (p->opn1.kind == ID) {
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
        cnt++;
    }
    if (succ_num == 2) {
        for (i = 0; i < size; i++) {
        if (in2[i] == NULL) continue;
        symbol = str_exist_strs(in2[i], temp, size);
        if (symbol != -1) {
            continue;
        }
        temp[cnt] = in2[i];
        cnt++; 
        }
    }
    for (i = 0; i < size; i++) {
        out[i] = temp[i];
    }
}

// 根据算法进行liveness分析
// 对于每一个基本块而言，通过对于use、def的操作，得到其in、out，对所有基本块同样处理
// in->live-in at n、out->live-out at n、def->结点所定义、use->结点所使用
void all_block_liveness (Block* cntr[], int num_block) {
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
        one_block_use_def(use[i], def[i], MAX_VAR, *cntr[i]);
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
    for (i = 0; i < num_block; i++) {
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
    }
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
    printf("\n");
    graph_coloring(in, out, num_block, MAX_REG);
}

// 遍历所有的函数，对每一个函数进行liveness分析与寄存器分配
void all_fun_reg (Blocks* head_fun) {
    while (head_fun != NULL) {
        printf("\nFUNCTION %s:", head_fun->name);
        all_block_liveness(head_fun->block, head_fun->count);
        head_fun = head_fun->next;
    }
    printf("\n");
}