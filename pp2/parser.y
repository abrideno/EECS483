/* File: parser.y
 * --------------
 * Yacc input file to generate the parser for the compiler.
 *
 * pp2: your job is to write a parser that will construct the parse tree
 *      and if no parse errors were found, print it.  The parser should 
 *      accept the language as described in specification, and as augmented 
 *      in the pp2 handout.
 */

%{

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

%}

/* The section before the first %% is the Definitions section of the yacc
 * input file. Here is where you declare tokens and types, add precedence
 * and associativity options, and so on.
 */
 
/* yylval 
 * ------
 * Here we define the type of the yylval global variable that is used by
 * the scanner to store attibute information about the token just scanned
 * and thus communicate that information to the parser. 
 *
 * pp2: You will need to add new fields to this union as you add different 
 *      attributes to your non-terminal symbols.
 */
%union {
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
}


/* Tokens
 * ------
 * Here we tell yacc about all the token types that we are using.
 * Yacc will assign unique numbers to these and export the #define
 * in the generated y.tab.h header file.
 */
%token   T_Void T_Bool T_Int T_Double T_String T_Class 
%token   T_LessEqual T_GreaterEqual T_Equal T_NotEqual T_Dims
%token   T_And T_Or T_Null T_Extends T_This T_Interface T_Implements
%token   T_While T_For T_If T_Else T_Return T_Break
%token   T_New T_NewArray T_Print T_ReadInteger T_ReadLine

%token   <identifier> T_Identifier
%token   <stringConstant> T_StringConstant 
%token   <integerConstant> T_IntConstant
%token   <doubleConstant> T_DoubleConstant
%token   <boolConstant> T_BoolConstant

%token   T_Increm T_Decrem T_Switch T_Case T_Default


/* Non-terminal types
 * ------------------
 * In order for yacc to assign/access the correct field of $$, $1, we
 * must to declare which field is appropriate for the non-terminal.
 * As an example, this first type declaration establishes that the DeclList
 * non-terminal uses the field named "declList" in the yylval union. This
 * means that when we are setting $$ for a reduction for DeclList ore reading
 * $n which corresponds to a DeclList nonterminal we are accessing the field
 * of the union named "declList" which is of type List<Decl*>.
 * pp2: You'll need to add many of these of your own.
 */
%type <declList>       DeclList 
%type <decl>           Decl
%type <variableDecl>   VarDecl
%type <varList>        VarDeclList
%type <classDecl>      ClassDecl
%type <interfaceDecl>  InterfaceDecl
%type <fnDecl>         FnDecl
%type <identList>      IdentList
%type <var>      	   Var 
%type <type>      	   Type 
%type <varList>        Formals 
%type <stmtBlk>        StmtBlock
%type <varList>        VarList
%type <decl>           Field
%type <declList>       FieldList 
%type <fnDecl>         Prototype  
%type <declList>       PrototypeList
%type <expr>           Expr 
%type <expr>           ExprCont
%type <stmt>           Stmt
%type <stmtList>       StmtList
%type <ifStmt>         IfStmt
%type <stmt>           Else
%type <forStmt>        ForStmt 
%type <whileStmt>      WhileStmt 
%type <breakStmt>      BreakStmt
%type <returnStmt>     ReturnStmt 
%type <printStmt>      PrintStmt 
%type <lVal>           Lvalue 
%type <exprList>       ExprList 
%type <call>           Call
%type <exprList>       Actuals
%type <expr>           Constant

 

%%
/* Rules
 * -----
 * All productions and actions should be placed between the start and stop
 * %% markers which delimit the Rules section.
	 
 */
Program   :    DeclList            { 
                                      @1; 
                                      /* pp2: The @1 is needed to convince 
                                       * yacc to set up yylloc. You can remove 
                                       * it once you have other uses of @n*/
                                      Program *program = new Program($1);
                                      // if no errors, advance to next phase
                                      if (ReportError::NumErrors() == 0) 
                                          program->Print(0);
                                    }
          ;

DeclList  :    DeclList Decl        { ($$=$1)->Append($2); }
          |    Decl                 { ($$ = new List<Decl*>)->Append($1); }
          ;

Decl      :    VarDecl              {$$=$1;}
          |    FnDecl               {$$=$1;}
          |    ClassDecl            {$$=$1;}
          |    InterfaceDecl        {$$=$1;}
          ;
          
VarDecl   :    Var ';'              {$$=$1;}             
          ; 

Var       :    Type T_Identifier    {Identifier *ident = new Identifier(@1,$2);
									 $$ = new VarDecl(ident,$1);}
          ;

VarList   :    VarList ',' Var {
               $$ = $1; 
               $$->Append($3); 
          }
          |    Var{
               $$ = new List<VarDecl*>; 
               $$->Append($1); 
          }
          ;
            
          
Type      :    T_Int                { $$ = new Type(*Type::intType); }
          |    T_Double             { $$ = new Type(*Type::doubleType); }
          |    T_Bool               { $$ = new Type(*Type::boolType); }
          |    T_String             { $$ = new Type(*Type::stringType); }
          |    T_Identifier         { Identifier *id = new Identifier(@1, $1);
                                      $$ = new NamedType(id); }
          |    Type T_Dims          { $$ = new ArrayType(@1, $1); }
          ;
          
FnDecl    :     Type T_Identifier '(' Formals ')' StmtBlock {
				Identifier *ident = new Identifier(@2,$2);
				$$ = new FnDecl(ident,$1,$4); 
				$$->SetFunctionBody($6);
	      }
		  |	   T_Void T_Identifier '(' Formals ')' StmtBlock{
			   	Identifier *ident = new Identifier(@2,$2); 
			   	Type       *type  = new Type(*Type::voidType);
			   	$$ = new FnDecl(ident,type,$4);
			   	$$->SetFunctionBody($6);
		  }
		  ; 

Prototype:   Type T_Identifier '(' Formals ')' ';' {
				Identifier *ident = new Identifier(@2,$2);
				$$ = new FnDecl(ident,$1,$4); 
		 }
		 |	 T_Void T_Identifier '(' Formals ')' ';'{
			   	Identifier *ident = new Identifier(@2,$2); 
			   	Type       *type  = new Type(*Type::voidType);
			   	$$ = new FnDecl(ident,type,$4);
		 }
		 ;  

PrototypeList:  PrototypeList Prototype { ($$=$1)->Append($2);}
         |                              { $$ = new List<Decl*>;}      
         ; 

Formals  : VarList  {$$ = $1;}
         | {$$ = new List<VarDecl*>;}
         ;

Field    : VarDecl {$$=$1;}
         | FnDecl  {$$=$1;}
         ;

FieldList: FieldList Field {
           $$ = $1; 
           $$->Append($2); 
         }
         | {$$=new List<Decl*>;}
         ;

IdentList: IdentList ',' T_Identifier { 
           Identifier *ident = new Identifier(@3,$3); 
           $$ = $1;
           $$->Append(ident); 
         }
         | T_Identifier {
           Identifier *ident = new Identifier(@1,$1);
           $$ = new List<Identifier*>; 
           $$->Append(ident); 
         }
         ;

ClassDecl: T_Class T_Identifier '{' FieldList '}' {
           Identifier *name = new Identifier(@2,$2); 
           List<NamedType*> *ntList = new List<NamedType*>;
           $$ = new ClassDecl(name,NULL,ntList,$4); 
         }
         | T_Class T_Identifier T_Extends T_Identifier '{' FieldList '}' {
           Identifier *name = new Identifier(@2,$2); 
           Identifier *ext  = new Identifier(@4,$4);
           NamedType *extends = new NamedType(ext);
           List<NamedType*> *ntList = new List<NamedType*>;
           $$ = new ClassDecl(name,extends,ntList,$6); 
         }
         | T_Class T_Identifier T_Implements IdentList '{' FieldList '}' {
           Identifier *name = new Identifier(@2,$2); 
           List<NamedType*> *ntList = new List<NamedType*>; 
           for(int i=0; i<$4->NumElements(); i++){
              NamedType *namedType = new NamedType($4->Nth(i));
              ntList->Append(namedType); 
           }
           $$ = new ClassDecl(name,NULL,ntList,$6); 
         }
         | T_Class T_Identifier T_Extends T_Identifier T_Implements IdentList '{' FieldList '}' {
           Identifier *name = new Identifier(@2,$2);
           Identifier *ext  = new Identifier(@4,$4);
           NamedType *extends = new NamedType(ext);
           List<NamedType*> *ntList = new List<NamedType*>; 
           for(int i=0; i<$6->NumElements(); i++){
              NamedType *namedType = new NamedType($6->Nth(i));
              ntList->Append(namedType); 
           }
           $$ = new ClassDecl(name,extends,ntList,$8); 
         }
         ;
              
InterfaceDecl: T_Interface T_Identifier '{' PrototypeList '}' {
               
               Identifier *ident = new Identifier(@2,$2); 
               $$ = new InterfaceDecl(ident,$4); 
             }
             ;
             
VarDeclList:  VarDeclList VarDecl { ($$=$1)->Append($2);}
           |                      { $$ = new List<VarDecl*>;}
           ; 

StmtList   :  StmtList Stmt {($$=$1)->Append($2);}
           |                { $$ = new List<Stmt*>;}
           ;     
             
StmtBlock: '{' VarDeclList StmtList '}' {
			$$ = new StmtBlock($2,$3);	
		 }
		 | '{' VarDeclList '}' {
		 	List<Stmt*> *sList = new List<Stmt*>;
		 	$$ = new StmtBlock($2,sList);	
		 }
		 | '{' StmtList '}' {
		 	List<VarDecl*> *vList = new List<VarDecl*>; 
		 	$$ = new StmtBlock(vList,$2); 
		 
		 }
		 | '{' '}' {
		 	List<VarDecl*> *vList = new List<VarDecl*>; 
		 	List<Stmt*> *sList = new List<Stmt*>;
		 	$$ = new StmtBlock(vList,sList); 
		 }
		 ;
		 
ExprCont : Expr   {$$=$1;}
         |        {$$ = new EmptyExpr; }
         ;

Stmt     : ExprCont ';' {$$=$1;}
         | IfStmt       {$$=$1;}
         | WhileStmt    {$$=$1;}
         | ForStmt      {$$=$1;}
         | BreakStmt    {$$=$1;}
         | ReturnStmt   {$$=$1;}
         | BreakStmt    {$$=$1;}
         | PrintStmt    {$$=$1;}
         | StmtBlock    {$$=$1;}
         ;
         
Else     :  T_Else Stmt %prec T_Else { $$ = $2; }
         |              %prec NoElse { $$ = NULL; }
         ;

IfStmt   : T_If '(' Expr ')' Stmt Else {
           $$ = new IfStmt($3,$5,$6); 
         }
         ; 


WhileStmt: T_While '(' Expr ')' Stmt {
              $$= new WhileStmt($3,$5); 
         }
         ;

ForStmt : T_For '(' ExprCont ';' Expr ';' ExprCont ')' Stmt {
              $$ = new ForStmt($3, $5, $7, $9); 
        } 
        ; 

ReturnStmt: T_Return ExprCont ';' {
             $$ = new ReturnStmt(@1,$2); 
          }
          ; 
           
BreakStmt : T_Break ';' {
            $$ = new BreakStmt(@1);
          }
          ;

ExprList  : ExprList ',' Expr {
            $$=$1;
            $$->Append($3);
          }
          | Expr {
            $$= new List<Expr*>;
            $$->Append($1); 
          }
          ;

PrintStmt : T_Print '(' ExprList ')' ';' {
            $$ = new PrintStmt($3); 
          }
          ;
          
Expr  : Lvalue '=' Expr { 
        Operator *eq = new Operator(@2,"="); 
        $$ = new AssignExpr($1,eq,$3);
      }
      | Constant { $$=$1; }
      | Lvalue   { $$=$1; }
      | T_This   { $$ = new This(@1); }
      | Call     { $$=$1; }
      | '(' Expr ')' { $$=$2; }
      | Expr '+' Expr {
        Operator *pl = new Operator(@2,"+"); 
        $$ = new ArithmeticExpr($1,pl,$3); 
      }
      | Expr '-' Expr {
        Operator *sub = new Operator(@2,"-"); 
        $$ = new ArithmeticExpr($1,sub,$3); 
      }
      | Expr '*' Expr {
        Operator *mult = new Operator(@2,"*"); 
        $$ = new ArithmeticExpr($1,mult,$3); 
      }
      | Expr '/' Expr {
        Operator *div = new Operator(@2,"/"); 
        $$ = new ArithmeticExpr($1,div,$3); 
      }
      | Expr '%' Expr {
        Operator *mod = new Operator(@2,"%"); 
        $$ = new ArithmeticExpr($1,mod,$3); 
      }
      | '-' Expr {
        Operator *dec = new Operator(@1,"-"); 
        $$ = new ArithmeticExpr(dec,$2); 
      }
      | Expr '<' Expr {
        Operator *lt = new Operator(@2,"<"); 
        $$ = new RelationalExpr($1,lt,$3); 
      }
      | Expr T_LessEqual Expr {
        Operator *lte = new Operator(@2,"<="); 
        $$ = new RelationalExpr($1,lte,$3); 
      }
      | Expr '>' Expr {
        Operator *gt = new Operator(@2,">"); 
        $$ = new RelationalExpr($1,gt,$3); 
      }
      | Expr T_GreaterEqual Expr {
        Operator *gte = new Operator(@2,">="); 
        $$ = new RelationalExpr($1,gte,$3); 
      }
      | Expr T_Equal Expr {
        Operator *ee = new Operator(@2,"=="); 
        $$ = new EqualityExpr($1,ee,$3); 
      }
      | Expr T_NotEqual Expr {
        Operator *ne = new Operator(@2,"!="); 
        $$ = new EqualityExpr($1,ne,$3); 
      }
      | Expr T_And Expr {
        Operator *aa = new Operator(@2,"&&"); 
        $$ = new LogicalExpr($1,aa,$3); 
      } 
      | Expr T_Or Expr {
        Operator *o = new Operator(@2,"||"); 
        $$ = new LogicalExpr($1,o,$3); 
      } 
      | '!' Expr {
        Operator *no = new Operator(@1,"!"); 
        $$ = new LogicalExpr(no,$2); 
      } 
      | T_ReadInteger '(' ')' { $$ = new ReadIntegerExpr(@1); }
      | T_ReadLine    '(' ')' { $$ = new ReadLineExpr(@1); }
      | T_New '(' T_Identifier ')' { 
        Identifier *n = new Identifier(@3,$3); 
        NamedType  *nameType = new NamedType(n); 
        $$ = new NewExpr (@1,nameType); 
      }
      | T_NewArray '(' Expr ',' Type ')' {  $$ = new NewArrayExpr (@1,$3,$5);  }
      ;

Lvalue: T_Identifier {
        Identifier *ident = new Identifier(@1,$1); 
        $$ = new FieldAccess(NULL,ident);
      } 
      | Expr '.' T_Identifier { 
        Identifier *ident = new Identifier(@3,$3); 
        $$ = new FieldAccess($1,ident); 
        } 
      | Expr '[' Expr ']' {$$ = new ArrayAccess(@1,$1,$3);}
      ; 
        
Call  : T_Identifier '(' Actuals ')' {
        Identifier *ident = new Identifier(@1,$1); 
        $$ = new Call(@1,NULL,ident,$3); 
      }
      | Expr '.' T_Identifier '(' Actuals ')' {
        Identifier *ident = new Identifier(@3,$3); 
        $$ = new Call(@1,$1,ident,$5); 
      }
      ;
        

Actuals : ExprList {$$ = $1;}
        |     {$$ = new List <Expr*>; }
        ; 

Constant : T_IntConstant        { $$ = new IntConstant(@1, $1); }
         | T_DoubleConstant     { $$ = new DoubleConstant(@1, $1); }
         | T_BoolConstant       { $$ = new BoolConstant(@1, $1); }
         | T_StringConstant     { $$ = new StringConstant(@1, $1); }
         | T_Null               { $$ = new NullConstant(@1); }
         ;

          



%%

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
