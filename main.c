#include "include/def.h"
#include "include/Dag.h"
extern FILE *yyin;
extern int yylineno;
int main(int argc, char *argv[])
{
  yyin = fopen(argv[1], "r");
  //  YY_BUFFER_STATE bp = yy_scan_string(argv[1]);
  // yy_scan_string(argv[1]);
  yylineno = 1;
  yyparse();
  return 0;
}
//语法分析入口
void Driver(struct node *T)
{
  symbolTable.index = 0;
  add_functions();
  symbol_scope_TX.TX[0] = 0; //外部变量在符号表中的起始序号为0
  symbol_scope_TX.top = 1;
  // T->offset = 0; // 外部变量在数据区的偏移量
  char gloal[32] = "global";
  Func_name = gloal;
  semantic_Analysis(T);
  // print_IR(T->code);
  // test_array();
  make_uid(&(T->code));  //给每个三地址代码进行编号，顺变把全局变量初始化移到开头
  change_label(T->code); //把三弟代码生成过程中产生的标号删除
  print_IR(T->code);     //打印比较初始的三地址代码
  int width = get_array_infunc("main");
  basic_block(T->code); //划分基本块
                        // get_count_var(head_block);
                        // invariant_Extrapolation(head_block);
  printf("optimize\n");
  dag_optimize(head_block);   // DAG优化
  make_uid_block(head_block); //在进入arm翻译前最后一次调整编号

  Blocks *cur_blocks = head_block;
  while (cur_blocks)
  {
    for (int i = 0; i < cur_blocks->count; i++)
    {
      print_IR(cur_blocks->block[i]->tac_list);
    }
    cur_blocks = cur_blocks->next;
  }
  check_immes(head_block);     //检验立即数合法性
  all_fun_reg(head_block);     //进行活跃变量分析
  add_label_block(head_block); //

  arminterface(head_block);

  // // invariant_Extrapolation(head_block);
  // // dag_optimize(head_block);

  // // // all_fun(head_block);
  // Blocks *cur_blocks = head_block;
  // while (cur_blocks)
  // {
  //   for (int i = 0; i < cur_blocks->count; i++)
  //   {
  //     print_IR(cur_blocks->block[i]->tac_list);
  //   }
  //   cur_blocks = cur_blocks->next;
  // }

  // // print_vars();

  printf("ending\n");
  // struct Block *block = divide_block(T->code);
}
