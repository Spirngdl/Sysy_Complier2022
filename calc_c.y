%{
    #include<stdlib.h>
    #include "def.h"
    #include <stdio.h>
    #include <math.h>
    extern int yylineno;
    extern char *yytext;
    int yylex(void);
    void yyerror(char* s, ...);
%}

%union{
    int         type_int;
    struct node * ptr;
    char        type_id[32];
};
//非终结符
%type <ptr> compunit decl constdecl constdef  constinitval vardecl vardecl_ vardef funcdef initval eqexp
%type <ptr>  funcfparams funcfparam block blockitem stmt exp cond lval  primaryexp 
%type <ptr> number unaryexp unaryop funcrparams mulexp addexp relexp landexp lorexp constexp
%type <ptr> constdecl_  constinitval_   funcfparam_ block_  initval_
%type <ptr> program

%left TOK_OR TOK_AND TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_MODULO 
%left TOK_LESS TOK_LESSEQ TOK_GREAT TOK_GREATEQ TOK_NOTEQ TOK_EQ 
%right TOK_ASSIGN TOK_NOT 

//终结符
%token <type_int> INTCONST
%token <type_id> IDENT 

//relop
%token <type_id> TOK_LESS TOK_LESSEQ TOK_GREAT TOK_GREATEQ TOK_NOTEQ TOK_EQ // < > >= <= == !=
//TYPE
%token <type_id> TOK_INT TOK_VOID TOK_CONST
//标识符
%token TOK_LPAR TOK_RPAR TOK_LBRACKET TOK_RBRACKET TOK_LSQUARE TOK_RSQUARE TOK_COMMA TOK_SEMICOLON TOK_BREAK TOK_CONTINUE
%token TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_ASSIGN
%token TOK_AND TOK_OR TOK_NOT TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN

%%
program: compunit                          {$$ = $1;DisplaySymbolTable($1);}
        ;
compunit: decl compunit                     {$$ = mknode(COMPUNIT_LIST,$1,$2,NULL,yylineno);}       
        | funcdef compunit                  {$$ = mknode(COMPUNIT_LIST,$1,$2,NULL,yylineno);}        
        | decl                              {$$ = $1;} 
        | funcdef                           {$$ = $1;} 
        ;
decl: constdecl                            {$$ = $1;}
    | vardecl                              {$$ = $1;}
     ;
//常量声明
constdecl: TOK_CONST TOK_INT constdecl_ TOK_SEMICOLON {struct node*temp = mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy(temp->type_id,"const int");temp->type = CONST_INT;
                                                        $$ = mknode(CONSTDEF,temp,$3,NULL,yylineno);}
         ;
constdecl_: constdef TOK_COMMA constdecl_          {$$ = mknode(CONSTDECL_LIST,$1,$3,NULL,yylineno);}
          | constdef                               {$$ = $1;}
          ;
constdef: IDENT                                         {$$ = mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);} //ID结点，标识符符号串存放结点的type_id
        | constdef TOK_LSQUARE INTCONST TOK_RSQUARE     {struct node *temp=mknode(INT,NULL,NULL,NULL,yylineno);temp->type_int=$3;
                                                        $$=mknode(ARRAY_DEC, $1, temp, NULL,yylineno);}
        | constdef TOK_ASSIGN constinitval              {$$=mknode(TOK_ASSIGN,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_ASSIGN");}
        ;
constinitval: constexp                                                  {$$ = $1;}
            | TOK_LBRACKET TOK_RBRACKET                                 {$$ = NULL;}
            | TOK_LBRACKET constinitval constinitval_ TOK_RBRACKET      {$$ = mknode(CONSTINITVAL_LIST,$2,$3,NULL,yylineno);}
            ;
constinitval_:TOK_COMMA constinitval constinitval_              {$$ = mknode(CONSTINITVAL_LIST,$2,$3,NULL,yylineno);}
             |                                                  {$$ = NULL;}
             ;
vardecl: TOK_INT vardef vardecl_ TOK_SEMICOLON                 {struct node*temp = mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy(temp->type_id,$1);temp->type = INT;
                                                                struct node*temp2= mknode(VAR_DECL_LIST,$2,$3,NULL,yylineno); $$ = mknode(VAR_DEF,temp,temp2,NULL,yylineno);}
       ;
vardecl_: TOK_COMMA vardef vardecl_                             {$$ = mknode(VAR_DECL_LIST,$2,$3,NULL,yylineno);}
        |                                                       {$$ = NULL;}
        ;
vardef: IDENT                                            {$$ = mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//ID结点，标识符符号串存放结点的type_id
      | vardef TOK_LSQUARE INTCONST TOK_RSQUARE          {//struct node *temp=mknode(INT,NULL,NULL,NULL,yylineno);temp->type_int=$3;$$=mknode(ARRAY_DEC, $1, temp, NULL,yylineno);
                                                          mkarraynode($1,$3);}
      | vardef TOK_ASSIGN initval                        {$$=mknode(TOK_ASSIGN,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_ASSIGN");}
      ;
initval: exp                                                    {$$ = $1;}
        | TOK_LBRACKET initval initval_ TOK_RBRACKET            {$$ = mknode(INITVAL_LIST,$2,$3,NULL,yylineno);}
        | TOK_LBRACKET TOK_RBRACKET                             {$$ = NULL;}
        ;
initval_: TOK_COMMA initval initval_                            {$$ = mknode(INITVAL_LIST,$2,$3,NULL,yylineno);}
        |                                                       {$$ = NULL;}
        ;
funcdef: TOK_INT IDENT TOK_LPAR funcfparams TOK_RPAR block     {struct node * temp1 = mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy(temp1->type_id,$1);temp1->type = INT;
                                                                struct node *temp2 = mknode(FUNC_DEC,$4,NULL,NULL,yylineno);strcpy(temp2->type_id,$2);
                                                                $$ = mknode(FUNC_DEF,temp1,temp2,$6,yylineno);}//该结点对应一个函数定义
       | TOK_VOID IDENT TOK_LPAR funcfparams TOK_RPAR block     {struct node * temp1 = mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy(temp1->type_id,$1);temp1->type = TOK_VOID;
                                                                struct node *temp2 = mknode(FUNC_DEC,$4,NULL,NULL,yylineno);strcpy(temp2->type_id,$2);
                                                                $$ = mknode(FUNC_DEF,temp1,temp2,$6,yylineno);}//该结点对应一个函数定义
       | TOK_INT IDENT TOK_LPAR TOK_RPAR block                 {struct node * temp1 = mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy(temp1->type_id,$1);temp1->type = INT;
                                                                struct node *temp2 = mknode(FUNC_DEC,NULL,NULL,NULL,yylineno);strcpy(temp2->type_id,$2);
                                                                $$ = mknode(FUNC_DEF,temp1,temp2,$5,yylineno);}//该结点对应一个函数定义
       | TOK_VOID IDENT TOK_LPAR TOK_RPAR block                 {struct node * temp1 = mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy(temp1->type_id,$1);temp1->type = TOK_VOID;
                                                                struct node *temp2 = mknode(FUNC_DEC,NULL,NULL,NULL,yylineno);strcpy(temp2->type_id,$2);
                                                                $$ = mknode(FUNC_DEF,temp1,temp2,$5,yylineno);}//该结点对应一个函数定义
        ;
funcfparams: funcfparam TOK_COMMA funcfparams                   {$$ = mknode(PARAM_LIST,$1,$3,NULL,yylineno);}
           | funcfparam                                         {$$ = mknode(PARAM_LIST,$1,NULL,NULL,yylineno);}
           ;
funcfparam: TOK_INT IDENT TOK_LSQUARE TOK_RSQUARE funcfparam_     {struct node * temp1 = mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy(temp1->type_id,$1);temp1->type = INT;
                                                                   struct node *temp2 = mknode(ID,NULL,NULL,NULL,yylineno);strcpy(temp2->type_id,$2);
                                                                   $$ = mknode(PARAM_ARRAY,temp1,temp2,$5,yylineno); }
          | TOK_INT IDENT                                         {struct node * temp1 = mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy(temp1->type_id,$1);temp1->type = INT;
                                                                   struct node *temp2 = mknode(ID,NULL,NULL,NULL,yylineno);strcpy(temp2->type_id,$2);
                                                                   $$ = mknode(PARAM_DEC,temp1,temp2,NULL,yylineno);}               
          ;
funcfparam_: TOK_LSQUARE INTCONST TOK_RSQUARE funcfparam_       {struct node *temp=mknode(INT,NULL,NULL,NULL,yylineno);temp->type_int=$2;
                                                                 $$=mknode(ARRAY_DEC, temp, $4, NULL,yylineno);}
            |                                                   {$$ = NULL;}
            ;
block: TOK_LBRACKET block_ TOK_RBRACKET                         {$$ = mknode(BLOCK,$2,NULL,NULL,yylineno);}
     ;
block_: blockitem block_                                        {$$ = mknode(BLOCK_LIST,$1,$2,NULL,yylineno);}
      |                                                         {$$ = NULL;}
        ;
blockitem: decl                                 {$$ = $1;}
    | stmt                                      {$$ = $1;}
    ;
stmt: lval TOK_ASSIGN exp TOK_SEMICOLON         {$$ = mknode(TOK_ASSIGN,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_ASSIGN");}//$$结点type_id空置未用，正好存放运算符
    | TOK_SEMICOLON                             {$$ = NULL;}
    | exp TOK_SEMICOLON                         {$$ = $1;}
    | block                                     {$$ = $1;}
    | TOK_IF TOK_LPAR cond TOK_RPAR stmt        {$$ = mknode(IF_THEN,$3,$5,NULL,yylineno);}
    | TOK_IF TOK_LPAR cond TOK_RPAR stmt TOK_ELSE stmt  {$$ = mknode(IF_THEN_ELSE,$3,$5,$7,yylineno);}
    | TOK_WHILE TOK_LPAR cond TOK_RPAR stmt     {$$ = mknode(TOK_WHILE,$3,$5,NULL,yylineno);}
    | TOK_BREAK TOK_SEMICOLON                   {$$ = mknode(TOK_BREAK,NULL,NULL,NULL,yylineno);}
    | TOK_CONTINUE TOK_SEMICOLON                {$$ = mknode(TOK_CONTINUE,NULL,NULL,NULL,yylineno);}
    | TOK_RETURN TOK_SEMICOLON                  {$$ = mknode(TOK_RETURN,NULL,NULL,NULL,yylineno);}
    | TOK_RETURN exp TOK_SEMICOLON              {$$ = mknode(TOK_RETURN,$2,NULL,NULL,yylineno);}
    ;
exp: addexp                                     {$$ = $1;}
    ;
cond: lorexp                                    {$$ = $1;}
    ;
//左值表达式
lval: IDENT                                     {$$  = mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);} 
    ;
primaryexp: TOK_LPAR exp TOK_RPAR               {$$ = $2;}
          | lval                                {$$ = $1;}
          | number                              {$$ = $1;}
          ;
number: INTCONST                                {$$ = mknode(INT,NULL,NULL,NULL,yylineno);$$->type_int = $1;$$->type = INT;}
        ;
unaryexp: primaryexp                            {$$ = $1;}
        | IDENT TOK_LPAR funcrparams TOK_RPAR   {$$ = mknode(FUNC_CALL,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
        | IDENT TOK_LPAR TOK_RPAR               {$$ = mknode(FUNC_CALL,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
        | unaryop unaryexp                      {$$ = mknode(UNARYEXP,$1,$2,NULL,yylineno);}
        ;
unaryop: TOK_ADD                                {$$ = mknode(TOK_ADD,NULL,NULL,NULL,yylineno);strcpy($$->type_id,"TOK_ADD");}
       | TOK_SUB                                {$$ = mknode(TOK_SUB,NULL,NULL,NULL,yylineno);strcpy($$->type_id,"TOK_SUB");}
       | TOK_NOT                                {$$ = mknode(TOK_NOT,NULL,NULL,NULL,yylineno);strcpy($$->type_id,"TOK_NOT");}
       ;
funcrparams: exp TOK_COMMA funcrparams          {$$ = mknode(ARGS,$1,$3,NULL,yylineno);}
           | exp                                {$$ = mknode(ARGS,$1,NULL,NULL,yylineno);}
           ;
mulexp: unaryexp                                {$$ = $1;}
      | mulexp TOK_MUL unaryexp                 {$$ = mknode(TOK_MUL,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_MUL");}
      | mulexp TOK_DIV unaryexp                 {$$ = mknode(TOK_DIV,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_DIV");}
      | mulexp TOK_MODULO unaryexp              {$$ = mknode(TOK_MODULO,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_MODULO");}
      ; 
addexp: mulexp                                  {$$ = $1;}
      | addexp TOK_ADD addexp                   {$$ = mknode(TOK_ADD,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_ADD");}
      | addexp TOK_SUB addexp                   {$$ = mknode(TOK_SUB,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_SUB");}    
      ;
relexp: addexp                                  {$$ = $1;}
      | relexp TOK_LESS addexp                  {$$ = mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}////词法分析关系运算符号自身值保存在$2中
      | relexp TOK_GREAT addexp                 {$$ = mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}       
      | relexp TOK_LESSEQ addexp                {$$ = mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}
      | relexp TOK_GREATEQ addexp               {$$ = mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}
      ;
eqexp: relexp                                   {$$ = $1;}
     | eqexp TOK_EQ relexp                      {$$ = mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}////词法分析关系运算符号自身值保存在$2中
     | eqexp TOK_NOTEQ relexp                   {$$ = mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}////词法分析关系运算符号自身值保存在$2中
     ;
landexp: eqexp                                  {$$ = $1;}
       | landexp TOK_AND eqexp                  {$$=mknode(TOK_AND,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_AND");}
       ;
lorexp: landexp                                 {$$ = $1;}
      | lorexp TOK_OR landexp                   {$$=mknode(TOK_OR,$1,$3,NULL,yylineno);strcpy($$->type_id,"TOK_OR");;}
      ;
constexp: addexp                                {$$ = $1;}
        ;

%%
int main(){
	yylineno=1;
	yyparse();
	return 0;
	}

#include<stdarg.h>
void yyerror(char* s, ...)
{
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d:error:", yylineno); //错误行号
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}
