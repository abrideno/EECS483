/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include <sstream>

using namespace std;

Slevel *Program::parentScope = new Slevel(); 

void Slevel::add(Decl *dec){
	Decl *temp = stable->Lookup(dec->id->name); 
	if(temp != NULL){
		ReportError::DeclConflict(dec,temp); 
		
		return; 
	}

	stable->Enter(dec->id->name, dec);
	return; 
}

Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void Program::addLevel() {
	int numElems = decls->NumElements(); 
	for(int i=0; i<numElems; i++){
		parentScope->add(decls->Nth(i)); 
	    ////cout << "global decls: " << decls->Nth(i) << endl;
	}
	////cout << "program passed decls" << endl;
	for(int i=0; i<numElems; i++){
	    ////cout << "adding new level..." << decls->Nth(i) << endl;
	    VarDecl* vd = dynamic_cast<VarDecl*>(decls->Nth(i));
	    if (!vd)
		    decls->Nth(i)->addLevel(parentScope); 
	}
	////cout << "program passed add level" << endl;
}

void Program::Check() {
    /* pp3: here is where the semantic analyzer is kicked off.
     *      The general idea is perform a tree traversal of the
     *      entire program, examining all constructs for compliance
     *      with the semantic rules.  Each node can have its own way of
     *      checking itself, which makes for a great use of inheritance
     *      and polymorphism in the node classes.
     */
     ////cout << "PROGRAM ADD LEVEL" << endl;
     addLevel(); 
     int numElems = decls->NumElements(); 
     ////cout << "PROGRAM CHECK" << endl;
     for(int i= 0; i< numElems ; i++){
     	decls->Nth(i)->Check(); 
     }     
}

void Stmt::addLevel(Slevel *parent){
    scope = new Slevel;
	scope->Parent = parent;
}


StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

void StmtBlock::addLevel(Slevel *parent){
    scope = new Slevel;
	scope->Parent = parent;	
	int numElems = decls->NumElements(); 
	for(int i=0; i<numElems; i++){
		scope->add(decls->Nth(i)); 
	}
	/*for(int i=0; i<numElems; i++){
		decls->Nth(i)->addLevel(scope); 
	} */
	////cout << "stmtblock added decls, going to stmts" << endl;
	numElems = stmts->NumElements(); 
	for(int i=0; i<numElems; i++){
		stmts->Nth(i)->addLevel(scope); 
	}
}

void StmtBlock::Check(){
	for(int i=0; i<decls->NumElements(); i++){
		//decls->Nth(i)->Check(); 
	} 
	int numElems = stmts->NumElements(); 
	for(int i=0; i<numElems; i++){
		stmts->Nth(i)->Check(); 
	}

}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) { 
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this); 
    (body=b)->SetParent(this);
}

void ConditionalStmt::addLevel(Slevel *parent){
    scope = new Slevel;
    scope->Parent = parent;	
    test->addLevel(scope); 
	body->addLevel(scope); 
}

void ConditionalStmt::Check(){
	test->Check(); 
	body->Check(); 
	
	if(!(test->CheckResultType() == Type::boolType)){
		ReportError::TestNotBoolean(test); 
	}

}

void LoopStmt::addLevel(Slevel *parent){
     scope = new Slevel;
	 scope->Parent = parent; 
	 scope->lStmt = this; 
	 test->addLevel(scope); 
	 body->addLevel(scope); 
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) { 
    Assert(i != NULL && t != NULL && s != NULL && b != NULL);
    (init=i)->SetParent(this);
    (step=s)->SetParent(this);
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) { 
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

void IfStmt::addLevel(Slevel *parent){
    scope = new Slevel;
	scope->Parent = parent; 
	
    ////cout << "if add level" << endl;
	test->addLevel(scope); 
	body->addLevel(scope); 
	////cout << "if add level complete" << endl;Type *Type::intArrType    = new Type("int[]");
	if(elseBody != NULL){
		elseBody->addLevel(scope); 
	}
	////cout << "else complete" << endl;
}

void IfStmt::Check(){
	test->Check(); 
	body->Check(); 
	
	if(!(test->CheckResultType() == Type::boolType)){
		//ReportError::TestNotBoolean(test); 
	}
	
	if(elseBody != NULL){
		elseBody->Check(); 
	}
}

void BreakStmt::Check(){
    
Slevel *temp = scope; 
	while(temp != NULL) {
		if( temp->getlStmt() != NULL){
			return; 
		}
	temp = temp->Parent; 
	}
	
	ReportError::BreakOutsideLoop(this); 
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) { 
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

void ReturnStmt::addLevel(Slevel *parent) {
	scope = parent; 
	
	expr->addLevel(scope); 
}
  
void ReturnStmt::Check() { 	
	expr->Check(); 
	Slevel *tempS = scope; 
	FnDecl *tempF = tempS->getfDecl(); 
    bool matched = false; 
	
	while(tempS != NULL){
		if(tempF != NULL) {
		    matched = true; 
			break; 
		}
		tempS = tempS->Parent;
		tempF = tempS->getfDecl(); 
	}	
	
	if(!matched){
		ReportError::Formatted(GetLocation(),"return is only allowed inside a function");
		return; 
	}
	
	Type *given = expr->CheckResultType(); 
	Type *expected = tempF->CheckResultType();  // returned value for function 
	ostringstream oss, oss2;
	oss << given;
	oss2 << expected;
	
	if(oss.str() != oss2.str()){
		ReportError::ReturnMismatch(this,given,expected); 
	}
}
PrintStmt::PrintStmt(List<Expr*> *a) {    
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

void PrintStmt::Check()
{
    ////cout << "PrintStmt CHECK" << endl;
	int numElem = args->NumElements(); 
	for(int i=0; i<numElem; i++){
		Type *given = args->Nth(i)->CheckResultType(); 
		ostringstream oss;
		oss << given;
		
		if((oss.str() != "bool") && (oss.str() != "string") && (oss.str() != "int") && (oss.str() != "int[]") && (oss.str() != "string[]") && (oss.str() != "bool[]")) {
		   ReportError::PrintArgMismatch(args->Nth(i), i+1, given); 
		}
	}
	
	for(int i=0; i<numElem; i++){
		args->Nth(i)->Check(); 
	}
}
void PrintStmt::addLevel(Slevel *parent){
    ////cout << "print stmt add level" << endl;
    scope = new Slevel; //XXX
	scope->Parent = parent; 
	
	for (int i = 0; i < args->NumElements(); i++)
	{
	    args->Nth(i)->addLevel(scope);
    }
    ////cout << "print stmt finished" << endl;
}




