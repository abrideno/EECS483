/* File: tac.cc
 * ------------
 * Implementation of Location class and Instruction class/subclasses.
 */
  
#include "tac.h"
#include "mips.h"
#include <string.h>
#include <deque>
#include <iostream>

using namespace std;

Location::Location(Segment s, int o, const char *name) :
  variableName(strdup(name)), segment(s), offset(o), reference(NULL), reg(Mips::zero) {}

string Instruction::TACString()
{
    string s = printed;
    return s;
}
 
void Instruction::Print() {
  printf("\t%s ;", printed);
  printf("\n");
}

void Instruction::Emit(Mips *mips) {
  if (*printed)
    mips->Emit("# %s", printed);   // emit TAC as comment into assembly
  EmitSpecific(mips);
}

LoadConstant::LoadConstant(Location *d, int v)
  : dst(d), val(v) {
  Assert(dst != NULL);
  sprintf(printed, "%s = %d", dst->GetName(), val);
}
void LoadConstant::EmitSpecific(Mips *mips) {
  mips->EmitLoadConstant(dst, val);
}
List<string> LoadConstant::KillSet()
{
    List<string> set;
    string destination;
    destination = dst->GetName();
    set.Append(destination);
    return set;
}
bool LoadConstant::isDead()
{
    string destination = dst->GetName();
    for (int i = 0; i < outSet.NumElements(); i++)
    {
        if (outSet.Nth(i) == destination)
            return false;
    }
    return true;
}

LoadStringConstant::LoadStringConstant(Location *d, const char *s)
  : dst(d) {
  Assert(dst != NULL && s != NULL);
  const char *quote = (*s == '"') ? "" : "\"";
  str = new char[strlen(s) + 2*strlen(quote) + 1];
  sprintf(str, "%s%s%s", quote, s, quote);
  quote = (strlen(str) > 50) ? "...\"" : "";
  sprintf(printed, "%s = %.50s%s", dst->GetName(), str, quote);
}
void LoadStringConstant::EmitSpecific(Mips *mips) {
  mips->EmitLoadStringConstant(dst, str);
}
List<string> LoadStringConstant::KillSet()
{
    List<string> set;
    string destination;
    destination = dst->GetName();
    set.Append(destination);
    return set;
}
bool LoadStringConstant::isDead()
{
    string destination = dst->GetName();
    for (int i = 0; i < outSet.NumElements(); i++)
    {
        if (outSet.Nth(i) == destination)
            return false;
    }
    return true;
}
     

LoadLabel::LoadLabel(Location *d, const char *l)
  : dst(d), label(strdup(l)) {
  Assert(dst != NULL && label != NULL);
  sprintf(printed, "%s = %s", dst->GetName(), label);
}
void LoadLabel::EmitSpecific(Mips *mips) {
  mips->EmitLoadLabel(dst, label);
}



Assign::Assign(Location *d, Location *s)
  : dst(d), src(s) {
  Assert(dst != NULL && src != NULL);
  sprintf(printed, "%s = %s", dst->GetName(), src->GetName());
}
void Assign::EmitSpecific(Mips *mips) {
  mips->EmitCopy(dst, src);
}
List<string> Assign::KillSet()
{
    List<string> set;
    string destination;
    destination = dst->GetName();
    set.Append(destination);
    return set;
}
List<string> Assign::GenSet()
{
    List<string> set;
    string source;
    source = src->GetName();
    set.Append(source);
    return set;
}
bool Assign::isDead()
{
    string destination = dst->GetName();
    for (int i = 0; i < outSet.NumElements(); i++)
    {
        if (outSet.Nth(i) == destination)
            return false;
    }
    return true;
}

Load::Load(Location *d, Location *s, int off)
  : dst(d), src(s), offset(off) {
  Assert(dst != NULL && src != NULL);
  if (offset) 
    sprintf(printed, "%s = *(%s + %d)", dst->GetName(), src->GetName(), offset);
  else
    sprintf(printed, "%s = *(%s)", dst->GetName(), src->GetName());
}
void Load::EmitSpecific(Mips *mips) {
  mips->EmitLoad(dst, src, offset);
}
List<string> Load::GenSet()
{
    List<string> set;
    string source;
    source = src->GetName();
    set.Append(source);
    string destination;
    destination = dst->GetName();
    set.Append(destination);
    return set;
}


Store::Store(Location *d, Location *s, int off)
  : dst(d), src(s), offset(off) {
  Assert(dst != NULL && src != NULL);
  if (offset)
    sprintf(printed, "*(%s + %d) = %s", dst->GetName(), offset, src->GetName());
  else
    sprintf(printed, "*(%s) = %s", dst->GetName(), src->GetName());
}
void Store::EmitSpecific(Mips *mips) {
  mips->EmitStore(dst, src, offset);
}
List<string> Store::GenSet()
{
    List<string> set;
    string source;
    source = src->GetName();
    set.Append(source);
    string destination;
    destination = dst->GetName();
    set.Append(destination);
    return set;
}
 
const char * const BinaryOp::opName[Mips::NumOps]  = {"+", "-", "*", "/", "%", "==", "<", "&&", "||"};;

Mips::OpCode BinaryOp::OpCodeForName(const char *name) {
  for (int i = 0; i < Mips::NumOps; i++) 
    if (opName[i] && !strcmp(opName[i], name))
	return (Mips::OpCode)i;
  Failure("Unrecognized Tac operator: '%s'\n", name);
  return Mips::Add; // can't get here, but compiler doesn't know that
}

BinaryOp::BinaryOp(Mips::OpCode c, Location *d, Location *o1, Location *o2)
  : code(c), dst(d), op1(o1), op2(o2) {
  Assert(dst != NULL && op1 != NULL && op2 != NULL);
  Assert(code >= 0 && code < Mips::NumOps);
  sprintf(printed, "%s = %s %s %s", dst->GetName(), op1->GetName(), opName[code], op2->GetName());
}
void BinaryOp::EmitSpecific(Mips *mips) {	  
  mips->EmitBinaryOp(code, dst, op1, op2);
}
List<string> BinaryOp::KillSet()
{
    List<string> set;
    string destination;
    destination = dst->GetName();
    set.Append(destination);
    return set;
}
List<string> BinaryOp::GenSet()
{
    List<string> set;
    string left, right;
    left = op1->GetName();
    right = op2->GetName();
    set.Append(left);
    set.Append(right);
    return set;
}
bool BinaryOp::isDead()
{
    string destination = dst->GetName();
    for (int i = 0; i < outSet.NumElements(); i++)
    {
        if (outSet.Nth(i) == destination)
            return false;
    }
    return true;
}

Label::Label(const char *l) : label(strdup(l)) {
  Assert(label != NULL);
  *printed = '\0';
}
void Label::Print() {
  printf("%s:\n", label);
}
void Label::EmitSpecific(Mips *mips) {
  mips->EmitLabel(label);
}


 
Goto::Goto(const char *l) : label(strdup(l)) {
  Assert(label != NULL);
  sprintf(printed, "Goto %s", label);
}
void Goto::EmitSpecific(Mips *mips) {	  
  mips->EmitGoto(label);
}
string Goto::getLabel()
{
    string s = label;
    return s;
}


IfZ::IfZ(Location *te, const char *l)
   : test(te), label(strdup(l)) {
  Assert(test != NULL && label != NULL);
  sprintf(printed, "IfZ %s Goto %s", test->GetName(), label);
}
void IfZ::EmitSpecific(Mips *mips) {	  
  mips->EmitIfZ(test, label);
}
string IfZ::getLabel()
{
    string s = label;
    return s;
}
List<string> IfZ::GenSet()
{
    List<string> set;
    string t;
    t = test->GetName();
    set.Append(t);
    return set;
}

BeginFunc::BeginFunc() {
  sprintf(printed,"BeginFunc (unassigned)");
  frameSize = -555; // used as sentinel to recognized unassigned value
}
void BeginFunc::SetFrameSize(int numBytesForAllLocalsAndTemps) {
  frameSize = numBytesForAllLocalsAndTemps; 
  sprintf(printed,"BeginFunc %d", frameSize);
}
void BeginFunc::EmitSpecific(Mips *mips) {
  mips->EmitBeginFunction(frameSize);
  /* pp5: need to load all parameters to the allocated registers.
   */
}


EndFunc::EndFunc() : Instruction() {
  sprintf(printed, "EndFunc");
}
void EndFunc::EmitSpecific(Mips *mips) {
  mips->EmitEndFunction();
}


 
Return::Return(Location *v) : val(v) {
  sprintf(printed, "Return %s", val? val->GetName() : "");
}
void Return::EmitSpecific(Mips *mips) {	  
  mips->EmitReturn(val);
}
List<string> Return::GenSet()
{
    List<string> set;
    string value;
    if (val)
    {
        value = val->GetName();
        set.Append(value);
    }
    return set;
}


PushParam::PushParam(Location *p)
  :  param(p) {
  Assert(param != NULL);
  sprintf(printed, "PushParam %s", param->GetName());
}
void PushParam::EmitSpecific(Mips *mips) {
  mips->EmitParam(param);
} 
List<string> PushParam::GenSet()
{
    List<string> set;
    string par;
    par = param->GetName();
    set.Append(par);
    return set;
}

PopParams::PopParams(int nb)
  :  numBytes(nb) {
  sprintf(printed, "PopParams %d", numBytes);
}
void PopParams::EmitSpecific(Mips *mips) {
  mips->EmitPopParams(numBytes);
} 




LCall::LCall(const char *l, Location *d)
  :  label(strdup(l)), dst(d) {
  sprintf(printed, "%s%sLCall %s", dst? dst->GetName(): "", dst?" = ":"", label);
}
void LCall::EmitSpecific(Mips *mips) {
  /* pp5: need to save registers before a function call
   * and restore them back after the call.
   */
  mips->EmitLCall(dst, label);
}
List<string> LCall::KillSet()
{
    List<string> set;    
    if (dst)
    {
        string destination;
        destination = dst->GetName();
        set.Append(destination);
    }
    return set;
}

ACall::ACall(Location *ma, Location *d)
  : dst(d), methodAddr(ma) {
  Assert(methodAddr != NULL);
  sprintf(printed, "%s%sACall %s", dst? dst->GetName(): "", dst?" = ":"",
	    methodAddr->GetName());
}
void ACall::EmitSpecific(Mips *mips) {
  /* pp5: need to save registers before a function call
   * and restore them back after the call.
   */
  mips->EmitACall(dst, methodAddr);
} 
List<string> ACall::KillSet()
{
    List<string> set;
    if (dst)
    {
        string destination;
        destination = dst->GetName();
        set.Append(destination);
    }
    return set;
}


VTable::VTable(const char *l, List<const char *> *m)
  : methodLabels(m), label(strdup(l)) {
  Assert(methodLabels != NULL && label != NULL);
  sprintf(printed, "VTable for class %s", l);
}

void VTable::Print() {
  printf("VTable %s =\n", label);
  for (int i = 0; i < methodLabels->NumElements(); i++) 
    printf("\t%s,\n", methodLabels->Nth(i));
  printf("; \n"); 
}
void VTable::EmitSpecific(Mips *mips) {
  mips->EmitVTable(label, methodLabels);
}


