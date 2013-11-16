//===-- LC3bSelectionDAGInfo.h - LC3b SelectionDAG Info ---------*- C++ -*-===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the Cpu0 subclass for TargetSelectionDAGInfo.
//
//===----------------------------------------------------------------------===//
#ifndef LC3bSELECTIONDAGINFO_H
#define LC3bSELECTIONDAGINFO_H
#include "llvm/Target/TargetSelectionDAGInfo.h"

namespace llvm {
		class LC3bTargetMachine;
		class LC3bSelectionDAGInfo : public TargetSelectionDAGInfo {
				public:
				explicit LC3bSelectionDAGInfo(const LC3bTargetMachine &TM);
				~LC3bSelectionDAGInfo();
		};
}
#endif

