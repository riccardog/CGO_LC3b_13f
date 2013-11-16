//===----------------------------------------------------------------------===//
//
// This file contains the CPU0 implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "cpu0-reg-info"
#include "Cpu0RegisterInfo.h"
#include "LC3b.h"
#include "LC3bSubtarget.h"
#include "LC3bMachineFunction.h"
#include "llvm/IR/Constants.h"
#include "llvm/DebugInfo.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h" 
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"

#define GET_REGINFO_TARGET_DESC
#include "Cpu0GenRegisterInfo.inc"
using namespace llvm;
LC3bRegisterInfo::Cpu0RegisterInfo(const Cpu0Subtarget &ST, const TargetInstrInfo &tii)
							: Cpu0GenRegisterInfo(LC3b::LR), Subtarget(ST), TII(tii) {}
//===----------------------------------------------------------------------===//
// Callee Saved Registers methods
//===----------------------------------------------------------------------===//
/// LC3b Callee Saved Registers
// In Cpu0CallConv.td,
// def CSR_O32 : CalleeSavedRegs<(add LR, FP, (sequence "S%u", 2, 0))>;
// llc create CSR_O32_SaveList and CSR_O32_RegMask from above defined.
const uint16_t* Cpu0RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
	return CSR_O32_SaveList;
}


const uint32_t* Cpu0RegisterInfo::getCallPreservedMask(CallingConv::ID) const {
	return CSR_O32_RegMask;
}
// pure virtual method
BitVector Cpu0RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
	static const uint16_t ReservedCPURegs[] = {LC3b::ZERO, LC3b::AT, LC3b::SP, LC3b::LR, LC3b::PC};
	BitVector Reserved(getNumRegs());
	typedef TargetRegisterClass::iterator RegIter;
	for (unsigned I = 0; I < array_lengthof(ReservedCPURegs); ++I)
		Reserved.set(ReservedCPURegs[I]);
	return Reserved;
}
// pure virtual method
// FrameIndex represent objects inside a abstract stack.
// We must replace FrameIndex with an stack/frame pointer
// direct reference.
void Cpu0RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj, unsigned FIOperandNum, RegScavenger *RS) const {}
// pure virtual method
unsigned Cpu0RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
	const TargetFrameLowering *TFI = MF.getTarget().getFrameLowering();
	return TFI->hasFP(MF) ? (LC3b::FP) : (LC3b::SP);
}


