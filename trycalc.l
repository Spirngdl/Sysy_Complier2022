%{
    #include <stdlib.h>
    #include <string.h>
    #include "include/def.h"
    int yycolumn = 1;
    void yyerror(char *s,...);
    // #define YY_USER_ACTION    	yylloc.first_line=yylloc.last_line=yylineno; \
	// yylloc.first_column=yycolumn;	yylloc.last_column=yycolumn+yyleng-1; yycolumn+=yyleng;
    // typedef union{
    //     int type_int;
    //     char type_id[32];
    //     past ptr;
    // }YYLVAL;
    // #define YYSTYPE YYLVAL
%}
%option yylineno
DIGIT   [0-9]
LETTER  [a-zA-Z]
%%

{DIGIT}+    {yylval.type_int = atoi(yytext); ;return INTCONST;}
[+]         {return TOK_ADD;}
[-]         {return TOK_SUB;}
[*]         {return TOK_MUL;}
[/]         {return TOK_DIV;}
[%]         {return TOK_MODULO;}
"("         {return TOK_LPAR;}
")"         {return TOK_RPAR;}
"["         {return TOK_LSQUARE;}
"]"         {return TOK_RSQUARE;}
"{"         {return TOK_LBRACKET;}
"}"         {return TOK_RBRACKET;}
","         {return TOK_COMMA;}
";"         {return TOK_SEMICOLON;}
"<"         {strcpy(yylval.type_id, yytext);return TOK_LESS;}
">"         {strcpy(yylval.type_id, yytext);return TOK_GREAT;}
"<="        {strcpy(yylval.type_id, yytext);return TOK_LESSEQ;}
">="        {strcpy(yylval.type_id, yytext);return TOK_GREATEQ;}
"!="        {strcpy(yylval.type_id, yytext);return TOK_NOTEQ;}
"=="        {strcpy(yylval.type_id, yytext);;return TOK_EQ;}
"&&"        {return TOK_AND;}
"||"        {return TOK_OR;}
"="         {return TOK_ASSIGN;}
"if"        {return TOK_IF;}
"else"      {return TOK_ELSE;}
"int"       {strcpy(yylval.type_id,  yytext);return TOK_INT;}
"void"      {strcpy(yylval.type_id,  yytext);return TOK_VOID;}
"while"     {return TOK_WHILE;}
"return"    {return TOK_RETURN;}
"const"     {strcpy(yylval.type_id,  yytext) ;return TOK_CONST;}
"break"     {return TOK_BREAK;}
"continue"  {return TOK_CONTINUE;}
[a-z_A-Z][a-z_A-Z0-9]*  {strcpy(yylval.type_id,yytext) ;return IDENT;}
[ \t\n]+    {yycolumn = 1;}
%%

int yywrap()
{
    return 1;
}