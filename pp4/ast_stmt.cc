/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"

using namespace std;

extern CodeGenerator CG;

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
    Mips* mips = new Mips;
    vector<Location*> listOfVars;
    int gpOffset = CodeGenerator::OffsetToFirstGlobal;
    for (int i = 0; i < decls->NumElements(); i++)
    {
        listOfVars.push_back(decls->Nth(i)->Emit(gpRelative, gpOffset, listOfVars).front());
        //listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
        gpOffset += CodeGenerator::VarSize;
    }
    CG.DoFinalCodeGen();
}

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

vector<Location*> StmtBlock::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    for (int i = 0; i < decls->NumElements(); i++)
    {
        vector<Location*> newListOfVars = decls->Nth(i)->Emit(seg, offset, varsInScope);
        listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
        offset -= newListOfVars.size() * CodeGenerator::VarSize;
    }
    varsInScope.insert(varsInScope.end(), listOfVars.begin(), listOfVars.end());
    for (int i = 0; i < varsInScope.size(); i++)
    {
        ////cout << varsInScope[i]->GetName() << endl;
    }
    for (int i = 0; i < stmts->NumElements(); i++)
    {
        vector<Location*> newListOfVars = stmts->Nth(i)->Emit(seg, offset, varsInScope);
        listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
        offset -= newListOfVars.size() * CodeGenerator::VarSize;
    }
    return listOfVars;
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

vector<Location*> ForStmt::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    
    listOfVars = init->Emit(seg, offset, varsInScope);
    offset -= listOfVars.size() * CodeGenerator::VarSize;
    varsInScope.insert(varsInScope.end(), listOfVars.begin(), listOfVars.end());
    
    const char* loopLabel = CG.NewLabel();
    CG.GenLabel(loopLabel);
    
    newListOfVars = test->Emit(seg, offset, varsInScope);
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    
    Location* testLoc = newListOfVars.back();
    const char * endLabel = CG.NewLabel();
    CG.GenIfZ(testLoc, endLabel);
    
    newListOfVars = body->Emit(seg, offset, varsInScope);
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    
    newListOfVars = step->Emit(seg, offset, varsInScope);
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    
    CG.GenGoto(loopLabel);
    CG.GenLabel(endLabel);
    
    return listOfVars;
}

vector<Location*> WhileStmt::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    
    const char* loopLabel = CG.NewLabel();
    CG.GenLabel(loopLabel);
    
    newListOfVars = test->Emit(seg, offset, varsInScope);
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    
    Location* testLoc = newListOfVars.back();
    const char * endLabel = CG.NewLabel();
    CG.GenIfZ(testLoc, endLabel);
    
    newListOfVars = body->Emit(seg, offset, varsInScope);
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    
    CG.GenGoto(loopLabel);
    CG.GenLabel(endLabel);
    
    return listOfVars;
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

vector<Location*> IfStmt::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    newListOfVars = test->Emit(seg, offset, varsInScope);
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    Location* testLoc = newListOfVars.back();
    const char * label = CG.NewLabel();
    const char * label2 = CG.NewLabel();
    if (elseBody)
        CG.GenIfZ(testLoc, label);
    else
        CG.GenIfZ(testLoc, label2);
    newListOfVars = body->Emit(seg, offset, varsInScope);
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    CG.GenGoto(label2);
    if (elseBody)
    {
        CG.GenLabel(label);
        newListOfVars = elseBody->Emit(seg, offset, varsInScope);
        listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    }
    CG.GenLabel(label2);
    return listOfVars;
    
    
    
    
}


ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

vector<Location*> ReturnStmt::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    listOfVars = expr->Emit(seg, offset, varsInScope);
    CG.GenReturn(listOfVars.back());
    return listOfVars;
}
  
PrintStmt::PrintStmt(List<Expr*> *a) {    
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

vector<Location*> PrintStmt::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    //cout << "PrintStmt::Emit" << endl;
    vector<Location*> listOfVars;
    for (int i = 0; i < args->NumElements(); i++)
    {
        vector<Location*> newListOfVars = args->Nth(i)->Emit(seg, offset, varsInScope);
        Type* t = newListOfVars.back()->GetType();
        if (t == Type::intType)
            CG.GenBuiltInCall(PrintInt, newListOfVars.back());
        else if (t == Type::boolType)
            CG.GenBuiltInCall(PrintBool, newListOfVars.back());
        else if (t == Type::stringType)
            CG.GenBuiltInCall(PrintString, newListOfVars.back());
        else
            Assert(NULL);
        listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    }
    //cout << "PrintStmt::Emit finished" << endl;
    return listOfVars;
    
}












