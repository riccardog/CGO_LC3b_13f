//===-- LC3bAsmPrinter.h - LC3b LLVM Assembly Printer ----------*- C++ -*--===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// LC3b Assembly printer class.
//
//===--------------------------------------------------------------------===//
#ifndef LC3bASMPRINTER_H
#define LC3bASMPRINTER_H
#include "LC3bMachineFunction.h"
#include "LC3bMCInstLower.h"
#include "LC3bSubtarget.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetMachine.h"
namespace llvm {
		class MCStreamer;
		class MachineInstr;
		class MachineBasicBlock;
		class Module;
		class raw_ostream;
		class LLVM_LIBRARY_VISIBILITY LC3bAsmPrinter : public AsmPrinter {
				void EmitInstrWithMacroNoAT(const MachineInstr *MI);
				public:
				const LC3bSubtarget *Subtarget;
				const LC3bFunctionInfo *LC3bFI;
				LC3bMCInstLower MCInstLowering;
				explicit LC3bAsmPrinter(TargetMachine &TM, MCStreamer &Streamer)
				: AsmPrinter(TM, Streamer), MCInstLowering(*this) {
						Subtarget = &TM.getSubtarget<LC3bSubtarget>();
				}
				virtual const char *getPassName() const {
						return "LC3b Assembly Printer";
				}
				virtual bool runOnMachineFunction(MachineFunction &MF);
				//- EmitInstruction() must exists or will have run time error.
				void EmitInstruction(const MachineInstr *MI);
				void printSavedRegsBitmask(raw_ostream &O);
				void printHex32(unsigned int Value, raw_ostream &O);
				void emitFrameDirective();
				const char *getCurrentABIString() const;
				virtual void EmitFunctionEntryLabel();
				virtual void EmitFunctionBodyStart();
				virtual void EmitFunctionBodyEnd();
				void EmitStartOfAsmFile(Module &M);
				virtual MachineLocation getDebugValueLocation(const MachineInstr *MI) const;
				void PrintDebugValueComment(const MachineInstr *MI, raw_ostream &OS);
		};
}
#endif

