/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_decl.h"
#include <string.h>

extern CodeGenerator CG;
extern unordered_map<string, vector<classVarMember> > classVars;

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
        ostringstream oss;
        oss << listOfVars.back()->GetType();
        string s = oss.str();
        if (s == "string")
            loc = CG.GenBuiltInCall(StringEqual, listOfVars.back(), newListOfVars.back(), offset);
        else
            loc = CG.GenBinaryOp("==", listOfVars.back(), newListOfVars.back(), offset);
        loc->setType(Type::boolType);
        newListOfVars.push_back(loc);
    }
    else if (!strcmp("!=", op->tokenString))
    {
        Location* loc2;
        ostringstream oss;
        oss << listOfVars.back()->GetType();
        string s = oss.str();
        if (s == "string")
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
    vector<Location*> listOfVars, newListOfVars;
    ostringstream oss, oss2;
    newListOfVars = left->Emit(seg, offset, varsInScope);
    Location* locLeft = newListOfVars.back();
    Location* locAddrLeft = NULL;
    Location* locAddrRight = NULL;
    if (newListOfVars.size() > 1)
        locAddrLeft = newListOfVars[newListOfVars.size() - 2];
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    //cout << "calling right assign emit" << endl;
    newListOfVars = right->Emit(seg, offset, varsInScope);
    //cout << "ending right assign emit" << endl;
    Location* locRight = newListOfVars.back();
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    
    
    if (locAddrLeft && locAddrLeft->GetType())
    {
        oss << locAddrLeft->GetType();
        string s = oss.str();
        //oss2 << locRight->GetType();
        //string s2 = oss2.str();
        if ((s[s.length() - 1] == ']'))
        {
            CG.GenStore(locAddrLeft, locRight);
            //cout << "STORED" << endl;
            //cout << locLeft->GetName() << '~' << locRight->GetName() << endl;
            return listOfVars;
        }
    }
    CG.GenAssign(locLeft, locRight);
    //cout << "ASSIGN" << endl;
    //cout << locLeft->GetName() << '=' << locRight->GetName() << endl;
   /* if (locRight && locRight->GetType())
    {
        oss2 << locRight->GetType();
        string s = oss2.str();
        if (s == "Array")
        {   
            //cout << "SETTING SIZE" << endl;
            locLeft->setSize(locRight->GetSize());
        }
    }*/
    //cout << locRight->GetSize() << endl;
    //cout << "SIZE SET" << endl;
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
    vector<Location*> listOfVars, newListOfVars;
    //cout << "arrayaccess base emit" << endl;
    listOfVars = base->Emit(seg, offset, varsInScope);
    //cout << "returned from base" << endl;
    Location* start;
    //cout << listOfVars.back() << endl;
    start = listOfVars.back();
    //cout << start << endl;
    offset -= listOfVars.size() * CodeGenerator::VarSize;
    
    newListOfVars = subscript->Emit(seg, offset, varsInScope);
    Location* sub = newListOfVars.back();
    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());    
    
    Location* negOne = CG.GenLoadConstant(-1, offset);
    listOfVars.push_back(negOne);
    offset -= CodeGenerator::VarSize;
    
    const char* testLabel = CG.NewLabel();    
    const char* errorLabel = CG.NewLabel();
    CG.GenGoto(testLabel);
    CG.GenLabel(errorLabel);
    
    Location* errorLoc = CG.GenLoadConstant("Decaf runtime error: Array subscript out of bounds\\n", offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(errorLoc);
    CG.GenBuiltInCall(PrintString, errorLoc);
    CG.GenBuiltInCall(Halt);
       
    CG.GenLabel(testLabel);
    

    Location* negative = CG.GenBinaryOp("<", negOne, sub, offset); //if -1 < subscript
    listOfVars.push_back(negative);
    offset -= CodeGenerator::VarSize;
    
    CG.GenIfZ(negative, errorLabel);
    
    Location* sizeOfArray = CG.GenLoad(start, 0, offset);
    listOfVars.push_back(sizeOfArray);
    offset -= CodeGenerator::VarSize;
    
    Location* outOfBounds = CG.GenBinaryOp("<", sub, sizeOfArray, offset); //if sub < size
    listOfVars.push_back(outOfBounds);
    offset -= CodeGenerator::VarSize;
    
    CG.GenIfZ(outOfBounds, errorLabel);
    
    Location* four = CG.GenLoadConstant(CodeGenerator::VarSize, offset);
    listOfVars.push_back(four);
    offset -= CodeGenerator::VarSize;
    
    Location* arrayOffset = CG.GenBinaryOp("*", four, sub, offset);
    listOfVars.push_back(arrayOffset);
    offset -= CodeGenerator::VarSize;
    
    Location* totalOffset = CG.GenBinaryOp("+", four, arrayOffset, offset);
    listOfVars.push_back(totalOffset);
    offset -= CodeGenerator::VarSize;
    
    Location* addr = CG.GenBinaryOp("+", start, totalOffset, offset);
    addr->setType(start->GetType());
    ////cout << addr->GetType() << '.' << endl;
    listOfVars.push_back(addr);
    offset -= CodeGenerator::VarSize;
    
    Location* loc = CG.GenLoad(addr, 0, offset);
    listOfVars.push_back(loc);
    
    ostringstream oss;
    oss << start->GetType();
    string s = oss.str();
    s.resize(s.size()-2);
    loc->setType(new Type(s.c_str()));
    ////cout << loc->GetType() << endl;
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
    Location* loc = NULL;
    
    //cout << "Field Access emit: " << loc << endl;
    for (auto it = varsInScope.rbegin(); it != varsInScope.rend(); it++)
    {
        if (!strcmp((*it)->GetName(),field->name))
        {
            loc = *it;
            break;
        }
    }
    //cout << "finished emit: " << loc << endl;
    Assert(loc);
    
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
    Location* loc = NULL;
    

    if (!strcmp(field->name, "length"))
    {
        //cout << "LENGTH CALL" << endl;
        if (base)
        {
            listOfVars = base->Emit(seg, offset, varsInScope);
            offset -= listOfVars.size() * CodeGenerator::VarSize;
            
            ostringstream oss;
            oss << listOfVars.back()->GetType();
            string s = oss.str();
            
            if (s[s.length() - 1] == ']')
            {
                loc = CG.GenLoad(listOfVars.back(), 0, offset);
                //cout << "LENGTH TYPE" << endl;
                Type* t = Type::intType;
                //cout << t << endl;
                //cout << loc << endl;
                loc->setType(t);
                //cout << "type set" << endl;
                //cout << loc->GetType() << endl;
                listOfVars.push_back(loc);
                
                return listOfVars;
                
            }
        }
        
        Assert(NULL);
    }
            
    //cout << field->name << endl;
    //cout << "CALLED" << endl;
    for (int i = 0; i < varsInScope.size(); i++)
    {
        //cout << varsInScope[i]->GetName() << endl;
    }
    for (auto it = varsInScope.rbegin(); it != varsInScope.rend(); it++)
    {
        if (!strcmp((*it)->GetName(),field->name))
        {
            loc = *it;
            //cout << "FOUND: " << (*it)->GetName() << endl;
            //cout << (*it)->GetType() << endl;
            //cout << loc->GetType() << endl;
            break;
        }
    }
    Assert(loc);
    //cout << loc->GetType() << endl;
    for (int i = actuals->NumElements() - 1; i >= 0; i--)
    {
        newListOfVars = actuals->Nth(i)->Emit(seg, offset, varsInScope);
        listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
        offset -= newListOfVars.size() * CodeGenerator::VarSize;
        CG.GenPushParam(newListOfVars.back());
    }
    //cout << loc->GetType() << endl;
    //cout << "LCALL TYPE" << endl;
    Location* loc2;
    if (loc->GetType() == Type::voidType)
        loc2 = CG.GenLCall(field->name, false, offset);
    else
        loc2 = CG.GenLCall(field->name, true, offset);
    loc2->setType(loc->GetType());
    //cout << loc2->GetSize() << endl;
    //cout << loc2->GetSize() << endl;
    listOfVars.push_back(loc2);
    CG.GenPopParams(actuals->NumElements() * CodeGenerator::VarSize);
    return listOfVars;
}

NewExpr::NewExpr(yyltype loc, NamedType *c) : Expr(loc) { 
  Assert(c != NULL);
  (cType=c)->SetParent(this);
}

vector<Location*> NewExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars, newListOfVars;
    
    ostringstream oss;
    oss << cType;
    string s = oss.str();
    
    vector<classVarMember> vars = classVars[s];
    int sizeOfClass = vars.size() * CodeGenerator::VarSize + CodeGenerator::VarSize;
    
    Location* sizeLoc = CG.GenLoadConstant(sizeOfClass, offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(sizeLoc);
    
    Location* ptr = CG.GenBuiltInCall(Alloc, sizeLoc, NULL, offset);
    offset -= CodeGenerator::VarSize;
    ptr->setType(cType);
    listOfVars.push_back(ptr);
    
    return listOfVars;
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
    Location* arraySize = newListOfVars.back();
    //cout << newListOfVars.back()->GetType() << endl;
    //arraySize->setType(Type::intType);
    
    Location* one = CG.GenLoadConstant(1, offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(one);
    
    Location* testSize = CG.GenBinaryOp("<", arraySize, one, offset);
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
    
    Location* four = CG.GenLoadConstant(CodeGenerator::VarSize, offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(four);
    Location* sizeInBytes = CG.GenBinaryOp("*", four, arraySize, offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(sizeInBytes);
    
    Location* totalSize = CG.GenBinaryOp("+", four, sizeInBytes, offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(totalSize);
    
    Location* ptr = CG.GenBuiltInCall(Alloc, totalSize, NULL, offset);
    offset -= CodeGenerator::VarSize;
    CG.GenStore(ptr, arraySize); //store size in array[0]
    //ostringstream oss;
    //oss << elemType;
    //oss << "[]";
    ptr->setType(new Type("Array"));
    //cout << ptr->GetSize() << ' ';
    //cout << ptr << ' ' << ptr->GetSize() << endl;
    listOfVars.push_back(ptr);
    
    return listOfVars;
    
}


vector<Location*> ReadIntegerExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    Location* loc = CG.GenBuiltInCall(ReadInteger);
    loc->setType(Type::intType);
    listOfVars.push_back(loc);
    return listOfVars;
}

vector<Location*> ReadLineExpr::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{   
    vector<Location*> listOfVars;
    Location* loc = CG.GenBuiltInCall(ReadLine);
    loc->setType(Type::stringType);
    listOfVars.push_back(loc);
    return listOfVars;
}











       
