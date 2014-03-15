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

void ArithmeticExpr::Check(){
	Type *throwaway = CheckResultType(); 
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

void RelationalExpr::Check(){
	Type *throwaway = CheckResultType(); 
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
    
    type = Type::boolType;
    return type;   
    
}

void AssignExpr::Check(){
	Type *throwaway = CheckResultType(); 
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
    if (L != R)
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

void EqualityExpr::Check(){
	Type *throwaway = CheckResultType(); 
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

void LogicalExpr::Check(){
	Type *throwaway = CheckResultType(); 
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

void CompoundExpr::addLevel(Slevel *parent){
	scope->Parent = parent; 
	
	if(left!=NULL){
		left->addLevel(scope); 
	}
	right->addLevel(scope); 
}

void CompoundExpr::Check(){
	if(left!=NULL){
		left->Check(); 
	}
	right->Check(); 
}
   
  
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

void ArrayAccess::addLevel(Slevel *parent){
	scope->Parent = parent; 
	
	subscript->addLevel(scope); 
	base->addLevel(scope); 

}

Type* ArrayAccess::CheckResultType(){
	ArrayType *temp = dynamic_cast<ArrayType*>(base->CheckResultType()); // is calling this function multiple times going to have issues with repeated Error handling
	if(temp == NULL){
		return Type::errorType; 
	}
   
	return temp->getArrayType(); //considering making public to prevent function use 
}

void ArrayAccess::Check(){
	base->Check(); 
	subscript->Check(); 

	if(dynamic_cast<ArrayType*>(base->CheckResultType()) == NULL){
		ReportError::BracketsOnNonArray(base);
	}
	Debug("yup");
	if(subscript->CheckResultType() != Type::intType){
		ReportError::SubscriptNotInteger(subscript); 
	}
}

Type* FieldAccess::CheckResultType()
{

   /*  //int scopeLevel = 0; //TODO: find scopeLevel
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
     }*/
     if (type)
        return type;
     Slevel* tempScope;
     if (base == NULL)
     {
        tempScope = scope;
        Decl* temp;
        while (tempScope != NULL)
        {
            temp = tempScope->stable->Lookup(field->name);
            if (!temp)
                tempScope = tempScope->Parent;
            else
                break;
        }
        if (!temp)
        {
            ReportError::IdentifierNotDeclared(field, LookingForVariable);
            type = Type::errorType;
            return type;
        }
        
        type = temp->CheckResultType();
        return type;
        
     }
 }
 
//void FieldAccess::addLevel(Slevel *parent){
   // scope->Parent = parent; 
    
// } 

void FieldAccess::Check(){
    Type *throwaway = CheckResultType(); 
}

     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
   
    //CheckResultType();
    
}



Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  
{
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
    
    
//  int scopeLevel = 0; //TODO: seriously, scope level
//     bool found = false;
//     int numArgs;
//     auto it = variablesInScope[scopeLevel].begin();
//     while (it != variablesInScope[scopeLevel].end())
//     {
//         ostringstream oss, oss2;
//         oss << *it;
//         oss2 << f;
//         if (oss.str() == oss2.str())
//         {
//             found = true;
//             numArgs = (*it)->numArgs();
//             break;
//         }
//         it++;
//     }
//     if (!found)
//     {
//         ReportError::IdentifierNotDeclared(f, LookingForFunction);
//         return;
//     }
//     if (a->NumElements() != numArgs)
//     {
//         ReportError::NumArgsMismatch(f, numArgs, a->NumElements());
//         return;
//     }
//     for (int i = 0; i < a->NumElements(); i++)
//     {
//         Type* type1 = a->Nth(i)->CheckResultType();
//         Type* type2 = (*it)->argType(i);
//         if (type1 != type2)
//         {
//             ReportError::ArgMismatch(a->Nth(i), i+1, type1, type2);
//         }
//     }
   
    
}



NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) { 
  Assert(c != NULL);
  (cType=c)->SetParent(this);
  type = cType;
}

void NewExpr::Check(){
	Decl *temp = Program::parentScope->stable->Lookup(cType->fetchKey()); 
	ClassDecl *cDec = dynamic_cast<ClassDecl*>(temp); 
	
	if(cDec == NULL){
		ReportError::IdentifierNotDeclared(cType->id, LookingForClass); 
	}
}



NewArrayExpr::NewArrayExpr(yyltype loc, Expr *sz, Type *et) : Expr(loc) {
    Assert(sz != NULL && et != NULL);
    (size=sz)->SetParent(this); 
    (elemType=et)->SetParent(this);
    
    Type* t = sz->CheckResultType();
    if (t != Type::intType)
    {
        ReportError::NewArraySizeNotInteger(sz);
        type = Type::errorType;
        return;
    }
    ostringstream oss;
    oss << et;
    oss << "[]";
    type = new Type(oss.str().c_str());
}

void NewArrayExpr::addLevel(Slevel *parent){
	scope->Parent = parent; 
	
	size->addLevel(scope); 
}

void NewArrayExpr::Check(){
	size->Check();  
	
	if(size->CheckResultType() != Type::intType){
		ReportError::NewArraySizeNotInteger(size); 
	}
	
	if(elemType->isBasicType()){
		return; 
	}
	
	Decl *temp = Program::parentScope->stable->Lookup(elemType->fetchKey());
	ClassDecl *cDec = dynamic_cast<ClassDecl*>(temp); 
	if(cDec == NULL){
		//ReportError::IdentifierNotDeclared(elemType->fetchKey(),LookingForType);
	} 
}   
