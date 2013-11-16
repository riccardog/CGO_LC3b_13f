//===-- LC3bMCAsmInfo.h - LC3b Asm Info ------------------------*- C++ -*--===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the Cpu0MCAsmInfo class.
//
//===----------------------------------------------------------------------===//
#ifndef LC3bTARGETASMINFO_H
#define LC3bTARGETASMINFO_H
#include "llvm/MC/MCAsmInfo.h"
namespace llvm {
		class StringRef;
		class Target;
		class LC3bMCAsmInfo : public MCAsmInfo {
				virtual void anchor();
				public:
				explicit LC3bMCAsmInfo(const Target &T, StringRef TT);
		};
} // namespace llvm
#endif

