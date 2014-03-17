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


void Expr::addLevel(Slevel *parent)
{
    scope = new Slevel;
    scope->Parent = parent;
}

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
	left->Check(); 
	right->Check();
    Type* R = right->CheckResultType();
    Type* L = left->CheckResultType();
    if (L == Type::errorType || R == Type::errorType)
    {
        type = Type::errorType;
        return type;
    }
    ostringstream oss, oss2;
    oss << L;
    oss2 << R;
    if (oss.str() != oss2.str())
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

Type* This::CheckResultType()
{
    if (type)
        return type;
    ClassDecl *cDecl = NULL; 
    Slevel *check = scope;
    while(check){
    	if(check->cDecl){
    		cDecl = check->cDecl;
    		break;
    	}
    	check = check->Parent;
    }
    if (cDecl)
    {
        type = cDecl->CheckResultType();
        return type;
    }
    else
    {
        ReportError::ThisOutsideClassScope(this);
        type = Type::errorType;
        return type;
    }
}

void This::Check()
{
    CheckResultType();
}
   
void This::addLevel(Slevel* parent)
{
    scope = new Slevel;
    scope->Parent = parent;
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

    /*
    Type *t = base->CheckResultType();
    ostringstream oss;
    oss << t;
    oss.str()*/
	if(dynamic_cast<ArrayType*>(base->CheckResultType()) == NULL){
		ReportError::BracketsOnNonArray(base);
	}

	if(subscript->CheckResultType() != Type::intType){
		ReportError::SubscriptNotInteger(subscript); 
	}
}

Type* FieldAccess::CheckResultType()
{  
     ////cout << "Field Access: " << endl;
     ////cout << field << endl;
     if (type)
        return type;
     Slevel* tempScope;
     tempScope = scope;
     Decl* temp;
     
     if (base == NULL)
     {
        int count = 0;
        while (tempScope != NULL)
        {
            count++;
            temp = tempScope->stable->Lookup(field->name);
            if (temp == NULL)
                tempScope = tempScope->Parent;
            else
                break;
        }
	    
        if (!temp)
        {
        	ClassDecl *cDec; 
            Slevel *check = scope;
        
       	  while(check != NULL){
    	   	if(check->Parent->cDecl != NULL){
    	     	cDec = check->Parent->cDecl;
    	    	break;
        	}
        	check = check->Parent;
           }
        
   	    	if(cDec != NULL){
   	    	   ////cout<<"GETTING HERE"<<endl;
               Type *t= cDec->checkExtends(cDec->extends,field->name); 
			   if(t == Type::errorType){
			      //cout << "HI THERE" << endl;
			      ReportError::IdentifierNotDeclared(field,LookingForVariable);
			      type = Type::errorType; 
			      return type;
			    }
			    else{
			    	return t;
			    } 
             }
        }
        type = temp->CheckResultType();
        return type;
        
     }
     else
     {
        Decl* cDecl;
        ostringstream oss;
        
        FieldAccess* fAcc = dynamic_cast<FieldAccess*>(base);
        Type *t = base->CheckResultType();
        int count = 0;
        while (tempScope != NULL)
        {
            count++;
            cDecl = tempScope->stable->Lookup(fAcc->field->name);
            if (cDecl == NULL)
                tempScope = tempScope->Parent;
            else
                break;
        }

        if (cDecl == NULL)
        {
            //cout << "HI THERE" << endl;
            ReportError::IdentifierNotDeclared(fAcc->field, LookingForVariable);
            type = Type::errorType;
            return type;
        }
        Slevel *topScope = Program::parentScope; 
        Decl* cDecl2 = topScope->stable->Lookup(cDecl->CheckResultType()->fetchKey());
        if (!cDecl2)
        {
            ReportError::FieldNotFoundInBase(field, t);
            type = Type::errorType;
            return type;
        }
        ClassDecl* cDecl3 = dynamic_cast<ClassDecl*>(cDecl2);
        if (!cDecl3)
        {
            //cout << "HI THERE" << endl;
            ReportError::IdentifierNotDeclared(cDecl->id, LookingForType);
            type = Type::errorType;
            return type;
        }
        else
        {
            bool found = false; 
            int numMem = cDecl3->members->NumElements();
            Decl* cDecl4;
            for(int i=0; i<numMem; i++){
                string s, s2;
                s = cDecl3->members->Nth(i)->id->name;
                s2 = field->name;
                if(s == s2){
                    type = cDecl3->members->Nth(i)->CheckResultType(); 
                    cDecl4 = cDecl3->members->Nth(i);
                    found = true; 
                    break; 
                }
            } 
            if(!found){
                ReportError::FieldNotFoundInBase(field, t);
                type = Type::errorType;
                return type;
            } 
            VarDecl* vd = dynamic_cast<VarDecl*>(cDecl4);
            if (vd)
            {
                ReportError::InaccessibleField(field, base->CheckResultType());
                type = Type::errorType;
                return type;
            }     
            else       
            {
                return type; 
            }
        }
    }
}
 
void FieldAccess::addLevel(Slevel *parent){
    scope = new Slevel;
    scope->Parent = parent; 
    Slevel* tempScope = scope;
    Decl* temp;
    ////cout << "field access add level: " << field << endl;
    if (base)
        base->addLevel(scope);
    
} 

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

void Call::addLevel(Slevel* parent)
{
    scope = new Slevel;
    scope->Parent = parent;
    if (base)
        base->addLevel(scope);
    for (int i = 0; i < actuals->NumElements(); i++)
        actuals->Nth(i)->addLevel(scope);
}

void Call::Check()
{
    CheckResultType();
}

Type* Call::CheckResultType()
{
     if (type)
        return type;
     Slevel* tempScope;
     tempScope = scope;
     Decl* temp;
     if (base == NULL)
     {
        while (tempScope != NULL)
        {
            temp = tempScope->stable->Lookup(field->name);
            if (temp == NULL)
                tempScope = tempScope->Parent;
            else
                break;
        }
        if (!temp)
        {
            //cout << "HI THERE" << endl;
            ReportError::IdentifierNotDeclared(field, LookingForFunction);
            type = Type::errorType;
            return type;
        }
        FnDecl* fd = dynamic_cast<FnDecl*>(temp);
        if (fd)
        {
            List<VarDecl*>* args = fd->formals;
            if (args->NumElements() != actuals->NumElements())
            {
                ReportError::NumArgsMismatch(field, args->NumElements(), actuals->NumElements());
                type = Type::errorType;
                return type;
            }
            for (int i = 0; i < args->NumElements(); i++)
            {
                ostringstream oss, oss2;
                oss << args->Nth(i)->CheckResultType();
                oss2 << actuals->Nth(i)->CheckResultType();
                if (oss.str() != oss2.str())
                {
                    if (args->Nth(i)->CheckResultType() != Type::errorType && actuals->Nth(i)->CheckResultType() != Type::errorType)
                        ReportError::ArgMismatch(actuals->Nth(i), i+1, actuals->Nth(i)->CheckResultType(), args->Nth(i)->CheckResultType());
                    type = Type::errorType;
                    return type;
                }
            }
            type = fd->returnType;
            return type;
        }     
        else       
        {
            return Type::errorType; 
        }        
     }
     else
     {
        Decl* cDecl;
        ostringstream oss;
        
        FieldAccess* fAcc = dynamic_cast<FieldAccess*>(base);
        Type *t = base->CheckResultType();
        while (tempScope != NULL)
        {
            cDecl = tempScope->stable->Lookup(fAcc->field->name);
            if (cDecl == NULL)
                tempScope = tempScope->Parent;
            else
                break;
        }
        if (cDecl == NULL)
        {
            //cout << "HI THERE" << endl;
            ReportError::IdentifierNotDeclared(fAcc->field, LookingForVariable);
            type = Type::errorType;
            return type;
        }
        Slevel *topScope = Program::parentScope; 
        //cout << cDecl << endl;
        Decl* cDecl2 = topScope->stable->Lookup(cDecl->CheckResultType()->fetchKey());
        ClassDecl* cDecl3 = dynamic_cast<ClassDecl*>(cDecl2);
        if (!cDecl3)
        {
            if (cDecl->CheckResultType()->isBasicType())
            {
                type = cDecl->CheckResultType();
                return type;
            }
            //cout << "HI THERE" << endl;
            ReportError::IdentifierNotDeclared(cDecl->id, LookingForType);
            type = Type::errorType;
            return type;
        }
        else
        {
            int Nth;
            bool found = false; 
            int numMem = cDecl3->members->NumElements();
            Decl* cDecl4;
            for(int i=0; i<numMem; i++){
                string s, s2;
                s = cDecl3->members->Nth(i)->id->name;
                s2 = field->name;
                if(s == s2){
                    type = cDecl3->members->Nth(i)->CheckResultType(); 
                    cDecl4 = cDecl3->members->Nth(i);
                    Nth = i;
                    found = true; 
                    break; 
                }
            } 
            if(!found){
                ReportError::FieldNotFoundInBase(field, t);
                type = Type::errorType;
                return type;
            } 
            FnDecl* fd = dynamic_cast<FnDecl*>(cDecl4);
            if (fd)
            {
                List<VarDecl*>* args = fd->formals;
                if (args->NumElements() != actuals->NumElements())
                {
                    ReportError::NumArgsMismatch(field, args->NumElements(), actuals->NumElements());
                    type = Type::errorType;
                    return type;
                }
                for (int i = 0; i < args->NumElements(); i++)
                {
                    if (args->Nth(i)->CheckResultType() != actuals->Nth(i)->CheckResultType())
                    {
                        ostringstream oss, oss2;
                        oss << args->Nth(i)->CheckResultType();
                        oss2 << actuals->Nth(i)->CheckResultType();
                        if (oss.str() != "error" && oss2.str() != "error")
                            ReportError::ArgMismatch(actuals->Nth(i), i+1, actuals->Nth(i)->CheckResultType(), args->Nth(i)->CheckResultType());
                        type = Type::errorType;
                        return type;
                    }
                }
                type = fd->returnType;
                return type;
            }     
            else       
            {
                return type; 
            }
        }
    }
}



Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr*> *a) : Expr(loc)  
{
    Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
    base = b;
    if (base) base->SetParent(this);
    (field=f)->SetParent(this);
    (actuals=a)->SetParentAll(this);
    
    
     bool found = false;
     int numArgs;
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
