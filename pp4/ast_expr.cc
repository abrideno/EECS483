/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>

extern CodeGenerator CG;

using namespace std;

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
}

vector<Location*> IntConstant::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    Location* loc = CG.GenLoadConstant(value, offset);
    listOfVars.push_back(loc);
    ////cout << value << endl;
    return listOfVars;
}

vector<Location*> BoolConstant::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    Location* loc = CG.GenLoadConstant(value, offset);
    listOfVars.push_back(loc);
    return listOfVars;
}


//XXX
vector<Location*> StringConstant::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    Location* loc = CG.GenLoadConstant(value, offset);
    listOfVars.push_back(loc);
    return listOfVars;
    
}


DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
    Assert(tok != NULL);
    strncpy(tokenString, tok, sizeof(tokenString));
}
CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r) 
  : Expr(Join(l->GetLocation(), r->GetLocation())) {
    Assert(l != NULL && o != NULL && r != NULL);
    (op=o)->SetParent(this);
    (left=l)->SetParent(this); 
    (right=r)->SetParent(this);
}



CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}

vector<Location*> CompoundExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    if (left)
    {
        listOfVars = left->Emit(seg, offset, varsInScope);
        offset -= listOfVars.size() * CodeGenerator::VarSize;
    }
    newListOfVars = right->Emit(seg, offset, varsInScope);
    offset -= listOfVars.size() * CodeGenerator::VarSize;
    
    Location* loc;
    if (left)
    {
        loc = CG.GenBinaryOp(op->tokenString, listOfVars.back(), newListOfVars.back(), offset);
        newListOfVars.push_back(loc);
    }
    else
    {
        if (!strcmp("-", op->tokenString))
        {
            offset -= CodeGenerator::VarSize;
            Location* loc2 = CG.GenLoadConstant(-1, offset);
            offset -= CodeGenerator::VarSize;
            loc = CG.GenBinaryOp("*", newListOfVars.back(), loc2, offset);
            newListOfVars.push_back(loc2);
            newListOfVars.push_back(loc);
        }
        else if (!strcmp("!", op->tokenString))
        {
            offset -= CodeGenerator::VarSize;
            Location* loc2 = CG.GenLoadConstant(0, offset);
            offset -= CodeGenerator::VarSize;
            loc = CG.GenBinaryOp("==", newListOfVars.back(), loc2, offset);
            newListOfVars.push_back(loc2);
            newListOfVars.push_back(loc);
        }
        else
            Assert(NULL);
    }
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    return listOfVars;
}

vector<Location*> RelationalExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    listOfVars = left->Emit(seg, offset, varsInScope);
    offset -= listOfVars.size() * CodeGenerator::VarSize;
    newListOfVars = right->Emit(seg, offset, varsInScope);
    offset -= listOfVars.size() * CodeGenerator::VarSize;
    
    Location* loc;
    if (!strcmp("<=", op->tokenString))
    {
        Location* loc2 = CG.GenBinaryOp("<", listOfVars.back(), newListOfVars.back(), offset);
        offset -= CodeGenerator::VarSize;
        Location* loc3 = CG.GenBinaryOp("==", listOfVars.back(), newListOfVars.back(), offset);
        offset -= CodeGenerator::VarSize;
        loc = CG.GenBinaryOp("||", loc2, loc3, offset);
        newListOfVars.push_back(loc2);
        newListOfVars.push_back(loc3);
        newListOfVars.push_back(loc);
    }
    else if (!strcmp(">", op->tokenString))
    {
        Location* loc2 = CG.GenBinaryOp("<", listOfVars.back(), newListOfVars.back(), offset);
        offset -= CodeGenerator::VarSize;
        Location* loc3 = CG.GenBinaryOp("==", listOfVars.back(), newListOfVars.back(), offset);
        offset -= CodeGenerator::VarSize;
        Location* loc4 = CG.GenBinaryOp("||", loc2, loc3, offset);
        offset -= CodeGenerator::VarSize;
        Location* loc5 = CG.GenLoadConstant(0, offset);
        offset -= CodeGenerator::VarSize;
        loc = CG.GenBinaryOp("==", loc4, loc5, offset);
        newListOfVars.push_back(loc2);
        newListOfVars.push_back(loc3);
        newListOfVars.push_back(loc4);
        newListOfVars.push_back(loc5);
        newListOfVars.push_back(loc);
    }
    else if (!strcmp(">=", op->tokenString))
    {
        Location* loc2 = CG.GenBinaryOp("<", listOfVars.back(), newListOfVars.back(), offset);
        offset -= CodeGenerator::VarSize;
        Location* loc3 = CG.GenLoadConstant(0, offset);
        offset -= CodeGenerator::VarSize;
        loc = CG.GenBinaryOp("==", loc2, loc3, offset);
        newListOfVars.push_back(loc2);
        newListOfVars.push_back(loc3);
        newListOfVars.push_back(loc);
    }
    else
    {
        loc = CG.GenBinaryOp(op->tokenString, listOfVars.back(), newListOfVars.back(), offset);
        newListOfVars.push_back(loc);
    }
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    return listOfVars;
}
vector<Location*> AssignExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    ////cout << "assignExpr::Emit" << endl;
    vector<Location*> listOfVars;
    vector<Location*> newListOfVars;
    listOfVars = left->Emit(seg, offset, varsInScope);
    Location* locLeft = listOfVars.back();
    newListOfVars = right->Emit(seg, offset, varsInScope);
    Location* locRight = newListOfVars.back();
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    ////cout << locLeft << " = " << locRight << endl;
    CG.GenAssign(locLeft, locRight);
    ////cout << "assignComplete" << endl;
    return listOfVars;
}


ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
}


//TODO base
vector<Location*> FieldAccess::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    Location* loc;
    //cout << loc << endl;
    //cout << field << endl;
    for (auto it = varsInScope.rbegin(); it != varsInScope.rend(); it++)
    {
        if (!strcmp((*it)->GetName(),field->name))
        {
            loc = *it;
            break;
        }
    }
    //cout << loc << endl;
    listOfVars.push_back(loc);
    return listOfVars;
}

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  {
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
}
 

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) { 
  Assert(c != NULL);
  (cType=c)->SetParent(this);
}


NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this); 
    (elemType=et)->SetParent(this);
}

       
