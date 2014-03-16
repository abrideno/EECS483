/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"

using namespace std;
        
         
Decl::Decl(Identifier *n) : Node(*n->GetLocation()) {
    //scope = new Slevel; 
    Assert(n != NULL);
    (id=n)->SetParent(this); 
}

void Decl::addLevel(Slevel *parent){
	scope->Parent = parent; 
}	


VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
}

void VarDecl::Check(){
	 if(type->isBasicType()){ 
 		return;   
 	}
	
	Slevel *tempS = scope; 
	while(tempS != NULL){
		Decl *tempD; 
		tempD = tempS->stable->Lookup(type->fetchKey()); 
		if(tempD != NULL){
			ClassDecl *tempC = dynamic_cast<ClassDecl*>(tempD); 
			InterfaceDecl *tempI = dynamic_cast<InterfaceDecl*>(tempD); 
			if(tempC == NULL && tempI == NULL){
				ReportError::IdentifierNotDeclared(id, LookingForType); 
				return; 
			}
		}	
		tempS= tempS->Parent; 
	}
	//type->ReportNotDeclaredIdentifier(LookingForType); 
}

ClassDecl::ClassDecl(Identifier *n, NamedType *ex, List<NamedType*> *imp, List<Decl*> *m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    Assert(n != NULL && imp != NULL && m != NULL);     
    extends = ex;
    type = new Type(n->name);
    if (extends) extends->SetParent(this);
    (implements=imp)->SetParentAll(this);
    (members=m)->SetParentAll(this);
}

void ClassDecl::addLevel(Slevel *parent){
    scope = new Slevel;
	scope->Parent = parent; 
	scope->cDecl = this; 
	cout << "ADD LEVEL" << endl;
	int numElem = members->NumElements(); 
	
	for(int i = 0; i<numElem; i++){
		scope->add(members->Nth(i)); 
	}
    cout << "what?" << endl;
    cout << numElem << endl;
	for(int i = 0; i<numElem; i++){
	    VarDecl* vd = dynamic_cast<VarDecl*>(members->Nth(i));
	    if (!vd) 
		    members->Nth(i)->addLevel(scope); 
		FnDecl* fd = dynamic_cast<FnDecl*>(members->Nth(i));
		if (fd)
		    cout << fd << " FUS RO DAH" << endl;
	}	
	cout << "finished add level" << endl;
}

void ClassDecl::Check(){
    cout << "fail?" << endl;
	int numElem = members->NumElements(); 
	cout << "CHECK" << endl;
	for(int i=0; i<numElem; i++){
		members->Nth(i)->Check(); 
	}
	
	
	if(extends != NULL ){
		Decl *temp = scope->Parent->stable->Lookup(extends->fetchKey());
		ClassDecl *cDec = dynamic_cast<ClassDecl*>(temp); 
		if(cDec == NULL){
		    //extends
			ReportError::IdentifierNotDeclared(id, LookingForClass); 
		}
	}
	
	if(implements->NumElements() != 0){
		for(int i=0; i<implements->NumElements(); i++){
			Decl *temp = scope->Parent->stable->Lookup(implements->Nth(i)->fetchKey());
			InterfaceDecl *itemp = dynamic_cast<InterfaceDecl*>(temp); 
			
			if(itemp == NULL){
				ReportError::IdentifierNotDeclared(implements->Nth(i)->id, LookingForInterface);
			}
		}
	}
	//TODO
	//extended members
	//implemented members 
	// interface implementation 
	
	

	return;
}
InterfaceDecl::InterfaceDecl(Identifier *n, List<Decl*> *m) : Decl(n) {
    Assert(n != NULL && m != NULL);
    type = new Type(n->name);
    (members=m)->SetParentAll(this);
}

void InterfaceDecl::addLevel(Slevel *parent){
	scope->Parent = parent; 
	
	int numElem = members->NumElements(); 
	
	for(int i=0; i<numElem; i++){
		scope->add(members->Nth(i)); 
	}
	
	for(int i=0; i<numElem; i++){
		members->Nth(i)->addLevel(scope); 
	}
}
	
void InterfaceDecl::Check(){
	int numElem = members->NumElements(); 
	for(int i=0; i<numElem; i++){
		members->Nth(i)->Check(); 
	}
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

void FnDecl::addLevel(Slevel *parent){
    scope = new Slevel;
	scope->Parent=parent; 
	scope->fDecl = this; 
	
	int numElem = formals->NumElements(); 
	cout << "FN DECL: elems=" << numElem << endl;
	for(int i=0; i<numElem; i++){
		scope->add(formals->Nth(i)); 
	}
	
	//XXX
	/*for(int i=0; i<numElem; i++){
		formals->Nth(i)->addLevel(scope); 
	}*/
	
    StmtBlock *block = dynamic_cast<StmtBlock*>(body); 
	if(block != NULL){
        block->addLevel(scope);
		//TODO: add stmtBlocks
	}
}

void FnDecl::Check(){
    cout << "FNDECL " << this << endl;
	int numElem = formals->NumElements(); 
	for(int i =0; i<numElem; i++){
		formals->Nth(i)->Check(); 
	}
    
	if(body!= NULL){
		body->Check(); 
	}
}

bool FnDecl::match(Decl *compare){
	FnDecl *temp = dynamic_cast<FnDecl*>(compare); 
	int numElem = formals->NumElements(); 
	
	if(temp == NULL){
		return false; 
	}
	
	int compnumElem = temp->formals->NumElements(); 
	if(numElem != compnumElem || (returnType != temp->returnType)){
		return false; 
	}
	
	for(int i=0; i<numElem; i++){
		if(!(formals->Nth(i)->CheckResultType() ==(temp->formals->Nth(i)->CheckResultType()))){
			return false; 
     	}
    }
    
    return true; 
}

