//===-- LC3bMachineFunctionInfo.h - Private data used for Cpu0 ----*- C++ -*-=//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Cpu0 specific subclass of MachineFunctionInfo.
//
//===----------------------------------------------------------------------===//
#ifndef LC3b_MACHINE_FUNCTION_INFO_H
#define LC3b_MACHINE_FUNCTION_INFO_H
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include <utility>

namespace llvm {
		/// LC3bFunctionInfo - This class is derived from MachineFunction private
		/// LC3b target-specific information for each MachineFunction.
		class LC3bFunctionInfo : public MachineFunctionInfo {
				MachineFunction& MF;
				unsigned MaxCallFrameSize;
				public: LC3bFunctionInfo(MachineFunction& MF) : MF(MF), MaxCallFrameSize(0)
				{}
				unsigned getMaxCallFrameSize() const { return MaxCallFrameSize; }
				void setMaxCallFrameSize(unsigned S) { MaxCallFrameSize = S; }
		};
} // end of namespace llvm
#endif // CPU0_MACHINE_FUNCTION_INFO_H

