#include "include/def.h"
extern FILE *yyin;
extern int yylineno;
int main(int argc, char *argv[])
{
    yyin = fopen(argv[1], "r");
  //  YY_BUFFER_STATE bp = yy_scan_string(argv[1]);
  //yy_scan_string(argv[1]);
    yylineno = 1;
    yyparse();
    return 0;
}
//语法分析入口
void DisplaySymbolTable(struct node *T)
{
    symbolTable.index = 0;
    symbol_scope_TX.TX[0] = 0; //外部变量在符号表中的起始序号为0
    symbol_scope_TX.top = 1;
    // T->offset = 0; // 外部变量在数据区的偏移量
    semantic_Analysis(T);
    make_uid(T->code);
    change_label(T->code);
    print_IR(T->code);
    basic_block(T->code);
    printf("ending\n");
    // struct Block *block = divide_block(T->code);
}
