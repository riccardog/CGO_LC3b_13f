//===- ConditionalConstantPropagation.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "ConditionalConstantPropagation World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/GraphTraits.h"
#include "llvm/Support/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <map>
#include <set>
#include <queue>

using namespace llvm;


namespace {
  // ConditionalConstantPropagation - The first implementation, without getAnalysisUsage.
	struct Lattice
{
	bool TOP;
	bool BOT;
	int64_t constant;
};    
	struct ConditionalConstantPropagation : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    ConditionalConstantPropagation() : FunctionPass(ID) {}

	void ExampleCode ( raw_ostream& O, Function & F);

	void RunCCP( Function & F);

	Lattice Evaluate( Instruction * instr);

	bool LatEquals(Lattice left, Lattice right);
	
	Lattice MeetFunc( Lattice left, Lattice right);

    virtual bool runOnFunction(Function &F) {
      errs() << "ConditionalConstantPropagation: ";
      errs().write_escaped(F.getName()) << '\n';
	  //ExampleCode(errs(), F);
	  RunCCP(F);
      return false;
    }

  };
}

char ConditionalConstantPropagation::ID = 0;
static RegisterPass<ConditionalConstantPropagation> X("conditional-constant-propagation", "Data-flow analysis: liveness analysis pass");




//declaration
static std::map<Instruction*, Lattice*> LatticeMap;
static std::queue<Instruction*> SSAWorklist;
static std::queue<BasicBlock*> CFGWorklist;
static std::map<StringRef, int> ExecutableMap;
static std::map<StringRef, int> ExecutableMax;

void ConditionalConstantPropagation::ExampleCode(raw_ostream& O, Function& F) {

	std::set<StringRef> var_set;
	var_set.clear();
	BasicBlock* bb_entry 	= NULL;
	BasicBlock* bb_if_then 	= NULL;
	BasicBlock* bb_if_else 	= NULL;
	BasicBlock* bb_if_end  	= NULL;


	// TA: The fol lowing code shows how you iterate over variables 
	// defined in this function
	O << "All Variables Defined in Function " << F.getName() <<   "\n";
	for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
		BasicBlock* BB = dyn_cast<BasicBlock>(&*FI);
		for (BasicBlock::iterator BI = BB->begin(), BE = BB->end();
			BI != BE; ++BI) {
			Instruction *instr = dyn_cast<Instruction>(&*BI);
			LatticeMap[instr] = new Lattice();
			LatticeMap[instr]->TOP = true;
			LatticeMap[instr]->BOT = false;
			if ( BI->hasName() ) 
			{
				StringRef var_name = BI->getValueName()->first();
				O << "VARIABLE:\t" << var_name << "\tdefined in ";
				BI->print(O);
				O << "\n";
			}
		}
		if ( !bb_entry )
			bb_entry = BB;
		else if ( !bb_if_then )
			bb_if_then = BB;
		else if ( !bb_if_else )
			bb_if_else = BB;
		else if ( !bb_if_end )
			bb_if_end = BB;
	}

/*

//intialization
CFGWorklist.push(bb_entry);  //need to initialize bb_entry as the first node, caution: I do not know what entry node contains
for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) 
{
	BasicBlock* BB = dyn_cast<BasicBlock>(&*FI);
	ExecutableMap[BB->getName()] = 0;
	int count = 0;	
	for (pred_iterator PI = pred_begin(BB), E = pred_end(BB); PI != E; ++PI) 
	{
	  count++;
	}
	ExecutableMax[BB->getName()] = count;	
}
//initialize the VALUE OBJECTS (THIS SHOULD ALREADY BE DONE ABOVE IN EXAMPLE CODE)



while (CFGWorklist.size()!=0 || SSAWorklist.size()!=0) 
{
	while(CFGWorklist.size()!=0)
	{
		BasicBlock* BB = CFGWorklist.front();
		CFGWorklist.pop();
		if((ExecutableMap[BB->getName()]==0) || (ExecutableMap[BB->getName()] < ExecutableMax[BB->getName()] ))
		{
			ExecutableMap[BB->getName()]+=1;
			//evaluate each phi-function in BB <----- can this be done in Evaluate()?
			for (BasicBlock::iterator BI = BB->begin(), BE = BB->end();BI != BE; ++BI) 
			{
				//evaluate BI (and update lattice?) <------
	 			Instruction* definition = dyn_cast<Instruction> (&*BI);
				O<< "reached before evaluate" << *definition  << BB->getName()<<"\n";
				O << "\ninst lattice of:" << *definition << "\n \t\t--TOP ::" << LatticeMap[definition]->TOP <<" \t " << "BOT::"<< LatticeMap[definition]->BOT <<" \t " << "constant:: " << LatticeMap[definition]->constant <<" \n\n "  ;
				*LatticeMap[definition] = Evaluate(O,definition);

				O<< "reached after evaluate" << *definition  << BB->getName()<<"\n";
				O << "\ninst lattice of:" << *definition << "\n \t\t--TOP ::" << LatticeMap[definition]->TOP <<" \t " << "BOT::"<< LatticeMap[definition]->BOT <<" \t " << "constant:: " << LatticeMap[definition]->constant <<" \n\n "  ;					
				for (Instruction::use_iterator UI= definition->use_begin(), UE=definition->use_end(); UI!=UE; ++UI) 
				{
					Instruction* usage = dyn_cast<Instruction> (*UI);
					///NOTE, WE CANNOT DO &* HERE LIKE FOR 'definition' ABOVE.  DOES THIS CAUSE PROBLEMS??
					BasicBlock* parent = usage->getParent();
					if (ExecutableMap[parent->getName()] > 0)
					{
						SSAWorklist.push(usage);
					}
					//DEBUGGING CODE IS COMMENTED OUT BELOW:
			
					//if ( UI->hasName() ) 
					//{							
						//StringRef var_name = BI->getValueName()->first();
						//O << "USE :::" << UI->getValueName()->first()<<"  " << "VARIABLE:\t" << var_name<<"\tdefined in ";
						//UI->print(O);
						//O << "\n";
					//}
				}
			}				
		}
	}
	while(SSAWorklist.size()!=0) 
	{
		Instruction* instr = SSAWorklist.front();
		SSAWorklist.pop();
		O << "SSA before evaluate" << *instr << "\n";
		O << "\ninst lattice of:" << *instr << "\n \t\t--TOP ::" << LatticeMap[instr]->TOP <<" \t " << "BOT::"<< LatticeMap[instr]->BOT <<" \t " << "constant:: " << LatticeMap[instr]->constant <<" \n\n "  ;			
		Lattice t = Evaluate(O,instr);// ********** need to make evaluate function
		O << "SSA after evaluate" << *instr << "\n";
		O << "\ninst lattice of:" << *instr << "\n \t\t--TOP ::" << LatticeMap[instr]->TOP <<" \t " << "BOT::"<< LatticeMap[instr]->BOT <<" \t " << "constant:: " << LatticeMap[instr]->constant <<" \n\n "  ;					
		if (!LatEquals(t,*LatticeMap[instr])) //do get equals function
		{
			//O << "\ninst values change from:" << *instr << "\n \t\t--TOP ::" << LatticeMap[instr]->TOP <<" \t " << "BOT::"<< LatticeMap[instr]->BOT <<" \t " << "constant:: " << LatticeMap[instr]->constant <<" \n\n "  ;
			*LatticeMap[instr] = MeetFunc(*LatticeMap[instr],t);
			//O << "\ninst values change2:" << *instr << "\n \t\t--TOP ::" << LatticeMap[instr]->TOP <<" \t " << "BOT::"<< LatticeMap[instr]->BOT <<" \t " << "constant:: " << LatticeMap[instr]->constant <<" \n\n "  ; 
			for (Instruction::use_iterator UI= instr->use_begin(), UE=instr->use_end(); UI!=UE; ++UI) 
			{
				Instruction* usage = dyn_cast<Instruction> (*UI);
				BasicBlock* parent = usage->getParent();
				if (ExecutableMap[parent->getName()] > 0)
				{
					SSAWorklist.push(usage);
				}
			}
		}
	}
}		

for (std::map<Instruction*, Lattice*> ::iterator it = LatticeMap.begin(); it!= LatticeMap.end(); ++it) {
	O << "\ninst ::" << *(it->first) << "\n \t\t--TOP ::" << it->second->TOP <<" \t " << "BOT::"<<it->second->BOT <<" \t " << "constant:: " << it->second->constant <<" \n\n "  ;
}
/*
for (std::map<Instruction*, Lattice*> ::iterator it = LatticeMap.begin(); it!= LatticeMap.end(); ++it) {

	Instruction * instr = dyn_cast<Instruction> (&*(it->first));
	O << *instr << "\n";
	if ( !(LatticeMap[instr]->TOP || LatticeMap[instr]->BOT) )
	{
		BasicBlock::iterator ii(instr);
		if( ICmpInst * icmpinst = dyn_cast<ICmpInst>(&*instr) ) 
		{
			int64_t taken = LatticeMap[instr]->constant;
			Constant *x = ConstantInt::get(instr->getType(), 0x1);
			ConstantInt *ret1 = dyn_cast<ConstantInt>(&*x);
			x = ConstantInt::get(instr->getType(), 0x0);
			ConstantInt *ret0 =dyn_cast<ConstantInt>(&*x);
			if(taken == 0)
			{
				llvm::ReplaceInstWithValue(instr->getParent()->getInstList(),ii,ret0);			
			}
			else
			{
				llvm::ReplaceInstWithValue(instr->getParent()->getInstList(),ii,ret1);
			}
		}
		else if( isa<PHINode>(instr) )
		{
			int64_t lat_constant = LatticeMap[instr]->constant;
			// convert lat_constant to ConstantInt
			Constant * newConst = ConstantInt::getSigned(instr->getType(),lat_constant);
			if (ConstantInt * ptr = dyn_cast<ConstantInt>(newConst)) 
			{
				ReplaceInstWithValue(instr->getParent()->getInstList(), ii, ptr);
			}
		}
		else if ( BranchInst *BI = dyn_cast<BranchInst>(instr))
		{
			int64_t taken = LatticeMap[instr]->constant;
			if(taken == 0)
			{
				O << *instr << " replacing with false value\n";
				//BranchInst * new_branch = BranchInst::Create(BI->getSuccessor(0),BI->getSuccessor(1),cast<Value>(&*ConstantInt::getFalse(BI->getContext())));
				BranchInst * new_branch = BranchInst::Create(BI->getSuccessor(1));
				O << *new_branch << " <- false value\n";
				ReplaceInstWithInst(BI->getParent()->getInstList(), ii, cast<Instruction>(&*new_branch));
			}
			else
			{
				O << *instr << " replacing with true value\n";
				BranchInst * new_branch = BranchInst::Create(BI->getSuccessor(0));
				O << *new_branch << " <- true value\n";
				ReplaceInstWithInst(BI->getParent()->getInstList(), ii, cast<Instruction>(&*new_branch));
			}			
		}
		else if ( instr->getOpcode() == Instruction::Sub || instr->getOpcode() == Instruction::Add)  
		{
			int64_t lat_constant = LatticeMap[instr]->constant;
			// convert lat_constant to ConstantInt
			Constant * newConst = ConstantInt::getSigned(instr->getType(),lat_constant);
			if (ConstantInt * ptr = dyn_cast<ConstantInt>(newConst)) 
			{
				ReplaceInstWithValue(instr->getParent()->getInstList(), ii, ptr);
			}
		}
	}
}


/*my propagation code:
for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) 
{
	BasicBlock* BB = dyn_cast<BasicBlock>(&*FI);
	for (BasicBlock::iterator BI = BB->begin(), BE = BB->end(); BI != BE; ++BI) 
	{
		
	}
}*/
/*
after running the above code 
for(each value in "Value" map)
{
	if( value is not TOP or BOT)
	{
		set original instruction RHS equal to value
	}
}



run simplify cfg to eliminate deadcode (values with TOP)
*/
/*
	//TA: The following code examines the last instruction of 
	// each basic block, and print if the last instruction is 
	// condtional or unconditional branch
	for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
		BasicBlock* BB = dyn_cast<BasicBlock>(&*FI);
		O << "----------------------------------------------------\n";
		O << "BB: " << BB ->getName() << '\n';
		TerminatorInst *TI = BB->getTerminator();
		O << "\tTerminatorInst: " << *TI << '\n';
		if (BranchInst *BI = dyn_cast<BranchInst>(TI)) {
			if (BI->isUnconditional())
				O << "\tInstruction is uncondtional\n";
			else  {
				O << "\tInstruction is conditional\n";
				Value * condition = BI->getCondition();
				O << "\t\tcondtion is " << *condition << "\n";
			}
		}
	}
	
	// TA: the following code shows how to perform the symbolic execution 
	for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) 
	{
		BasicBlock* BB = dyn_cast<BasicBlock>(&*FI);
		for (BasicBlock::iterator BI = BB->begin(), BE = BB->end();
			BI != BE; ++BI) {
			if ( BI->getOpcode() == Instruction::Sub)  
			{
				O << "Sub Instr: " << *BI << "\n";
				unsigned operand_size = BI->getNumOperands();
				assert ( operand_size == 2 );
				if ( dyn_cast<ConstantInt>(&*BI->getOperand(0)) && dyn_cast<ConstantInt>(&*BI->getOperand(1)) ) 
				{
					Constant * expr_result = ConstantExpr::getSub(dyn_cast<Constant>(&*BI->getOperand(0)), dyn_cast<Constant>(&*BI->getOperand(1)) );
					if ( ConstantInt * int_result = dyn_cast<ConstantInt>(&*expr_result) ) {
						O << "Both operands are constant, result is: " << int_result->getValue() << "\n"; 
						// replace instruction using 
						// ReplaceInstWithValue
						//BasicBlock::iterator ii(instToReplace);
						ReplaceInstWithValue(BI->getParent()->getInstList(), BI, int_result );
					}
				}
			}
		}
	}

	// TA: the following code shows how to change the IF condition
	// when it always executes false path
	TerminatorInst *TI; 
	TI = bb_entry->getTerminator();
	O << "TerminatorInst in Entry Block: " << *TI;
	if (BranchInst *BI = dyn_cast<BranchInst>(TI)) {
		if (BI->isUnconditional())
			O << " (Uncondtional)\n";
		else if (BI->isConditional())
			O << " (Condtional)\n";
	}
	BasicBlock::iterator BI = bb_entry->begin();
	if ( ICmpInst * icmpinst = dyn_cast<ICmpInst>(&*BI) ) {
		O << " ICmpInst " << *icmpinst << "\n";
		if ( icmpinst->isSigned() ) {
			if (icmpinst->getSignedPredicate () == CmpInst::ICMP_SGT ) {
				O << icmpinst->getSignedPredicate ()  << ": int signed greater than\n";
				Value * left_hand_side  = icmpinst->getOperand(0);
				Value * right_hand_side = icmpinst->getOperand(1);
				if ( dyn_cast<ConstantInt> (&*left_hand_side) && dyn_cast<ConstantInt>(&*right_hand_side)) {
					ConstantInt * left_const  = dyn_cast<ConstantInt> (&*left_hand_side);
					ConstantInt * right_const = dyn_cast<ConstantInt> (&*right_hand_side);   
					O << "LHS is constant: " << *left_const << ", value: " << left_const->getValue() << "\n";
					O << "RHS is constant: " << *right_const<< ", value: " << right_const->getValue() << "\n";
					int64_t left_value = left_const->getValue().getSExtValue();
					int64_t right_value = right_const->getValue().getSExtValue();
					if ( left_value > right_value ) {
						O << "this if statement will always be true\n";
					} else {
						O << "this if statement will always be false\n";
						// if this is the case, we need to replace the terminator instruction (condtional branch ) with new one
						BasicBlock::iterator ii(TI);
						BranchInst * new_branch = BranchInst::Create(TI->getSuccessor(0),TI->getSuccessor(1),cast<Value>(&*ConstantInt::getFalse(TI->getContext())));
						//TI->getParent());
						ReplaceInstWithInst(TI->getParent()->getInstList(), ii, cast<Instruction>(&*new_branch));
					}
				}
			}
		}
	}*/
}

//---------------------------EVALUATE INSTRUCTION FUNCTION-----------------------------------------
//
// This function takes an instruction pointer as an argument and evaluates the next
// lattice values.  It returns the newly evaluated lattice.
//
// There are 4 kinds of instructions to consider:
//
//	1. Branches:
//		a. Unconditional (jump) -> This sets lattice to BOT and pushes next BB onto 
//			the CFGWorklist
//		b. Conditional (branch) -> Checks the condition, sets lattice accordingly
//			and pushes one or both BB's onto the worklist
//	
//	2. Compare Instructions: sets lattice to TOP, BOT, or a constan 0 or 1 (untaken or taken)
//
//	3. PHI node: performs a meet function on all operands and returns the result lattice
//
//	4. Add or Sub instruction: evaluates the expression and or performs a meet if an operand
//		is TOP or BOT.
//
//---------------------------------------------------------------------------------------------------
 
Lattice ConditionalConstantPropagation::Evaluate(Instruction* instr)
{
	Lattice tempLat;
	tempLat = *LatticeMap[instr]; //double check
	if (BranchInst *BI = dyn_cast<BranchInst>(instr)) 
	{
		//JUMP INSTRUCTION
		if (BI->isUnconditional())
		{
			//To evaluate a jump place it's target on CFGWorkList
			tempLat.TOP=false;			
			tempLat.BOT=true;
			CFGWorklist.push(BI->getSuccessor(0));
			return tempLat;			
		}
		//BRANCH INSTRUCTION
		else  
		{
			Value * condition = BI->getCondition();
			Instruction * branch_condition = dyn_cast<Instruction>(&*condition);
			tempLat = *LatticeMap[branch_condition];
			if(LatticeMap[branch_condition]->BOT || LatticeMap[branch_condition]->TOP)
			{
				CFGWorklist.push(BI->getSuccessor(0));
				CFGWorklist.push(BI->getSuccessor(1));
				return tempLat;				
			}
			else if(LatticeMap[branch_condition]->constant == 1)
			{
				CFGWorklist.push(BI->getSuccessor(0));
				return tempLat;
			}
			else if(LatticeMap[branch_condition]->constant == 0)
			{
				CFGWorklist.push(BI->getSuccessor(1));
				return tempLat;
			}
			else{
				//this should never be reached!!!!!
			}
			
		}
	}else if (ICmpInst * icmpinst = dyn_cast<ICmpInst>(instr)) //COMPARE INSTRUCITON
	{
			//lattice for right operand (declaration)
			Lattice rightLat;
			//lattice for left operand (declaration)
			Lattice leftLat;
			rightLat.TOP = false;
			rightLat.BOT = false;
			leftLat.TOP = false;
			leftLat.BOT = false;
		
			Value *left_op  = icmpinst->getOperand(0);
			Value *right_op = icmpinst->getOperand(1);
		
			int64_t left_value  = 0;
			int64_t right_value = 0;

			//check if both operands are constant
			if (dyn_cast<ConstantInt> (&*left_op) && dyn_cast<ConstantInt>(&*right_op)) {

				ConstantInt *left_const  = dyn_cast<ConstantInt>(&*left_op);
				ConstantInt *right_const = dyn_cast<ConstantInt>(&*right_op); 
			
				if (icmpinst->isSigned()) 
				{
					left_value  = left_const->getValue().getSExtValue();
					right_value = right_const->getValue().getSExtValue();
				} else {
					left_value  = left_const->getValue().getZExtValue();
					right_value = right_const->getValue().getZExtValue();
				}
			}
			else if(dyn_cast<ConstantInt> (&*left_op))
			{
				//Right instruction is variable
				ConstantInt *left_const  = dyn_cast<ConstantInt>(&*left_op);
				Instruction * right_instr = dyn_cast<Instruction>(&*right_op);
				rightLat = *LatticeMap[right_instr];
				if (icmpinst->isSigned()) 
				{
					left_value  = left_const->getValue().getSExtValue();
					right_value = rightLat.constant; 
				} else {
					left_value  = left_const->getValue().getZExtValue();
					right_value = rightLat.constant; 
				}
			}
			else if(dyn_cast<ConstantInt> (&*right_op))
			{
				//left instruction is variable
				Instruction * left_instr = dyn_cast<Instruction>(&*left_op);
				leftLat = *LatticeMap[left_instr];
				ConstantInt *right_const = dyn_cast<ConstantInt>(&*right_op); 
				if (icmpinst->isSigned()) 
				{
					left_value  = leftLat.constant; 
					right_value = right_const->getValue().getSExtValue(); 
				} else {
					left_value  = leftLat.constant; 
					right_value = right_const->getValue().getZExtValue();  
				}
			}
			else
			{
				//both instructions are variable
				Instruction * left_instr = dyn_cast<Instruction>(&*left_op);
				leftLat = *LatticeMap[left_instr];
				Instruction * right_instr = dyn_cast<Instruction>(&*right_op);
				rightLat = *LatticeMap[right_instr];
				if (icmpinst->isSigned()) 
				{
					left_value  = leftLat.constant; 
					right_value = rightLat.constant;
				} else {
					left_value  = leftLat.constant; 
					right_value = rightLat.constant;}				
			}
			
			//performing the correct comparison
			int64_t resultValue = 0;
			if (icmpinst->getSignedPredicate() == CmpInst::ICMP_SGT) 
			{ 
				// signed >
				resultValue = left_value > right_value;
			} 
			else if (icmpinst->getPredicate() == CmpInst::ICMP_UGT) 
			{ 
				// unsigned >
				resultValue = left_value > right_value;
			} 
			else if (icmpinst->getSignedPredicate() == CmpInst::ICMP_SGE) 
			{ 
				// signed >=
				resultValue = left_value >= right_value;
			} 
			else if (icmpinst->getPredicate() == CmpInst::ICMP_UGE) 
			{ 
				// unsigned >=	
				resultValue = left_value >= right_value;
			} 
			else if (icmpinst->getSignedPredicate() == CmpInst::ICMP_SLT) 
			{ 
				// signed <
				resultValue = left_value < right_value;
			} 
			else if (icmpinst->getPredicate() == CmpInst::ICMP_ULT) 
			{ 
				// unsigned <
				resultValue = left_value < right_value;
			} 
			else if (icmpinst->getSignedPredicate() == CmpInst::ICMP_SLE) 
			{ 
				// signed <=
				resultValue = left_value <= right_value;
			} 
			else if (icmpinst->getPredicate() == CmpInst::ICMP_ULE) 
			{ 
				// unsigned <=
				resultValue = left_value <= right_value;
			} 
			else if (icmpinst->getSignedPredicate() == CmpInst::ICMP_EQ) 
			{ 
				// =
				resultValue = left_value == right_value;
			} 
			else if (icmpinst->getSignedPredicate() == CmpInst::ICMP_NE) 
			{ 
				// !=
				resultValue = left_value != right_value;
			}
			

			//modify the result based on TOP and BOT values to do expression meet function:
			if(leftLat.BOT || rightLat.BOT)
			{
				tempLat.BOT = true;
				tempLat.TOP = false;
				return tempLat;
			}
			else if(leftLat.TOP)
			{
				tempLat = rightLat; 
				return tempLat;	
			}
			else if(rightLat.TOP)
			{
				tempLat = leftLat;
				return tempLat;
			}
			else if(resultValue)
			{
				//branch taken
				tempLat.BOT = false;
				tempLat.TOP = false;
				tempLat.constant = 1; //for branch, one means taken
				return tempLat;
			}
			else
			{
				//branch untaken
				tempLat.BOT = false;
				tempLat.TOP = false;
				tempLat.constant = 0; //for branch, zero means untaken
				return tempLat;
			}	
	} else 
	{	
		//if phi node then 
		if(isa<PHINode>(instr))
		{
			Lattice currentLat;
			currentLat.TOP = true;
			currentLat.BOT = false;
			PHINode *phi = dyn_cast<llvm::PHINode>(&*instr);
			//iterate through every operand in a phi node function
			//theoretically phi node can have many values

			for(unsigned int x = 0; x < phi->getNumIncomingValues(); x++)
			{
				Value* current_op = phi->getIncomingValue(x);
				if(UndefValue * undef = dyn_cast<UndefValue>(&*current_op))
				{
					//skip 'undef' values
					// this happens in PHI nodes for loops with variables not yet initialized
					continue;
				}
				if(ConstantInt * current_const = dyn_cast<ConstantInt>(&*current_op))
				{
					StringRef BBname = phi->getIncomingBlock(x)->getName();
					if(ExecutableMap[BBname] > 0)
					{
						Lattice constant_Lattice;
						constant_Lattice.TOP = false;
						constant_Lattice.BOT = false;
						constant_Lattice.constant = current_const->getValue().getSExtValue();
						currentLat = MeetFunc(currentLat, constant_Lattice);
					}				
				}
				else
				{
					StringRef BBname = phi->getIncomingBlock(x)->getName();
					if(ExecutableMap[BBname] > 0)
					{
						Instruction * current_instr = dyn_cast<Instruction>(&*current_op);
						currentLat = MeetFunc(currentLat,*LatticeMap[current_instr]);
					}
				}
			}
			tempLat = currentLat;
			return tempLat;
		}
		//else it is a regular instruction:
		else if ( instr->getOpcode() == Instruction::Sub || instr->getOpcode() == Instruction::Add)  
		{
			bool subOrAdd = (instr->getOpcode() == Instruction::Add);
			if ( dyn_cast<ConstantInt>(&*instr->getOperand(0)) && dyn_cast<ConstantInt>(&*instr->getOperand(1)) ) 
			{
				
				Constant * expr_result;
				if(subOrAdd == false)
				{
					expr_result = ConstantExpr::getSub(dyn_cast<Constant>(&*instr->getOperand(0)), dyn_cast<Constant>(&*instr->getOperand(1)) );
				}else if(subOrAdd == true)
				{
					expr_result = ConstantExpr::getAdd(dyn_cast<Constant>(&*instr->getOperand(0)), dyn_cast<Constant>(&*instr->getOperand(1)) );
				}
				if ( ConstantInt * int_result = dyn_cast<ConstantInt>(&*expr_result) ) 
				{
					tempLat.TOP = false;
					tempLat.BOT = false;
					tempLat.constant = int_result->getValue().getSExtValue();
					return tempLat; 				}
			}
			else
			{
				if(ConstantInt * left_const = dyn_cast<ConstantInt>(&*instr->getOperand(0)))
				{
								
					//left operand is constant, right is variable
					Instruction * right_instr = dyn_cast<Instruction>(&*instr->getOperand(1));
					Lattice left_lat;
					left_lat.TOP = false;
					left_lat.BOT = false;
					left_lat.constant = left_const->getValue().getSExtValue();
					
					Lattice right_lat = *LatticeMap[right_instr];
					if(right_lat.TOP == false && right_lat.BOT == false)
					{
						tempLat.TOP = false;
						tempLat.BOT = false;
						if(subOrAdd)
						{
							tempLat.constant = left_lat.constant + right_lat.constant;
						}
						else
						{
							tempLat.constant = left_lat.constant - right_lat.constant;
						}
					}
					else
					{
						tempLat = MeetFunc(left_lat,right_lat);
					}
					return tempLat;		
				}
				else
				{
					if(ConstantInt * right_const = dyn_cast<ConstantInt>(&*instr->getOperand(1)))
					{
						//left op is variable, right is constant
						Instruction * left_instr = dyn_cast<Instruction>(&*instr->getOperand(0));
						Lattice right_lat;
						right_lat.TOP = false;
						right_lat.BOT = false;
						right_lat.constant = right_const->getValue().getSExtValue();
						Lattice left_lat = *LatticeMap[left_instr];
						if(left_lat.TOP == false && left_lat.BOT == false)
						{
							tempLat.TOP = false;
							tempLat.BOT = false;
							if(subOrAdd)
							{
								tempLat.constant = left_lat.constant + right_lat.constant;
							}
							else
							{
								tempLat.constant = left_lat.constant - right_lat.constant;
							}
						}
						else
						{
							tempLat = MeetFunc(left_lat,right_lat);
						}
						return tempLat;
					}
					else
					{
						
						Instruction * left_instr = dyn_cast<Instruction>(&*instr->getOperand(0));
						Instruction * right_instr = dyn_cast<Instruction>(&*instr->getOperand(1));
						Lattice left_lat = *LatticeMap[left_instr];
						Lattice right_lat = *LatticeMap[right_instr];
						if(left_lat.TOP == false && left_lat.BOT == false && right_lat.TOP == false && right_lat.BOT == false)
						{
							tempLat.TOP = false;
							tempLat.BOT = false;
							if(subOrAdd)
							{
								tempLat.constant = left_lat.constant + right_lat.constant;
							}
							else
							{
								tempLat.constant = left_lat.constant - right_lat.constant;
							}
						}
						else
						{
							tempLat = MeetFunc(left_lat,right_lat);
						}						
						return tempLat;
					}
				}
			}
		}
	}
	return tempLat;
}

//---------------------------LATTICE MEET FUNCTION--------------------------------------
//
// performs the meet function between two lattice functions and returns the result lattice
//
//--------------------------------------------------------------------------------------
Lattice ConditionalConstantPropagation::MeetFunc(Lattice x, Lattice y)
{
	if(x.TOP)
	{
		return y;
	}else if(y.TOP)
	{
		return x;
	}else if(x.BOT) 
	{
		return x;
	}else if(y.BOT)
	{
		return y;
	}
	else
	{
		if(x.constant == y.constant)
		{
			return x;
		}else
		{
			x.TOP = false;
			x.BOT = true;
			return x;
		}
	}
}

//---------------------------LATTICE COMPARISON FUNCTION--------------------------------
//
// compares two lattices and returns true if they are equal
//
//--------------------------------------------------------------------------------------
bool ConditionalConstantPropagation::LatEquals(Lattice left, Lattice right)
{
	if((left.BOT == false ) && (right.BOT == false) && (left.TOP == false) && (right.TOP == false))
	{
		if(left.constant == right.constant)
		{
			return true;
		}
		else
		{
			return false;
		}
	}else if((left.BOT == right.BOT) && (left.TOP == right.TOP))
	{
		return true;
	}else
	{
		return false;
	}
}

void ConditionalConstantPropagation::RunCCP( Function & F) {


//-------------------------------INITIALIZATION SECTION--------------------------------
	BasicBlock* bb_entry 	= NULL;
	for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) 
	{
		BasicBlock* BB = dyn_cast<BasicBlock>(&*FI);
		for (BasicBlock::iterator BI = BB->begin(), BE = BB->end();BI != BE; ++BI) 
		{
			Instruction *instr = dyn_cast<Instruction>(&*BI);
			LatticeMap[instr] = new Lattice();
			LatticeMap[instr]->TOP = true;
			LatticeMap[instr]->BOT = false;
			
		}
		if ( !bb_entry )
			bb_entry = BB;
	}

	CFGWorklist.push(bb_entry);  //need to initialize bb_entry as the first node

	//node can be executed in CFG work list as many times as it has predecessors
	// so executable max needs to be set as num predecessors
	for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) 
	{
		BasicBlock* BB = dyn_cast<BasicBlock>(&*FI);
		ExecutableMap[BB->getName()] = 0;
		int count = 0;	
		for (pred_iterator PI = pred_begin(BB), E = pred_end(BB); PI != E; ++PI) 
		{
		  count++;
		}
		ExecutableMax[BB->getName()] = count;	
	}

//-------------------------------LATTICE ADJUSTMENT SECTION--------------------------------
	while (CFGWorklist.size()!=0 || SSAWorklist.size()!=0) 
	{
		while(CFGWorklist.size()!=0)
		{
			BasicBlock* BB = CFGWorklist.front();
			CFGWorklist.pop();
			//check if a node has been executed as many times as it has predecessors
			if((ExecutableMap[BB->getName()]==0) || (ExecutableMap[BB->getName()] < ExecutableMax[BB->getName()] ))
			{
				//increase executable count
				ExecutableMap[BB->getName()]+=1;
				for (BasicBlock::iterator BI = BB->begin(), BE = BB->end();BI != BE; ++BI) 
				{
					//iterate through all instructions and evaluate
		 			Instruction* definition = dyn_cast<Instruction> (&*BI);
					Lattice t = Evaluate(definition);
					//compare new and old lattice, if different then propagate & update
					if (!LatEquals(t,*LatticeMap[definition]))
					{
						*LatticeMap[definition] = MeetFunc(*LatticeMap[definition],t);
						for (Instruction::use_iterator UI= definition->use_begin(), UE=definition->use_end(); UI!=UE; ++UI) 
						{
							Instruction* usage = dyn_cast<Instruction> (*UI);
							BasicBlock* parent = usage->getParent();
							if (ExecutableMap[parent->getName()] > 0)
							{
								SSAWorklist.push(usage);
							}
						}
					}
					
				}				
			}
		}
		while(SSAWorklist.size()!=0) 
		{
			//worklist for propagating and updating instructions
			Instruction* instr = SSAWorklist.front();
			SSAWorklist.pop();
			Lattice t = Evaluate(instr);
			//compare new and old lattice, if different then propagate & update
			if (!LatEquals(t,*LatticeMap[instr]))
			{
				*LatticeMap[instr] = MeetFunc(*LatticeMap[instr],t);
				for (Instruction::use_iterator UI= instr->use_begin(), UE=instr->use_end(); UI!=UE; ++UI) 
				{
					Instruction* usage = dyn_cast<Instruction> (*UI);
					BasicBlock* parent = usage->getParent();
					if (ExecutableMap[parent->getName()] > 0)
					{
						SSAWorklist.push(usage);
					}
				}
			}
		}
	}		

//-------------------------------CONSTANT PROPAGAION SECTION--------------------------------
	for (std::map<Instruction*, Lattice*> ::iterator it = LatticeMap.begin(); it!= LatticeMap.end(); ++it) 
	{
		Instruction * instr = dyn_cast<Instruction> (&*(it->first));
		//do nothing if undefined (TOP), simplify cfg takes care of this
		//do nothing if overdefined (BOT), instruction cannot be simplified & propagated
		if ( !(LatticeMap[instr]->TOP || LatticeMap[instr]->BOT) )
		{
			BasicBlock::iterator ii(instr);
			// update a compare instruction to true or false and propagate
			if( ICmpInst * icmpinst = dyn_cast<ICmpInst>(&*instr) ) 
			{
				int64_t taken = LatticeMap[instr]->constant;
				Constant *x = ConstantInt::get(instr->getType(), 0x1);
				ConstantInt *next1 = dyn_cast<ConstantInt>(&*x);
				x = ConstantInt::get(instr->getType(), 0x0);
				ConstantInt *next0 =dyn_cast<ConstantInt>(&*x);
				if(taken == 0)
				{
					llvm::ReplaceInstWithValue(instr->getParent()->getInstList(),ii,next0);			
				}
				else
				{
					llvm::ReplaceInstWithValue(instr->getParent()->getInstList(),ii,next1);
				}
			}//update phi node instruction if constant and propagate
			else if( isa<PHINode>(instr) )   
			{
				int64_t lat_constant = LatticeMap[instr]->constant;
				// convert lat_constant to ConstantInt
				Constant * newConst = ConstantInt::getSigned(instr->getType(),lat_constant);
				if (ConstantInt * ptr = dyn_cast<ConstantInt>(newConst)) 
				{
					ReplaceInstWithValue(instr->getParent()->getInstList(), ii, ptr);
				}
			}// convert branch to unconditional if always taken/untaken
			else if ( BranchInst *BI = dyn_cast<BranchInst>(instr)) 
			{
				int64_t taken = LatticeMap[instr]->constant;
				if(taken == 0)
				{
					BranchInst * new_branch = BranchInst::Create(BI->getSuccessor(1));
					ReplaceInstWithInst(BI->getParent()->getInstList(), ii, cast<Instruction>(&*new_branch));
				}
				else
				{
					BranchInst * new_branch = BranchInst::Create(BI->getSuccessor(0));
					ReplaceInstWithInst(BI->getParent()->getInstList(), ii, cast<Instruction>(&*new_branch));
				}			
			}//propagate add/sub
			else if ( instr->getOpcode() == Instruction::Sub || instr->getOpcode() == Instruction::Add)  
			{
				int64_t lat_constant = LatticeMap[instr]->constant;
				// convert lat_constant to ConstantInt
				Constant * newConst = ConstantInt::getSigned(instr->getType(),lat_constant);
				if (ConstantInt * ptr = dyn_cast<ConstantInt>(newConst)) 
				{
					ReplaceInstWithValue(instr->getParent()->getInstList(), ii, ptr);
				}
			}
		}
	}
}
