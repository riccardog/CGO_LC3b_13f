//===-- BasicBasicBlockOptimization.cpp - Basic Block Code Layout optimization ----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements a very simple profile guided basic block placement
// algorithm.  The idea is to put frequently executed blocks together at the
// start of the function, and hopefully increase the number of fall-through
// conditional branches.  If there is no profile information for a particular
// function, this pass basically orders blocks in depth-first order
//
// The algorithm implemented here is basically "Algo1" from "Profile Guided Code
// Positioning" by Pettis and Hansen, except that it uses basic block counts
// instead of edge counts.  This should be improved in many ways, but is very
// simple for now.
//
// Basically we "place" the entry block, then loop over all successors in a DFO,
// placing the most frequently executed successor until we run out of blocks.  I
// told you this was _extremely_ simplistic. :) This is also much slower than it
// could be.  When it becomes important, this pass will be rewritten to use a
// better algorithm, and then we can worry about efficiency.
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/ProfileInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/CFG.h"
#include <set>
using namespace llvm;

namespace {
  struct BasicBlockOptimization : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    BasicBlockOptimization() : FunctionPass(ID) {}

	bool ExampleCode(Function &F);

    virtual bool runOnFunction(Function &F);

    virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.setPreservesCFG();
      AU.addRequired<ProfileInfo>();
    }

	void BasicBlockLayOut(Function &F);

  private:
    /// PI - The profile information that is guiding us.
    ///
    ProfileInfo *PI;

  };
}

bool BasicBlockOptimization::ExampleCode( Function &F) {

 	std::map<StringRef, unsigned> bb_id_map;
	BasicBlock * if_else, * if_then;
	unsigned bb_id = 0;
	for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
		BasicBlock* BB = dyn_cast<BasicBlock>(&*FI);

		llvm::errs() << "BB" << bb_id << " (" << BB->getName() << "): ";
		llvm::errs() << "Executed " << PI->getExecutionCount(BB);
		llvm::errs() << "\n";

		bb_id_map[BB->getName()] = bb_id;
		if ( bb_id == 3 )
			if_then = BB;
		if ( bb_id == 4 )
			if_else = BB;

		bb_id ++;
	}
	
	llvm::errs() << "\n\n";


	// edges
	for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
		BasicBlock* BB = dyn_cast<BasicBlock>(&*FI);

    	succ_iterator SI = succ_begin(BB), E = succ_end(BB);

		for ( ; SI != E; SI ++) {
			BasicBlock * succ_bb = *SI;
			llvm::errs() << "Edge " << "BB" << bb_id_map[BB->getName()] << " (" << BB->getName() << ") ";
			llvm::errs() << "->" << " BB" << bb_id_map[succ_bb->getName()] << " (" << succ_bb->getName() << ") ";
			
			llvm::ProfileInfo::Edge edge = std::make_pair(BB, succ_bb);

			llvm::errs() << ": Executed " << PI->getEdgeWeight(edge);
			llvm::errs() << "\n";

		}


	}

	// insert if_else before if_then
    Function::BasicBlockListType &Blocks = if_then->getParent()->getBasicBlockList();
    Blocks.splice(if_then, Blocks, if_else);

	return true;

}


bool BasicBlockOptimization::runOnFunction(Function &F) {
  // get profiling information
  PI = &getAnalysis<ProfileInfo>();


  ExampleCode(F);

  BasicBlockLayOut(F);
  return true;

}


void BasicBlockOptimization::BasicBlockLayOut(Function &F) {
  // MISSING: implement profile-guided basic block lay out algorithm

}

char BasicBlockOptimization::ID = 0;
static RegisterPass<BasicBlockOptimization> X("basicblockoptimization", "Profile Guided Basic Block Placement Optimization");
