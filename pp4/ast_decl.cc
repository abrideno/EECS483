/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "codegen.h"
        
CodeGenerator CG;
         
using namespace std;
         
Decl::Decl(Identifier *n) : Node(*n->GetLocation()) {
    Assert(n != NULL);
    (id=n)->SetParent(this); 
}


VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}

vector<Location*> VarDecl::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    Location* loc = new Location(seg, offset, id->name,getType());
    listOfVars.push_back(loc);
    ////cout << loc->GetName() << ' ' << loc->GetOffset() << endl;
    ////cout << loc << endl;
    return listOfVars;
}
  
//TODO: Vtable so that we can get type from field access 
ClassDecl::ClassDecl(Identifier *n, NamedType *ex, List<NamedType*> *imp, List<Decl*> *m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    Assert(n != NULL && imp != NULL && m != NULL);     
    extends = ex;
    if (extends) extends->SetParent(this);
    (implements=imp)->SetParentAll(this);
    (members=m)->SetParentAll(this);
}


InterfaceDecl::InterfaceDecl(Identifier *n, List<Decl*> *m) : Decl(n) {
    Assert(n != NULL && m != NULL);
    (members=m)->SetParentAll(this);
}

vector<Location*> FnDecl::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    if (seg == gpRelative) //XXX wont be true for class functions
    {
        Location* loc = new Location(seg, offset, id->name, returnType);
        listOfVars.push_back(loc);
        //cout << loc->GetType() << endl;
        return listOfVars;
    }
    //cout << id->name << "!!!!!" << endl;
    CG.GenLabel(id->name);
    BeginFunc* BF = CG.GenBeginFunc();
    int localOffset = CodeGenerator::OffsetToFirstLocal;
    int paramOffset = CodeGenerator::OffsetToFirstParam;
    for (int i = 0; i < formals->NumElements(); i++)
    {
        vector<Location*> newListOfVars = formals->Nth(i)->Emit(fpRelative, paramOffset, varsInScope);
        listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
        paramOffset += newListOfVars.size() * CodeGenerator::VarSize;
    }
    varsInScope.insert(varsInScope.end(), listOfVars.begin(), listOfVars.end());
    vector<Location*> newListOfVars = body->Emit(fpRelative, localOffset, varsInScope);
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    localOffset -= newListOfVars.size() * CodeGenerator::VarSize;
    BF->SetFrameSize(listOfVars.size() * CodeGenerator::VarSize); //SetFrameSize(int numBytesForAllLocalsAndTemps);
    if (returnType == Type::voidType)
        CG.GenReturn();
    CG.GenEndFunc();
    return listOfVars;
}
	
FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
}

void FnDecl::SetFunctionBody(Stmt *b) { 
    (body=b)->SetParent(this);
}



