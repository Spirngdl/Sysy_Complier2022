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
    //FLOAT   ([0-9]*\.[0-9]+)|([0-9]+\.)
%}
%option yylineno

HEX     0(X|x)[0-9a-fA-F]+ 
OCTAL   0[0-7]+
DECIMAL [0-9]+
INTEGER {HEX}|{OCTAL}|{DECIMAL}

HEX_FLOAT {HEX}(\.[0-9a-fA-F]+)?((P|p){INTEGER})?
DECIMAL_FLOAT {DECIMAL}(\.[0-9]+)?((E|e){INTEGER})?
FLOAT       {DECIMAL_FLOAT}(f|F|l|L)?
FLOAT_HEX {HEX_FLOAT}(f|F|l|L)?
%%

{INTEGER}   {yylval.type_int = strtol(yytext,NULL,0);return INTCONST;}
{FLOAT}     {yylval.type_float = atof(yytext);return FLOAT;}
{FLOAT_HEX} {yylval.type_float = hex_atof(yytext);return FLOAT;}
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
"int"       {return TOK_INT;}
"float"     {return TOK_FLOAT;}
"void"      {return TOK_VOID;}
"while"     {return TOK_WHILE;}
"return"    {return TOK_RETURN;}
"const"     {strcpy(yylval.type_id,  yytext) ;return TOK_CONST;}
"break"     {return TOK_BREAK;}
"continue"  {return TOK_CONTINUE;}
[a-z_A-Z][a-z_A-Z0-9]*  {strcpy(yylval.type_id,yytext) ;return IDENT;}
[ \t\n]+    {yycolumn = 1;}
"//"[^\n]*  {/* 注释 */}
"/*"([^\*]|(\*)*[^\*/])*(\*)*"*/" {/* 注释 */}
%%

int yywrap()
{
    return 1;
}