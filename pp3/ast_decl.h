/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 *
 * pp3: You will need to extend the Decl classes to implement 
 * semantic processing including detection of declaration conflicts 
 * and managing scoping issues.
 */

#ifndef _H_ast_decl
#define _H_ast_decl

#include "ast.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "list.h"

using namespace std;

class Identifier;
class Type;
class Stmt;
class InterfaceDecl; 

class Decl : public Node 
{
  public:
    Identifier *id;
    Type* type;
    Slevel *scope; 

  public:
    virtual Type* CheckResultType() = 0;
    Decl(Identifier *name);
    

    virtual int numArgs() { return 0; }
    virtual Type* argType(int n) { return type; }
    friend std::ostream& operator<<(std::ostream& out, Decl *d) { return out << d->id; }
    virtual bool match(Decl* compare); 
    virtual void addLevel(Slevel *parent);
    virtual void Check(); 
};

class VarDecl : public Decl 
{
  protected:
    Type *type; 

  public:
    Type* CheckResultType() { return type; }
    VarDecl(Identifier *name, Type *type);
    void Check(); 
};

class ClassDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    NamedType *extends;
    List<NamedType*> *implements;
    
  public:
    Type* CheckResultType() { return type; }
    ClassDecl(Identifier *name, NamedType *extends, 
              List<NamedType*> *implements, List<Decl*> *members);
    void addLevel(Slevel *parent); 
    void Check(); 
    
    //extends implements TODO 
};

class InterfaceDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    
  public:
   // Type* CheckResultType() { return type; }
    InterfaceDecl(Identifier *name, List<Decl*> *members);
    
    void addLevel(Slevel *parent); 
    void Check(); 
};

class FnDecl : public Decl 
{
  protected:
    List<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;
    
  public:
    Type* CheckResultType() { return returnType; }
    int numArgs() { return formals->NumElements(); }
    Type* argType(int n);
    FnDecl(Identifier *name, Type *returnType, List<VarDecl*> *formals);
    void SetFunctionBody(Stmt *b);
    bool match(Decl *compare); 
    void addLevel(Slevel *parent); 
    void Check();
};


// extern unordered_map<int, vector<Decl*> >variablesInScope; 
// extern unordered_map<int, vector<Decl*> >functionsInScope;

#endif
