/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "codegen.h"
        
CodeGenerator CG;
unordered_map<string, vector<classVarMember> > classVars;
unordered_map<string, vector<classVarMember> > classMethods;
unordered_map<string, int> classSize;

bool inClass = false;
Type* curClass;
Location* curThis;
         
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
    //////////cout << loc->GetName() << ' ' << loc->GetOffset() << endl;
    //////////cout << loc << endl;
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

vector<Location*> ClassDecl::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{

    vector<Location*> listOfVars;
    if (seg == fpRelative)
        return listOfVars;
    ////cout << "CLASS DECL: BEGIN" << endl;
    inClass = true;
    curClass = new Type(id->name);
    List<const char*>* memberNames = new List<const char*>; 
    
    vector<classVarMember> vars;
    vector<classVarMember> methods;
    int vTableOffset = 0;	
    offset = 0;
    int varOffset = CodeGenerator::VarSize;
    if (extends)
    {
        bool found = false;
        ostringstream oss;
        oss << extends;
        string s = oss.str();
        for (int i = 0; i < varsInScope.size(); i++)
        {
            if (!strcmp(varsInScope[i]->GetName(), s.c_str()));
            {
                found = true;
            }
        }
        if (!found)
            return listOfVars;
            
        vector<classVarMember> varsInParent = classVars[s];
        vector<classVarMember> methodsInParent = classMethods[s];
        vars = varsInParent;
        varOffset += vars.size() * 4;
        
        for (int i = 0; i < methodsInParent.size(); i++)
        {
            string temp = s;
            string temp2 = methodsInParent[i].first;
            string final;
            final.append(temp);
            final.push_back('.');
            final.append(temp2);
            names.push_back(final);
            methods.push_back(methodsInParent[i]);
            vTableOffset += 4;
        }
        
        
    }
    

    
    //Location* locThis = new Location(seg, offset, "this");
    //varsInScope.push_back(locThis);
    //offset -= CodeGenerator::VarSize;
    
    vector<Location*> newListOfVars; 
    
    for(int i=0; i<members->NumElements(); i++){
    	FnDecl *fn = dynamic_cast<FnDecl*>(members->Nth(i)); 
    	VarDecl *vd = dynamic_cast<VarDecl*>(members->Nth(i));


		if (vd)
	    {
		    newListOfVars = members->Nth(i)->Emit(fpRelative, offset, varsInScope);
		    varsInScope.push_back(newListOfVars.back());
		    offset -= newListOfVars.size() * CodeGenerator::VarSize;
		    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
		    
		    classVarMember var;
		    string s = vd->id->name;
		    var.first = s;
		    var.second = varOffset;
		    varOffset += CodeGenerator::VarSize;
		    var.third = vd->getType();
		    vars.push_back(var);
		    classVars[id->name] = vars;
		}
    	else if(fn){
    	    string temp = id->name;
    	    string temp2 = members->Nth(i)->id->name; 
    	    string final; 
    	    final.append(temp);
    	    final.push_back('.');
    	    final.append(temp2);
       		names.push_back(final);
       		CG.GenLabel(names.back().c_str());
    	    newListOfVars = members->Nth(i)->EmitMore(fpRelative, offset, varsInScope, curClass);
    	    varsInScope.push_back(newListOfVars.back());
    	    offset -= newListOfVars.size() * CodeGenerator::VarSize;
    	    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    	    
    	    classVarMember method;
    	    string s = fn->id->name;
    	    method.first = s;
    	    method.second = vTableOffset;
    	    vTableOffset += CodeGenerator::VarSize;
    	    method.third = fn->returnType;
    	    methods.push_back(method);
    	    classMethods[id->name] = methods;
    	}
    	else
    	    Assert(NULL);
    }
    
    for (int i = 0; i < names.size(); i++)
    {
        ////////cout << names[i] << endl;
        //cout << names[i] << ' ' << id->name << endl;
	    memberNames->Append(names[i].c_str()); 
	}
	for (int i = 0; i < memberNames->NumElements(); i++)
    {
        //cout << memberNames->Nth(i) << endl;
    }
    if(memberNames->NumElements()>0){
    	CG.GenVTable(id->name,memberNames); 
    }
    string s = id->name;
    classVars[s] = vars;
    //////cout << "MY VARS" << endl;
    for (int i = 0; i < vars.size(); i++)
    {   
        //////cout << vars[i].first << endl;
    }
    //////cout << "END OF MY VARS" << endl;
    //////cout << s << "CLASS DECL" << endl;
    classMethods[s] = methods;
    //////cout << classVars["Cow"].size() << endl;
    ////////cout << "Class: " << id->name << endl;
    for (int i = 0; i < vars.size(); i++)
    {
        ////////cout << vars[i].first << ' ' << vars[i].second << endl;
    }
    classSize[s] = varOffset - 4;
    Location* loc = new Location(seg, offset, id->name);
    listOfVars.push_back(loc);
    inClass = false;
    ////cout << "CLASS DECL: END" << endl;
    return listOfVars;
}


InterfaceDecl::InterfaceDecl(Identifier *n, List<Decl*> *m) : Decl(n) {
    Assert(n != NULL && m != NULL);
    (members=m)->SetParentAll(this);
}

vector<Location*> FnDecl::EmitMore(Segment seg, int offset, vector<Location*> varsInScope, Type* t)
{
    vector<Location*> listOfVars;
    if (seg == gpRelative) //XXX wont be true for class functions
    {
        Location* loc = new Location(seg, offset, id->name, returnType);
        listOfVars.push_back(loc);
        ////////cout << loc->GetType() << endl;
        return listOfVars;
    }
    ////////cout << id->name << "!!!!!" << endl;
    BeginFunc* BF = CG.GenBeginFunc();
    int localOffset = CodeGenerator::OffsetToFirstLocal;
    int paramOffset = CodeGenerator::OffsetToFirstParam;

    Location* locThis = new Location(seg, paramOffset, "this");
    curThis = locThis;
    paramOffset += CodeGenerator::VarSize;
    listOfVars.push_back(locThis);
    for (int i = 0; i < formals->NumElements(); i++)
    {
        vector<Location*> newListOfVars = formals->Nth(i)->Emit(fpRelative, paramOffset, varsInScope);
        listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
        paramOffset += newListOfVars.size() * CodeGenerator::VarSize;
    }
    varsInScope.insert(varsInScope.end(), listOfVars.begin(), listOfVars.end());
    vector<Location*> newListOfVars = body->EmitMore(fpRelative, localOffset, varsInScope, t);
    listOfVars.insert(listOfVars.end(), newListOfVars.begin(), newListOfVars.end());
    localOffset -= newListOfVars.size() * CodeGenerator::VarSize;
    BF->SetFrameSize(listOfVars.size() * CodeGenerator::VarSize); //SetFrameSize(int numBytesForAllLocalsAndTemps);
    CG.GenEndFunc();
    listOfVars.push_back(new Location(seg, offset, id->name, returnType));
    
    return listOfVars;
}

vector<Location*> FnDecl::Emit(Segment seg, int offset, vector<Location*> varsInScope)
{
    vector<Location*> listOfVars;
    if (seg == gpRelative) //XXX wont be true for class functions
    {
        Location* loc = new Location(seg, offset, id->name, returnType);
        listOfVars.push_back(loc);
        ////////cout << loc->GetType() << endl;
        return listOfVars;
    }
    ////////cout << id->name << "!!!!!" << endl;
    CG.GenLabel(id->name);
    BeginFunc* BF = CG.GenBeginFunc();
    int localOffset = CodeGenerator::OffsetToFirstLocal;
    int paramOffset = CodeGenerator::OffsetToFirstParam;
    paramOffset = 4;
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



