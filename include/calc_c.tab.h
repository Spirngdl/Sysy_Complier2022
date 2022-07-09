/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_CALC_TAB_H_INCLUDED
# define YY_YY_CALC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOK_OR = 258,
    TOK_AND = 259,
    TOK_ADD = 260,
    TOK_SUB = 261,
    TOK_MUL = 262,
    TOK_DIV = 263,
    TOK_MODULO = 264,
    TOK_LESS = 265,
    TOK_LESSEQ = 266,
    TOK_GREAT = 267,
    TOK_GREATEQ = 268,
    TOK_NOTEQ = 269,
    TOK_EQ = 270,
    TOK_ASSIGN = 271,
    TOK_NOT = 272,
    INTCONST = 273,
    IDENT = 274,
    FLOAT = 275,
    TOK_CONST = 276,
    TOK_LPAR = 277,
    TOK_RPAR = 278,
    TOK_LBRACKET = 279,
    TOK_RBRACKET = 280,
    TOK_LSQUARE = 281,
    TOK_RSQUARE = 282,
    TOK_COMMA = 283,
    TOK_SEMICOLON = 284,
    TOK_BREAK = 285,
    TOK_CONTINUE = 286,
    TOK_IF = 287,
    TOK_ELSE = 288,
    TOK_WHILE = 289,
    TOK_RETURN = 290,
    TOK_INT = 291,
    TOK_VOID = 292,
    TOK_FLOAT = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 13 "calc.y"

    int         type_int;
    float       type_float;
    struct node * ptr;
    char        type_id[32];

#line 103 "calc.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CALC_TAB_H_INCLUDED  */
