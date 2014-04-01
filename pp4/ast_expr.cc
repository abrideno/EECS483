/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
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
    loc->setType(Type::intType); 
    listOfVars.push_back(loc);
    ////////cout << value << endl;
    return listOfVars;
}

vector<Location*> BoolConstant::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    Location* loc = CG.GenLoadConstant(value, offset);
    loc->setType(Type::boolType);
    listOfVars.push_back(loc);
    return listOfVars;
}


//XXX
vector<Location*> StringConstant::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    Location* loc = CG.GenLoadConstant(value, offset);
    loc->setType(Type::stringType);
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
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    
    Location* loc;
    if (left)
    {
        loc = CG.GenBinaryOp(op->tokenString, listOfVars.back(), newListOfVars.back(), offset);
        loc->setType(listOfVars.back()->GetType()); 
        newListOfVars.push_back(loc);
    }
    else
    {
        if (!strcmp("-", op->tokenString))
        {
            Location* loc2 = CG.GenLoadConstant(-1, offset);
            loc2->setType(Type::intType); 
            offset -= CodeGenerator::VarSize;
            loc = CG.GenBinaryOp("*", newListOfVars.back(), loc2, offset);
            loc->setType(newListOfVars.back()->GetType()); 
            newListOfVars.push_back(loc2);
            newListOfVars.push_back(loc);
        }
        else if (!strcmp("!", op->tokenString))
        {
            Location* loc2 = CG.GenLoadConstant(0, offset);
            loc2->setType(Type::intType); 
            offset -= CodeGenerator::VarSize;
            loc = CG.GenBinaryOp("==", newListOfVars.back(), loc2, offset);
            loc->setType(Type::boolType);
            newListOfVars.push_back(loc2);
            newListOfVars.push_back(loc);
        }
        else
            Assert(NULL);
    }
    
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    return listOfVars;
}

Type* ArithmeticExpr::getType() {
   if(type)
   	  return type; 
    Assert(right); 
    Type *R = right->getType(); 
    if(left)
    {
    	Type* L = left->getType();
    	type = L; 
    	return type; 
    }
    else
    {
    	type = R; 
    	return type; 
    }
}


vector<Location*> RelationalExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    listOfVars = left->Emit(seg, offset, varsInScope);
    offset -= listOfVars.size() * CodeGenerator::VarSize;
    newListOfVars = right->Emit(seg, offset, varsInScope);
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    

    Location* loc;
    if (!strcmp("<=", op->tokenString))
    {
        Location* loc2 = CG.GenBinaryOp("<", listOfVars.back(), newListOfVars.back(), offset);
        loc2->setType(Type::boolType); 
        offset -= CodeGenerator::VarSize;
        Location* loc3 = CG.GenBinaryOp("==", listOfVars.back(), newListOfVars.back(), offset);
        loc3->setType(Type::boolType); 
        offset -= CodeGenerator::VarSize;
        loc = CG.GenBinaryOp("||", loc2, loc3, offset);
        loc->setType(Type::boolType); 
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
        loc2->setType(Type::boolType); 
        loc3->setType(Type::boolType); 
        loc4->setType(Type::boolType); 
        loc5->setType(Type::boolType); 
        loc->setType(Type::boolType); 
        newListOfVars.push_back(loc2);
        newListOfVars.push_back(loc3);
        newListOfVars.push_back(loc4);
        newListOfVars.push_back(loc5);
        newListOfVars.push_back(loc);
    }
    else if (!strcmp(">=", op->tokenString))
    {
        Location* loc2 = CG.GenBinaryOp("<", listOfVars.back(), newListOfVars.back(), offset);
        loc2->setType(Type::boolType); 
        offset -= CodeGenerator::VarSize;
        Location* loc3 = CG.GenLoadConstant(0, offset);
        loc3->setType(Type::boolType); 
        offset -= CodeGenerator::VarSize;
        loc = CG.GenBinaryOp("==", loc2, loc3, offset);
        loc->setType(Type::boolType); 
        newListOfVars.push_back(loc2);
        newListOfVars.push_back(loc3);
        newListOfVars.push_back(loc);
    }
    else
    {
        loc = CG.GenBinaryOp(op->tokenString, listOfVars.back(), newListOfVars.back(), offset);
        loc->setType(Type::boolType); 
        newListOfVars.push_back(loc);
    }
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    return listOfVars;
}

vector<Location*> EqualityExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    listOfVars = left->Emit(seg, offset, varsInScope);
    offset -= listOfVars.size() * CodeGenerator::VarSize;
    newListOfVars = right->Emit(seg, offset, varsInScope);
    offset -= listOfVars.size() * CodeGenerator::VarSize;
    
    Location* loc;
    if (!strcmp("==", op->tokenString))
    {
        if (listOfVars.back()->GetType() == Type::stringType)
            loc = CG.GenBuiltInCall(StringEqual, listOfVars.back(), newListOfVars.back(), offset);
        else
            loc = CG.GenBinaryOp("==", listOfVars.back(), newListOfVars.back(), offset);
        loc->setType(Type::boolType);
        newListOfVars.push_back(loc);
    }
    else if (!strcmp("!=", op->tokenString))
    {
        Location* loc2;
        if (listOfVars.back()->GetType() == Type::stringType)
            loc2 = CG.GenBuiltInCall(StringEqual, listOfVars.back(), newListOfVars.back(), offset);
        else
            loc2 = CG.GenBinaryOp("==", listOfVars.back(), newListOfVars.back(), offset);
        loc2->setType(Type::boolType);
        newListOfVars.push_back(loc2);
        offset -= CodeGenerator::VarSize;
        Location* loc3 = CG.GenLoadConstant(0, offset);
        offset -= CodeGenerator::VarSize;
        loc = CG.GenBinaryOp("==", loc2, loc3, offset);
        loc->setType(Type::boolType);
        newListOfVars.push_back(loc);
    }
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    return listOfVars;
}

vector<Location*> AssignExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    ////////cout << "assignExpr::Emit" << endl;
    vector<Location*> listOfVars, newListOfVars;
    ostringstream oss;
    newListOfVars = left->Emit(seg, offset, varsInScope);
    Location* locLeft = newListOfVars.back();
    Location* locAddrLeft = NULL;
    Location* locAddrRight = NULL;
    if (newListOfVars.size() > 1)
        locAddrLeft = newListOfVars[newListOfVars.size() - 2];
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    newListOfVars = right->Emit(seg, offset, varsInScope);
    Location* locRight = newListOfVars.back();
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    
    
    if (locAddrLeft && locAddrLeft->GetType())
    {
        oss << locAddrLeft->GetType();
        string s = oss.str();
        if ((s[s.length() - 1] == ']'))
        {
            CG.GenStore(locAddrLeft, locRight);
            return listOfVars;
        }
    }
    
    CG.GenAssign(locLeft, locRight);
    return listOfVars;
}

Type* AssignExpr::getType()
{
	Assert(left && right); 
	Type* R = right->getType(); 
	Type* L = left->getType(); 
	
	type = L; 
	return type; 
}


ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
    (base=b)->SetParent(this); 
    (subscript=s)->SetParent(this);
}

vector<Location*> ArrayAccess::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    ////cout << "access" << endl;
    vector<Location*> listOfVars, newListOfVars;
    listOfVars = base->Emit(seg, offset, varsInScope);
    Location* start = listOfVars.back();
    offset -= listOfVars.size() * CodeGenerator::VarSize;
    
    newListOfVars = subscript->Emit(seg, offset, varsInScope);
    Location* sub = newListOfVars.back();
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());    
    
    Location* four = CG.GenLoadConstant(4, offset);
    listOfVars.push_back(four);
    offset -= CodeGenerator::VarSize;
    
    Location* arrayOffset = CG.GenBinaryOp("*", four, sub, offset);
    listOfVars.push_back(arrayOffset);
    offset -= CodeGenerator::VarSize;
    
    Location* addr = CG.GenBinaryOp("+", start, arrayOffset, offset);
    addr->setType(start->GetType());
    ////cout << "fail" << endl;
    //cout << addr->GetType() << '.' << endl;
    listOfVars.push_back(addr);
    offset -= CodeGenerator::VarSize;
    
    Location* loc = CG.GenLoad(addr, 0, offset);
    listOfVars.push_back(loc);
    
    ostringstream oss;
    oss << start->GetType();
    string s = oss.str();
    s.resize(s.size()-2);
    loc->setType(new Type(s.c_str()));
    //cout << loc->GetType() << endl;
    return listOfVars;
    
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
    //////cout << loc << endl;
    //////cout << field << endl;
    
    for (auto it = varsInScope.rbegin(); it != varsInScope.rend(); it++)
    {
        if (!strcmp((*it)->GetName(),field->name))
        {
            loc = *it;
            break;
        }
    }
    //////cout << loc << endl;
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
 
vector<Location*> Call::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    Location* loc;
    //////cout << loc << endl;
    //////cout << field << endl;
    
    for (auto it = varsInScope.rbegin(); it != varsInScope.rend(); it++)
    {
        if (!strcmp((*it)->GetName(),field->name))
        {
            loc = *it;
            break;
        }
    }
    //////cout << loc << endl;
    for (int i = actuals->NumElements() - 1; i >= 0; i--)
    {
        newListOfVars = actuals->Nth(i)->Emit(seg, offset, varsInScope);
        listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
        offset -= newListOfVars.size() * CodeGenerator::VarSize;
        CG.GenPushParam(newListOfVars.back());
    }
    if (loc->GetType() == Type::voidType)
        loc = CG.GenLCall(field->name, false, offset);
    else
        loc = CG.GenLCall(field->name, true, offset);
    listOfVars.push_back(loc);
    CG.GenPopParams(actuals->NumElements() * CodeGenerator::VarSize);
    return listOfVars;
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

vector<Location*> NewArrayExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    newListOfVars = size->Emit(seg, offset, varsInScope);
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    
    Location* one = CG.GenLoadConstant(1, offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(one);
    
    Location* testSize = CG.GenBinaryOp("<", newListOfVars.back(), one, offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(testSize);
    
    const char * label = CG.NewLabel();
    CG.GenIfZ(testSize, label);
    Location* loc = CG.GenLoadConstant("Decaf runtime error: Array size is <= 0\\n", offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(loc);
    CG.GenBuiltInCall(PrintString, loc);
    CG.GenBuiltInCall(Halt);
    
    CG.GenLabel(label);
    
    Location* four = CG.GenLoadConstant(4, offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(four);
    Location* sizeInBytes = CG.GenBinaryOp("*", four, newListOfVars.back(), offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(sizeInBytes);
    
    Location* ptr = CG.GenBuiltInCall(Alloc, sizeInBytes, NULL, offset);
    //ostringstream oss;
    //oss << elemType;
    //oss << "[]";
    ptr->setType(new Type("Array"));
    listOfVars.push_back(ptr);
    
    return listOfVars;
    
}






       
