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
  symbol_scope_TX.TX[0] = 0; //外部变量在符号表中的起始序号为0
  symbol_scope_TX.top = 1;
  // T->offset = 0; // 外部变量在数据区的偏移量
  semantic_Analysis(T);
  // test_array();
  make_uid(&(T->code));    //给每个三地址代码进行编号，顺变把全局变量初始化移到开头
  change_label(T->code);   //把三弟代码生成过程中产生的标号删除
  print_IR(T->code);       //打印比较初始的三地址代码
  basic_block(T->code);    //划分基本块
  
  // print_vars();
  check_immes(head_block);    //检验立即数合法性
  all_fun_reg(head_block); //进行活跃变量分析
  make_uid_block(head_block); //在进入arm翻译前最后一次调整编号
  for (int i = 0; i < head_block->count; i++)
  {
    print_IR(head_block->block[i]->tac_list); //打印一下
  }
  arminterface(head_block);
  // printf("optimize\n");
  // dag_optimize(head_block);
  // // all_fun(head_block);
  // for (int i = 0; i < head_block->count; i++)
  // {
  //   print_IR(head_block->block[i]->tac_list);
  // }
  printf("ending\n");
  // struct Block *block = divide_block(T->code);
}
