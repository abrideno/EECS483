/* File: ast_decl.h
 * ----------------
 * In our parse tree, Decl nodes are used to represent and
 * manage declarations. There are 4 subclasses of the base class,
 * specialized for declarations of variables, functions, classes,
 * and interfaces.
 *
 * pp4: You will need to extend the Decl classes to implement 
 * code generation for declarations.
 */

#ifndef _H_ast_decl
#define _H_ast_decl

#include "ast.h"
#include "ast_type.h"
#include "list.h"
#include "tac.h"
#include <vector>
#include "codegen.h"
#include <cstring>
#include <unordered_map>
#include <string>

class Identifier;
class Stmt;
class Type;

using namespace std;

class Decl : public Node 
{
  public:
    Identifier *id;
  
  public:
    virtual vector<Location*> Emit(Segment segment, int offset, vector<Location*> varsInScope) { Assert(0); vector<Location*> empty; return empty;}
    Decl(Identifier *name);
    friend std::ostream& operator<<(std::ostream& out, Decl *d) { return out << d->id; }
};

class VarDecl : public Decl 
{
  protected:
    Type *type;
    
  public:
    vector<Location*> Emit(Segment segment, int offset, vector<Location*> varsInScope);
    VarDecl(Identifier *name, Type *type);
    Type* getType(){return type;}
};

class ClassDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    NamedType *extends;
    List<NamedType*> *implements;

  public:
    vector<Location*> Emit(Segment segment, int offset, vector<Location*> varsInScope);
    ClassDecl(Identifier *name, NamedType *extends, 
              List<NamedType*> *implements, List<Decl*> *members);
};

class InterfaceDecl : public Decl 
{
  protected:
    List<Decl*> *members;
    
  public:
    InterfaceDecl(Identifier *name, List<Decl*> *members);
};

class FnDecl : public Decl 
{
  protected:
    List<VarDecl*> *formals;
    Type *returnType;
    Stmt *body;
    
  public:
    vector<Location*> Emit(Segment segment, int offset, vector<Location*> varsInScope);
    FnDecl(Identifier *name, Type *returnType, List<VarDecl*> *formals);
    void SetFunctionBody(Stmt *b);
};

#endif
