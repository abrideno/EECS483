
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 11 "parser.y"


/* Just like lex, the text within this first region delimited by %{ and %}
 * is assumed to be C/C++ code and will be copied verbatim to the y.tab.c
 * file ahead of the definitions of the yyparse() function. Add other header
 * file inclusions or C++ variable declarations/prototypes that are needed
 * by your code here.
 */
#include "scanner.h" // for yylex
#include "parser.h"
#include "errors.h"

void yyerror(const char *msg); // standard error-handling routine



/* Line 189 of yacc.c  */
#line 90 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_Void = 258,
     T_Bool = 259,
     T_Int = 260,
     T_Double = 261,
     T_String = 262,
     T_Class = 263,
     T_LessEqual = 264,
     T_GreaterEqual = 265,
     T_Equal = 266,
     T_NotEqual = 267,
     T_Dims = 268,
     T_And = 269,
     T_Or = 270,
     T_Null = 271,
     T_Extends = 272,
     T_This = 273,
     T_Interface = 274,
     T_Implements = 275,
     T_While = 276,
     T_For = 277,
     T_If = 278,
     T_Else = 279,
     T_Return = 280,
     T_Break = 281,
     T_New = 282,
     T_NewArray = 283,
     T_Print = 284,
     T_ReadInteger = 285,
     T_ReadLine = 286,
     T_Identifier = 287,
     T_StringConstant = 288,
     T_IntConstant = 289,
     T_DoubleConstant = 290,
     T_BoolConstant = 291,
     T_Increm = 292,
     T_Decrem = 293,
     T_Switch = 294,
     T_Case = 295,
     T_Default = 296,
     NoElse = 297
   };
#endif
/* Tokens.  */
#define T_Void 258
#define T_Bool 259
#define T_Int 260
#define T_Double 261
#define T_String 262
#define T_Class 263
#define T_LessEqual 264
#define T_GreaterEqual 265
#define T_Equal 266
#define T_NotEqual 267
#define T_Dims 268
#define T_And 269
#define T_Or 270
#define T_Null 271
#define T_Extends 272
#define T_This 273
#define T_Interface 274
#define T_Implements 275
#define T_While 276
#define T_For 277
#define T_If 278
#define T_Else 279
#define T_Return 280
#define T_Break 281
#define T_New 282
#define T_NewArray 283
#define T_Print 284
#define T_ReadInteger 285
#define T_ReadLine 286
#define T_Identifier 287
#define T_StringConstant 288
#define T_IntConstant 289
#define T_DoubleConstant 290
#define T_BoolConstant 291
#define T_Increm 292
#define T_Decrem 293
#define T_Switch 294
#define T_Case 295
#define T_Default 296
#define NoElse 297




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 41 "parser.y"

    int  integerConstant;
    bool boolConstant;
    char *stringConstant;
    double doubleConstant;
    char identifier[MaxIdentLen+1]; // +1 for terminating null
    Decl *decl;
    List<Decl*> *declList;
    VarDecl *variableDecl; 
    ClassDecl *classDecl; 
    InterfaceDecl *interfaceDecl; 
    FnDecl *fnDecl; 
    List<Identifier*> *identList;
    VarDecl *var; 
    Type *type;
    StmtBlock *stmtBlk; 
    List<VarDecl*> *varList;
    Expr  *expr; 
    List<Expr*> *exprList;
    Stmt    *stmt;
    List<Stmt*> *stmtList;
    IfStmt  *ifStmt; 
    ForStmt *forStmt; 
    WhileStmt *whileStmt;
    BreakStmt *breakStmt;
    ReturnStmt *returnStmt;
    PrintStmt *printStmt;
	LValue *lVal; 
	Call *call; 



/* Line 214 of yacc.c  */
#line 243 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 268 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   450

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  103
/* YYNRULES -- Number of states.  */
#define YYNSTATES  208

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   297

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    57,     2,     2,     2,    54,     2,     2,
      44,    45,    52,    50,    43,    51,    58,    53,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    42,
      55,    49,    56,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    59,     2,    60,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,     2,    47,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    48
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      21,    24,    28,    30,    32,    34,    36,    38,    40,    43,
      50,    57,    64,    71,    74,    75,    77,    78,    80,    82,
      85,    86,    90,    92,    98,   106,   114,   124,   130,   133,
     134,   137,   138,   143,   147,   151,   154,   156,   157,   160,
     162,   164,   166,   168,   170,   172,   174,   176,   179,   180,
     187,   193,   203,   207,   210,   214,   216,   222,   226,   228,
     230,   232,   234,   238,   242,   246,   250,   254,   258,   261,
     265,   269,   273,   277,   281,   285,   289,   293,   296,   300,
     304,   309,   316,   318,   322,   327,   332,   339,   341,   342,
     344,   346,   348,   350
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      62,     0,    -1,    63,    -1,    63,    64,    -1,    64,    -1,
      65,    -1,    69,    -1,    76,    -1,    77,    -1,    66,    42,
      -1,    68,    32,    -1,    67,    43,    66,    -1,    66,    -1,
       5,    -1,     6,    -1,     4,    -1,     7,    -1,    32,    -1,
      68,    13,    -1,    68,    32,    44,    72,    45,    80,    -1,
       3,    32,    44,    72,    45,    80,    -1,    68,    32,    44,
      72,    45,    42,    -1,     3,    32,    44,    72,    45,    42,
      -1,    71,    70,    -1,    -1,    67,    -1,    -1,    65,    -1,
      69,    -1,    74,    73,    -1,    -1,    75,    43,    32,    -1,
      32,    -1,     8,    32,    46,    74,    47,    -1,     8,    32,
      17,    32,    46,    74,    47,    -1,     8,    32,    20,    75,
      46,    74,    47,    -1,     8,    32,    17,    32,    20,    75,
      46,    74,    47,    -1,    19,    32,    46,    71,    47,    -1,
      78,    65,    -1,    -1,    79,    82,    -1,    -1,    46,    78,
      79,    47,    -1,    46,    78,    47,    -1,    46,    79,    47,
      -1,    46,    47,    -1,    91,    -1,    -1,    81,    42,    -1,
      84,    -1,    85,    -1,    86,    -1,    88,    -1,    87,    -1,
      88,    -1,    90,    -1,    80,    -1,    24,    82,    -1,    -1,
      23,    44,    91,    45,    82,    83,    -1,    21,    44,    91,
      45,    82,    -1,    22,    44,    81,    42,    91,    42,    81,
      45,    82,    -1,    25,    81,    42,    -1,    26,    42,    -1,
      89,    43,    91,    -1,    91,    -1,    29,    44,    89,    45,
      42,    -1,    92,    49,    91,    -1,    95,    -1,    92,    -1,
      18,    -1,    93,    -1,    44,    91,    45,    -1,    91,    50,
      91,    -1,    91,    51,    91,    -1,    91,    52,    91,    -1,
      91,    53,    91,    -1,    91,    54,    91,    -1,    51,    91,
      -1,    91,    55,    91,    -1,    91,     9,    91,    -1,    91,
      56,    91,    -1,    91,    10,    91,    -1,    91,    11,    91,
      -1,    91,    12,    91,    -1,    91,    14,    91,    -1,    91,
      15,    91,    -1,    57,    91,    -1,    30,    44,    45,    -1,
      31,    44,    45,    -1,    27,    44,    32,    45,    -1,    28,
      44,    91,    43,    68,    45,    -1,    32,    -1,    91,    58,
      32,    -1,    91,    59,    91,    60,    -1,    32,    44,    94,
      45,    -1,    91,    58,    32,    44,    94,    45,    -1,    89,
      -1,    -1,    34,    -1,    35,    -1,    36,    -1,    33,    -1,
      16,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   148,   148,   160,   161,   164,   165,   166,   167,   170,
     173,   177,   181,   188,   189,   190,   191,   192,   194,   197,
     202,   210,   214,   221,   222,   225,   226,   229,   230,   233,
     237,   240,   245,   252,   257,   264,   273,   286,   293,   294,
     297,   298,   301,   304,   308,   313,   320,   321,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   335,   336,   339,
     345,   350,   355,   360,   365,   369,   375,   380,   384,   385,
     386,   387,   388,   389,   393,   397,   401,   405,   409,   413,
     417,   421,   425,   429,   433,   437,   441,   445,   449,   450,
     451,   456,   459,   463,   467,   470,   474,   481,   482,   485,
     486,   487,   488,   489
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_Void", "T_Bool", "T_Int", "T_Double",
  "T_String", "T_Class", "T_LessEqual", "T_GreaterEqual", "T_Equal",
  "T_NotEqual", "T_Dims", "T_And", "T_Or", "T_Null", "T_Extends", "T_This",
  "T_Interface", "T_Implements", "T_While", "T_For", "T_If", "T_Else",
  "T_Return", "T_Break", "T_New", "T_NewArray", "T_Print", "T_ReadInteger",
  "T_ReadLine", "T_Identifier", "T_StringConstant", "T_IntConstant",
  "T_DoubleConstant", "T_BoolConstant", "T_Increm", "T_Decrem", "T_Switch",
  "T_Case", "T_Default", "';'", "','", "'('", "')'", "'{'", "'}'",
  "NoElse", "'='", "'+'", "'-'", "'*'", "'/'", "'%'", "'<'", "'>'", "'!'",
  "'.'", "'['", "']'", "$accept", "Program", "DeclList", "Decl", "VarDecl",
  "Var", "VarList", "Type", "FnDecl", "Prototype", "PrototypeList",
  "Formals", "Field", "FieldList", "IdentList", "ClassDecl",
  "InterfaceDecl", "VarDeclList", "StmtList", "StmtBlock", "ExprCont",
  "Stmt", "Else", "IfStmt", "WhileStmt", "ForStmt", "ReturnStmt",
  "BreakStmt", "ExprList", "PrintStmt", "Expr", "Lvalue", "Call",
  "Actuals", "Constant", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,    59,    44,    40,    41,   123,   125,   297,    61,
      43,    45,    42,    47,    37,    60,    62,    33,    46,    91,
      93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    63,    63,    64,    64,    64,    64,    65,
      66,    67,    67,    68,    68,    68,    68,    68,    68,    69,
      69,    70,    70,    71,    71,    72,    72,    73,    73,    74,
      74,    75,    75,    76,    76,    76,    76,    77,    78,    78,
      79,    79,    80,    80,    80,    80,    81,    81,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    83,    83,    84,
      85,    86,    87,    88,    89,    89,    90,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    92,    92,    92,    93,    93,    94,    94,    95,
      95,    95,    95,    95
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     2,
       2,     3,     1,     1,     1,     1,     1,     1,     2,     6,
       6,     6,     6,     2,     0,     1,     0,     1,     1,     2,
       0,     3,     1,     5,     7,     7,     9,     5,     2,     0,
       2,     0,     4,     3,     3,     2,     1,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     0,     6,
       5,     9,     3,     2,     3,     1,     5,     3,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     3,     3,
       4,     6,     1,     3,     4,     4,     6,     1,     0,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    15,    13,    14,    16,     0,     0,    17,     0,
       2,     4,     5,     0,     0,     6,     7,     8,     0,     0,
       0,     1,     3,     9,    18,    10,    26,     0,     0,    30,
      24,    26,    12,    25,     0,     0,     0,    32,     0,     0,
       0,     0,     0,    10,     0,     0,    30,     0,    30,    33,
      27,    28,    29,     0,    37,     0,    23,     0,    11,    39,
      20,     0,     0,    31,     0,     0,     0,    19,    45,    41,
      47,    30,    34,    35,    26,    26,    43,    38,    47,   103,
      70,     0,     0,     0,    47,     0,     0,     0,     0,     0,
       0,    92,   102,    99,   100,   101,     0,    44,     0,     0,
      56,     0,    40,    49,    50,    51,    53,    52,    55,    46,
      69,    71,    68,     0,     0,     0,    42,     0,    47,     0,
       0,    63,     0,     0,     0,     0,     0,    98,     0,    78,
      87,    48,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    62,     0,     0,     0,    65,    88,
      89,    97,     0,    72,    80,    82,    83,    84,    85,    86,
      73,    74,    75,    76,    77,    79,    81,    93,     0,    67,
      22,    21,    47,     0,    47,    90,     0,     0,     0,    95,
      98,    94,    60,     0,    58,     0,    64,    66,     0,    47,
      47,    59,    91,    96,     0,    57,    47,    61
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     9,    10,    11,    50,    13,    33,    14,    51,    56,
      40,    35,    52,    39,    38,    16,    17,    69,    70,   100,
     101,   102,   201,   103,   104,   105,   106,   107,   161,   108,
     109,   110,   111,   162,   112
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -84
static const yytype_int16 yypact[] =
{
     417,    -4,   -84,   -84,   -84,   -84,     7,     9,   -84,    45,
     417,   -84,   -84,    12,    -5,   -84,   -84,   -84,    35,    -2,
      41,   -84,   -84,   -84,   -84,    47,    19,    61,    67,   -84,
     -84,    19,   -84,    64,    89,    69,   -17,   -84,   -36,   167,
     233,    72,    19,   -84,    63,    67,   -84,    88,   -84,   -84,
     -84,   -84,   -84,    91,   -84,    90,   -84,    63,   -84,    71,
     -84,    62,   245,   -84,   250,    99,   106,   -84,   -84,   237,
     278,   -84,   -84,   -84,    19,    19,   -84,   -84,   315,   -84,
     -84,   109,   110,   111,   383,    92,   112,   113,   114,   115,
     117,   118,   -84,   -84,   -84,   -84,   383,   -84,   383,   383,
     -84,    93,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   220,
     126,   -84,   -84,   255,   122,   131,   -84,   383,   383,   383,
     135,   -84,   153,   383,   383,   141,   149,   383,    74,   220,
     220,   -84,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   166,   383,   383,   -84,   158,
     159,    86,   160,   137,   -84,   170,   154,   -32,   220,   -84,
     -84,   173,   172,   -84,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   174,    22,   220,
     -84,   -84,   352,   383,   352,   -84,    19,   383,   161,   -84,
     383,   -84,   -84,   169,   202,    -7,   220,   -84,   188,   383,
     352,   -84,   -84,   -84,   200,   -84,   352,   -84
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -84,   -84,   -84,   236,     2,   -22,   -84,   -26,    94,   -84,
     -84,    37,   -84,    44,   218,   -84,   -84,   -84,   178,   -27,
     -83,   244,   -84,   -84,   -84,   -84,   -84,   -84,   140,   -84,
     -77,   -84,   -84,    76,   -84
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      34,   120,    12,    45,    32,    34,    24,    47,    24,    32,
      48,   187,    12,   188,    55,    27,    34,    60,    28,   128,
      58,   129,   130,     2,     3,     4,     5,    25,    18,    46,
      67,   132,   133,   134,   135,   152,   136,   137,   202,    19,
     151,    20,   153,    34,    29,    21,   156,   158,    34,    34,
     158,     8,    32,    32,    23,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,    41,   178,
     179,    77,   138,   139,   140,   141,   142,   143,   144,    26,
     145,   146,   191,   132,   133,   134,   135,    30,   136,   137,
      62,    31,    64,    36,    15,   132,   133,   134,   135,    37,
     136,   137,    24,    24,    15,    47,   193,    42,    71,    59,
     196,   114,   115,   158,    44,   113,   204,    57,    68,   163,
      63,    43,    66,    65,   138,   139,   140,   141,   142,   143,
     144,   182,   145,   146,   121,   131,   138,   139,   140,   141,
     142,   143,   144,    74,   145,   146,   132,   133,   134,   135,
      75,   136,   137,   117,   118,   119,   122,   123,   124,   125,
     195,   126,   127,   132,   133,   134,   135,   149,   136,   137,
       1,     2,     3,     4,     5,   147,   150,   154,   132,   133,
     134,   135,   184,   136,   137,   155,   159,   138,   139,   140,
     141,   142,   143,   144,   160,   145,   146,   186,   177,     8,
     180,   181,   183,   197,   138,   139,   140,   141,   142,   143,
     144,   199,   145,   146,    49,   185,   187,   189,   190,   138,
     139,   140,   141,   142,   143,   144,   200,   145,   146,   132,
     133,   134,   135,   203,   136,   137,    53,     2,     3,     4,
       5,     2,     3,     4,     5,   206,    22,    78,     1,     2,
       3,     4,     5,     1,     2,     3,     4,     5,     1,     2,
       3,     4,     5,    61,   157,     8,   198,     0,     0,     8,
     138,   139,   140,   141,   142,   143,   144,     8,   145,   146,
      54,     0,     8,     0,    76,     0,     0,     8,     0,     0,
       0,     0,    72,     0,    79,     0,    80,    73,     0,    81,
      82,    83,   148,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,     0,     0,     0,     0,     0,
       0,     0,    96,     0,    59,    97,     0,     0,     0,    98,
       0,    79,     0,    80,     0,    99,    81,    82,    83,     0,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,     0,     0,     0,     0,     0,     0,     0,    96,
       0,    59,   116,     0,     0,     0,    98,     0,    79,     0,
      80,     0,    99,    81,    82,    83,     0,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,     0,
       0,     0,     0,     0,     0,     0,    96,     0,    59,    79,
       0,    80,     0,    98,     0,     0,     0,     0,     0,    99,
      86,    87,     0,    89,    90,    91,    92,    93,    94,    95,
       1,     2,     3,     4,     5,     6,   192,    96,   194,     0,
       0,     0,     0,     0,    98,     0,     7,     0,     0,     0,
      99,     0,     0,     0,   205,     0,     0,     0,     0,     8,
     207
};

static const yytype_int16 yycheck[] =
{
      26,    84,     0,    20,    26,    31,    13,    43,    13,    31,
      46,    43,    10,    45,    40,    17,    42,    44,    20,    96,
      42,    98,    99,     4,     5,     6,     7,    32,    32,    46,
      57,     9,    10,    11,    12,   118,    14,    15,    45,    32,
     117,    32,   119,    69,    46,     0,   123,   124,    74,    75,
     127,    32,    74,    75,    42,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,    31,   146,
     147,    69,    50,    51,    52,    53,    54,    55,    56,    44,
      58,    59,    60,     9,    10,    11,    12,    46,    14,    15,
      46,    44,    48,    32,     0,     9,    10,    11,    12,    32,
      14,    15,    13,    13,    10,    43,   183,    43,    46,    46,
     187,    74,    75,   190,    45,    71,   199,    45,    47,    45,
      32,    32,    32,    32,    50,    51,    52,    53,    54,    55,
      56,    45,    58,    59,    42,    42,    50,    51,    52,    53,
      54,    55,    56,    44,    58,    59,     9,    10,    11,    12,
      44,    14,    15,    44,    44,    44,    44,    44,    44,    44,
     186,    44,    44,     9,    10,    11,    12,    45,    14,    15,
       3,     4,     5,     6,     7,    49,    45,    42,     9,    10,
      11,    12,    45,    14,    15,    32,    45,    50,    51,    52,
      53,    54,    55,    56,    45,    58,    59,    43,    32,    32,
      42,    42,    42,    42,    50,    51,    52,    53,    54,    55,
      56,    42,    58,    59,    47,    45,    43,    45,    44,    50,
      51,    52,    53,    54,    55,    56,    24,    58,    59,     9,
      10,    11,    12,    45,    14,    15,     3,     4,     5,     6,
       7,     4,     5,     6,     7,    45,    10,    69,     3,     4,
       5,     6,     7,     3,     4,     5,     6,     7,     3,     4,
       5,     6,     7,    45,   124,    32,   190,    -1,    -1,    32,
      50,    51,    52,    53,    54,    55,    56,    32,    58,    59,
      47,    -1,    32,    -1,    47,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    47,    -1,    16,    -1,    18,    47,    -1,    21,
      22,    23,    47,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    47,    -1,    -1,    -1,    51,
      -1,    16,    -1,    18,    -1,    57,    21,    22,    23,    -1,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    46,    47,    -1,    -1,    -1,    51,    -1,    16,    -1,
      18,    -1,    57,    21,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    16,
      -1,    18,    -1,    51,    -1,    -1,    -1,    -1,    -1,    57,
      27,    28,    -1,    30,    31,    32,    33,    34,    35,    36,
       3,     4,     5,     6,     7,     8,   182,    44,   184,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    19,    -1,    -1,    -1,
      57,    -1,    -1,    -1,   200,    -1,    -1,    -1,    -1,    32,
     206
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    19,    32,    62,
      63,    64,    65,    66,    68,    69,    76,    77,    32,    32,
      32,     0,    64,    42,    13,    32,    44,    17,    20,    46,
      46,    44,    66,    67,    68,    72,    32,    32,    75,    74,
      71,    72,    43,    32,    45,    20,    46,    43,    46,    47,
      65,    69,    73,     3,    47,    68,    70,    45,    66,    46,
      80,    75,    74,    32,    74,    32,    32,    80,    47,    78,
      79,    46,    47,    47,    44,    44,    47,    65,    79,    16,
      18,    21,    22,    23,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    44,    47,    51,    57,
      80,    81,    82,    84,    85,    86,    87,    88,    90,    91,
      92,    93,    95,    74,    72,    72,    47,    44,    44,    44,
      81,    42,    44,    44,    44,    44,    44,    44,    91,    91,
      91,    42,     9,    10,    11,    12,    14,    15,    50,    51,
      52,    53,    54,    55,    56,    58,    59,    49,    47,    45,
      45,    91,    81,    91,    42,    32,    91,    89,    91,    45,
      45,    89,    94,    45,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    32,    91,    91,
      42,    42,    45,    42,    45,    45,    43,    43,    45,    45,
      44,    60,    82,    91,    82,    68,    91,    42,    94,    42,
      24,    83,    45,    45,    81,    82,    45,    82
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 148 "parser.y"
    { 
                                      (yylsp[(1) - (1)]); 
                                      /* pp2: The @1 is needed to convince 
                                       * yacc to set up yylloc. You can remove 
                                       * it once you have other uses of @n*/
                                      Program *program = new Program((yyvsp[(1) - (1)].declList));
                                      // if no errors, advance to next phase
                                      if (ReportError::NumErrors() == 0) 
                                          program->Print(0);
                                    }
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 160 "parser.y"
    { ((yyval.declList)=(yyvsp[(1) - (2)].declList))->Append((yyvsp[(2) - (2)].decl)); }
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 161 "parser.y"
    { ((yyval.declList) = new List<Decl*>)->Append((yyvsp[(1) - (1)].decl)); }
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 164 "parser.y"
    {(yyval.decl)=(yyvsp[(1) - (1)].variableDecl);}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 165 "parser.y"
    {(yyval.decl)=(yyvsp[(1) - (1)].fnDecl);}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 166 "parser.y"
    {(yyval.decl)=(yyvsp[(1) - (1)].classDecl);}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 167 "parser.y"
    {(yyval.decl)=(yyvsp[(1) - (1)].interfaceDecl);}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 170 "parser.y"
    {(yyval.variableDecl)=(yyvsp[(1) - (2)].var);}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 173 "parser.y"
    {Identifier *ident = new Identifier((yylsp[(1) - (2)]),(yyvsp[(2) - (2)].identifier));
									 (yyval.var) = new VarDecl(ident,(yyvsp[(1) - (2)].type));}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 177 "parser.y"
    {
               (yyval.varList) = (yyvsp[(1) - (3)].varList); 
               (yyval.varList)->Append((yyvsp[(3) - (3)].var)); 
          }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 181 "parser.y"
    {
               (yyval.varList) = new List<VarDecl*>; 
               (yyval.varList)->Append((yyvsp[(1) - (1)].var)); 
          }
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 188 "parser.y"
    { (yyval.type) = new Type(*Type::intType); }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 189 "parser.y"
    { (yyval.type) = new Type(*Type::doubleType); }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 190 "parser.y"
    { (yyval.type) = new Type(*Type::boolType); }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 191 "parser.y"
    { (yyval.type) = new Type(*Type::stringType); }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 192 "parser.y"
    { Identifier *id = new Identifier((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].identifier));
                                      (yyval.type) = new NamedType(id); }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 194 "parser.y"
    { (yyval.type) = new ArrayType((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].type)); }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 197 "parser.y"
    {
				Identifier *ident = new Identifier((yylsp[(2) - (6)]),(yyvsp[(2) - (6)].identifier));
				(yyval.fnDecl) = new FnDecl(ident,(yyvsp[(1) - (6)].type),(yyvsp[(4) - (6)].varList)); 
				(yyval.fnDecl)->SetFunctionBody((yyvsp[(6) - (6)].stmtBlk));
	      }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 202 "parser.y"
    {
			   	Identifier *ident = new Identifier((yylsp[(2) - (6)]),(yyvsp[(2) - (6)].identifier)); 
			   	Type       *type  = new Type(*Type::voidType);
			   	(yyval.fnDecl) = new FnDecl(ident,type,(yyvsp[(4) - (6)].varList));
			   	(yyval.fnDecl)->SetFunctionBody((yyvsp[(6) - (6)].stmtBlk));
		  }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 210 "parser.y"
    {
				Identifier *ident = new Identifier((yylsp[(2) - (6)]),(yyvsp[(2) - (6)].identifier));
				(yyval.fnDecl) = new FnDecl(ident,(yyvsp[(1) - (6)].type),(yyvsp[(4) - (6)].varList)); 
		 }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 214 "parser.y"
    {
			   	Identifier *ident = new Identifier((yylsp[(2) - (6)]),(yyvsp[(2) - (6)].identifier)); 
			   	Type       *type  = new Type(*Type::voidType);
			   	(yyval.fnDecl) = new FnDecl(ident,type,(yyvsp[(4) - (6)].varList));
		 }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 221 "parser.y"
    { ((yyval.declList)=(yyvsp[(1) - (2)].declList))->Append((yyvsp[(2) - (2)].fnDecl));}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 222 "parser.y"
    { (yyval.declList) = new List<Decl*>;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 225 "parser.y"
    {(yyval.varList) = (yyvsp[(1) - (1)].varList);}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 226 "parser.y"
    {(yyval.varList) = new List<VarDecl*>;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 229 "parser.y"
    {(yyval.decl)=(yyvsp[(1) - (1)].variableDecl);}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 230 "parser.y"
    {(yyval.decl)=(yyvsp[(1) - (1)].fnDecl);}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 233 "parser.y"
    {
           (yyval.declList) = (yyvsp[(1) - (2)].declList); 
           (yyval.declList)->Append((yyvsp[(2) - (2)].decl)); 
         }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 237 "parser.y"
    {(yyval.declList)=new List<Decl*>;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 240 "parser.y"
    { 
           Identifier *ident = new Identifier((yylsp[(3) - (3)]),(yyvsp[(3) - (3)].identifier)); 
           (yyval.identList) = (yyvsp[(1) - (3)].identList);
           (yyval.identList)->Append(ident); 
         }
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 245 "parser.y"
    {
           Identifier *ident = new Identifier((yylsp[(1) - (1)]),(yyvsp[(1) - (1)].identifier));
           (yyval.identList) = new List<Identifier*>; 
           (yyval.identList)->Append(ident); 
         }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 252 "parser.y"
    {
           Identifier *name = new Identifier((yylsp[(2) - (5)]),(yyvsp[(2) - (5)].identifier)); 
           List<NamedType*> *ntList = new List<NamedType*>;
           (yyval.classDecl) = new ClassDecl(name,NULL,ntList,(yyvsp[(4) - (5)].declList)); 
         }
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 257 "parser.y"
    {
           Identifier *name = new Identifier((yylsp[(2) - (7)]),(yyvsp[(2) - (7)].identifier)); 
           Identifier *ext  = new Identifier((yylsp[(4) - (7)]),(yyvsp[(4) - (7)].identifier));
           NamedType *extends = new NamedType(ext);
           List<NamedType*> *ntList = new List<NamedType*>;
           (yyval.classDecl) = new ClassDecl(name,extends,ntList,(yyvsp[(6) - (7)].declList)); 
         }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 264 "parser.y"
    {
           Identifier *name = new Identifier((yylsp[(2) - (7)]),(yyvsp[(2) - (7)].identifier)); 
           List<NamedType*> *ntList = new List<NamedType*>; 
           for(int i=0; i<(yyvsp[(4) - (7)].identList)->NumElements(); i++){
              NamedType *namedType = new NamedType((yyvsp[(4) - (7)].identList)->Nth(i));
              ntList->Append(namedType); 
           }
           (yyval.classDecl) = new ClassDecl(name,NULL,ntList,(yyvsp[(6) - (7)].declList)); 
         }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 273 "parser.y"
    {
           Identifier *name = new Identifier((yylsp[(2) - (9)]),(yyvsp[(2) - (9)].identifier));
           Identifier *ext  = new Identifier((yylsp[(4) - (9)]),(yyvsp[(4) - (9)].identifier));
           NamedType *extends = new NamedType(ext);
           List<NamedType*> *ntList = new List<NamedType*>; 
           for(int i=0; i<(yyvsp[(6) - (9)].identList)->NumElements(); i++){
              NamedType *namedType = new NamedType((yyvsp[(6) - (9)].identList)->Nth(i));
              ntList->Append(namedType); 
           }
           (yyval.classDecl) = new ClassDecl(name,extends,ntList,(yyvsp[(8) - (9)].declList)); 
         }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 286 "parser.y"
    {
               
               Identifier *ident = new Identifier((yylsp[(2) - (5)]),(yyvsp[(2) - (5)].identifier)); 
               (yyval.interfaceDecl) = new InterfaceDecl(ident,(yyvsp[(4) - (5)].declList)); 
             }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 293 "parser.y"
    { ((yyval.varList)=(yyvsp[(1) - (2)].varList))->Append((yyvsp[(2) - (2)].variableDecl));}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 294 "parser.y"
    { (yyval.varList) = new List<VarDecl*>;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 297 "parser.y"
    {((yyval.stmtList)=(yyvsp[(1) - (2)].stmtList))->Append((yyvsp[(2) - (2)].stmt));}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 298 "parser.y"
    { (yyval.stmtList) = new List<Stmt*>;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 301 "parser.y"
    {
			(yyval.stmtBlk) = new StmtBlock((yyvsp[(2) - (4)].varList),(yyvsp[(3) - (4)].stmtList));	
		 }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 304 "parser.y"
    {
		 	List<Stmt*> *sList = new List<Stmt*>;
		 	(yyval.stmtBlk) = new StmtBlock((yyvsp[(2) - (3)].varList),sList);	
		 }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 308 "parser.y"
    {
		 	List<VarDecl*> *vList = new List<VarDecl*>; 
		 	(yyval.stmtBlk) = new StmtBlock(vList,(yyvsp[(2) - (3)].stmtList)); 
		 
		 }
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 313 "parser.y"
    {
		 	List<VarDecl*> *vList = new List<VarDecl*>; 
		 	List<Stmt*> *sList = new List<Stmt*>;
		 	(yyval.stmtBlk) = new StmtBlock(vList,sList); 
		 }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 320 "parser.y"
    {(yyval.expr)=(yyvsp[(1) - (1)].expr);}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 321 "parser.y"
    {(yyval.expr) = new EmptyExpr; }
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 324 "parser.y"
    {(yyval.stmt)=(yyvsp[(1) - (2)].expr);}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 325 "parser.y"
    {(yyval.stmt)=(yyvsp[(1) - (1)].ifStmt);}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 326 "parser.y"
    {(yyval.stmt)=(yyvsp[(1) - (1)].whileStmt);}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 327 "parser.y"
    {(yyval.stmt)=(yyvsp[(1) - (1)].forStmt);}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 328 "parser.y"
    {(yyval.stmt)=(yyvsp[(1) - (1)].breakStmt);}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 329 "parser.y"
    {(yyval.stmt)=(yyvsp[(1) - (1)].returnStmt);}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 330 "parser.y"
    {(yyval.stmt)=(yyvsp[(1) - (1)].breakStmt);}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 331 "parser.y"
    {(yyval.stmt)=(yyvsp[(1) - (1)].printStmt);}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 332 "parser.y"
    {(yyval.stmt)=(yyvsp[(1) - (1)].stmtBlk);}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 335 "parser.y"
    { (yyval.stmt) = (yyvsp[(2) - (2)].stmt); }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 336 "parser.y"
    { (yyval.stmt) = NULL; }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 339 "parser.y"
    {
           (yyval.ifStmt) = new IfStmt((yyvsp[(3) - (6)].expr),(yyvsp[(5) - (6)].stmt),(yyvsp[(6) - (6)].stmt)); 
         }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 345 "parser.y"
    {
              (yyval.whileStmt)= new WhileStmt((yyvsp[(3) - (5)].expr),(yyvsp[(5) - (5)].stmt)); 
         }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 350 "parser.y"
    {
              (yyval.forStmt) = new ForStmt((yyvsp[(3) - (9)].expr), (yyvsp[(5) - (9)].expr), (yyvsp[(7) - (9)].expr), (yyvsp[(9) - (9)].stmt)); 
        }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 355 "parser.y"
    {
             (yyval.returnStmt) = new ReturnStmt((yylsp[(1) - (3)]),(yyvsp[(2) - (3)].expr)); 
          }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 360 "parser.y"
    {
            (yyval.breakStmt) = new BreakStmt((yylsp[(1) - (2)]));
          }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 365 "parser.y"
    {
            (yyval.exprList)=(yyvsp[(1) - (3)].exprList);
            (yyval.exprList)->Append((yyvsp[(3) - (3)].expr));
          }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 369 "parser.y"
    {
            (yyval.exprList)= new List<Expr*>;
            (yyval.exprList)->Append((yyvsp[(1) - (1)].expr)); 
          }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 375 "parser.y"
    {
            (yyval.printStmt) = new PrintStmt((yyvsp[(3) - (5)].exprList)); 
          }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 380 "parser.y"
    { 
        Operator *eq = new Operator((yylsp[(2) - (3)]),"="); 
        (yyval.expr) = new AssignExpr((yyvsp[(1) - (3)].lVal),eq,(yyvsp[(3) - (3)].expr));
      }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 384 "parser.y"
    { (yyval.expr)=(yyvsp[(1) - (1)].expr); }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 385 "parser.y"
    { (yyval.expr)=(yyvsp[(1) - (1)].lVal); }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 386 "parser.y"
    { (yyval.expr) = new This((yylsp[(1) - (1)])); }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 387 "parser.y"
    { (yyval.expr)=(yyvsp[(1) - (1)].call); }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 388 "parser.y"
    { (yyval.expr)=(yyvsp[(2) - (3)].expr); }
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 389 "parser.y"
    {
        Operator *pl = new Operator((yylsp[(2) - (3)]),"+"); 
        (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr),pl,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 393 "parser.y"
    {
        Operator *sub = new Operator((yylsp[(2) - (3)]),"-"); 
        (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr),sub,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 397 "parser.y"
    {
        Operator *mult = new Operator((yylsp[(2) - (3)]),"*"); 
        (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr),mult,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 401 "parser.y"
    {
        Operator *div = new Operator((yylsp[(2) - (3)]),"/"); 
        (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr),div,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 405 "parser.y"
    {
        Operator *mod = new Operator((yylsp[(2) - (3)]),"%"); 
        (yyval.expr) = new ArithmeticExpr((yyvsp[(1) - (3)].expr),mod,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 409 "parser.y"
    {
        Operator *dec = new Operator((yylsp[(1) - (2)]),"-"); 
        (yyval.expr) = new ArithmeticExpr(dec,(yyvsp[(2) - (2)].expr)); 
      }
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 413 "parser.y"
    {
        Operator *lt = new Operator((yylsp[(2) - (3)]),"<"); 
        (yyval.expr) = new RelationalExpr((yyvsp[(1) - (3)].expr),lt,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 417 "parser.y"
    {
        Operator *lte = new Operator((yylsp[(2) - (3)]),"<="); 
        (yyval.expr) = new RelationalExpr((yyvsp[(1) - (3)].expr),lte,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 421 "parser.y"
    {
        Operator *gt = new Operator((yylsp[(2) - (3)]),">"); 
        (yyval.expr) = new RelationalExpr((yyvsp[(1) - (3)].expr),gt,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 425 "parser.y"
    {
        Operator *gte = new Operator((yylsp[(2) - (3)]),">="); 
        (yyval.expr) = new RelationalExpr((yyvsp[(1) - (3)].expr),gte,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 429 "parser.y"
    {
        Operator *ee = new Operator((yylsp[(2) - (3)]),"=="); 
        (yyval.expr) = new EqualityExpr((yyvsp[(1) - (3)].expr),ee,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 433 "parser.y"
    {
        Operator *ne = new Operator((yylsp[(2) - (3)]),"!="); 
        (yyval.expr) = new EqualityExpr((yyvsp[(1) - (3)].expr),ne,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 437 "parser.y"
    {
        Operator *aa = new Operator((yylsp[(2) - (3)]),"&&"); 
        (yyval.expr) = new LogicalExpr((yyvsp[(1) - (3)].expr),aa,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 441 "parser.y"
    {
        Operator *o = new Operator((yylsp[(2) - (3)]),"||"); 
        (yyval.expr) = new LogicalExpr((yyvsp[(1) - (3)].expr),o,(yyvsp[(3) - (3)].expr)); 
      }
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 445 "parser.y"
    {
        Operator *no = new Operator((yylsp[(1) - (2)]),"!"); 
        (yyval.expr) = new LogicalExpr(no,(yyvsp[(2) - (2)].expr)); 
      }
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 449 "parser.y"
    { (yyval.expr) = new ReadIntegerExpr((yylsp[(1) - (3)])); }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 450 "parser.y"
    { (yyval.expr) = new ReadLineExpr((yylsp[(1) - (3)])); }
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 451 "parser.y"
    { 
        Identifier *n = new Identifier((yylsp[(3) - (4)]),(yyvsp[(3) - (4)].identifier)); 
        NamedType  *nameType = new NamedType(n); 
        (yyval.expr) = new NewExpr ((yylsp[(1) - (4)]),nameType); 
      }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 456 "parser.y"
    {  (yyval.expr) = new NewArrayExpr ((yylsp[(1) - (6)]),(yyvsp[(3) - (6)].expr),(yyvsp[(5) - (6)].type));  }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 459 "parser.y"
    {
        Identifier *ident = new Identifier((yylsp[(1) - (1)]),(yyvsp[(1) - (1)].identifier)); 
        (yyval.lVal) = new FieldAccess(NULL,ident);
      }
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 463 "parser.y"
    { 
        Identifier *ident = new Identifier((yylsp[(3) - (3)]),(yyvsp[(3) - (3)].identifier)); 
        (yyval.lVal) = new FieldAccess((yyvsp[(1) - (3)].expr),ident); 
        }
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 467 "parser.y"
    {(yyval.lVal) = new ArrayAccess((yylsp[(1) - (4)]),(yyvsp[(1) - (4)].expr),(yyvsp[(3) - (4)].expr));}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 470 "parser.y"
    {
        Identifier *ident = new Identifier((yylsp[(1) - (4)]),(yyvsp[(1) - (4)].identifier)); 
        (yyval.call) = new Call((yylsp[(1) - (4)]),NULL,ident,(yyvsp[(3) - (4)].exprList)); 
      }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 474 "parser.y"
    {
        Identifier *ident = new Identifier((yylsp[(3) - (6)]),(yyvsp[(3) - (6)].identifier)); 
        (yyval.call) = new Call((yylsp[(1) - (6)]),(yyvsp[(1) - (6)].expr),ident,(yyvsp[(5) - (6)].exprList)); 
      }
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 481 "parser.y"
    {(yyval.exprList) = (yyvsp[(1) - (1)].exprList);}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 482 "parser.y"
    {(yyval.exprList) = new List <Expr*>; }
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 485 "parser.y"
    { (yyval.expr) = new IntConstant((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].integerConstant)); }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 486 "parser.y"
    { (yyval.expr) = new DoubleConstant((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].doubleConstant)); }
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 487 "parser.y"
    { (yyval.expr) = new BoolConstant((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].boolConstant)); }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 488 "parser.y"
    { (yyval.expr) = new StringConstant((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].stringConstant)); }
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 489 "parser.y"
    { (yyval.expr) = new NullConstant((yylsp[(1) - (1)])); }
    break;



/* Line 1455 of yacc.c  */
#line 2622 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 496 "parser.y"


/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */

/* Function: InitParser
 * --------------------
 * This function will be called before any calls to yyparse().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the parser (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yydebug that controls whether yacc prints debugging information about
 * parser actions (shift/reduce) and contents of state stack during parser.
 * If set to false, no information is printed. Setting it to true will give
 * you a running trail that might be helpful when debugging your parser.
 * Please be sure the variable is set to false when submitting your final
 * version.
 */
void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}

