//===-- LC3bISelLowering.h - LC3b DAG Lowering Interface --------*- C++ -*-===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that LC3b uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//
#ifndef LC3bISELLOWERING_H
#define LC3bISELLOWERING_H
#include "LC3b.h"
#include "LC3bSubtarget.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Target/TargetLowering.h"
namespace llvm {
		namespace LC3bISD {
				enum NodeType {
						// Start the numbering from where ISD NodeType finishes.
						FIRST_NUMBER = ISD::BUILTIN_OP_END,
						Ret
				};
		}
		//===----------------------------
		// TargetLowering Implementation
		//===--------------------------------------------------------------------===//
		class LC3bTargetLowering : public TargetLowering {
				public:
				explicit LC3bTargetLowering(LC3bTargetMachine &TM);
				private:
				// Subtarget Info
				const LC3bSubtarget *Subtarget;
				//- must be exist without function all
				virtual SDValue LowerFormalArguments(SDValue Chain,CallingConv::ID CallConv, bool isVarArg,const SmallVectorImpl<ISD::InputArg> &Ins, DebugLoc dl, SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const;
				//- must be exist without function all
				virtual SDValue LowerReturn(SDValue Chain,CallingConv::ID CallConv, bool isVarArg,const SmallVectorImpl<ISD::OutputArg> &Outs,const SmallVectorImpl<SDValue> &OutVals,DebugLoc dl, SelectionDAG &DAG) const;
		};
}
#endif // LC3bISELLOWERING_H

