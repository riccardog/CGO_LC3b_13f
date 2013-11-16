//===-- LC3bTargetMachine.h - Define TargetMachine for LC3b -----*- C++ -*-===//
//
//
The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the LC3b specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//
#ifndef LC3BTARGETMACHINE_H
#define LC3BTARGETMACHINE_H
#include "LC3bFrameLowering.h"
#include "LC3bInstrInfo.h"
#include "LC3bISelLowering.h"
#include "LC3bSelectionDAGInfo.h"
#include "LC3bSubtarget.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetFrameLowering.h"






namespace llvm {

class formatted_raw_ostream;

class LC3bTargetMachine : public LLVMTargetMachine {
	LC3bSubtarget		Subtarget;
	const DataLayout	DL; 			// Calculates type size & alignment
	LC3bInstrInfo		InstrInfo;		//- Instructions
	LC3bFrameLowering	FrameLowering;	//- Stack(Frame) and Stack direction
	LC3bTargetLowering 	TLInfo;			//- Stack(Frame) and Stack direction
	LC3bSelectionDAGInfo TSInfo;		//- Map .bc DAG to backend DAG
public:
	LC3bTargetMachine(	const Target &T, StringRef TT,
						StringRef CPU, StringRef FS, const TargetOptions &Options,
						Reloc::Model RM, CodeModel::Model CM,
						CodeGenOpt::Level OL, bool isLittle);
	virtual const LC3bInstrInfo const *getInstrInfo() { return &InstrInfo; }
	virtual const TargetFrameLowering *getFrameLowering() { return &FrameLowering; }
	virtual const LC3bSubtarget	*getSubtargetImpl() const { return &Subtarget; }
	virtual const DataLayout *getDataLayout() const { return &DL;}
	virtual const LC3bRegisterInfo *getRegisterInfo() const { return &InstrInfo.getRegisterInfo();}

	virtual const LC3bTargetLowering *getTargetLowering() const { return &TLInfo; }
	virtual const LC3bSelectionDAGInfo* getSelectionDAGInfo() const { return &TSInfo; }
	// Pass Pipeline Configuration
	virtual TargetPassConfig *createPassConfig(PassManagerBase &PM);
};
/// LC3bTargetMachine - LC3b little endian target machine.
/// Little Endian Check!	FIXME
class LC3bebTargetMachine : public LC3bTargetMachine {
	virtual void anchor();
public:
	LC3bebTargetMachine(const Target &T, StringRef TT,
	StringRef CPU, StringRef FS, const TargetOptions &Options,
	Reloc::Model RM, CodeModel::Model CM,
	CodeGenOpt::Level OL);
};
/// LC3belTargetMachine - LC3b32 little endian target machine.
///	FIXME
class LC3belTargetMachine : public LC3bTargetMachine {
virtual void anchor();
public:
	LC3belTargetMachine(const Target &T, StringRef TT,
						StringRef CPU, StringRef FS, const TargetOptions &Options,
						Reloc::Model RM, CodeModel::Model CM,
						CodeGenOpt::Level OL);
};

} // End llvm namespace
#endif


