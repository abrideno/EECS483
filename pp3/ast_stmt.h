/* File: ast_stmt.h
 * ----------------
 * The Stmt class and its subclasses are used to represent
 * statements in the parse tree.  For each statment in the
 * language (for, if, return, etc.) there is a corresponding
 * node class for that construct. 
 *
 * pp3: You will need to extend the Stmt classes to implement
 * semantic analysis for rules pertaining to statements.
 */


#ifndef _H_ast_stmt
#define _H_ast_stmt

#include "list.h"
#include "ast.h"
#include "hashtable.h"

class Decl;
class VarDecl;
class Expr;
class ClassDecl; 
class LoopStmt; 
class FnDecl; 

class Slevel
{
	public: 
	Hashtable <Decl*> stable; 
	Slevel *Parent;	
	ClassDecl *cDecl; 
	LoopStmt *lStmt;
	FnDecl* fDecl;  
 		 
 		
 	public: 
 		Slevel() : stable(new Hashtable <Decl*>) , Parent(NULL), cDecl(NULL), lStmt(NULL), fDecl(NULL) {}

 	    void add(Decl *decl); 
 	    
 	    FnDecl* getfDecl() { return fDecl; } 
 	    
 	    LoopStmt* getlStmt(){ return lStmt; }
 	
};
  
class Program : public Node
{
  public:
  	 static Slevel *parentScope; 
  	 
  protected:
     List<Decl*> *decls;
     
  public:
     Program(List<Decl*> *declList);
     void Check();
     void addLevel(); 
};

class Stmt : public Node
{
  public: 
  	Slevel *scope; 
  	
  public:
     Stmt() : Node(), scope(new Slevel) {}
     Stmt(yyltype loc) : Node(loc), scope(new Slevel) {}
     
     virtual void addLevel(Slevel *parent); 
     virtual void Check(); 
     
};

class StmtBlock : public Stmt 
{
  protected:
    List<VarDecl*> *decls;
    List<Stmt*> *stmts;
    
  public:
    StmtBlock(List<VarDecl*> *variableDeclarations, List<Stmt*> *statements);
    void addLevel(Slevel *parent); 
    void Check(); 
};

  
class ConditionalStmt : public Stmt
{
  protected:
    Expr *test;
    Stmt *body;
  
  public:
    ConditionalStmt(Expr *testExpr, Stmt *body);
    virtual void addLevel(Slevel *parent); 
    virtual void Check(); 
};

class LoopStmt : public ConditionalStmt 
{
  public:
    LoopStmt(Expr *testExpr, Stmt *body)
            : ConditionalStmt(testExpr, body) {}
    void addLevel(Slevel *parent); 
};

class ForStmt : public LoopStmt 
{
  protected:
    Expr *init, *step;
  
  public:
    ForStmt(Expr *init, Expr *test, Expr *step, Stmt *body);
};

class WhileStmt : public LoopStmt 
{
  public:
    WhileStmt(Expr *test, Stmt *body) : LoopStmt(test, body) {}
};

class IfStmt : public ConditionalStmt 
{
  protected:
    Stmt *elseBody;
  
  public:
    IfStmt(Expr *test, Stmt *thenBody, Stmt *elseBody);
    void addLevel(Slevel *parent); 
    void Check(); 
};

class BreakStmt : public Stmt 
{
  public:
    BreakStmt(yyltype loc) : Stmt(loc) {}
    void Check(); 
};

class ReturnStmt : public Stmt  
{
  protected:
    Expr *expr;
  
  public:
    ReturnStmt(yyltype loc, Expr *expr);
    
    void addLevel(Slevel *parent); 
    void Check(); 
    
};

class PrintStmt : public Stmt
{
  protected:
    List<Expr*> *args;
    
  public:
    PrintStmt(List<Expr*> *arguments);
    
    void addLevel(Slevel *parent); 
    void Check(); 
};


#endif
