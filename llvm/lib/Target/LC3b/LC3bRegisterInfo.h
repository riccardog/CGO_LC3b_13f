//===-- Cpu0RegisterInfo.h - LC3b Register Information Impl -----*- C++ -*-===//
//
//
//					The LLVM Compiler Infrastructure
//
//
//===----------------------------------------------------------------------===//
//
// This file contains the LC3b implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//
#ifndef LC3BREGISTERINFO_H
#define LC3BREGISTERINFO_H
#include "LC3b.h"
#include "llvm/Target/TargetRegisterInfo.h"
#define GET_REGINFO_HEADER
#include "Cpu0GenRegisterInfo.inc"
namespace llvm {
class Cpu0Subtarget;
class TargetInstrInfo;
class Type;

struct Cpu0RegisterInfo : public Cpu0GenRegisterInfo {
	const Cpu0Subtarget &Subtarget;
	const TargetInstrInfo &TII;
	Cpu0RegisterInfo(const Cpu0Subtarget &Subtarget, const TargetInstrInfo &tii);
	/// getRegisterNumbering - Given the enum value for some register, e.g.
	/// LC3b::RA, return the number that it corresponds to (e.g. 31).
	static unsigned getRegisterNumbering(unsigned RegEnum);
	/// Code Generation virtual methods...
	const uint16_t *getCalleeSavedRegs(const MachineFunction* MF = 0) const;
	const uint32_t *getCallPreservedMask(CallingConv::ID) const;
	// pure virtual method
	BitVector getReservedRegs(const MachineFunction &MF) const;
	// pure virtual method
	/// Stack Frame Processing Methods
	void eliminateFrameIndex(MachineBasicBlock::iterator II,
	int SPAdj, unsigned FIOperandNum,
	RegScavenger *RS = NULL) const;
	// pure virtual method
	/// Debug information queries.
	unsigned getFrameRegister(const MachineFunction &MF) const;
};

} // end namespace llvm
#endif

