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
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  185

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
      78,    79,    81,    87,    88,    92,    96,   100,   101,   103,
     104,   105,   106,   108,   109,   111,   113,   115,   117,   120,
     121,   123,   124,   126,   127,   131,   133,   134,   136,   137,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   151,   153,   156,   158,   159,   161,   162,   163,   165,
     167,   168,   169,   170,   172,   173,   174,   176,   177,   179,
     180,   181,   182,   184,   185,   186,   188,   189,   190,   191,
     192,   194,   195,   196,   198,   199,   201,   202,   204
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
  "constinitval_", "vardecl", "vardecl_", "vardef", "vardef_", "initval",
  "initval_", "funcdef", "funcfparams", "funcfparam", "funcfparam_",
  "block", "block_", "blockitem", "stmt", "exp", "cond", "lval", "lval_",
  "primaryexp", "number", "unaryexp", "unaryop", "funcrparams", "mulexp",
  "addexp", "relexp", "eqexp", "landexp", "lorexp", "constexp", YY_NULLPTR
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

#define YYPACT_NINF (-144)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      22,   -16,    -7,    11,  -144,    48,  -144,    22,  -144,    50,
    -144,    22,    57,    59,    64,  -144,  -144,    62,    67,  -144,
      71,    85,   -14,    73,   175,    82,    50,    90,   103,   106,
      57,   100,   108,  -144,   113,   122,   121,   108,   131,  -144,
    -144,  -144,  -144,    -1,   175,  -144,  -144,  -144,  -144,   175,
     142,    12,   130,   149,    67,  -144,   135,   168,    85,  -144,
      44,  -144,   132,   108,    31,  -144,   108,    10,   175,  -144,
     147,    12,  -144,   175,   175,   175,   175,   175,    62,    65,
    -144,  -144,  -144,    71,   141,  -144,  -144,  -144,  -144,   143,
     148,   154,   156,   124,  -144,  -144,   139,    44,  -144,   150,
     163,   157,  -144,  -144,  -144,  -144,   155,   162,   164,  -144,
    -144,  -144,  -144,  -144,  -144,  -144,  -144,   161,  -144,  -144,
     170,  -144,  -144,   175,   175,  -144,   167,  -144,  -144,  -144,
     175,   173,   175,  -144,   174,   175,   176,   168,   177,   180,
      12,   114,    40,   199,   201,   183,  -144,   178,   189,  -144,
    -144,  -144,   161,   149,   170,  -144,    86,   175,   175,   175,
     175,   175,   175,   175,   175,    86,  -144,   182,  -144,  -144,
    -144,   179,    12,    12,    12,    12,   114,   114,    40,   199,
    -144,   173,    86,  -144,  -144
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,    16,     0,     2,     5,     7,     0,
       8,     6,     0,     0,     0,     1,     3,    28,    24,     4,
      14,    11,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,     0,    15,     0,     0,    40,     0,     0,    74,
      75,    76,    69,    65,     0,    67,    70,    68,    79,     0,
      83,    98,     0,    32,    24,    22,     0,     0,    11,     9,
      47,    37,    42,     0,     0,    38,     0,     0,     0,    63,
       0,    61,    73,     0,     0,     0,     0,     0,    28,     0,
      26,    29,    23,    14,     0,    12,    17,    10,    51,     0,
       0,     0,     0,     0,    48,    53,     0,    47,    49,     0,
      67,     0,    35,    39,    36,    72,    78,     0,     0,    66,
      80,    81,    82,    84,    85,    27,    31,    34,    13,    18,
      21,    57,    58,     0,     0,    59,     0,    45,    46,    52,
       0,    44,     0,    71,    65,     0,     0,     0,     0,     0,
      86,    91,    94,    96,    62,     0,    60,     0,     0,    41,
      77,    64,    34,    32,    21,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,     0,    33,    30,
      20,    54,    87,    89,    88,    90,    93,    92,    95,    97,
      56,    44,     0,    43,    55
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -144,  -144,    34,   -53,  -144,   151,   184,   127,   -18,   -81,
      58,  -144,   159,   190,   137,    66,    68,  -144,   -17,  -144,
      36,   -23,   125,  -144,  -143,   -42,    94,   -59,    87,  -144,
    -144,   -39,  -144,    91,  -144,   -24,   -60,    61,    63,  -144,
     202
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,     8,    31,    21,    29,     9,    85,
     138,    10,    27,    18,    25,    80,   136,    11,    35,    36,
     149,    95,    96,    97,    98,    99,   139,    45,    69,    46,
      47,    48,    49,   107,    50,    71,   141,   142,   143,   144,
      86
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      51,   100,    70,   120,    34,    34,    38,    94,    32,    61,
      72,    81,    13,   171,    65,    39,    40,    76,    77,    12,
      67,    33,   180,     4,    68,   106,   108,    41,    42,    43,
      14,    44,   105,    51,   110,   111,   112,   117,   100,   184,
     102,    16,     1,   104,    94,    19,    34,   103,    15,    39,
      40,   126,   113,   114,   161,   162,   154,     2,     3,     4,
      51,    41,    42,    43,     1,    44,    33,    60,     4,    17,
      39,    40,    88,    89,    90,    91,    20,    92,    93,    33,
      22,     4,    41,    42,    43,    23,    44,    24,   147,   116,
     106,    39,    40,   152,    26,    37,    28,   100,    53,   140,
     140,   176,   177,    41,    42,    43,   100,    44,    33,    60,
       4,    81,    30,    51,    88,    89,    90,    91,    55,    92,
      93,    56,    57,   100,   157,   158,   159,   160,    59,    39,
      40,    60,    62,   172,   173,   174,   175,   140,   140,   140,
     140,    41,    42,    43,    63,    44,    39,    40,    64,    73,
      74,    75,   125,    66,    39,    40,    78,   101,    41,    42,
      43,    83,    44,   127,    84,   119,    41,    42,    43,   109,
      44,   121,    79,    39,    40,   123,   122,   124,   129,   130,
      39,    40,   132,   131,   133,    41,    42,    43,   135,    44,
     134,    84,    41,    42,    43,   146,    44,   137,   148,    68,
     153,   155,   156,   163,   164,   165,   166,   167,   181,    87,
     118,   182,   170,    82,    58,   115,    54,   183,   145,   169,
     168,   151,   128,   150,   178,     0,    52,   179
};

static const yytype_int16 yycheck[] =
{
      24,    60,    44,    84,    22,    23,    23,    60,    22,    32,
      49,    53,    19,   156,    37,     5,     6,     5,     6,    35,
      21,    35,   165,    37,    25,    67,    68,    17,    18,    19,
      19,    21,    22,    57,    73,    74,    75,    79,    97,   182,
      63,     7,    20,    66,    97,    11,    64,    64,     0,     5,
       6,    93,    76,    77,    14,    15,   137,    35,    36,    37,
      84,    17,    18,    19,    20,    21,    35,    23,    37,    19,
       5,     6,    28,    29,    30,    31,    19,    33,    34,    35,
      21,    37,    17,    18,    19,    21,    21,    25,   130,    24,
     132,     5,     6,   135,    27,    22,    25,   156,    16,   123,
     124,   161,   162,    17,    18,    19,   165,    21,    35,    23,
      37,   153,    27,   137,    28,    29,    30,    31,    28,    33,
      34,    18,    16,   182,    10,    11,    12,    13,    28,     5,
       6,    23,    19,   157,   158,   159,   160,   161,   162,   163,
     164,    17,    18,    19,    22,    21,     5,     6,    27,     7,
       8,     9,    28,    22,     5,     6,    26,    25,    17,    18,
      19,    26,    21,    24,    23,    24,    17,    18,    19,    22,
      21,    28,    23,     5,     6,    21,    28,    21,    28,    16,
       5,     6,    27,    26,    22,    17,    18,    19,    27,    21,
      26,    23,    17,    18,    19,    28,    21,    27,    25,    25,
      24,    24,    22,     4,     3,    22,    28,    18,    26,    58,
      83,    32,   154,    54,    30,    78,    26,   181,   124,   153,
     152,   134,    97,   132,   163,    -1,    24,   164
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    20,    35,    36,    37,    39,    40,    41,    42,    46,
      49,    55,    35,    19,    19,     0,    40,    19,    51,    40,
      19,    44,    21,    21,    25,    52,    27,    50,    25,    45,
      27,    43,    22,    35,    46,    56,    57,    22,    56,     5,
       6,    17,    18,    19,    21,    65,    67,    68,    69,    70,
      72,    73,    78,    16,    51,    28,    18,    16,    44,    28,
      23,    59,    19,    22,    27,    59,    22,    21,    25,    66,
      63,    73,    69,     7,     8,     9,     5,     6,    26,    23,
      53,    63,    50,    26,    23,    47,    78,    43,    28,    29,
      30,    31,    33,    34,    41,    59,    60,    61,    62,    63,
      65,    25,    59,    56,    59,    22,    63,    71,    63,    22,
      69,    69,    69,    73,    73,    52,    24,    63,    45,    24,
      47,    28,    28,    21,    21,    28,    63,    24,    60,    28,
      16,    26,    27,    22,    26,    27,    54,    27,    48,    64,
      73,    74,    75,    76,    77,    64,    28,    63,    25,    58,
      71,    66,    63,    24,    47,    24,    22,    10,    11,    12,
      13,    14,    15,     4,     3,    22,    28,    18,    54,    53,
      48,    62,    73,    73,    73,    73,    74,    74,    75,    76,
      62,    26,    32,    58,    62
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    38,    39,    40,    40,    40,    40,    41,    41,    42,
      43,    43,    44,    45,    45,    46,    46,    47,    47,    47,
      48,    48,    49,    50,    50,    51,    51,    52,    52,    53,
      53,    53,    53,    54,    54,    55,    55,    55,    55,    56,
      56,    57,    57,    58,    58,    59,    60,    60,    61,    61,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    63,    64,    65,    66,    66,    67,    67,    67,    68,
      69,    69,    69,    69,    70,    70,    70,    71,    71,    72,
      72,    72,    72,    73,    73,    73,    74,    74,    74,    74,
      74,    75,    75,    75,    76,    76,    77,    77,    78
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     2,     1,     1,     1,     1,     5,
       3,     0,     4,     4,     0,     1,     1,     1,     2,     4,
       3,     0,     4,     3,     0,     2,     4,     4,     0,     1,
       5,     2,     0,     3,     0,     6,     6,     5,     5,     3,
       1,     5,     2,     4,     0,     3,     2,     0,     1,     1,
       4,     1,     2,     1,     5,     7,     5,     2,     2,     2,
       3,     1,     1,     2,     4,     0,     3,     1,     1,     1,
       1,     4,     3,     2,     1,     1,     1,     3,     1,     1,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     1,     3,     1,     3,     1
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
                                           {(yyval.ptr) = (yyvsp[0].ptr);DisplaySymbolTable((yyvsp[0].ptr));}
#line 1492 "calc_c.tab.c"
    break;

  case 3:
#line 45 "calc_c.y"
                                            {(yyval.ptr) = mknode(COMPUNIT_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1498 "calc_c.tab.c"
    break;

  case 4:
#line 46 "calc_c.y"
                                            {(yyval.ptr) = mknode(COMPUNIT_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1504 "calc_c.tab.c"
    break;

  case 5:
#line 47 "calc_c.y"
                                            {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1510 "calc_c.tab.c"
    break;

  case 6:
#line 48 "calc_c.y"
                                            {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1516 "calc_c.tab.c"
    break;

  case 7:
#line 50 "calc_c.y"
                                           {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1522 "calc_c.tab.c"
    break;

  case 8:
#line 51 "calc_c.y"
                                           {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1528 "calc_c.tab.c"
    break;

  case 9:
#line 54 "calc_c.y"
                                                                {(yyvsp[-2].ptr)->type = CONST_INT;if((yyvsp[-2].ptr)->kind == ID)(yyvsp[-2].ptr)->kind = VAR_DECL;
                                                                if((yyvsp[-1].ptr) != NULL)
                                                                {(yyval.ptr) = mknode(VAR_DECL_LIST,(yyvsp[-2].ptr),(yyvsp[-1].ptr),NULL,yylineno);}
                                                                else
                                                                {(yyval.ptr) = (yyvsp[-2].ptr);}}
#line 1538 "calc_c.tab.c"
    break;

  case 10:
#line 60 "calc_c.y"
                                                {(yyvsp[-1].ptr)->type=CONST_INT;(yyvsp[-1].ptr)->kind = VAR_DEF;(yyval.ptr) = mknode(VAR_DECL_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1544 "calc_c.tab.c"
    break;

  case 11:
#line 61 "calc_c.y"
                                                {(yyval.ptr) = NULL;}
#line 1550 "calc_c.tab.c"
    break;

  case 12:
#line 63 "calc_c.y"
                                                           {struct node *temp;if((yyvsp[-2].ptr) == NULL){temp=mknode(ID,NULL,NULL,NULL,yylineno);strcpy(temp->type_id,(yyvsp[-3].type_id));}
                                                             else{temp = mkparray(ARRAY_DEC,(yyvsp[-3].type_id),(yyvsp[-2].ptr),yylineno);temp->type = TOK_INT;strcpy(temp->type_id,(yyvsp[-3].type_id));}
                                                             temp->ptr[0]=(yyvsp[0].ptr);(yyval.ptr) = temp;}
#line 1558 "calc_c.tab.c"
    break;

  case 13:
#line 67 "calc_c.y"
                                                           {struct node * temp = mknode(TOK_INT,(yyvsp[0].ptr),NULL,NULL,yylineno);temp->type_int = (yyvsp[-2].type_int);(yyval.ptr) =temp;}
#line 1564 "calc_c.tab.c"
    break;

  case 14:
#line 68 "calc_c.y"
                                                           {(yyval.ptr) = NULL;}
#line 1570 "calc_c.tab.c"
    break;

  case 15:
#line 71 "calc_c.y"
                        {(yyval.ptr) = mknode(TOK_INT,NULL,NULL,NULL,yylineno);}
#line 1576 "calc_c.tab.c"
    break;

  case 16:
#line 72 "calc_c.y"
                        {(yyval.ptr) = mknode(TOK_FLOAT,NULL,NULL,NULL,yylineno);}
#line 1582 "calc_c.tab.c"
    break;

  case 17:
#line 74 "calc_c.y"
                                                                        {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1588 "calc_c.tab.c"
    break;

  case 18:
#line 75 "calc_c.y"
                                                                        {(yyval.ptr) = NULL;}
#line 1594 "calc_c.tab.c"
    break;

  case 19:
#line 76 "calc_c.y"
                                                                        {(yyval.ptr) = mknode(CONSTINITVAL_LIST,(yyvsp[-2].ptr),(yyvsp[-1].ptr),NULL,yylineno);}
#line 1600 "calc_c.tab.c"
    break;

  case 20:
#line 78 "calc_c.y"
                                                                {(yyval.ptr) = mknode(CONSTINITVAL_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1606 "calc_c.tab.c"
    break;

  case 21:
#line 79 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1612 "calc_c.tab.c"
    break;

  case 22:
#line 81 "calc_c.y"
                                                               {(yyvsp[-2].ptr)->type = (yyvsp[-3].ptr)->kind;if((yyvsp[-2].ptr)->kind == ID)(yyvsp[-2].ptr)->kind = VAR_DECL;
                                                               if((yyvsp[-1].ptr) != NULL)
                                                                 {struct node * temp = mknode(VAR_DECL_LIST,(yyvsp[-2].ptr),(yyvsp[-1].ptr),NULL,yylineno);temp->type = (yyvsp[-3].ptr)->kind;(yyval.ptr) = temp;}
                                                               else
                                                                 {(yyval.ptr)=(yyvsp[-2].ptr);}}
#line 1622 "calc_c.tab.c"
    break;

  case 23:
#line 87 "calc_c.y"
                                                                {(yyvsp[-1].ptr)->kind = VAR_DECL;(yyval.ptr) = mknode(VAR_DECL_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1628 "calc_c.tab.c"
    break;

  case 24:
#line 88 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1634 "calc_c.tab.c"
    break;

  case 25:
#line 92 "calc_c.y"
                                                            {if((yyvsp[0].ptr) == NULL)
                                                               {(yyval.ptr) =mknode(ID,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
                                                             else
                                                               {struct node*temp = mkparray(ARRAY_DEC,(yyvsp[-1].type_id),(yyvsp[0].ptr),yylineno);strcpy(temp->type_id,(yyvsp[-1].type_id));(yyval.ptr) = temp;}}
#line 1643 "calc_c.tab.c"
    break;

  case 26:
#line 96 "calc_c.y"
                                                            {struct node *temp;if((yyvsp[-2].ptr) == NULL){temp =mknode(ID,NULL,NULL,NULL,yylineno);strcpy(temp->type_id,(yyvsp[-3].type_id));}
                                                             else{temp = mkparray(ARRAY_DEC,(yyvsp[-3].type_id),(yyvsp[-2].ptr),yylineno);strcpy(temp->type_id,(yyvsp[-3].type_id));}
                                                             temp->ptr[0]=(yyvsp[0].ptr);(yyval.ptr) = temp;}
#line 1651 "calc_c.tab.c"
    break;

  case 27:
#line 100 "calc_c.y"
                                                           {struct node * temp = mknode(TOK_INT,(yyvsp[0].ptr),NULL,NULL,yylineno);temp->type_int = const_exp((yyvsp[-2].ptr));(yyval.ptr) =temp;}
#line 1657 "calc_c.tab.c"
    break;

  case 28:
#line 101 "calc_c.y"
                                                            {(yyval.ptr) = NULL;}
#line 1663 "calc_c.tab.c"
    break;

  case 29:
#line 103 "calc_c.y"
                                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1669 "calc_c.tab.c"
    break;

  case 30:
#line 104 "calc_c.y"
                                                                {struct node *temp = mknode(EXP_LIST,(yyvsp[-3].ptr),(yyvsp[-2].ptr),NULL,yylineno);(yyval.ptr) = mknode(INITVAL_LIST,temp,(yyvsp[0].ptr),NULL,yylineno);}
#line 1675 "calc_c.tab.c"
    break;

  case 31:
#line 105 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1681 "calc_c.tab.c"
    break;

  case 32:
#line 106 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1687 "calc_c.tab.c"
    break;

  case 33:
#line 108 "calc_c.y"
                                                                {(yyval.ptr) = mknode(EXP_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1693 "calc_c.tab.c"
    break;

  case 34:
#line 109 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1699 "calc_c.tab.c"
    break;

  case 35:
#line 111 "calc_c.y"
                                                               {struct node* temp = mknode(FUNC_DEF,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy(temp->type_id,(yyvsp[-4].type_id));
                                                                temp->type = TOK_INT;(yyval.ptr) = temp;}
#line 1706 "calc_c.tab.c"
    break;

  case 36:
#line 113 "calc_c.y"
                                                                {struct node* temp = mknode(FUNC_DEF,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy(temp->type_id,(yyvsp[-4].type_id));
                                                                temp->type = TOK_VOID;(yyval.ptr) = temp;}
#line 1713 "calc_c.tab.c"
    break;

  case 37:
#line 115 "calc_c.y"
                                                               {struct node* temp = mknode(FUNC_DEF,NULL,(yyvsp[0].ptr),NULL,yylineno);strcpy(temp->type_id,(yyvsp[-3].type_id));
                                                                temp->type = TOK_INT;(yyval.ptr) = temp;}
#line 1720 "calc_c.tab.c"
    break;

  case 38:
#line 117 "calc_c.y"
                                                                {struct node* temp = mknode(FUNC_DEF,NULL,(yyvsp[0].ptr),NULL,yylineno);strcpy(temp->type_id,(yyvsp[-3].type_id));
                                                                temp->type = TOK_VOID;(yyval.ptr) = temp;}
#line 1727 "calc_c.tab.c"
    break;

  case 39:
#line 120 "calc_c.y"
                                                                {(yyval.ptr) = mknode(PARAM_LIST,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1733 "calc_c.tab.c"
    break;

  case 40:
#line 121 "calc_c.y"
                                                                {(yyval.ptr) = mknode(PARAM_LIST,(yyvsp[0].ptr),NULL,NULL,yylineno);}
#line 1739 "calc_c.tab.c"
    break;

  case 41:
#line 123 "calc_c.y"
                                                                {struct node*temp = mkparray(PARAM_ARRAY,(yyvsp[-3].type_id),(yyvsp[0].ptr),yylineno);temp->type = (yyvsp[-4].ptr)->kind;strcpy(temp->type_id,(yyvsp[-3].type_id));(yyval.ptr) = temp;}
#line 1745 "calc_c.tab.c"
    break;

  case 42:
#line 124 "calc_c.y"
                                                                {struct node*temp = mknode(PARAM_DEC,NULL,NULL,NULL,yylineno); strcpy(temp->type_id,(yyvsp[0].type_id));temp->type = (yyvsp[-1].ptr)->type;(yyval.ptr) = temp;}
#line 1751 "calc_c.tab.c"
    break;

  case 43:
#line 126 "calc_c.y"
                                                               {struct node * temp = mknode(TOK_INT,NULL,NULL,NULL,yylineno);temp->type_int = (yyvsp[-2].type_int);temp->ptr[0] = (yyvsp[0].ptr);(yyval.ptr)= temp;}
#line 1757 "calc_c.tab.c"
    break;

  case 44:
#line 127 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1763 "calc_c.tab.c"
    break;

  case 45:
#line 131 "calc_c.y"
                                                                {(yyval.ptr) = mknode(BLOCK,(yyvsp[-1].ptr),NULL,NULL,yylineno);}
#line 1769 "calc_c.tab.c"
    break;

  case 46:
#line 133 "calc_c.y"
                                                                {(yyval.ptr) = mknode(BLOCK_LIST,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1775 "calc_c.tab.c"
    break;

  case 47:
#line 134 "calc_c.y"
                                                                {(yyval.ptr) = NULL;}
#line 1781 "calc_c.tab.c"
    break;

  case 48:
#line 136 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1787 "calc_c.tab.c"
    break;

  case 49:
#line 137 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1793 "calc_c.tab.c"
    break;

  case 50:
#line 139 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_ASSIGN,(yyvsp[-3].ptr),(yyvsp[-1].ptr),NULL,yylineno);}
#line 1799 "calc_c.tab.c"
    break;

  case 51:
#line 140 "calc_c.y"
                                                {(yyval.ptr) = NULL;}
#line 1805 "calc_c.tab.c"
    break;

  case 52:
#line 141 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 1811 "calc_c.tab.c"
    break;

  case 53:
#line 142 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1817 "calc_c.tab.c"
    break;

  case 54:
#line 143 "calc_c.y"
                                                {(yyval.ptr) = mknode(IF_THEN,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1823 "calc_c.tab.c"
    break;

  case 55:
#line 144 "calc_c.y"
                                                        {(yyval.ptr) = mknode(IF_THEN_ELSE,(yyvsp[-4].ptr),(yyvsp[-2].ptr),(yyvsp[0].ptr),yylineno);}
#line 1829 "calc_c.tab.c"
    break;

  case 56:
#line 145 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_WHILE,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1835 "calc_c.tab.c"
    break;

  case 57:
#line 146 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_BREAK,NULL,NULL,NULL,yylineno);}
#line 1841 "calc_c.tab.c"
    break;

  case 58:
#line 147 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_CONTINUE,NULL,NULL,NULL,yylineno);}
#line 1847 "calc_c.tab.c"
    break;

  case 59:
#line 148 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_RETURN,NULL,NULL,NULL,yylineno);}
#line 1853 "calc_c.tab.c"
    break;

  case 60:
#line 149 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_RETURN,(yyvsp[-1].ptr),NULL,NULL,yylineno);}
#line 1859 "calc_c.tab.c"
    break;

  case 61:
#line 151 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1865 "calc_c.tab.c"
    break;

  case 62:
#line 153 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1871 "calc_c.tab.c"
    break;

  case 63:
#line 156 "calc_c.y"
                                                {(yyval.ptr) = mknode(ID,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));if((yyvsp[0].ptr) != NULL){(yyval.ptr)->kind = EXP_ARRAY;(yyval.ptr)->ptr[0] = (yyvsp[0].ptr);}}
#line 1877 "calc_c.tab.c"
    break;

  case 64:
#line 158 "calc_c.y"
                                           {(yyval.ptr) = mknode(EXP_ARRAY,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1883 "calc_c.tab.c"
    break;

  case 65:
#line 159 "calc_c.y"
                                           {(yyval.ptr) = NULL;}
#line 1889 "calc_c.tab.c"
    break;

  case 66:
#line 161 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[-1].ptr);}
#line 1895 "calc_c.tab.c"
    break;

  case 67:
#line 162 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1901 "calc_c.tab.c"
    break;

  case 68:
#line 163 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1907 "calc_c.tab.c"
    break;

  case 69:
#line 165 "calc_c.y"
                                                {(yyval.ptr) = mknode(INT,NULL,NULL,NULL,yylineno);(yyval.ptr)->type_int = (yyvsp[0].type_int);(yyval.ptr)->type = INT;}
#line 1913 "calc_c.tab.c"
    break;

  case 70:
#line 167 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1919 "calc_c.tab.c"
    break;

  case 71:
#line 168 "calc_c.y"
                                                {(yyval.ptr) = mknode(FUNC_CALL,(yyvsp[-1].ptr),NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-3].type_id));}
#line 1925 "calc_c.tab.c"
    break;

  case 72:
#line 169 "calc_c.y"
                                                {(yyval.ptr) = mknode(FUNC_CALL,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-2].type_id));}
#line 1931 "calc_c.tab.c"
    break;

  case 73:
#line 170 "calc_c.y"
                                                {(yyval.ptr) = mknode(UNARYEXP,(yyvsp[-1].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1937 "calc_c.tab.c"
    break;

  case 74:
#line 172 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_ADD,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_ADD");}
#line 1943 "calc_c.tab.c"
    break;

  case 75:
#line 173 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_SUB,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_SUB");}
#line 1949 "calc_c.tab.c"
    break;

  case 76:
#line 174 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_NOT,NULL,NULL,NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_NOT");}
#line 1955 "calc_c.tab.c"
    break;

  case 77:
#line 176 "calc_c.y"
                                                {(yyval.ptr) = mknode(ARGS,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);}
#line 1961 "calc_c.tab.c"
    break;

  case 78:
#line 177 "calc_c.y"
                                                {(yyval.ptr) = mknode(ARGS,(yyvsp[0].ptr),NULL,NULL,yylineno);}
#line 1967 "calc_c.tab.c"
    break;

  case 79:
#line 179 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1973 "calc_c.tab.c"
    break;

  case 80:
#line 180 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_MUL,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_MUL");}
#line 1979 "calc_c.tab.c"
    break;

  case 81:
#line 181 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_DIV,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_DIV");}
#line 1985 "calc_c.tab.c"
    break;

  case 82:
#line 182 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_MODULO,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_MODULO");}
#line 1991 "calc_c.tab.c"
    break;

  case 83:
#line 184 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 1997 "calc_c.tab.c"
    break;

  case 84:
#line 185 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_ADD,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_ADD");}
#line 2003 "calc_c.tab.c"
    break;

  case 85:
#line 186 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_SUB,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_SUB");}
#line 2009 "calc_c.tab.c"
    break;

  case 86:
#line 188 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2015 "calc_c.tab.c"
    break;

  case 87:
#line 189 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_LESS,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2021 "calc_c.tab.c"
    break;

  case 88:
#line 190 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_GREAT,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2027 "calc_c.tab.c"
    break;

  case 89:
#line 191 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_LESSEQ,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2033 "calc_c.tab.c"
    break;

  case 90:
#line 192 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_GREATEQ,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2039 "calc_c.tab.c"
    break;

  case 91:
#line 194 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2045 "calc_c.tab.c"
    break;

  case 92:
#line 195 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_EQ,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2051 "calc_c.tab.c"
    break;

  case 93:
#line 196 "calc_c.y"
                                                {(yyval.ptr) = mknode(TOK_NOTEQ,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,(yyvsp[-1].type_id));}
#line 2057 "calc_c.tab.c"
    break;

  case 94:
#line 198 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2063 "calc_c.tab.c"
    break;

  case 95:
#line 199 "calc_c.y"
                                                {(yyval.ptr)=mknode(TOK_AND,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_AND");}
#line 2069 "calc_c.tab.c"
    break;

  case 96:
#line 201 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2075 "calc_c.tab.c"
    break;

  case 97:
#line 202 "calc_c.y"
                                                {(yyval.ptr)=mknode(TOK_OR,(yyvsp[-2].ptr),(yyvsp[0].ptr),NULL,yylineno);strcpy((yyval.ptr)->type_id,"TOK_OR");;}
#line 2081 "calc_c.tab.c"
    break;

  case 98:
#line 204 "calc_c.y"
                                                {(yyval.ptr) = (yyvsp[0].ptr);}
#line 2087 "calc_c.tab.c"
    break;


#line 2091 "calc_c.tab.c"

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
#line 207 "calc_c.y"



#include<stdarg.h>
void yyerror(char* s, ...)
{
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d:error:", yylineno); //错误行号
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}
