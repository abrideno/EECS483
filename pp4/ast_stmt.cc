/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "tac.h"
#include "codegen.h"

using namespace std;

Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void Program::Check() {
    Emit();
    /* You can use your pp3 semantic analysis or leave it out if
     * you want to avoid the clutter.  We won't test pp4 against 
     * semantically-invalid programs.
     */
}  

  
/* pp4: here is where the code generation is kicked off.
 *      The general idea is perform a tree traversal of the
 *      entire program, generating instructions as you go.
 *      Each node can have its own way of translating itself,
 *      which makes for a great use of inheritance and
 *      polymorphism in the node classes.
 */
void Program::Emit() 
{
    int gpOffset = CodeGenerator::OffsetToFirstGlobal;
    for (int i = 0; i < decls->NumElements(); i++)
    {
        decls->Nth(i)->Emit(gpRelative, gpOffset);
        gpOffset += CodeGenerator::VarSize;
    }
}

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

int StmtBlock::Emit(Segment seg, int offset)
{
    int size = 0;
    for (int i = 0; i < decls->NumElements(); i++)
    {
        decls->Nth(i)->Emit(seg, offset);
        offset -= CodeGenerator::VarSize;
        size += CodeGenerator::VarSize;
    }
    for (int i = 0; i < stmts->NumElements(); i++)
    {
        int sizeOfStmt = 0;
        sizeOfStmt = stmts->Nth(i)->Emit(seg, offset);
        size += sizeOfStmt;
        offset -= sizeOfStmt;
    }
    return size;
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) { 
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this); 
    (body=b)->SetParent(this);
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) { 
    Assert(i != NULL && t != NULL && s != NULL && b != NULL);
    (init=i)->SetParent(this);
    (step=s)->SetParent(this);
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}


ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}
  
PrintStmt::PrintStmt(List<Expr*> *a) {    
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}


