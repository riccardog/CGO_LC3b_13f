//===-- LC3bSubtarget.cpp - LC3b Subtarget Information --------------------===//
//
// The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the Cpu0 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//
#include "LC3bSubtarget.h"
#include "LC3b.h"
#include "llvm/Support/TargetRegistry.h"
#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "LC3bGenSubtargetInfo.inc"
using namespace llvm;
void LC3bSubtarget::anchor() { }
LC3bSubtarget::LC3bSubtarget(const std::string &TT, const std::string &CPU, const std::string &FS, bool little) : LC3bGenSubtargetInfo(TT, CPU, FS), LC3bABI(UnknownABI), IsLittle(little) {
		std::string CPUName = CPU;
		if (CPUName.empty())
		CPUName = "LC3b";
		// Parse features string.
		ParseSubtargetFeatures(CPUName, FS);
		// Initialize scheduling itinerary for the specified CPU.
		InstrItins = getInstrItineraryForCPU(CPUName);
		// Set Cpu0ABI if it hasn’t been set yet.
		if (LC3bABI == UnknownABI)
		LC3bABI = O32; //FIXME
}

