/* File: codegen.cc
 * ----------------
 * Implementation for the CodeGenerator class. The methods don't do anything
 * too fancy, mostly just create objects of the various Tac instruction
 * classes and append them to the list.
 */

#include "codegen.h"
#include <string.h>
#include "tac.h"
#include "mips.h"
#include "ast_decl.h"
#include "errors.h"
#include <vector>
#include <string>
#include <stack>
  
using namespace std;
  
CodeGenerator::CodeGenerator()
{
  code = new List<Instruction*>();
  labels = new unordered_map<string, Instruction*>;
  deletedCode = new vector<Instruction*>;
  interGraph = new List<Location*>();
  curGlobalOffset = 0;
}

void CodeGenerator::createCFG(int begin)
{
    BeginFunc* bf = dynamic_cast<BeginFunc*>(code->Nth(begin));
    Assert(bf); //always start at BeginFunc
    Goto* gt;
    IfZ* iz;
    EndFunc* ef;
    Return* ret;
    /*
    dynamic casts to paste in as needed
    LoadConstant* lc = dynamic_cast<LoadConstant*>(code->Nth(//XXX));
    LoadStringConstant* lcs = dynamic_cast<LoadStringConstant*>(code->Nth(//XXX));
    LoadLabel* ll = dynamic_cast<LoadLabel*>(code->Nth(//XXX));
    Assign* assign = dynamic_cast<Assign*>(code->Nth(//XXX));
    Load* load = dynamic_cast<Load*>(code->Nth(//XXX));
    Store* store = dynamic_cast<Store*>(code->Nth(//XXX));
    BinaryOp* bo = dynamic_cast<BinaryOp*>(code->Nth(//XXX));
    Label* label = dynamic_cast<Label*>(code->Nth(//XXX));
    Goto* gt = dynamic_cast<Goto*>(code->Nth(//XXX));
    IfZ* iz = dynamic_cast<IfZ*>(code->Nth(//XXX));
    EndFunc* ef = dynamic_cast<EndFunc*>(code->Nth(//XXX));
    Return* ret = dynamic_cast<Return*>(code->Nth(//XXX));
    PushParam* pp = dynamic_cast<PushParam*>(code->Nth(//XXX));
    RemoveParams* rp = dynamic_cast<RemoveParams*>(code->Nth(//XXX));
    LCall* lCall = dynamic_cast<LCall*>(code->Nth(//XXX));
    ACall* aCall = dynamic_cast<ACall*>(code->Nth(//XXX));
    VTable* vt = dynamic_cast<VTable*>(code->Nth(//XXX));
    */
    for (int i = begin; i < code->NumElements(); i++)
    {
        //If EndFunc, finished
        ef = dynamic_cast<EndFunc*>(code->Nth(i));
        if (ef)
            break;
            
        //If Goto, find label, add to edges
        gt = dynamic_cast<Goto*>(code->Nth(i));
        if (gt)
        {
            string s = gt->getLabel();
            gt->addEdge((*labels)[s]);
            continue;
        }
        
        //If IfZ, find label, add next instruction and label to edges
        iz = dynamic_cast<IfZ*>(code->Nth(i));
        if (iz)
        {
            string s = iz->getLabel();
            iz->addEdge((*labels)[s]);
            
            iz->addEdge(code->Nth(i+1));
            continue;
        }
        Return* ret = dynamic_cast<Return*>(code->Nth(i));
        if (ret)
        {
            continue;
        }
        code->Nth(i)->addEdge(code->Nth(i+1)); //if instruction doesnt fit any above, add next instruction
    }
    do //when dead code is removed, livenessAnalysis must restart
    {
        livenessAnalysis(begin);
        for (int i = begin; i < code->NumElements(); i++)
        {
            //cout << code->Nth(i)->TACString() << "\n----\n";
            //cout << "OutSet" << endl; 
            for (int j = 0; j < code->Nth(i)->outSet.NumElements(); j++)
            {
                //cout << code->Nth(i)->outSet.Nth(j)->GetName() << ' ';
            } 
            //cout << endl << "KillSet" << endl;
            for (int j = 0; j < code->Nth(i)->KillSet().NumElements(); j++)
            {
                //cout << code->Nth(i)->KillSet().Nth(j)->GetName() << ' ';
            }
            //cout << "\n\n";
        }
    }
    while (deadCodeAnalysis(begin));

    interferenceGraph(begin);

    //cout << "interGraph" << endl;
    for (int i = 0; i < interGraph->NumElements(); i++)
    {
        //cout << "---------- " << interGraph->Nth(i)->GetName() << " --------------" << endl;
        for (int j = 0; j < interGraph->Nth(i)->getNumEdges(); j++)
        {
            //cout << interGraph->Nth(i)->getEdge(j)->GetName() << endl;
        }
    }

    deletedCode->clear();
}

void CodeGenerator::livenessAnalysis(int begin)
{
    Instruction* instruction;
    Instruction* edge;

    BeginFunc* bf = dynamic_cast<BeginFunc*>(code->Nth(begin));
    Assert(bf);
    bool changed = true;
    List<Location*> emptyList;
    for (int i = begin; i < code->NumElements(); i++)
        code->Nth(i)->inSet = emptyList; //inSets must be recomputed every time liveness is called
        
    while (changed)
    {
        changed = false;

        for (int i = begin; i < code->NumElements(); i++) //for each TAC in CFG:
        {
            List<Location*> outSet;
            instruction = code->Nth(i);
            for (int j = 0; j < instruction->getNumEdges(); j++) //Out[TAC] = Union(In[Succ(TAC)])
            {
                edge = instruction->getEdge(j);
                bool deleted = false;
                for (int k = 0; k < deletedCode->size(); k++)
                {
                    if (edge == (*deletedCode)[k])
                    {
                        deleted = true;
                        break;
                    }
                }
                if (deleted)
                    continue;
                    
                for (int k = 0; k < edge->inSet.NumElements(); k++) //Union every elem into outSet
                {
                    Location* elem = edge->inSet.Nth(k);
                    bool found = false;
                    for (int l = 0; l < outSet.NumElements(); l++)  //check to see if elem is already in outSet
                    {
                        if (outSet.Nth(l) == elem)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        outSet.Append(elem); //if elem isn't already in outSet, add it
                }
            }
            instruction->outSet = outSet;
            //In'[TAC] = Out[TAC] - Kill[TAC] + Gen[TAC]
            List<Location*> inPrimeSet = outSet; //= Out[TAC]
            List<Location*> killSet = instruction->KillSet();
            List<Location*> genSet = instruction->GenSet();

            for (int j = 0; j < inPrimeSet.NumElements(); j++) //- Kill[TAC]
            {
                for (int k = 0; k < killSet.NumElements(); k++)
                {
                    if (inPrimeSet.Nth(j) == killSet.Nth(k))
                    {
                        inPrimeSet.RemoveAt(j);
                        j--; //so elements dont get skipped
                        break;
                    }
                }
            }
            for (int j = 0; j < genSet.NumElements(); j++) //+ Gen[TAC]
            {
                bool found = false;
                for (int k = 0; k < inPrimeSet.NumElements(); k++)
                {
                    if (inPrimeSet.Nth(k) == genSet.Nth(j))
                    {
                        found = true;
                        break;
                    }
                }
                
                if (!found)
                    inPrimeSet.Append(genSet.Nth(j));
            }
            
            List<Location*> tempInPrimeSet = inPrimeSet; 
            
            bool foundInOutSet = false;
            bool setsAreDifferent = false;
            for(int j=0; j< instruction->inSet.NumElements(); j++)
            {
            	for(int k=0; k<tempInPrimeSet.NumElements(); k++)
            	{
            		if(instruction->inSet.Nth(j) == tempInPrimeSet.Nth(k))
            		{
            			tempInPrimeSet.RemoveAt(k); 
            			foundInOutSet = true; 
            			break;
            		}
            	}
            	
            	if(foundInOutSet)
            	{
            		foundInOutSet = false; 
            		continue; 
            	}
            	else
            	{
            		setsAreDifferent = true;
            		break;
            	}
            }            
            
            if(setsAreDifferent || tempInPrimeSet.NumElements() != 0)
            {
            	instruction->inSet = inPrimeSet; 
            	changed = true; 
            }
            
        }
    }
}

bool CodeGenerator::deadCodeAnalysis(int begin)
{
    Instruction* instruction;
    bool altered = false;
    for (int i = begin; i < code->NumElements(); i++)
    {
        instruction = code->Nth(i);
        Assert(instruction);
        if (instruction->isDead())
        {
            ////cout << "removing" << endl;
            code->RemoveAt(i);
            ////cout << instruction->TACString() << endl;
            i--; //to prevent skipping instructions
            altered = true;
            deletedCode->push_back(instruction);
        }
        
    }
    return altered;
}

void CodeGenerator::interferenceGraph(int begin)
{
    List<Location*> killSet, inSet, outSet;
    inSet = code->Nth(begin)->inSet;
    for (int i = 0; i < inSet.NumElements(); i++)
    {
        for (int j = i + 1; j < inSet.NumElements(); j++)
        {
            inSet.Nth(i)->addEdge(inSet.Nth(j));
        }
    }
    for (int i = begin + 1; i < code->NumElements(); i++)
    {
        outSet = code->Nth(i)->outSet;
        killSet = code->Nth(i)->KillSet();
        interGraph->AppendAll(outSet);
        interGraph->Unique();
        for (int j = 0; j < killSet.NumElements(); j++)
        {
            for (int k = 0; k < outSet.NumElements(); k++)
            {
                if (outSet.Nth(k) != killSet.Nth(j))
                {
                    // //cout << outSet.Nth(k)->GetName() << " <-> " << killSet.Nth(j)->GetName() << endl;
                    outSet.Nth(k)->addEdge(killSet.Nth(j));
                }
            }
        }
    }
}


void CodeGenerator::kColoring()
{
 	  bool canColor = false; 			// Indicator to keep loop going until coloring found
 	  stack<Location*> degree;			// Stack to keep track of Nodes with degree under K 
 	  List<Location*> removed; 			// Keep track of locations that have been "removed" from interGraph 
 	  while(!canColor)
 	  {
 	  	int temp; 
 	  	while((temp = findNode(removed)) != -1) // degree satisfies 
 	  	{
 	  		Location* satisfies = interGraph->Nth(temp); 	
 	  		removed.Append(satisfies); 					// Add node to removed 
 	  		degree.push(satisfies);  // push node to stack to be setReg later 
 	  	}
 	  	
 	  	if(removed.NumElements() == interGraph->NumElements()) // Means K Colorable .. stack is full of all nodes 
		{
			Location *node = degree.top(); 
			Assert(node);
			degree.pop(); 
			node->SetRegister(Mips::Register(7));	// Set first reg to t0 
			bool foundSameReg = false;  // Indicator for whether found loc with same reg 
			while(!degree.empty())		
			{
				node = degree.top(); 
				degree.pop(); 
				for(int i=8; i<=25; i++) // best way to filter out gp regs? 
				{
					for(int j=0; j<node->getNumEdges(); j++)
					{
						if(node->getEdge(j)->GetRegister() == Mips::Register(i))	// Check with edge nodes if any adjacent have the same reg 
						{
							foundSameReg = true; 
							break; 
						}
					}
					if(foundSameReg)
					{
						foundSameReg = false; 
						continue; 
					}
					else 
					{
						node->SetRegister(Mips::Register(i)); 
					}				
				}
				
				if(node->GetRegister() == Mips::Register(0))
				{
					cout<<"Could not find K coloring which should not be happening. Fuck. "<<endl; 
				}
				else
				{
					cout<<"It Works"<<endl;
				}
			}
			
			canColor = true; 
		
		}
		else // Cant be Kcolored
		{	
			int index = findMaxKNode(removed); 	//Returns largest degree index 
			if(index != -1)
			{
				cout<<"Good index .. spilling "<<interGraph->Nth(index)->GetName()<<endl; 
				removed.Append(interGraph->Nth(index));	// Append to remove list 
			//	Mips mips; 
			//	mips.SpillRegister(interGraph->Nth(index),interGraph->Nth(index)->GetRegister()); 	// Not sure if this is right but spill the largest degreed reg 
			}
			else
			{
				cout<<"Bad Index"<<endl; 	// Checker 
			}
		}
 	 }
}


int CodeGenerator::findNode(List<Location*> removed)
{
	int count = 0; 
	for(int i=0; i<interGraph->NumElements(); i++)
	{
		int numEdges = interGraph->Nth(i)->getNumEdges();
		
		if(wasRemoved(interGraph->Nth(i),removed))      // Has this location been removed already  
			continue;
			 
		if(numEdges < Mips::NumGeneralPurposeRegs) // if it is already below the max num then it can return here 
		{
			return i; 
		}
		else
		{
			for(int j= 0; j<numEdges; j++)	
			{
				if(!wasRemoved(interGraph->Nth(i)->getEdge(j),removed))	// Checks if it was not removed .. if so, add to count 
					count++;
			}
			
			if(count < Mips::NumGeneralPurposeRegs)	// Check count
				return i;
		}
	}
	
	return -1; 
}

int CodeGenerator::findMaxKNode(List<Location*> removed) //Like find node but for max node 
{
	int max = -1;
	int count = 0;
	int index = -1;  
	for(int i=0; i<interGraph->NumElements(); i++)
	{
		for(int j=0; j<interGraph->Nth(i)->getNumEdges(); j++)
		{
			if(wasRemoved(interGraph->Nth(i)->getEdge(j),removed))
				continue; 
			else
				count ++; 
		}
		
		if(count > max)
		{
			max = count; 
			index = i; 
		}	
	}

	return index; 
}

bool CodeGenerator::wasRemoved(Location* check,List<Location*> removed)	// Checks to see if location exists in removed list 
{
	for(int i=0; i<removed.NumElements() ; i++)
	{
		if(check == removed.Nth(i))
		{
			return true;
		}
	}
	
	return false; 
}

char *CodeGenerator::NewLabel()
{
  static int nextLabelNum = 0;
  char temp[10];
  sprintf(temp, "_L%d", nextLabelNum++);
  return strdup(temp);
}


Location *CodeGenerator::GenTempVar()
{
  static int nextTempNum;
  char temp[10];
  Location *result = NULL;
  sprintf(temp, "_tmp%d", nextTempNum++);
  return GenLocalVariable(temp);
}

  
Location *CodeGenerator::GenLocalVariable(const char *varName)
{            
    curStackOffset -= VarSize;
    return new Location(fpRelative, curStackOffset+4,  varName);
}

Location *CodeGenerator::GenGlobalVariable(const char *varName)
{
    curGlobalOffset += VarSize;
    return new Location(gpRelative, curGlobalOffset -4, varName);
}


Location *CodeGenerator::GenLoadConstant(int value)
{
  Location *result = GenTempVar();
  code->Append(new LoadConstant(result, value));
  return result;
}

Location *CodeGenerator::GenLoadConstant(const char *s)
{
  Location *result = GenTempVar();
  code->Append(new LoadStringConstant(result, s));
  return result;
} 

Location *CodeGenerator::GenLoadLabel(const char *label)
{
  Location *result = GenTempVar();
  code->Append(new LoadLabel(result, label));
  return result;
} 


void CodeGenerator::GenAssign(Location *dst, Location *src)
{
  code->Append(new Assign(dst, src));
}


Location *CodeGenerator::GenLoad(Location *ref, int offset)
{
  Location *result = GenTempVar();
  code->Append(new Load(result, ref, offset));
  return result;
}

void CodeGenerator::GenStore(Location *dst,Location *src, int offset)
{
  code->Append(new Store(dst, src, offset));
}


Location *CodeGenerator::GenBinaryOp(const char *opName, Location *op1,
						     Location *op2)
{
  Location *result = GenTempVar();
  code->Append(new BinaryOp(BinaryOp::OpCodeForName(opName), result, op1, op2));
  return result;
}


void CodeGenerator::GenLabel(const char *label)
{
  Instruction* instruction = new Label(label);
  code->Append(instruction);
  string s = label;
  (*labels)[s] = instruction;
}

void CodeGenerator::GenIfZ(Location *test, const char *label)
{
  code->Append(new IfZ(test, label));
}

void CodeGenerator::GenGoto(const char *label)
{
  code->Append(new Goto(label));
}

void CodeGenerator::GenReturn(Location *val)
{
  code->Append(new Return(val));
}


BeginFunc *CodeGenerator::GenBeginFunc(FnDecl *fn)
{
  BeginFunc *result = new BeginFunc;
  code->Append(insideFn = result);
  List<VarDecl*> *formals = fn->GetFormals();
  int start = OffsetToFirstParam;
  if (fn->IsMethodDecl()) start += VarSize;
  for (int i = 0; i < formals->NumElements(); i++)
    formals->Nth(i)->rtLoc = new Location(fpRelative, i*VarSize + start, formals->Nth(i)->GetName());
  curStackOffset = OffsetToFirstLocal;
  return result;
}

void CodeGenerator::GenEndFunc()
{
  code->Append(new EndFunc());
  insideFn->SetFrameSize(OffsetToFirstLocal-curStackOffset);
  insideFn = NULL;
}

void CodeGenerator::GenPushParam(Location *param)
{
  code->Append(new PushParam(param));
}

void CodeGenerator::GenPopParams(int numBytesOfParams)
{
  Assert(numBytesOfParams >= 0 && numBytesOfParams % VarSize == 0); // sanity check
  if (numBytesOfParams > 0)
    code->Append(new PopParams(numBytesOfParams));
}

Location *CodeGenerator::GenLCall(const char *label, bool fnHasReturnValue)
{
  Location *result = fnHasReturnValue ? GenTempVar() : NULL;
  code->Append(new LCall(label, result));
  return result;
}
  
Location *CodeGenerator::GenFunctionCall(const char *fnLabel, List<Location*> *args, bool hasReturnValue)
{
  for (int i = args->NumElements()-1; i >= 0; i--) // push params right to left
    GenPushParam(args->Nth(i));
  Location *result = GenLCall(fnLabel, hasReturnValue);
  GenPopParams(args->NumElements()*VarSize);
  return result;
}

Location *CodeGenerator::GenACall(Location *fnAddr, bool fnHasReturnValue)
{
  Location *result = fnHasReturnValue ? GenTempVar() : NULL;
  code->Append(new ACall(fnAddr, result));
  return result;
}
  
Location *CodeGenerator::GenMethodCall(Location *rcvr,
			     Location *meth, List<Location*> *args, bool fnHasReturnValue)
{
  for (int i = args->NumElements()-1; i >= 0; i--)
    GenPushParam(args->Nth(i));
  GenPushParam(rcvr);	// hidden "this" parameter
  Location *result= GenACall(meth, fnHasReturnValue);
  GenPopParams((args->NumElements()+1)*VarSize);
  return result;
}
 
 
static struct _builtin {
  const char *label;
  int numArgs;
  bool hasReturn;
} builtins[] =
 {{"_Alloc", 1, true},
  {"_ReadLine", 0, true},
  {"_ReadInteger", 0, true},
  {"_StringEqual", 2, true},
  {"_PrintInt", 1, false},
  {"_PrintString", 1, false},
  {"_PrintBool", 1, false},
  {"_Halt", 0, false}};

Location *CodeGenerator::GenBuiltInCall(BuiltIn bn,Location *arg1, Location *arg2)
{
  Assert(bn >= 0 && bn < NumBuiltIns);
  struct _builtin *b = &builtins[bn];
  Location *result = NULL;

  if (b->hasReturn) result = GenTempVar();
                // verify appropriate number of non-NULL arguments given
  Assert((b->numArgs == 0 && !arg1 && !arg2)
	|| (b->numArgs == 1 && arg1 && !arg2)
	|| (b->numArgs == 2 && arg1 && arg2));
  if (arg2) code->Append(new PushParam(arg2));
  if (arg1) code->Append(new PushParam(arg1));
  code->Append(new LCall(b->label, result));
  GenPopParams(VarSize*b->numArgs);
  return result;
}


void CodeGenerator::GenVTable(const char *className, List<const char *> *methodLabels)
{
  code->Append(new VTable(className, methodLabels));
}


void CodeGenerator::DoFinalCodeGen()
{
  if (IsDebugOn("tac")) { // if debug don't translate to mips, just print Tac
    for (int i = 0; i < code->NumElements(); i++)
	code->Nth(i)->Print();
   }  else {
     Mips mips;
     mips.EmitPreamble();
     for (int i = 0; i < code->NumElements(); i++)
	 code->Nth(i)->Emit(&mips);
  }
}



Location *CodeGenerator::GenArrayLen(Location *array)
{
  return GenLoad(array, -4);
}

Location *CodeGenerator::GenNew(const char *vTableLabel, int instanceSize)
{
  Location *size = GenLoadConstant(instanceSize);
  Location *result = GenBuiltInCall(Alloc, size);
  Location *vt = GenLoadLabel(vTableLabel);
  GenStore(result, vt);
  return result;
}


Location *CodeGenerator::GenDynamicDispatch(Location *rcvr, int vtableOffset, List<Location*> *args, bool hasReturnValue)
{
  Location *vptr = GenLoad(rcvr); // load vptr
  Assert(vtableOffset >= 0);
  Location *m = GenLoad(vptr, vtableOffset*4);
  return GenMethodCall(rcvr, m, args, hasReturnValue);
}

// all variables (ints, bools, ptrs, arrays) are 4 bytes in for code generation
// so this simplifies the math for offsets
Location *CodeGenerator::GenSubscript(Location *array, Location *index)
{
  Location *zero = GenLoadConstant(0);
  Location *isNegative = GenBinaryOp("<", index, zero);
  Location *count = GenLoad(array, -4);
  Location *isWithinRange = GenBinaryOp("<", index, count);
  Location *pastEnd = GenBinaryOp("==", isWithinRange, zero);
  Location *outOfRange = GenBinaryOp("||", isNegative, pastEnd);
  const char *pastError = NewLabel();
  GenIfZ(outOfRange, pastError);
  GenHaltWithMessage(err_arr_out_of_bounds);
  GenLabel(pastError);
  Location *four = GenLoadConstant(VarSize);
  Location *offset = GenBinaryOp("*", four, index);
  Location *elem = GenBinaryOp("+", array, offset);
  return new Location(elem, 0); 
}



Location *CodeGenerator::GenNewArray(Location *numElems)
{
  Location *one = GenLoadConstant(1);
  Location *isNonpositive = GenBinaryOp("<", numElems, one);
  const char *pastError = NewLabel();
  GenIfZ(isNonpositive, pastError);
  GenHaltWithMessage(err_arr_bad_size);
  GenLabel(pastError);

  // need (numElems+1)*VarSize total bytes (extra 1 is for length)
  Location *arraySize = GenLoadConstant(1);
  Location *num = GenBinaryOp("+", arraySize, numElems);
  Location *four = GenLoadConstant(VarSize);
  Location *bytes = GenBinaryOp("*", num, four);
  Location *result = GenBuiltInCall(Alloc, bytes);
  GenStore(result, numElems);
  return GenBinaryOp("+", result, four);
}

void CodeGenerator::GenHaltWithMessage(const char *message)
{
   Location *msg = GenLoadConstant(message);
   GenBuiltInCall(PrintString, msg);
   GenBuiltInCall(Halt, NULL);
}
