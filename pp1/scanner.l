/* File:	scanner.l
 * ----------------
 * Lex inupt file to generate the scanner for the compiler.
 */

%{

/* The text within this first region delimited by %{ and %} is assumed to
 * be C/C++ code and will be copied verbatim to the lex.yy.c file ahead
 * of the definitions of the yylex() function. Add other header file inclusions
 * or C++ variable declarations/prototypes that are needed by your code here.
 */

#include <string.h>
#include "scanner.h"
#include "utility.h" // for PrintDebug()
#include "errors.h"
#include <sstream>
#include <math.h>

using namespace std;

/* Global variable: yylval
 * -----------------------
 * This global variable is how we get attribute information about the token
 * just scanned to the client. The scanner sets the global variable
 * appropriately and since it's global the client can just read it.	In the
 * future, this variable will be declared for us in the y.tab.c file
 * produced by Yacc, but for now, we declare it manually.
 */
YYSTYPE yylval;	// manually declared for pp1, later Yacc provides

/* Global variable: yylloc
 * -----------------------
 * This global variable is how we get position information about the token
 * just scanned to the client. (Operates similarly to yylval above)
 */
struct yyltype yylloc; // manually dclared for pp1, later Yacc provides

/* Macro: YY_USER_ACTION 
 * ---------------------
 * This flex built-in macro can be defined to provide an action which is
 * always executed prior to any matched rule's action. Basically, it is
 * a way of having a piece of code common to all actions factored out to
 * this routine.	We already defined it for you and left the empty
 * function DoBeforeEachAction ready for your use as needed. It will
 * be called once for each pattern scanned from the file, before
 * executing its action.
 */
static void DoBeforeEachAction(); 
#define YY_USER_ACTION DoBeforeEachAction();


int lineNum = 1;
int colNum = 1;
istringstream iss;
ReportError error;


%}

 /* The section before the first %% is the Definitions section of the lex
	* input file. Here is where you set options for the scanner, define lex
	* states, and can set up definitions to give names to regular expressions
	* as a simple substitution mechanism that allows for more readable
	* entries in the Rules section later. 
	*/


VOID					void
INT						int
DOUBLE				double
BOOL					bool
STRING				string
EXTENDS				extends
IMPLEMENTS		implements
FOR						for
WHILE					while
CLASS					class
INTERFACE			interface
IF						if
ELSE					else
RETURN				return
NULL					null
THIS					this
BREAK					break
NEW						New
NEWARRAY			NewArray
IDENTIFIER		[a-zA-Z][a-zA-Z0-9_]*

OPERATOR			\+|-|\*|\/|%|<|>|=|!|;|,|\.|\[|\]|\(|\)|\{|\}
LESSEQUAL			<=
GREATEQUAL		>=
EQUAL					==
NOTEQUAL			!=
AND						&&
OR						\|\|

LITERAL				\"[^"\n]*\"
HANDLE				\"[^"]*\n

INTCONST			[0-9]+
HEX						0[x|X][0-9a-fA-F]+
DOUBCONST			[0-9]+\.[0-9]*
DOUBEXP				[0-9]+\.[0-9]*[E|e][\+|-]?[0-9]+
BOOLCONST			true|false
SPACE					[ ]



%%						 /* BEGIN RULES SECTION */
 /* All patterns and actions should be placed between the start and stop
	* %% markers which delimit the Rules section. 
	*/ 






{VOID}				return T_Void;
{INT}					return T_Int;
{DOUBLE}			return T_Double;
{BOOL}				return T_Bool;
{STRING}			return T_String;
{EXTENDS}			return T_Extends;
{IMPLEMENTS}	return T_Implements;
{FOR}					return T_For;
{WHILE}				return T_While;
{CLASS}				return T_Class;
{INTERFACE}		return T_Interface;
{IF}					return T_If;
{ELSE}				return T_Else;
{RETURN}			return T_Return;
{NULL}				return T_Null;
{THIS}				return T_This;
{BREAK}				return T_Break;
{NEW}					return T_New;
{NEWARRAY}		return T_NewArray;

{HANDLE}			{
								string s = yytext;
								//erase all newlines
								for (auto it = s.begin(); it != s.end(); ++it)
								{
									if (*it == '\n')
									{
										s.erase(it);
										it--;
										lineNum++;
									}
								}
								error.UntermString(&yylloc, s.c_str()); 
								colNum = 1;
							}
{LITERAL}			yylval.stringConstant = yytext; return T_StringConstant;
{BOOLCONST}		yylval.boolConstant = strcmp(yytext, "false"); return T_BoolConstant;

{IDENTIFIER}	{
								//if length > 32, report error
								if (yyleng > 31)
								{
									error.LongIdentifier(&yylloc, yytext);
									yyleng = 31;
								}
								//put yytext into identifier
								int i;
								for (i = 0; i < yyleng; i++)
									yylval.identifier[i] = *(yytext + i);
								yylval.identifier[i] = '\0';
								return T_Identifier;
							}

{OPERATOR}		return *yytext;

{LESSEQUAL}		return T_LessEqual;
{GREATEQUAL}	return T_GreaterEqual;
{EQUAL}				return T_Equal;
{NOTEQUAL}		return T_NotEqual;
{AND}					return T_And;
{OR}					return T_Or;

{DOUBEXP}			yylval.doubleConstant = atof(yytext); return T_DoubleConstant;
{DOUBCONST}		yylval.doubleConstant = atof(yytext); return T_DoubleConstant;

{HEX}					yylval.integerConstant = strtol(yytext, NULL, 16); return T_IntConstant;
{INTCONST}		yylval.integerConstant = atoi(yytext); return T_IntConstant;





\t						colNum += 7; //apparently the solution views \t as 8 columns, yyleng+7=8
\n						lineNum++; colNum = 1;
{SPACE}
.							error.UnrecogChar(&yylloc, *yytext);



%%
/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated lex.yy.c file.
 * This section is where you put definitions of helper functions.
 */


/* Function: InitScanner
 * ---------------------
 * This function will be called before any calls to yylex().	It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the scanner (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yy_flex_debug that controls whether flex prints debugging information
 * about each token and what rule was matched. If set to false, no information
 * is printed. Setting it to true will give you a running trail that might
 * be helpful when debugging your scanner. Please be sure the variable is
 * set to false when submitting your final version.
 */
void InitScanner()
{
	PrintDebug("lex", "Initializing scanner");
	yy_flex_debug = false;
}





/* Function: DoBeforeEachAction()
 * ------------------------------
 * This function is installed as the YY_USER_ACTION. This is a place
 * to group code common to all actions.
 */
static void DoBeforeEachAction()
{
	yylloc.first_column = colNum;
	yylloc.last_column = colNum + yyleng - 1;
	yylloc.first_line = lineNum;
	colNum += yyleng;
}
