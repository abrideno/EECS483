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
    type = Type::intType;
}

DoubleConstant::DoubleConstant(yyltype loc, double val) : Expr(loc) {
    value = val;
    type = Type::doubleType;
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
    value = val;
    type = Type::boolType;
}

StringConstant::StringConstant(yyltype loc, const char *val) : Expr(loc) {
    Assert(val != NULL);
    value = strdup(val);
    type = Type::stringType;
}

NullConstant::NullConstant(yyltype loc) : Expr(loc) {
    type = Type::nullType;
}

Type* ArithmeticExpr::CheckResultType()
{
    if (type)
        return type;
    Assert(right);
    Type* R = right->CheckResultType();
    if (left) //op == (+ - * / %)
    {
        Type* L = left->CheckResultType();
        if (L == Type::errorType || R == Type::errorType)
        {
            type = Type::errorType;
            return type;
        }            
        else if (L != R)
        {
            ReportError::IncompatibleOperands(op, L, R);
            type = Type::errorType;
            return type;
        }
        else if ((L != Type::intType && L != Type::doubleType) || (R != Type::intType && R != Type::doubleType))
        {
            ReportError::IncompatibleOperands(op, L, R);
            type = Type::errorType;
            return type;
        }
        else
        {
            type = L;
            return type;   
        }
    }
    else //op == '-'
    {
        if (R == Type::errorType)
        {
            type = Type::errorType;
            return type;
        }
        else if (R != Type::intType && R != Type::doubleType)
        {
            ReportError::IncompatibleOperand(op, R);
            type = Type::errorType;
            return type;
        }
        else
        {
            type = R;
            return type;
        }
    }
}

Type* RelationalExpr::CheckResultType() //op == (< > <= >=)
{
    if (type)
        return type;
    Assert(left && right);
    Type* R = right->CheckResultType();
    Type* L = left->CheckResultType();
    if (L == Type::errorType || R == Type::errorType)
    {
        type = Type::errorType;
        return type;
    }            
    else if (L != R)
    {
        ReportError::IncompatibleOperands(op, L, R);
        type = Type::errorType;
        return type;
    }
    else if ((L != Type::intType && L != Type::doubleType) || (R != Type::intType && R != Type::doubleType))
    {
        ReportError::IncompatibleOperands(op, L, R);
        type = Type::errorType;
        return type;
    }
    else
    {
        type = Type::boolType;
        return type;   
    }
}

Type* AssignExpr::CheckResultType() //op == '='
{
    Assert(left && right);
    Type* R = right->CheckResultType();
    Type* L = left->CheckResultType();
    if (L == Type::errorType || R == Type::errorType)
    {
        type = Type::errorType;
        return type;
    }
    else if (L != R)
    {
        ReportError::IncompatibleOperands(op, L, R);
        type = Type::errorType;
        return type;
    }
    else
    {
        type = L;
        return type;
    }
}

//XXX the two operands may also be two objects or an object and null
Type* EqualityExpr::CheckResultType() //op == (== !=)
{
    if (type)
        return type;
    Assert(left && right);
    Type* R = right->CheckResultType();
    Type* L = left->CheckResultType();
    if (L == Type::errorType || R == Type::errorType)
    {
        type = Type::errorType;
        return type;
    }
    else if (L != R)
    {
        ReportError::IncompatibleOperands(op, L, R);
        type = Type::errorType;
        return type;
    }
    else
    {
        type = Type::boolType;
        return type;
    }
}

Type* LogicalExpr::CheckResultType()
{
    if (type)
        return type;
    Assert(right);
    Type* R = right->CheckResultType();
    if (left) //op == (&& ||)
    {
        Type* L = left->CheckResultType();
        if (L == Type::errorType || R == Type::errorType)
        {
            type = Type::errorType;
            return type;
        }            
        else if (L != Type::boolType || R != Type::boolType)
        {
            ReportError::IncompatibleOperands(op, L, R);
            type = Type::errorType;
            return type;
        }
        else
        {
            type = Type::boolType;
            return type;   
        }
    }
    else //op == '!'
    {
        if (R == Type::errorType)
        {
            type = Type::errorType;
            return type;
        }
        else if (R != Type::boolType)
        {
            ReportError::IncompatibleOperand(op, R);
            type = Type::errorType;
            return type;
        }
        else
        {
            type = Type::boolType;
            return type;
        }
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
    ReportError::IdentifierNotDeclared(field, LookingForVariable);
    return Type::errorType;
}
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
}



Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  
{
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
    
    
    int scopeLevel = 0; //TODO: seriously, scope level
    bool found = false;
    int numArgs;
    for (auto it = variablesInScope[scopeLevel].begin(); it != variablesInScope[scopeLevel].end(); it++)
    {
        ostringstream oss, oss2;
        oss << *it;
        oss2 << f;
        if (oss.str() == oss2.str())
        {
            found = true;
            numArgs = (*it)->numArgs();
            break;
        }
    }
    if (!found)
    {
        ReportError::IdentifierNotDeclared(f, LookingForFunction);
        return;
    }
    if (a->NumElements() != numArgs)
    {
        ReportError::NumArgsMismatch(f, numArgs, a->NumElements());
        return;
    }
    
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

       
