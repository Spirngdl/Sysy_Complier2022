%{
    #include<stdlib.h>
    #include<stdio.h>
    #include "ast.h"
    int yylex(void);
    void yyerror(char *s,...);
    static past astRoot;
%}

%union{
    int         iValue;
    past        pAst;
};

%token <pAst> IDENT
%token <pAst> INTCONST
%token <pAst> TOK_INT
%token <pAst> TOK_VOID
%token <pAst> TOK_CONST
%token <pAst> TOK_IF
%token <pAst> TOK_ELSE
%token <pAst> TOK_WHILE
%token <pAst> TOK_BREAK
%token <pAst> TOK_CONTINUE
%token <pAst> TOK_RETURN
%token <pAst> TOK_ADD
%token <pAst> TOK_SUB
%token <pAst> TOK_MUL
%token <pAst> TOK_DIV
%token <pAst> TOK_MODULO
%token <pAst> TOK_LESS
%token <pAst> TOK_LESSEQ
%token <pAst> TOK_GREAT
%token <pAst> TOK_GREATEQ
%token <pAst> TOK_NOTEQ
%token <pAst> TOK_EQ
%token <pAst> TOK_NOT
%token <pAst> TOK_AND
%token <pAst> TOK_OR
%token <pAst> TOK_ASSIGN
%token <pAst> TOK_LPAR
%token <pAst> TOK_RPAR
%token <pAst> TOK_LBRACKET
%token <pAst> TOK_RBRACKET
%token <pAst> TOK_LSQUARE
%token <pAst> TOK_RSQUARE
%token <pAst> TOK_COMMA
%token <pAst> TOK_SEMICOLON
%token <pAst> TOK_ERRNO 
%type <pAst> compunit decl constdecl constdef  constinitval vardecl vardef funcdef initval eqexp
%type <pAst> funcfparams funcfparam block blockitem stmt exp cond lval primaryexp
%type <pAst> number unaryexp unaryop funcrparams mulexp addexp relexp landexp lorexp constexp
%type <pAst> constdecl_ constdef_ constinitval_ vardecl_ vardef_  funcfparam_ block_ lval_ initval_
%type <pAst> program

%left TOK_OR  TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_MODULO 
%left TOK_LESS TOK_LESSEQ TOK_GREAT TOK_GREATEQ TOK_NOT TOK_NOTEQ TOK_EQ  TOK_AND
%left TOK_LPAR TOK_RPAR TOK_LSQUARE TOK_RSQUARE TOK_LBRACKET TOK_RBRACKET
%right TOK_ASSIGN 
%%
program: compunit                          {$$ = $1;printf("tree:\n");eval($$,0);astRoot = $1;}
        ;
compunit: compunit decl                    {$$ = addast($1,1,$2);}       
        | compunit funcdef                 {$$ = addast($1,1,$2);}        
        | decl                             {$$ = newast("Compuit",1,$1);} 
        | funcdef                          {$$ = newast("Compuit",1,$1);} 
        ;
decl: constdecl                            {$$ = $1;}
    | vardecl                              {$$ = $1;}
     ;
constdecl: TOK_CONST TOK_INT constdecl_ TOK_SEMICOLON {$$ = newast("ConstDecl",3,$1,$2,$3);}
         ;
constdecl_: constdef TOK_COMMA constdecl_          {$$ = newast("ConstDecl_",2,$1,$3);}
          | constdef                               {$$ = $1;}
          ;
constdef: IDENT constdef_ TOK_ASSIGN constinitval  {$$ = newast("ConstDef",4,$1,$2,$3,$4);}
        ;
constdef_: TOK_LSQUARE constexp TOK_RSQUARE constdef_  {$$ = newast("ConstDef_",2,$2,$4);}
         |                                              {$$ = newast("ConstDef_",0,-1);}
         ;
constinitval: constexp                                          {$$ = newast("ConstInitval",1,$1);}
            | TOK_LBRACKET TOK_RBRACKET                         {$$ = newast("ConstInitval",0,-1);}
            | TOK_LBRACKET constinitval constinitval_ TOK_RBRACKET {$$ = newast("ConstInitval",2,$2,$3);}
            ;
constinitval_:TOK_COMMA constinitval constinitval_              {$$ = newast("ConstInitval_",2,$2,$3);}
             |                                                  {$$ = newast("ConstInitval_",0,-1);}
             ;
vardecl: TOK_INT vardef vardecl_ TOK_SEMICOLON                   {$$ = newast("Vardecl",3,$1,$2,$3);}
        ;
vardecl_: TOK_COMMA vardef vardecl_                             {$$ = newast("Vardecl_",2,$2,$3);}
        |                                                       {$$ = newast("Vardecl_",0,-1);}
        ;
vardef: IDENT vardef_                                           {$$ = newast("None",2,$1,$2);}
      | IDENT vardef_ TOK_ASSIGN initval                        {$$ = newast("None",4,$1,$2,$3,$4);}
      ;
vardef_: TOK_LSQUARE constexp TOK_RSQUARE vardef_               {$$ = newast("None",2,$2,$4);}
       |                                                        {$$ = newast("None",0,-1);}
      ;
initval: exp                                                    {$$ = $1;}
        | TOK_LBRACKET initval initval_ TOK_RBRACKET            {$$ = newast("InitVal",2,$2,$3);}
        | TOK_LBRACKET TOK_RBRACKET                             {$$ = newast("InitVal",0,-1);}
        ;
initval_: TOK_COMMA initval initval_                            {$$ = newast("InitVal_",2,$2,$3);}
        |                                                       {$$ = newast("InitVal_",0,-1);}
        ;
funcdef: TOK_INT IDENT TOK_LPAR funcfparams TOK_RPAR block     {$$ = newast("FuncDef",4,$1,$2,$4,$6);}
       | TOK_VOID IDENT TOK_LPAR funcfparams TOK_RPAR block     {$$ = newast("FuncDef",4,$1,$2,$4,$6);}
       | TOK_INT IDENT TOK_LPAR TOK_RPAR block                 {$$ = newast("FuncDef",3,$1,$2,$5);}
       | TOK_VOID IDENT TOK_LPAR TOK_RPAR block                 {$$ = newast("FuncDef",3,$1,$2,$5);}
        ;
funcfparams: funcfparam TOK_COMMA funcfparams                   {$$ = newast("FuncfPrarams",2,$1,$3);}
           | funcfparam                                         {$$ = $1;}
           ;
funcfparam: TOK_INT IDENT TOK_LSQUARE TOK_RSQUARE funcfparam_     {$$ = newast("FuncfParam",5,$1,$2,$3,$4,$5);}
          | TOK_INT IDENT                                         {$$ = newast("FuncfPraram",2,$1,$2);}               
          ;
funcfparam_: TOK_LSQUARE exp TOK_RSQUARE funcfparam_            {$$ = newast("FuncfParam_",2,$2,$4);}
            |                                                   {$$ = newast("FuncfParam_",0,-1);}
            ;
block: TOK_LBRACKET block_ TOK_RBRACKET                         {$$ = $2;}
     ;
block_: blockitem block_                                        {$$ = newast("Block",2,$1,$2);}
      |                                                         {$$ = newast("Block",0,-1);}
        ;
blockitem: decl                                 {$$ = $1;}
    | stmt                                      {$$ = $1;}
    ;
stmt: lval TOK_ASSIGN exp TOK_SEMICOLON         {$$ = newast("ASSIGNStmt",2,$1,$3);}
    | TOK_SEMICOLON                             {$$ = newast("Stmt",0,-1);}
    | exp TOK_SEMICOLON                         {$$ = $1;}
    | block                                     {$$ = $1;}
    | TOK_IF TOK_LPAR cond TOK_RPAR stmt        {$$ = newast("IFStmt",3,$1,$3,$5);}
    | TOK_IF TOK_LPAR cond TOK_RPAR stmt TOK_ELSE stmt  {$$ = newast("IFELSEStmt",5,$1,$3,$5,$6,$7);}
    | TOK_WHILE TOK_LPAR cond TOK_RPAR stmt     {$$ = newast("WHILEStmt",3,$1,$3,$5);}
    | TOK_BREAK TOK_SEMICOLON                   {$$ = newast("BREAKStmt",1,$1);}
    | TOK_CONTINUE TOK_SEMICOLON                {$$ = newast("CONTINUEStmt",1,$1);}
    | TOK_RETURN TOK_SEMICOLON                  {$$ = newast("RETURNStmt",0,-1);}
    | TOK_RETURN exp TOK_SEMICOLON              {$$ = newast("RETURNStmt",1,$2);}
    ;
exp: addexp                                     {$$ = $1;}
    ;
cond: lorexp                                    {$$ = $1;}
    ;
lval: IDENT lval_                               {$$ = newast("None",2,$1,$2);}
    ;
lval_: TOK_LSQUARE exp TOK_RSQUARE lval_        {$$ = newast("None",2,$2,$4);}
     |                                          {$$ = newast("None",0,-1);}
     ;
primaryexp: TOK_LPAR exp TOK_RPAR               {$$ = $2;}
          | lval                                {$$ = $1;}
          | number                              {$$ = $1;}
          ;
number: INTCONST                                {$$ = $1;}
        ;
unaryexp: primaryexp                            {$$ = $1;}
        | IDENT TOK_LPAR funcrparams TOK_RPAR   {$$ = newast("UnaryExp",2,$1,$3);}
        | IDENT TOK_LPAR TOK_RPAR               {$$ = $1;}
        | unaryop unaryexp                      {$$ = newast("UnaryExp",2,$1,$2);}
        ;
unaryop: TOK_ADD                                {$$ = $1;}
       | TOK_SUB                                {$$ = $1;}
       | TOK_NOT                                {$$ = $1;}
       ;
funcrparams: exp TOK_COMMA funcrparams          {$$ = newast("FuncrParams",2,$1,$3);}
           | exp                                {$$ = $1;}
           ;
mulexp: unaryexp                                {$$ = $1;}
      | mulexp TOK_MUL unaryexp                 {$$ = newast("Exp",3,$1,$2,$3);}
      | mulexp TOK_DIV unaryexp                 {$$ = newast("Exp",3,$1,$2,$3);}
      | mulexp TOK_MODULO unaryexp              {$$ = newast("Exp",3,$1,$2,$3);}
      ; 
addexp: mulexp                                  {$$ = $1;}
      | addexp TOK_ADD addexp                   {$$ = newast("Exp",3,$2,$1,$3);}
      | addexp TOK_SUB addexp                   {$$ = newast("Exp",3,$2,$1,$3);}    
      ;
relexp: addexp                                  {$$ = $1;}
      | relexp TOK_LESS addexp                  {$$ = newast("Exp",3,$2,$1,$3);}
      | relexp TOK_GREAT addexp                 {$$ = newast("Exp",3,$2,$1,$3);}       
      | relexp TOK_LESSEQ addexp                {$$ = newast("Exp",3,$2,$1,$3);}
      | relexp TOK_GREATEQ addexp               {$$ = newast("Exp",3,$2,$1,$3);}
      ;
eqexp: relexp                                   {$$ = $1;}
     | eqexp TOK_EQ relexp                      {$$ = newast("Exp",3,$2,$1,$3);}
     | eqexp TOK_NOTEQ relexp                   {$$ = newast("Exp",3,$2,$1,$3);}
     ;
landexp: eqexp                                  {$$ = $1;}
       | landexp TOK_AND eqexp                  {$$ = newast("Exp",3,$2,$1,$3);}
       ;
lorexp: landexp                                 {$$ = $1;}
      | lorexp TOK_OR landexp                   {$$ = newast("Exp",3,$2,$1,$3);}
      ;
constexp: addexp                                {$$ = $1;}
        ;
%%