//===-- LC3bInstrInfo.h - LC3b Instruction Information ----------*- C++ -*-===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Cpu0 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//
#ifndef LC3bINSTRUCTIONINFO_H
#define LC3bINSTRUCTIONINFO_H

#include "LC3b.h"
#include "LC3bRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "LC3bGenInstrInfo.inc"


#define GET_INSTRINFO_HEADER
#include "LC3bGenInstrInfo.inc"

namespace llvm {
	class LC3bInstrInfo : public LC3bGenInstrInfo {
		LC3bTargetMachine &TM;
		const LC3bRegisterInfo RI;
	public:
		explicit LC3bInstrInfo(LC3bTargetMachine &TM);
		/// getRegisterInfo - TargetInstrInfo is a superset of MRegister info. As
		/// such, whenever a client has an instance of instruction info, it should
		/// always be able to get register info as well (through this method).
		///
		virtual const LC3bRegisterInfo &getRegisterInfo() const;
	};

	class LC3bInstrInfo : public LC3bGenInstrInfo {
		LC3bTargetMachine &TM;
	public:
		explicit LC3bInstrInfo(LC3bTargetMachine &TM);
	};
}
#endif

