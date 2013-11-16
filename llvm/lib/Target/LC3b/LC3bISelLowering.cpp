//===-- LC3bISelLowering.cpp - LC3b DAG Lowering Implementation -----------===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that Cpu0 uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "LC3b-lower"
#include "LC3bISelLowering.h"
#include "LC3bTargetMachine.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "LC3bSubtarget.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/CallingConv.h"


#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
LC3bTargetLowering:: LC3bTargetLowering(LC3bTargetMachine &TM) : TargetLowering(TM, new TargetLoweringObjectFileELF()), Subtarget(&TM.getSubtarget<LC3bSubtarget>()) {
}
#include "LC3bGenCallingConv.inc"
/// LowerFormalArguments - transform physical registers into virtual registers
/// and generate load operations for arguments places on the stack.
SDValue LC3bTargetLowering::LowerFormalArguments(SDValue Chain,CallingConv::ID CallConv, bool isVarArg, const SmallVectorImpl<ISD::InputArg> &Ins, DebugLoc dl, SelectionDAG &DAG,SmallVectorImpl<SDValue> &InVals) const {
		return Chain;
}
//===----------------------------------------------------------------------===//
//Return Value Calling Convention Implementation
//===----------------------------------------------------------------------===//
SDValue LC3bTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg, const SmallVectorImpl<ISD::OutputArg> &Outs,const SmallVectorImpl<SDValue> &OutVals, DebugLoc dl, SelectionDAG &DAG) const {
		return DAG.getNode(LC3bISD::Ret, dl, MVT::Other, Chain, DAG.getRegister(LC3b::LR, MVT::i32));
}

