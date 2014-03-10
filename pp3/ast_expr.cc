/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include <string.h>
#include "errors.h"
#include "parser.h"
#include <sstream>

using namespace std;




IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
    value = val;
    
}

Type* IntConstant::CheckResultType()
{
    return Type::intType;
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
}

Type* DoubleConstant::CheckResultType()
{
    return Type::doubleType;
}


BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
}

Type* BoolConstant::CheckResultType()
{
    return Type::boolType;
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
}

Type* StringConstant::CheckResultType()
{
    return Type::stringType;
}

Type* NullConstant::CheckResultType()
{
    return Type::nullType;
}

Type* CompoundExpr::CheckResultType()
{
    if (left)
    {
        if (left->CheckResultType() == Type::errorType || right->CheckResultType() == Type::errorType)
            return Type::errorType;
        else if (left->CheckResultType() != right->CheckResultType())
        {
            ReportError::IncompatibleOperands(op, left->CheckResultType(), right->CheckResultType());
            return Type::errorType;
        }
        else
            return left->CheckResultType();
    }
    else
    {
        return right->CheckResultType();
    }
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
    CheckResultType();
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r) 
  : Expr(Join(o->GetLocation(), r->GetLocation())) {
    Assert(o != NULL && r != NULL);
    left = NULL; 
    (op=o)->SetParent(this);
    (right=r)->SetParent(this);
}
   
  
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

Type* FieldAccess::CheckResultType()
{
    int scopeLevel = 0; //TODO: find scopeLevel
    for (int i = 0; i <= scopeLevel; i++)
    {
        for (auto it = variablesInScope[scopeLevel].begin(); it != variablesInScope[scopeLevel].end(); it++)
        {
            ostringstream oss, oss2;
            oss << *it;
            oss2 << field;
            if (oss.str() == oss2.str())
            {
                return (*it)->GetType();
            }
        }
    }
    return Type::errorType;
}
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
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

       
