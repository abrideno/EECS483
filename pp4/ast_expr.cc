/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */
#include "ast_expr.h"
#include "ast_decl.h"
#include <string.h>

extern CodeGenerator CG;
extern unordered_map<string, vector<classVarMember> > classVars;
extern unordered_map<string, int> classSize;
extern unordered_map<string, vector<classVarMember> > classMethods;
extern bool inClass;
extern bool inClass2;
extern Type* curClass;
extern Location* curThis;

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


vector<Location*> AssignExpr::EmitMore(Segment seg, int offset, vector<Location*> varsInScope, Type* t)
{
    vector<Location*> listOfVars, newListOfVars;
    ostringstream oss, oss2;
    if (inClass)
    {
        inClass = false;
        newListOfVars = left->Emit(seg, offset, varsInScope);
        inClass = true;
        //////////cout << newListOfVars.back()->GetName() << endl;
    }
    else
        Assert(NULL);
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
    bool foundLeft = false, foundRight = false;
    
    ostringstream oss3;
    oss3 << t;
    ////////////cout << t << endl;
    string s = oss3.str();
    string sLeft = locLeft->GetName();
    string sRight = locRight->GetName();
    
    vector<classVarMember> vars = classVars[s];
    ////////////cout << classVars["Cow"].size() <<" cunt"<< endl;
    ////////////cout << "SIZES" << endl;
    for (auto it = classVars.begin(); it != classVars.end(); ++it)
    {
        ////////////cout << it->second.size() << endl;
    }
    ////////////cout << "END SIZES" << endl;
    classVarMember var;
    ////////////cout << ":OWQJE:OQEW:OQWE:OHWQE" << endl;
    ////////////cout << sLeft << endl;
    for (int i = 0; i < vars.size(); i++)
    {
        ////////////cout << "FOR LOO{P" << endl;
        if (vars[i].first == sLeft)
        {
            foundLeft = true;
            var = vars[i];
            break;
        }
    }
    if (foundLeft)
    {   
        ////////////cout << "FOUND LEFT" << endl;
        Location* thisLoc;
        for (int i = 0; i < varsInScope.size(); i++)
        {
            if (!strcmp(varsInScope[i]->GetName(),"this"))
            {
                thisLoc = varsInScope[i];
                break;
            }
        }
        /*Location* addrOffset = CG.GenLoadConstant(var.second, offset);
        offset -= 4;
        listOfVars.push_back(addrOffset);
        Location* addr = CG.GenBinaryOp("+", thisLoc, var.second, offset);
        offset -= 4;
        listOfVars.push_back(addr);*/
        CG.GenStore(thisLoc, locRight, var.second);
        return listOfVars;
    }
    
   
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
    //////////////cout << "arrayaccess base emit" << endl;
    if (inClass != inClass2)
        inClass = true;
    listOfVars = base->Emit(seg, offset, varsInScope);
    //////////////cout << "returned from base" << endl;
    Location* start;
    //////////////cout << listOfVars.back() << endl;
    start = listOfVars.back();
    //////////////cout << start << endl;
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
    ////////////////cout << addr->GetType() << '.' << endl;
    listOfVars.push_back(addr);
    offset -= CodeGenerator::VarSize;
    
    Location* loc = CG.GenLoad(addr, 0, offset);
    listOfVars.push_back(loc);
    
    ostringstream oss;
    oss << start->GetType();
    string s = oss.str();
    s.resize(s.size()-2);
    loc->setType(new Type(s.c_str()));
    ////////////////cout << loc->GetType() << endl;
    return listOfVars;
    
}
     
FieldAccess::FieldAccess(Expr *b, Identifier *f) 
  : LValue(b? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
    Assert(f != NULL); // b can be be NULL (just means no explicit base)
    base = b; 
    if (base) base->SetParent(this); 
    (field=f)->SetParent(this);
}

vector<Location*> FieldAccess::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    vector<Location*> newListOfVars; 
    Location* loc = NULL;
    bool found = false;
    
    classVarMember var;
    if (inClass)
    {
        //////cout << "IN CLASS" << endl;
        //////cout << "VARS In SCOPE: FIELDACCESS" << endl;
        for (int i = 0; i < varsInScope.size(); i++)
        {
            //////cout << varsInScope[i]->GetName() << endl;
        }
        //////cout << "VARSINSCOPE end" << endl;
        ostringstream oss;
        oss << curClass;
        string s = oss.str();
        
        vector<classVarMember> vars = classVars[s];
        
        //////cout << "CLASS VARS" << endl;
        //////cout << "field name: " << field->name << endl;
        for (int i = 0; i < vars.size(); i++)
        {
            //////cout << vars[i].first << endl;
            if (!strcmp(vars[i].first.c_str(), field->name))
            {
                ////////////cout<<"Looking at "<<vars[i].first<<" in "<<s<<endl;
                var = vars[i];
                found = true;
                //////cout << "FOUND" << endl;
                break;
            }
        }
        //////cout << "CLASS VARS END" << endl;
    }
    if (found)
    {  
    
        Location* thisLoc = NULL;
        for (int i = 0; i < varsInScope.size(); i++)
        {
            if (!strcmp(varsInScope[i]->GetName(),"this"))
            {
                thisLoc = varsInScope[i];
              //  //////////cout<<"GETTING IN HERE"<<endl;
                break;
            }
        }
        //////cout << thisLoc << endl;
        //////////cout << thisLoc << endl;
        // Problem here 
        /*Location* addrOffset = CG.GenLoadConstant(var.second, offset);
        offset -= 4;
        listOfVars.push_back(addrOffset);
        Location* addr = CG.GenBinaryOp("+", thisLoc, addrOffset, offset);
        offset -= 4;
        listOfVars.push_back(addr);*/
        //addr->setType(new Type("this"));
        loc = CG.GenLoad(thisLoc, var.second, offset);
        ////////////cout<<" var second "<< var.second<<endl;
        Assert(thisLoc);
        offset -= 4;
        loc->setType(var.third);
        listOfVars.push_back(loc);
        Assert(loc);
        return listOfVars;
    }

  	for (auto it = varsInScope.rbegin(); it != varsInScope.rend(); it++)
    {
        if (!strcmp((*it)->GetName(),field->name))
        {
            loc = *it;
            break;
        }
    }
    //////////////cout << "finished emit: " << loc << endl;
    if(!loc)
    {
        loc = new Location(fpRelative, offset, field->name);
    }
    
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
    
    if (inClass)
    {
        ////cout << "IN CLASS" << endl;
        bool found;
        ostringstream oss;
        oss << curClass;
        string s = oss.str();
        vector<classVarMember> methodsInClass = classMethods[s];
        classVarMember method;
        ////cout << curClass << endl;
        for (int i = 0; i < methodsInClass.size(); i++)
        {
            ////cout << methodsInClass[i].first << endl; 
            if (!strcmp(field->name, methodsInClass[i].first.c_str()))
            {
                ////cout << "FOUND" << endl;
                method = methodsInClass[i];
                found = true;
                break;
            }
        }
        if (found)
        {
            Location* thisLoc = curThis;
            
            Location* vTable = CG.GenLoad(curThis, 0, offset);
            offset -= 4;
            listOfVars.push_back(vTable);
            
            int methodOffset = method.second;
            
            Location* funcAddr = CG.GenLoad(vTable, methodOffset, offset);
            offset -= CodeGenerator::VarSize;
            listOfVars.push_back(funcAddr);
            
            
            for (int i = actuals->NumElements() - 1; i >= 0; i--)
            {
                newListOfVars = actuals->Nth(i)->Emit(seg, offset, varsInScope);
                listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
                offset -= newListOfVars.size() * CodeGenerator::VarSize;
                CG.GenPushParam(newListOfVars.back());
            }
            listOfVars.push_back(thisLoc);
            CG.GenPushParam(thisLoc);
            
            
            ostringstream oss2;
            oss2 << method.third;
            string s3 = oss2.str();
            Location* aCallAddr;
            if (s3 == "void")
                CG.GenACall(funcAddr, false, offset);
            else
            {
                aCallAddr = CG.GenACall(funcAddr, true, offset);
                aCallAddr->setType(method.third);
                listOfVars.push_back(aCallAddr);
            }
            
            
            CG.GenPopParams(actuals->NumElements() * CodeGenerator::VarSize + 4);
         
      	    return listOfVars;
        }
    }
    if (!strcmp(field->name, "length"))
    {
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
                Type* t = Type::intType;
                loc->setType(t);
                listOfVars.push_back(loc);
                
                return listOfVars;
                
            }
        }
        
        Assert(NULL);
    }
    
    if(base)
    {
        for (int i = 0; i < varsInScope.size(); i++)
        {
            //////////cout << varsInScope[i]->GetName() << endl;
        }
        ostringstream oss;
        newListOfVars = base->Emit(seg, offset, varsInScope);
        offset -= newListOfVars.size() * CodeGenerator::VarSize;
        listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
        
        Location* ptr = newListOfVars.back();
        Type* baseType = ptr->GetType();
        oss << baseType;
        string s = oss.str();
        
        Location* vTable = CG.GenLoad(ptr, 0, offset);
        offset -= CodeGenerator::VarSize;
        listOfVars.push_back(vTable);
         
        vector<classVarMember> methodsInClass = classMethods[s];
        classVarMember method;
        string s2 = field->name;
        for (int i = 0; i < methodsInClass.size(); i++)
        {
            if (methodsInClass[i].first == s2)
            {
                method = methodsInClass[i];
                break;
            }
        }
        
        int methodOffset = method.second;
        //cout<<"THis is method offset "<<methodOffset<<endl;
        Location* funcAddr = CG.GenLoad(vTable, methodOffset, offset);
        offset -= CodeGenerator::VarSize;
        listOfVars.push_back(funcAddr);
        
        
        for (int i = actuals->NumElements() - 1; i >= 0; i--)
        {
            newListOfVars = actuals->Nth(i)->Emit(seg, offset, varsInScope);
            listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
            offset -= newListOfVars.size() * CodeGenerator::VarSize;
            CG.GenPushParam(newListOfVars.back());
        }
        Assert(ptr);
        CG.GenPushParam(ptr);
        
        ////////cout << ptr << endl;
        
        ostringstream oss2;
        oss2 << method.third;
        string s3 = oss2.str();
        Location* aCallAddr;
        if (s3 == "void")
            CG.GenACall(funcAddr, false, offset);
        else
        {
            aCallAddr = CG.GenACall(funcAddr, true, offset);
            aCallAddr->setType(method.third);
            listOfVars.push_back(aCallAddr);
        }
        
        
        CG.GenPopParams(actuals->NumElements() * CodeGenerator::VarSize + 4);
     
  	    return listOfVars;
  
    }
    else
    {

            
        for (int i = 0; i < varsInScope.size(); i++)
        {
            //////////////cout << varsInScope[i]->GetName() << endl;
        }
        for (auto it = varsInScope.rbegin(); it != varsInScope.rend(); it++)
        {
            if (!strcmp((*it)->GetName(),field->name))
            {
                loc = *it;
                break;
            }
        }
        Assert(loc);
        for (int i = actuals->NumElements() - 1; i >= 0; i--)
        {
            newListOfVars = actuals->Nth(i)->Emit(seg, offset, varsInScope);
            listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
            offset -= newListOfVars.size() * CodeGenerator::VarSize;
            CG.GenPushParam(newListOfVars.back());
        }
        Location* loc2;
        if (loc->GetType() == Type::voidType)
            loc2 = CG.GenLCall(field->name, false, offset);
        else
            loc2 = CG.GenLCall(field->name, true, offset);
        loc2->setType(loc->GetType());
        listOfVars.push_back(loc2);
        CG.GenPopParams(actuals->NumElements() * CodeGenerator::VarSize);
        return listOfVars;
    }
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
    
    int sizeOfClass = classSize[s];

    Location* sizeLoc = CG.GenLoadConstant(sizeOfClass + 4, offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(sizeLoc);
    
    Location* ptr = CG.GenBuiltInCall(Alloc, sizeLoc, NULL, offset);
    offset -= CodeGenerator::VarSize;
    ptr->setType(cType);
    
    Location* vTable = CG.GenLoadLabel(s.c_str(), offset);
    offset -= CodeGenerator::VarSize;
    listOfVars.push_back(vTable);
    
    
    CG.GenStore(ptr, vTable);
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
    //////////////cout << newListOfVars.back()->GetType() << endl;
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
    //////////////cout << ptr->GetSize() << ' ';
    //////////////cout << ptr << ' ' << ptr->GetSize() << endl;
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











       
