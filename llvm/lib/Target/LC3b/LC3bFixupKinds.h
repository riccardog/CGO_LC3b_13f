//===-- LC3bFixupKinds.h - LC3b Specific Fixup Entries ----------*- C++ -*-===//
//
//The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LC3b_LC3bFIXUPKINDS_H
#define LLVM_LC3b_LC3bFIXUPKINDS_H
#include "llvm/MC/MCFixup.h"
namespace llvm {
		namespace LC3b {
		// Although most of the current fixup types reflect a unique relocation
		// one can have multiple fixup types for a given relocation and thus need
		// to be uniquely named.
		//
		// This table *must* be in the save order of
		// MCFixupKindInfo Infos[Cpu0::NumTargetFixupKinds]
		// in Cpu0AsmBackend.cpp.
		//
				enum Fixups {
				// Branch fixups resulting in R_CPU0_16.
				fixup_LC3b_16 = FirstTargetFixupKind,
				// Marker
				LastTargetFixupKind,
				NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
				};
		} // namespace LC3b
} // namespace llvm
#endif // LLVM_LC3b_LC3bFIXUPKINDS_H

