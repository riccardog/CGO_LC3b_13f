//===-- LC3bInstrInfo.cpp - LC3b Instruction Information ------------------===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

/
// This file contains the LC3b implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//
#include "LC3bInstrInfo.h"
#include "LC3bTargetMachine.h"
#define GET_INSTRINFO_CTOR
#include "LC3bGenInstrInfo.inc"
using namespace llvm;
LC3bInstrInfo::LC3bInstrInfo(LC3bTargetMachine &tm) : TM(tm), RI(*TM.getSubtargetImpl(), *this) {}

const LC3bRegisterInfo &LC3bInstrInfo::getRegisterInfo() const {
		return RI;
}



