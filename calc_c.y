%{
    #include<stdlib.h>
    #include "include/def.h"
    #include <stdio.h>
    #include <math.h>
    extern int yylineno;
    extern FILE *yyin;
    extern char *yytext;
    int yylex(void);
    void yyerror(char* s, ...);
%}

%union{
    int         type_int;
    float       type_float;
    struct node * ptr;
    char        type_id[32];
};
//非终结符
%type <ptr> compunit decl constdecl constdef   vardecl  vardef funcdef initval eqexp
%type <ptr> funcfparams funcfparam block blockitem stmt exp cond lval  primaryexp 
%type <ptr> number unaryexp unaryop funcrparams mulexp addexp relexp landexp lorexp constexp
%type <ptr> constdecl_   block_   vardecl_ funcfparam_  vardef_ lval_
%type <ptr> program btype defone defarray initvalarray initvalarraylist
// constdef constdef_ constinitval constinitval_
%left TOK_OR TOK_AND TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_MODULO 
%left TOK_LESS TOK_LESSEQ TOK_GREAT TOK_GREATEQ TOK_NOTEQ TOK_EQ 
%right TOK_ASSIGN TOK_NOT 

//终结符
%token <type_int> INTCONST
%token <type_id> IDENT 
%token <type_float> FLOAT
//relop
%token <type_id> TOK_LESS TOK_LESSEQ TOK_GREAT TOK_GREATEQ TOK_NOTEQ TOK_EQ // < > >= <= == !=
//TYPE
%token <type_id> TOK_CONST
//标识符
%token TOK_LPAR TOK_RPAR TOK_LBRACKET TOK_RBRACKET TOK_LSQUARE TOK_RSQUARE TOK_COMMA TOK_SEMICOLON TOK_BREAK TOK_CONTINUE
%token TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_ASSIGN
%token TOK_AND TOK_OR TOK_NOT TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_INT TOK_VOID  TOK_FLOAT

%%
program: compunit                          {$$ = $1;Driver($1);}
        ;
compunit: decl compunit                     {$$ = mknode(COMPUNIT_LIST,$1,$2,NULL,yylineno);}       
        | funcdef compunit                  {$$ = mknode(COMPUNIT_LIST,$1,$2,NULL,yylineno);}        
        | decl                              {$$ = $1;} 
        | funcdef                           {$$ = $1;} 
        ;
decl: constdecl TOK_SEMICOLON                           {$$ = $1;}
    | vardecl TOK_SEMICOLON                             {$$ = $1;}
     ;
//常量声明
constdecl: TOK_CONST btype constdef constdecl_        {$3->type =$2->kind /*($2->kind == TOK_INT ? CONST_TOK_INT:CONST_TOK_FLOAT)*/;
                                                        if($3->kind == ID)$3->kind = CONSTDECL;
                                                        if($4 != NULL)
                                                                {$$ = mknode(CONSTDECL_LIST,$3,$4,NULL,yylineno);}
                                                        else
                                                                {$$ = $3;}}
         ;
constdecl_: TOK_COMMA constdef constdecl_              {if($2->kind == ID){$2->kind = CONSTDECL;}$$ = mknode(CONSTDECL_LIST,$2,$3,NULL,yylineno);} 
          |                                            {$$ = NULL;}
          ;
constdef: IDENT TOK_ASSIGN constexp                     {struct node* temp_exp = mknode(FLOAT_LITERAL,NULL,NULL,NULL,yylineno);temp_exp->type_float = const_exp($3);
                                                         struct node *temp = mkconstnode(ID,temp_exp,yylineno,$1);$$ = temp;}                          
        | IDENT TOK_LSQUARE constexp TOK_RSQUARE vardef_ TOK_ASSIGN initvalarray {struct node * temp = mknode(TOK_INT,$5,NULL,NULL,yylineno);temp->type_int = const_exp($3);
                                                                                 $$ = mkparray(ARRAY_DEC,$1,temp,yylineno);strcpy($$->type_id,$1);$$->ptr[0] = $7;}
        ;
// constdef: IDENT constdef_ TOK_ASSIGN constinitval          {struct node *temp;if($2 == NULL){temp=mknode(ID,NULL,NULL,NULL,yylineno);strcpy(temp->type_id,$1);}
//                                                              else{temp = mkparray(ARRAY_DEC,$1,$2,yylineno);temp->type = TOK_INT;strcpy(temp->type_id,$1);}
//                                                              temp->ptr[0]=$4;$$ = temp;}
//         ;
// constdef_: TOK_LSQUARE INTCONST TOK_RSQUARE constdef_      {struct node * temp = mknode(TOK_INT,$4,NULL,NULL,yylineno);temp->type_int = $2;$$ =temp;}
//          |                                                 {$$ = NULL;}
//          ;
         
btype: TOK_INT         {$$ = mknode(TOK_INT,NULL,NULL,NULL,yylineno);}
     | TOK_FLOAT       {$$ = mknode(TOK_FLOAT,NULL,NULL,NULL,yylineno);}
      ;                                           
// constinitval: constexp                                                  {$$ = $1;}
//             | TOK_LBRACKET TOK_RBRACKET                                 {$$ = NULL;}
//             | TOK_LBRACKET constinitval constinitval_ TOK_RBRACKET      {$$ = mknode(CONSTINITVAL_LIST,$2,$3,NULL,yylineno);}
//             ;
// constinitval_:TOK_COMMA constinitval constinitval_              {$$ = mknode(CONSTINITVAL_LIST,$2,$3,NULL,yylineno);}
//              |                                                  {$$ = NULL;}
//             ;
vardecl: btype vardef vardecl_                  { $2->type = $1->kind;if($2->kind == ID)$2->kind = VAR_DECL;
                                                if($3 != NULL)
                                                 {struct node * temp = mknode(VAR_DECL_LIST,$2,$3,NULL,yylineno);temp->type = $1->kind;$$ = temp;}
                                                else
                                                 {$$=$2;}}
                                                
        ;
vardecl_: TOK_COMMA vardef vardecl_                             {if($2->kind == ID)$2->kind = VAR_DECL;$$ = mknode(VAR_DECL_LIST,$2,$3,NULL,yylineno);}
        |                                                       {$$ = NULL;}
        ;

vardef: defone                                          {$$ = $1;}
      | defarray                                        {$$ = $1;}
      ;
defone: IDENT                                           {$$ = mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
      | IDENT TOK_ASSIGN initval                        {struct node *temp = mknode(ID,NULL,NULL,NULL,yylineno);strcpy(temp->type_id,$1);temp->ptr[0] = $3;$$ = temp;}
      ;
defarray: IDENT TOK_LSQUARE constexp TOK_RSQUARE vardef_                                 {struct node * temp = mknode(TOK_INT,$5,NULL,NULL,yylineno);temp->type_int = const_exp($3);$$ = mkparray(ARRAY_DEC,$1,temp,yylineno);strcpy($$->type_id,$1);}
        | IDENT TOK_LSQUARE constexp TOK_RSQUARE vardef_ TOK_ASSIGN initvalarray         {struct node * temp = mknode(TOK_INT,$5,NULL,NULL,yylineno);temp->type_int = const_exp($3);$$ = mkparray(ARRAY_DEC,$1,temp,yylineno);strcpy($$->type_id,$1);$$->ptr[0] = $7;}
        ;
vardef_: TOK_LSQUARE constexp TOK_RSQUARE vardef_               {struct node * temp = mknode(TOK_INT,$4,NULL,NULL,yylineno);temp->type_int = const_exp($2);$$ =temp;}
       |                                                        {$$ = NULL;}
       ;                                             
initval: exp                                                    {$$ = $1;}
        ;
initvalarray: TOK_LBRACKET TOK_RBRACKET                         {$$ = mknode(INITARRAY,NULL,NULL,NULL,yylineno);}
            | TOK_LBRACKET initvalarraylist TOK_RBRACKET        {$$ = mknode(INITARRAY,$2,NULL,NULL,yylineno);}
            ;
initvalarraylist: initvalarray                                  {$$ = $1;}     //多一层大括号
                | initvalarraylist TOK_COMMA initvalarray       {push_initarray($1,$3);$$ = $1;}   //
                | initval                                       {$$ = $1;}
                | initvalarraylist TOK_COMMA initval            {push_initarray($1,$3);$$ = $1;}
                ;
funcdef: btype IDENT TOK_LPAR funcfparams TOK_RPAR block        {struct node* temp = mknode(FUNC_DEF,$4,$6,NULL,yylineno);strcpy(temp->type_id,$2);
                                                                temp->type = $1->kind;$$ = temp;}//该结点对应一个函数定义
       | TOK_VOID IDENT TOK_LPAR funcfparams TOK_RPAR block     {struct node* temp = mknode(FUNC_DEF,$4,$6,NULL,yylineno);strcpy(temp->type_id,$2);
                                                                temp->type = TOK_VOID;$$ = temp;}//该结点对应一个函数定义
       | btype IDENT TOK_LPAR TOK_RPAR block                    {struct node* temp = mknode(FUNC_DEF,NULL,$5,NULL,yylineno);strcpy(temp->type_id,$2);
                                                                temp->type = $1->kind;$$ = temp;}//该结点对应一个函数定义
       | TOK_VOID IDENT TOK_LPAR TOK_RPAR block                 {struct node* temp = mknode(FUNC_DEF,NULL,$5,NULL,yylineno);strcpy(temp->type_id,$2);
                                                                temp->type = TOK_VOID;$$ = temp;}//该结点对应一个函数定义
        ;
funcfparams: funcfparam TOK_COMMA funcfparams                   {$$ = mknode(PARAM_LIST,$1,$3,NULL,yylineno);}
           | funcfparam                                         {$$ = mknode(PARAM_LIST,$1,NULL,NULL,yylineno);}
           ;
funcfparam: btype IDENT TOK_LSQUARE TOK_RSQUARE funcfparam_     {struct node*temp = mkparray(PARAM_ARRAY,$2,$5,yylineno);temp->type = $1->kind;strcpy(temp->type_id,$2);$$ = temp;}
          | btype IDENT                                         {struct node*temp = mknode(PARAM_DEC,NULL,NULL,NULL,yylineno); strcpy(temp->type_id,$2);temp->type = $1->kind;$$ = temp;}               
          ;
funcfparam_: TOK_LSQUARE constexp TOK_RSQUARE funcfparam_      {struct node * temp = mknode(TOK_INT,$4,NULL,NULL,yylineno);temp->type_int = const_exp($2);$$= temp;}
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
stmt: lval TOK_ASSIGN exp TOK_SEMICOLON         {$$ = mknode(TOK_ASSIGN,$1,$3,NULL,yylineno);}//$$结点type_id空置未用，正好存放运算符
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
lval: IDENT lval_                               {$$ = mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);if($2 != NULL){$$->kind = EXP_ARRAY;$$->ptr[0] = $2;}}
    ;
lval_: TOK_LSQUARE exp TOK_RSQUARE lval_        {$$ = mknode(EXP_ARRAY,$2,$4,NULL,yylineno);}
     |                                          {$$ = NULL;}
     ;
primaryexp: TOK_LPAR exp TOK_RPAR               {$$ = $2;}
          | lval                                {$$ = $1;}
          | number                              {$$ = $1;}
          ;
number: INTCONST                                {$$ = mknode(LITERAL,NULL,NULL,NULL,yylineno);$$->type_int = $1;$$->type = LITERAL;}
        | FLOAT                                 {$$ = mknode(FLOAT_LITERAL,NULL,NULL,NULL,yylineno);$$->type_float = $1;$$->type = FLOAT_LITERAL;}
        ;
unaryexp: primaryexp                            {$$ = $1;}
        | IDENT TOK_LPAR funcrparams TOK_RPAR   {$$ = mknode(FUNC_CALL,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
        | IDENT TOK_LPAR TOK_RPAR               {$$ = mknode(FUNC_CALL,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
        | unaryop unaryexp                      {$$ = mkunarynode(UNARYEXP,$1,$2,yylineno);}
        ;
unaryop: TOK_ADD                                {$$ = mknode(TOK_ADD,NULL,NULL,NULL,yylineno);strcpy($$->type_id,"TOK_ADD");}
       | TOK_SUB                                {$$ = mknode(TOK_SUB,NULL,NULL,NULL,yylineno);strcpy($$->type_id,"TOK_SUB");}
       | TOK_NOT                                {$$ = mknode(TOK_NOT,NULL,NULL,NULL,yylineno);strcpy($$->type_id,"TOK_NOT");}
       ;
funcrparams: exp TOK_COMMA funcrparams          {$$ = mknode(ARGS,$1,$3,NULL,yylineno);}
           | exp                                {$$ = mknode(ARGS,$1,NULL,NULL,yylineno);}
           ;
mulexp: unaryexp                                {$$ = $1;}
      | mulexp TOK_MUL unaryexp                 {$$ = mkopnode(TOK_MUL,$1,$3,yylineno);}
      | mulexp TOK_DIV unaryexp                 {$$ = mkopnode(TOK_DIV,$1,$3,yylineno);}
      | mulexp TOK_MODULO unaryexp              {$$ = mkopnode(TOK_MODULO,$1,$3,yylineno);}
      ; 
addexp: mulexp                                  {$$ = $1;}
      | addexp TOK_ADD addexp                   {$$ = mkopnode(TOK_ADD,$1,$3,yylineno);}
      | addexp TOK_SUB addexp                   {$$ = mkopnode(TOK_SUB,$1,$3,yylineno);}    
      ;
relexp: addexp                                  {$$ = $1;}
      | relexp TOK_LESS addexp                  {$$ = mknode(TOK_LESS,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}////词法分析关系运算符号自身值保存在$2中
      | relexp TOK_GREAT addexp                 {$$ = mknode(TOK_GREAT,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}       
      | relexp TOK_LESSEQ addexp                {$$ = mknode(TOK_LESSEQ,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}
      | relexp TOK_GREATEQ addexp               {$$ = mknode(TOK_GREATEQ,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}
      ;
eqexp: relexp                                   {$$ = $1;}
     | eqexp TOK_EQ relexp                      {$$ = mknode(TOK_EQ,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}////词法分析关系运算符号自身值保存在$2中
     | eqexp TOK_NOTEQ relexp                   {$$ = mknode(TOK_NOTEQ,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}////词法分析关系运算符号自身值保存在$2中
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


#include<stdarg.h>
void yyerror(char* s, ...)
{
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d:error:", yylineno); //错误行号
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}
