//===-- Cpu0TargetMachine.h - Define TargetMachine for Cpu0 -----*- C++ -*-===//
//
//
The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Cpu0 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//
#ifndef LC3BTARGETMACHINE_H
#define LC3BTARGETMACHINE_H
#include "Cpu0FrameLowering.h"
#include "Cpu0InstrInfo.h"
#include "Cpu0ISelLowering.h"
#include "Cpu0SelectionDAGInfo.h"
#include "Cpu0Subtarget.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetFrameLowering.h"






namespace llvm {

class formatted_raw_ostream;

class Cpu0TargetMachine : public LLVMTargetMachine {
	Cpu0SubtargetSubtarget;
	const DataLayout	DL; // Calculates type size & alignment
	Cpu0InstrInfo	InstrInfo;
	//- Instructions
	Cpu0FrameLowering	FrameLowering;
	//- Stack(Frame) and Stack direction
	Cpu0TargetLowering TLInfo;
	//- Stack(Frame) and Stack direction
	Cpu0SelectionDAGInfo TSInfo;
	//- Map .bc DAG to backend DAG
public:
	Cpu0TargetMachine(	const Target &T, StringRef TT,
						StringRef CPU, StringRef FS, const TargetOptions &Options,
						Reloc::Model RM, CodeModel::Model CM,
						CodeGenOpt::Level OL, bool isLittle);
	virtual const Cpu0InstrInfo const *getInstrInfo() { return &InstrInfo; }
	virtual const TargetFrameLowering *getFrameLowering() { return &FrameLowering; }
	virtual const Cpu0Subtarget	*getSubtargetImpl() const { return &Subtarget; }
	virtual const DataLayout *getDataLayout() const { return &DL;}
	virtual const Cpu0RegisterInfo *getRegisterInfo() const { return &InstrInfo.getRegisterInfo();}

	virtual const Cpu0TargetLowering *getTargetLowering() const { return &TLInfo; }
	virtual const Cpu0SelectionDAGInfo* getSelectionDAGInfo() const { return &TSInfo; }
	// Pass Pipeline Configuration
	virtual TargetPassConfig *createPassConfig(PassManagerBase &PM);
};
/// Cpu0ebTargetMachine - Cpu032 big endian target machine.
///
class Cpu0ebTargetMachine : public Cpu0TargetMachine {
	virtual void anchor();
public:
	Cpu0ebTargetMachine(const Target &T, StringRef TT,
	StringRef CPU, StringRef FS, const TargetOptions &Options,
	Reloc::Model RM, CodeModel::Model CM,
	CodeGenOpt::Level OL);
};
/// Cpu0elTargetMachine - Cpu032 little endian target machine.
///
class Cpu0elTargetMachine : public Cpu0TargetMachine {
virtual void anchor();
public:
	Cpu0elTargetMachine(const Target &T, StringRef TT,
						StringRef CPU, StringRef FS, const TargetOptions &Options,
						Reloc::Model RM, CodeModel::Model CM,
						CodeGenOpt::Level OL);
};

} // End llvm namespace
#endif



//- TargetMachine.h
class TargetMachine {
	TargetMachine(const TargetMachine &) LLVM_DELETED_FUNCTION;
	void operator=(const TargetMachine &) LLVM_DELETED_FUNCTION;

public:
// Interfaces to the major aspects of target machine information:
// -- Instruction opcode and operand information
// -- Pipelines and scheduling information
// -- Stack frame information
// -- Selection DAG lowering information
//
	virtual const TargetInstrInfo *getInstrInfo() const { return 0; }
	virtual const TargetFrameLowering *getFrameLowering() const { return 0; }
	virtual const TargetLowering *getTargetLowering() const { return 0; }
	virtual const TargetSelectionDAGInfo *getSelectionDAGInfo() const{ return 0; }
	virtual const DataLayout *getDataLayout() const { return 0; }
/// getSubtarget - This method returns a pointer to the specified type of
/// TargetSubtargetInfo. In debug builds, it verifies that the object being
/// returned is of the correct type.

	template<typename STC> const STC &getSubtarget() const {
		return *static_cast<const STC*>(getSubtargetImpl());
	}
}

class LLVMTargetMachine : public TargetMachine {
protected: // Can only create subclasses.
	LLVMTargetMachine(const Target &T, StringRef TargetTriple,
	StringRef CPU, StringRef FS, TargetOptions Options,
	Reloc::Model RM, CodeModel::Model CM,
	CodeGenOpt::Level OL);
};

