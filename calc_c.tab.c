/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "calc_c.y"

    #include<stdlib.h>
    #include "include/def.h"
    #include <stdio.h>
    #include <math.h>
    extern int yylineno;
    extern FILE *yyin;
    extern char *yytext;
    int yylex(void);
    void yyerror(char* s, ...);

#line 82 "calc_c.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_CALC_C_TAB_H_INCLUDED
# define YY_YY_CALC_C_TAB_H_INCLUDED
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
    TOK_CONST = 275,
    TOK_LPAR = 276,
    TOK_RPAR = 277,
    TOK_LBRACKET = 278,
    TOK_RBRACKET = 279,
    TOK_LSQUARE = 280,
    TOK_RSQUARE = 281,
    TOK_COMMA = 282,
    TOK_SEMICOLON = 283,
    TOK_BREAK = 284,
    TOK_CONTINUE = 285,
    TOK_IF = 286,
    TOK_ELSE = 287,
    TOK_WHILE = 288,
    TOK_RETURN = 289,
    TOK_INT = 290,
    TOK_VOID = 291,
    TOK_FLOAT = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 13 "calc_c.y"

    int         type_int;
    struct node * ptr;
    char        type_id[32];

#line 178 "calc_c.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CALC_C_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   240

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  103
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  192

#define YYUNDEFTOK  2
#define YYMAXUTOK   292


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    43,    43,    45,    46,    47,    48,    50,    51,    54,
      60,    61,    63,    67,    68,    71,    72,    74,    75,    76,
      78,    79,    81,    89,    90,    93,    94,    96,    97,    99,
     100,   102,   103,   105,   107,   108,   110,   111,   112,   113,
     115,   117,   119,   121,   124,   125,   127,   128,   130,   131,
     135,   137,   138,   140,   141,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   155,   157,   160,   162,
     163,   165,   166,   167,   169,   171,   172,   173,   174,   176,
     177,   178,   180,   181,   183,   184,   185,   186,   188,   189,
     190,   192,   193,   194,   195,   196,   198,   199,   200,   202,
     203,   205,   206,   208
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_OR", "TOK_AND", "TOK_ADD",
  "TOK_SUB", "TOK_MUL", "TOK_DIV", "TOK_MODULO", "TOK_LESS", "TOK_LESSEQ",
  "TOK_GREAT", "TOK_GREATEQ", "TOK_NOTEQ", "TOK_EQ", "TOK_ASSIGN",
  "TOK_NOT", "INTCONST", "IDENT", "TOK_CONST", "TOK_LPAR", "TOK_RPAR",
  "TOK_LBRACKET", "TOK_RBRACKET", "TOK_LSQUARE", "TOK_RSQUARE",
  "TOK_COMMA", "TOK_SEMICOLON", "TOK_BREAK", "TOK_CONTINUE", "TOK_IF",
  "TOK_ELSE", "TOK_WHILE", "TOK_RETURN", "TOK_INT", "TOK_VOID",
  "TOK_FLOAT", "$accept", "program", "compunit", "decl", "constdecl",
  "constdecl_", "constdef", "constdef_", "btype", "constinitval",
  "constinitval_", "vardecl", "vardecl_", "vardef", "defone", "defarray",
  "vardef_", "initval", "initvalarray", "initvalarraylist", "funcdef",
  "funcfparams", "funcfparam", "funcfparam_", "block", "block_",
  "blockitem", "stmt", "exp", "cond", "lval", "lval_", "primaryexp",
  "number", "unaryexp", "unaryop", "funcrparams", "mulexp", "addexp",
  "relexp", "eqexp", "landexp", "lorexp", "constexp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292
};
# endif

#define YYPACT_NINF (-147)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -11,     5,  -147,    11,  -147,    64,  -147,   -11,    42,    73,
      49,   -11,    82,    83,  -147,  -147,  -147,    68,    86,  -147,
    -147,  -147,  -147,    90,    91,    39,   204,    65,   204,   103,
    -147,   106,   111,    82,  -147,   121,   110,   127,   128,  -147,
    -147,  -147,  -147,    32,   204,  -147,  -147,  -147,  -147,  -147,
    -147,   204,    14,    57,   121,   135,    57,   132,    -5,    86,
     139,   102,    91,   133,  -147,   134,   121,    17,    10,   204,
    -147,   147,  -147,   204,   204,   204,   204,   204,  -147,   121,
     148,  -147,    90,    62,  -147,  -147,  -147,  -147,   146,   149,
     154,   155,    54,  -147,   103,  -147,   156,   133,  -147,   150,
     163,   160,  -147,  -147,  -147,   161,   159,   167,  -147,  -147,
    -147,  -147,  -147,  -147,  -147,   204,   176,  -147,  -147,   171,
    -147,  -147,   204,   204,  -147,   173,  -147,  -147,  -147,   204,
     157,   204,  -147,   180,   181,   188,   102,   189,   190,    57,
     130,   131,   210,   212,   194,  -147,   191,   199,  -147,  -147,
    -147,   148,    93,  -147,   171,  -147,   166,   204,   204,   204,
     204,   204,   204,   204,   204,   166,  -147,   192,  -147,  -147,
    -147,  -147,    67,  -147,   195,    57,    57,    57,    57,   130,
     130,   131,   210,  -147,   157,  -147,   185,   166,  -147,  -147,
    -147,  -147
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    15,     0,    16,     0,     2,     5,     0,     0,
       0,     6,     0,     0,     1,     3,     7,    27,    24,    25,
      26,     8,     4,    14,    11,     0,     0,     0,     0,     0,
      22,     0,     0,     0,     9,     0,     0,     0,    45,    79,
      80,    81,    74,    70,     0,    28,    33,    72,    75,    73,
      84,     0,    88,    66,     0,     0,   103,     0,    27,    24,
       0,     0,    11,    52,    43,    47,     0,     0,     0,     0,
      68,     0,    78,     0,     0,     0,     0,     0,    42,     0,
      32,    23,    14,     0,    12,    17,    10,    56,     0,     0,
       0,     0,     0,    53,     0,    58,     0,    52,    54,     0,
      72,     0,    41,    44,    77,    83,     0,     0,    71,    85,
      86,    87,    89,    90,    40,     0,    29,    13,    18,    21,
      62,    63,     0,     0,    64,     0,    50,    51,    57,     0,
      49,     0,    76,    70,     0,     0,     0,     0,     0,    91,
      96,    99,   101,    67,     0,    65,     0,     0,    46,    82,
      69,    32,     0,    30,    21,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,    31,    34,
      38,    36,     0,    20,    59,    92,    94,    93,    95,    98,
      97,   100,   102,    61,    49,    35,     0,     0,    48,    39,
      37,    60
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -147,  -147,    58,   -58,  -147,   158,   193,   142,   -19,   -81,
      74,  -147,   170,   201,  -147,  -147,    80,  -140,  -135,  -147,
    -147,   -20,  -147,    48,   -21,   136,  -147,  -146,   -26,   112,
     -59,   101,  -147,  -147,   -38,  -147,   105,  -147,   -27,   -14,
      75,    76,  -147,   -25
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    34,    24,    32,     9,    84,
     137,    10,    30,    18,    19,    20,   116,    45,   153,   172,
      11,    37,    38,   148,    95,    96,    97,    98,    99,   138,
      47,    70,    48,    49,    50,    51,   106,    52,    53,   140,
     141,   142,   143,    85
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      46,    56,   119,    57,   100,    93,    36,    55,    36,     1,
     174,    26,   170,    72,    64,    39,    40,   171,    71,   183,
      28,    73,    74,    75,     2,     3,     4,    41,    42,    43,
      13,    44,   104,    78,    56,   109,   110,   111,   100,    93,
      12,   191,   105,   107,    94,   102,   189,   103,    36,   112,
     113,   190,     2,    68,     4,   154,    56,    69,   114,    39,
      40,    35,    76,    77,    14,    15,   125,    39,    40,    22,
      16,    41,    42,    43,     2,    44,     4,    21,    94,    41,
      42,    43,   124,    44,    26,    83,   118,    54,    56,    27,
     134,   185,    17,    28,   186,   139,   139,   100,    39,    40,
       2,    23,     4,   146,    25,   105,   100,    39,    40,    56,
      41,    42,    43,    29,    44,    31,   152,   169,    33,    41,
      42,    43,    58,    44,    60,    83,    46,    61,   100,    65,
     175,   176,   177,   178,   139,   139,   139,   139,    39,    40,
     157,   158,   159,   160,    63,   161,   162,   179,   180,    66,
      41,    42,    43,     1,    44,    67,    63,    79,    80,   101,
      46,    87,    88,    89,    90,    82,    91,    92,     2,   108,
       4,    39,    40,   115,   120,   122,   123,   121,   128,   129,
     126,   132,   147,    41,    42,    43,   130,    44,   131,    63,
      39,    40,   135,   133,    87,    88,    89,    90,   136,    91,
      92,   145,    41,    42,    43,    69,    44,   151,   152,    39,
      40,   152,   156,   155,   163,   164,   165,   167,   184,   166,
      86,    41,    42,    43,   117,    44,    62,   187,   173,    81,
      59,   168,   188,   127,   150,   144,   149,     0,   181,     0,
     182
};

static const yytype_int16 yycheck[] =
{
      26,    28,    83,    28,    63,    63,    25,    27,    27,    20,
     156,    16,   152,    51,    35,     5,     6,   152,    44,   165,
      25,     7,     8,     9,    35,    36,    37,    17,    18,    19,
      19,    21,    22,    54,    61,    73,    74,    75,    97,    97,
      35,   187,    68,    69,    63,    66,   186,    67,    67,    76,
      77,   186,    35,    21,    37,   136,    83,    25,    79,     5,
       6,    22,     5,     6,     0,     7,    92,     5,     6,    11,
      28,    17,    18,    19,    35,    21,    37,    28,    97,    17,
      18,    19,    28,    21,    16,    23,    24,    22,   115,    21,
     115,    24,    19,    25,    27,   122,   123,   156,     5,     6,
      35,    19,    37,   129,    21,   131,   165,     5,     6,   136,
      17,    18,    19,    27,    21,    25,    23,    24,    27,    17,
      18,    19,    19,    21,    18,    23,   152,    16,   187,    19,
     157,   158,   159,   160,   161,   162,   163,   164,     5,     6,
      10,    11,    12,    13,    23,    14,    15,   161,   162,    22,
      17,    18,    19,    20,    21,    27,    23,    22,    26,    25,
     186,    28,    29,    30,    31,    26,    33,    34,    35,    22,
      37,     5,     6,    25,    28,    21,    21,    28,    28,    16,
      24,    22,    25,    17,    18,    19,    26,    21,    27,    23,
       5,     6,    16,    26,    28,    29,    30,    31,    27,    33,
      34,    28,    17,    18,    19,    25,    21,    26,    23,     5,
       6,    23,    22,    24,     4,     3,    22,    18,    26,    28,
      62,    17,    18,    19,    82,    21,    33,    32,   154,    59,
      29,   151,   184,    97,   133,   123,   131,    -1,   163,    -1,
     164
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    20,    35,    36,    37,    39,    40,    41,    42,    46,
      49,    58,    35,    19,     0,    40,    28,    19,    51,    52,
      53,    28,    40,    19,    44,    21,    16,    21,    25,    27,
      50,    25,    45,    27,    43,    22,    46,    59,    60,     5,
       6,    17,    18,    19,    21,    55,    66,    68,    70,    71,
      72,    73,    75,    76,    22,    59,    76,    81,    19,    51,
      18,    16,    44,    23,    62,    19,    22,    27,    21,    25,
      69,    66,    72,     7,     8,     9,     5,     6,    62,    22,
      26,    50,    26,    23,    47,    81,    43,    28,    29,    30,
      31,    33,    34,    41,    46,    62,    63,    64,    65,    66,
      68,    25,    62,    59,    22,    66,    74,    66,    22,    72,
      72,    72,    76,    76,    62,    25,    54,    45,    24,    47,
      28,    28,    21,    21,    28,    66,    24,    63,    28,    16,
      26,    27,    22,    26,    81,    16,    27,    48,    67,    76,
      77,    78,    79,    80,    67,    28,    66,    25,    61,    74,
      69,    26,    23,    56,    47,    24,    22,    10,    11,    12,
      13,    14,    15,     4,     3,    22,    28,    18,    54,    24,
      55,    56,    57,    48,    65,    76,    76,    76,    76,    77,
      77,    78,    79,    65,    26,    24,    27,    32,    61,    55,
      56,    65
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    40,    40,    40,    40,    41,    41,    42,
      43,    43,    44,    45,    45,    46,    46,    47,    47,    47,
      48,    48,    49,    50,    50,    51,    51,    52,    52,    53,
      53,    54,    54,    55,    56,    56,    57,    57,    57,    57,
      58,    58,    58,    58,    59,    59,    60,    60,    61,    61,
      62,    63,    63,    64,    64,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    66,    67,    68,    69,
      69,    70,    70,    70,    71,    72,    72,    72,    72,    73,
      73,    73,    74,    74,    75,    75,    75,    75,    76,    76,
      76,    77,    77,    77,    77,    77,    78,    78,    78,    79,
      79,    80,    80,    81
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     2,     1,     1,     2,     2,     4,
       3,     0,     4,     4,     0,     1,     1,     1,     2,     4,
       3,     0,     3,     3,     0,     1,     1,     1,     3,     5,
       7,     4,     0,     1,     2,     3,     1,     3,     1,     3,
       6,     6,     5,     5,     3,     1,     5,     2,     4,     0,
       3,     2,     0,     1,     1,     4,     1,     2,     1,     5,
       7,     5,     2,     2,     2,     3,     1,     1,     2,     4,
       0,     3,     1,     1,     1,     1,     4,     3,     2,     1,
       1,     1,     3,     1,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 43 "calc_c.y"
                                           {(yyval.ptr) = (yyvsp[0].ptr);Driver((yyvsp[0].ptr));}
#line 1503 "calc_c.tab.c"
    break;

  case 3:
#line 45 "calc_c.y"
                                            {(yyval.ptr) = mknode(COMPUNIT_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1509 "calc_c.tab.c"
    break;

  case 4:
#line 46 "calc_c.y"
                                            {(yyval.ptr) = mknode(COMPUNIT_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1515 "calc_c.tab.c"
    break;

  case 5:
#line 47 "calc_c.y"
                                            {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1521 "calc_c.tab.c"
    break;

  case 6:
#line 48 "calc_c.y"
                                            {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1527 "calc_c.tab.c"
    break;

  case 7:
#line 50 "calc_c.y"
                                                        {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 1533 "calc_c.tab.c"
    break;

  case 8:
#line 51 "calc_c.y"
                                                        {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 1539 "calc_c.tab.c"
    break;

  case 9:
#line 54 "calc_c.y"
                                                   {(yyvsp[-1].ptr)->type = CONST_INT;if((yyvsp[-1].ptr)->kind == ID)(yyvsp[-1].ptr)->kind = VAR_DECL;
                                                                if((yyvsp[0].ptr) != NULL)
                                                                {(yyval.ptr) = mknode(VAR_DECL_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
                                                                else
                                                                {(yyval.ptr) = (yyvsp[-1].ptr);}}
#line 1549 "calc_c.tab.c"
    break;

  case 10:
#line 60 "calc_c.y"
                                                {(yyvsp[-1].ptr)->type=CONST_INT;(yyvsp[-1].ptr)->kind = VAR_DEF;(yyval.ptr) = mknode(VAR_DECL_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1555 "calc_c.tab.c"
    break;

  case 11:
#line 61 "calc_c.y"
                                                {(yyval.ptr) = NULL;}
#line 1561 "calc_c.tab.c"
    break;

  case 12:
#line 63 "calc_c.y"
                                                           {struct node *temp;if((yyvsp[-2].ptr) == NULL){temp=mknode(ID,NULL,NULL,NULL,yylineno);strcpy(temp->type_id,(yyvsp[-3].type_id));}
                                                             else{temp = mkparray(ARRAY_DEC,(yyvsp[-3].type_id),(yyvsp[-2].ptr),yylineno);temp->type = TOK_INT;strcpy(temp->type_id,(yyvsp[-3].type_id));}
                                                             temp->ptr[0]=(yyvsp[0].ptr);(yyval.ptr) = temp;}
#line 1569 "calc_c.tab.c"
    break;

  case 13:
#line 67 "calc_c.y"
                                                           {struct node * temp = mknode(TOK_INT,(yyvsp[0].ptr),NULL,NULL,yylineno);temp->type_int = (yyvsp[-2].type_int);(yyval.ptr) =temp;}
#line 1575 "calc_c.tab.c"
    break;

  case 14:
#line 68 "calc_c.y"
                                                           {(yyval.ptr) = NULL;}
#line 1581 "calc_c.tab.c"
    break;

  case 15:
#line 71 "calc_c.y"
                       {(yyval.ptr) = mknode(TOK_INT,NULL,NULL,NULL,yylineno);}
#line 1587 "calc_c.tab.c"
    break;

  case 16:
#line 72 "calc_c.y"
                       {(yyval.ptr) = mknode(TOK_FLOAT,NULL,NULL,NULL,yylineno);}
#line 1593 "calc_c.tab.c"
    break;

  case 17:
#line 74 "calc_c.y"
                                                                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1599 "calc_c.tab.c"
    break;

  case 18:
#line 75 "calc_c.y"
                                                                        {(yyval.ptr) = NULL;}
#line 1605 "calc_c.tab.c"
    break;

  case 19:
#line 76 "calc_c.y"
                                                                        {(yyval.ptr) = mknode(CONSTINITVAL_LIST,(yyvsp[-2].ptr),(yyvsp[-1].ptr),NULL,yylineno);}
#line 1611 "calc_c.tab.c"
    break;

  case 20:
#line 78 "calc_c.y"
                                                                {(yyval.ptr) = mknode(CONSTINITVAL_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1617 "calc_c.tab.c"
    break;

  case 21:
#line 79 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1623 "calc_c.tab.c"
    break;

  case 22:
#line 81 "calc_c.y"
                                                {
                                                (yyvsp[-1].ptr)->type = (yyvsp[-2].ptr)->kind;if((yyvsp[-1].ptr)->kind == ID)(yyvsp[-1].ptr)->kind = VAR_DECL;
                                                if((yyvsp[0].ptr) != NULL)
                                                 {struct node * temp = mknode(VAR_DECL_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);temp->type = (yyvsp[-2].ptr)->kind;(yyval.ptr) = temp;}
                                                else
                                                 {(yyval.ptr)=(yyvsp[-1].ptr);}
                                               }
#line 1635 "calc_c.tab.c"
    break;

  case 23:
#line 89 "calc_c.y"
                                                                {(yyvsp[-1].ptr)->kind = VAR_DECL;(yyval.ptr) = mknode(VAR_DECL_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1641 "calc_c.tab.c"
    break;

  case 24:
#line 90 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1647 "calc_c.tab.c"
    break;

  case 25:
#line 93 "calc_c.y"
                                                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1653 "calc_c.tab.c"
    break;

  case 26:
#line 94 "calc_c.y"
                                                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1659 "calc_c.tab.c"
    break;

  case 27:
#line 96 "calc_c.y"
                                                        {(yyval.ptr) = mknode(ID,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[0].type_id));}
#line 1665 "calc_c.tab.c"
    break;

  case 28:
#line 97 "calc_c.y"
                                                        {struct node *temp = mknode(ID,NULL,NULL,NULL,yylineno);strcpy(temp->type_id,(yyvsp[-2].type_id));temp->ptr[0] = (yyvsp[0].ptr);(yyval.ptr) = temp;}
#line 1671 "calc_c.tab.c"
    break;

  case 29:
#line 99 "calc_c.y"
                                                                                         {struct node * temp = mknode(TOK_INT,(yyvsp[0].ptr),NULL,NULL,yylineno);temp->type_int = const_exp((yyvsp[-2].ptr));(yyval.ptr) = mkparray(ARRAY_DEC,(yyvsp[-4].type_id),temp,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-4].type_id));}
#line 1677 "calc_c.tab.c"
    break;

  case 30:
#line 100 "calc_c.y"
                                                                                         {struct node * temp = mknode(TOK_INT,(yyvsp[-2].ptr),NULL,NULL,yylineno);temp->type_int = const_exp((yyvsp[-4].ptr));(yyval.ptr) = mkparray(ARRAY_DEC,(yyvsp[-6].type_id),temp,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-6].type_id));(yyval.ptr)->ptr[0] = (yyvsp[0].ptr);}
#line 1683 "calc_c.tab.c"
    break;

  case 31:
#line 102 "calc_c.y"
                                                           {struct node * temp = mknode(TOK_INT,(yyvsp[0].ptr),NULL,NULL,yylineno);temp->type_int = const_exp((yyvsp[-2].ptr));(yyval.ptr) =temp;}
#line 1689 "calc_c.tab.c"
    break;

  case 32:
#line 103 "calc_c.y"
                                                            {(yyval.ptr) = NULL;}
#line 1695 "calc_c.tab.c"
    break;

  case 33:
#line 105 "calc_c.y"
                                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1701 "calc_c.tab.c"
    break;

  case 34:
#line 107 "calc_c.y"
                                                                {(yyval.ptr) = mknode(INITARRAY,NULL,NULL,NULL,yylineno);}
#line 1707 "calc_c.tab.c"
    break;

  case 35:
#line 108 "calc_c.y"
                                                                {(yyval.ptr) = mknode(INITARRAY,(yyvsp[-1].ptr),NULL,NULL,yylineno);}
#line 1713 "calc_c.tab.c"
    break;

  case 36:
#line 110 "calc_c.y"
                                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1719 "calc_c.tab.c"
    break;

  case 37:
#line 111 "calc_c.y"
                                                                {push_initarray((yyvsp[-2].ptr),(yyvsp[0].ptr));(yyval.ptr) = (yyvsp[-2].ptr);}
#line 1725 "calc_c.tab.c"
    break;

  case 38:
#line 112 "calc_c.y"
                                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1731 "calc_c.tab.c"
    break;

  case 39:
#line 113 "calc_c.y"
                                                                {push_initarray((yyvsp[-2].ptr),(yyvsp[0].ptr));(yyval.ptr) = (yyvsp[-2].ptr);}
#line 1737 "calc_c.tab.c"
    break;

  case 40:
#line 115 "calc_c.y"
                                                             {struct node* temp = mknode(FUNC_DEF,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy(temp->type_id,(yyvsp[-4].type_id));
                                                                temp->type = TOK_INT;(yyval.ptr) = temp;}
#line 1744 "calc_c.tab.c"
    break;

  case 41:
#line 117 "calc_c.y"
                                                                {struct node* temp = mknode(FUNC_DEF,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy(temp->type_id,(yyvsp[-4].type_id));
                                                                temp->type = TOK_VOID;(yyval.ptr) = temp;}
#line 1751 "calc_c.tab.c"
    break;

  case 42:
#line 119 "calc_c.y"
                                                             {struct node* temp = mknode(FUNC_DEF,NULL,(yyvsp[0].ptr),NULL,yylineno);strcpy(temp->type_id,(yyvsp[-3].type_id));
                                                                temp->type = TOK_INT;(yyval.ptr) = temp;}
#line 1758 "calc_c.tab.c"
    break;

  case 43:
#line 121 "calc_c.y"
                                                                {struct node* temp = mknode(FUNC_DEF,NULL,(yyvsp[0].ptr),NULL,yylineno);strcpy(temp->type_id,(yyvsp[-3].type_id));
                                                                temp->type = TOK_VOID;(yyval.ptr) = temp;}
#line 1765 "calc_c.tab.c"
    break;

  case 44:
#line 124 "calc_c.y"
                                                                {(yyval.ptr) = mknode(PARAM_LIST,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1771 "calc_c.tab.c"
    break;

  case 45:
#line 125 "calc_c.y"
                                                                {(yyval.ptr) = mknode(PARAM_LIST,(yyvsp[0].ptr),NULL,NULL,yylineno);}
#line 1777 "calc_c.tab.c"
    break;

  case 46:
#line 127 "calc_c.y"
                                                                {struct node*temp = mkparray(PARAM_ARRAY,(yyvsp[-3].type_id),(yyvsp[0].ptr),yylineno);temp->type = (yyvsp[-4].ptr)->kind;strcpy(temp->type_id,(yyvsp[-3].type_id));(yyval.ptr) = temp;}
#line 1783 "calc_c.tab.c"
    break;

  case 47:
#line 128 "calc_c.y"
                                                                {struct node*temp = mknode(PARAM_DEC,NULL,NULL,NULL,yylineno); strcpy(temp->type_id,(yyvsp[0].type_id));temp->type = (yyvsp[-1].ptr)->type;(yyval.ptr) = temp;}
#line 1789 "calc_c.tab.c"
    break;

  case 48:
#line 130 "calc_c.y"
                                                               {struct node * temp = mknode(TOK_INT,NULL,NULL,NULL,yylineno);temp->type_int = (yyvsp[-2].type_int);temp->ptr[0] = (yyvsp[0].ptr);(yyval.ptr)= temp;}
#line 1795 "calc_c.tab.c"
    break;

  case 49:
#line 131 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1801 "calc_c.tab.c"
    break;

  case 50:
#line 135 "calc_c.y"
                                                                {(yyval.ptr) = mknode(BLOCK,(yyvsp[-1].ptr),NULL,NULL,yylineno);}
#line 1807 "calc_c.tab.c"
    break;

  case 51:
#line 137 "calc_c.y"
                                                                {(yyval.ptr) = mknode(BLOCK_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1813 "calc_c.tab.c"
    break;

  case 52:
#line 138 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1819 "calc_c.tab.c"
    break;

  case 53:
#line 140 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1825 "calc_c.tab.c"
    break;

  case 54:
#line 141 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1831 "calc_c.tab.c"
    break;

  case 55:
#line 143 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_ASSIGN,(yyvsp[-3].ptr),(yyvsp[-1].ptr),NULL,yylineno);}
#line 1837 "calc_c.tab.c"
    break;

  case 56:
#line 144 "calc_c.y"
                                                {(yyval.ptr) = NULL;}
#line 1843 "calc_c.tab.c"
    break;

  case 57:
#line 145 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 1849 "calc_c.tab.c"
    break;

  case 58:
#line 146 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1855 "calc_c.tab.c"
    break;

  case 59:
#line 147 "calc_c.y"
                                                {(yyval.ptr) = mknode(IF_THEN,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1861 "calc_c.tab.c"
    break;

  case 60:
#line 148 "calc_c.y"
                                                        {(yyval.ptr) = mknode(IF_THEN_ELSE,(yyvsp[-4].ptr),(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);}
#line 1867 "calc_c.tab.c"
    break;

  case 61:
#line 149 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_WHILE,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1873 "calc_c.tab.c"
    break;

  case 62:
#line 150 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_BREAK,NULL,NULL,NULL,yylineno);}
#line 1879 "calc_c.tab.c"
    break;

  case 63:
#line 151 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_CONTINUE,NULL,NULL,NULL,yylineno);}
#line 1885 "calc_c.tab.c"
    break;

  case 64:
#line 152 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_RETURN,NULL,NULL,NULL,yylineno);}
#line 1891 "calc_c.tab.c"
    break;

  case 65:
#line 153 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_RETURN,(yyvsp[-1].ptr),NULL,NULL,yylineno);}
#line 1897 "calc_c.tab.c"
    break;

  case 66:
#line 155 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1903 "calc_c.tab.c"
    break;

  case 67:
#line 157 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1909 "calc_c.tab.c"
    break;

  case 68:
#line 160 "calc_c.y"
                                                {(yyval.ptr) = mknode(ID,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));if((yyvsp[0].ptr) != NULL){(yyval.ptr)->kind = EXP_ARRAY;(yyval.ptr)->ptr[0] = (yyvsp[0].ptr);}}
#line 1915 "calc_c.tab.c"
    break;

  case 69:
#line 162 "calc_c.y"
                                           {(yyval.ptr) = mknode(EXP_ARRAY,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1921 "calc_c.tab.c"
    break;

  case 70:
#line 163 "calc_c.y"
                                           {(yyval.ptr) = NULL;}
#line 1927 "calc_c.tab.c"
    break;

  case 71:
#line 165 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 1933 "calc_c.tab.c"
    break;

  case 72:
#line 166 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1939 "calc_c.tab.c"
    break;

  case 73:
#line 167 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1945 "calc_c.tab.c"
    break;

  case 74:
#line 169 "calc_c.y"
                                                {(yyval.ptr) = mknode(LITERAL,NULL,NULL,NULL,yylineno);(yyval.ptr)->type_int = (yyvsp[0].type_int);(yyval.ptr)->type = INT;}
#line 1951 "calc_c.tab.c"
    break;

  case 75:
#line 171 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1957 "calc_c.tab.c"
    break;

  case 76:
#line 172 "calc_c.y"
                                                {(yyval.ptr) = mknode(FUNC_CALL,(yyvsp[-1].ptr),NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-3].type_id));}
#line 1963 "calc_c.tab.c"
    break;

  case 77:
#line 173 "calc_c.y"
                                                {(yyval.ptr) = mknode(FUNC_CALL,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-2].type_id));}
#line 1969 "calc_c.tab.c"
    break;

  case 78:
#line 174 "calc_c.y"
                                                {(yyval.ptr) = mknode(UNARYEXP,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1975 "calc_c.tab.c"
    break;

  case 79:
#line 176 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_ADD,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_ADD");}
#line 1981 "calc_c.tab.c"
    break;

  case 80:
#line 177 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_SUB,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_SUB");}
#line 1987 "calc_c.tab.c"
    break;

  case 81:
#line 178 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_NOT,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_NOT");}
#line 1993 "calc_c.tab.c"
    break;

  case 82:
#line 180 "calc_c.y"
                                                {(yyval.ptr) = mknode(ARGS,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1999 "calc_c.tab.c"
    break;

  case 83:
#line 181 "calc_c.y"
                                                {(yyval.ptr) = mknode(ARGS,(yyvsp[0].ptr),NULL,NULL,yylineno);}
#line 2005 "calc_c.tab.c"
    break;

  case 84:
#line 183 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2011 "calc_c.tab.c"
    break;

  case 85:
#line 184 "calc_c.y"
                                                {(yyval.ptr) = mkopnode(TOK_MUL,(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);}
#line 2017 "calc_c.tab.c"
    break;

  case 86:
#line 185 "calc_c.y"
                                                {(yyval.ptr) = mkopnode(TOK_DIV,(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);}
#line 2023 "calc_c.tab.c"
    break;

  case 87:
#line 186 "calc_c.y"
                                                {(yyval.ptr) = mkopnode(TOK_MODULO,(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);}
#line 2029 "calc_c.tab.c"
    break;

  case 88:
#line 188 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2035 "calc_c.tab.c"
    break;

  case 89:
#line 189 "calc_c.y"
                                                {(yyval.ptr) = mkopnode(TOK_ADD,(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);}
#line 2041 "calc_c.tab.c"
    break;

  case 90:
#line 190 "calc_c.y"
                                                {(yyval.ptr) = mkopnode(TOK_SUB,(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);}
#line 2047 "calc_c.tab.c"
    break;

  case 91:
#line 192 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2053 "calc_c.tab.c"
    break;

  case 92:
#line 193 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_LESS,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2059 "calc_c.tab.c"
    break;

  case 93:
#line 194 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_GREAT,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2065 "calc_c.tab.c"
    break;

  case 94:
#line 195 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_LESSEQ,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2071 "calc_c.tab.c"
    break;

  case 95:
#line 196 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_GREATEQ,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2077 "calc_c.tab.c"
    break;

  case 96:
#line 198 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2083 "calc_c.tab.c"
    break;

  case 97:
#line 199 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_EQ,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2089 "calc_c.tab.c"
    break;

  case 98:
#line 200 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_NOTEQ,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2095 "calc_c.tab.c"
    break;

  case 99:
#line 202 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2101 "calc_c.tab.c"
    break;

  case 100:
#line 203 "calc_c.y"
                                                {(yyval.ptr)=mknode(TOK_AND,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_AND");}
#line 2107 "calc_c.tab.c"
    break;

  case 101:
#line 205 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2113 "calc_c.tab.c"
    break;

  case 102:
#line 206 "calc_c.y"
                                                {(yyval.ptr)=mknode(TOK_OR,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_OR");;}
#line 2119 "calc_c.tab.c"
    break;

  case 103:
#line 208 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2125 "calc_c.tab.c"
    break;


#line 2129 "calc_c.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 211 "calc_c.y"



#include<stdarg.h>
void yyerror(char* s, ...)
{
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d:error:", yylineno); //错误行号
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}
