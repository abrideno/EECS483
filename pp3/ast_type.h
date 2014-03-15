/* File: ast_type.h
 * ----------------
 * In our parse tree, Type nodes are used to represent and
 * store type information. The base Type class is used
 * for built-in types, the NamedType for classes and interfaces,
 * and the ArrayType for arrays of other types.  
 *
 * pp3: You will need to extend the Type classes to implement
 * the type system and rules for type equivalency and compatibility.
 */
 
#ifndef _H_ast_type
#define _H_ast_type

#include "ast.h"
#include "list.h"
#include <iostream>

#define DEBUG 1

using namespace std;

void Debug(string s)
{
    if (DEBUG)
        cout << s << endl;
}

class Type : public Node 
{
  protected:
    char *typeName;

  public :
    static Type *intType, *doubleType, *boolType, *voidType,
                *nullType, *stringType, *errorType;

    Type(yyltype loc) : Node(loc) {}
    Type(const char *str);
    virtual void PrintToStream(std::ostream& out) { out << typeName; }
    friend std::ostream& operator<<(std::ostream& out, Type *t) { t->PrintToStream(out); return out; }
//     virtual bool match(Type *other) { return this == other; }
 	 virtual bool isBasicType(){return true;}
	 virtual const char* fetchKey(){return typeName;}
};

class NamedType : public Type 
{
  public:
    Identifier *id;
    
  public:
    NamedType(Identifier *id);
    const char* fetchKey(){return id->name;}
    void PrintToStream(std::ostream& out) { out << id; }
    bool isBasicType(){return false;}

};

class ArrayType : public Type 
{
  protected:
    Type *elemType;

  public:
    ArrayType(yyltype loc, Type *elemType);
    Type* getArrayType(){return elemType;}
    const char* fetchKey(){return elemType->fetchKey();}
    void PrintToStream(std::ostream& out) { out << elemType << "[]"; }
//      bool isBasicType(){return false;}

};

 
#endif
