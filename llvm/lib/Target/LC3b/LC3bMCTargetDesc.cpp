//===----------------------------------------------------------------------===//
//
// This file provides LC3b specific target descriptions.
//
//===----------------------------------------------------------------------===//
#include "LC3bMCTargetDesc.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "LC3bGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "LC3bGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "LC3bGenRegisterInfo.inc"

using namespace llvm;

extern "C" void LLVMInitializeLC3bTargetMC () {

	
}



static std::string ParseLC3bTriple(StringRef TT, StringRef CPU) {
		std::string LC3bArchFeature;
		size_t DashPosition = 0;
		StringRef TheTriple;
		// Let’s see if there is a dash, like cpu0-unknown-linux.
		DashPosition = TT.find(’-’);
		if (DashPosition == StringRef::npos) {
				// No dash, we check the string size.
				TheTriple = TT.substr(0);
		} else {
				// We are only interested in substring before dash.
				TheTriple = TT.substr(0,DashPosition);
		}
		if (TheTriple == "LC3b" || TheTriple == "LC3bel") {
				if (CPU.empty() || CPU == "cpu032") {  //FIXME : cpu0 name
						LC3bArchFeature = "+cpu032";
				}
		}
		return LC3bArchFeature;
}
static MCInstrInfo *createLC3bMCInstrInfo() {
		MCInstrInfo *X = new MCInstrInfo();
		InitLC3bMCInstrInfo(X); // defined in LC3bGenInstrInfo.inc
		return X;
}
static MCRegisterInfo *createLC3bMCRegisterInfo(StringRef TT) {
		MCRegisterInfo *X = new MCRegisterInfo();
		InitLC3bMCRegisterInfo(X, LC3b::LR); // defined in LC3bGenRegisterInfo.inc   //FIXME what is LC3b::LR
		return X;
}
static MCSubtargetInfo *createLC3bMCSubtargetInfo(StringRef TT, StringRef CPU,
StringRef FS) {
		std::string ArchFS = ParseLC3bTriple(TT,CPU);
		if (!FS.empty()) {
				if (!ArchFS.empty())
						ArchFS = ArchFS + "," + FS.str();
				else
						ArchFS = FS;
		}
		MCSubtargetInfo *X = new MCSubtargetInfo();
		InitLC3bMCSubtargetInfo(X, TT, CPU, ArchFS); // defined in LC3bGenSubtargetInfo.inc
		return X;
}
static MCAsmInfo *createLC3bMCAsmInfo(const Target &T, StringRef TT) {
		MCAsmInfo *MAI = new LC3bMCAsmInfo(T, TT);
		MachineLocation Dst(MachineLocation::VirtualFP);
		MachineLocation Src(LC3b::SP, 0);
		MAI->addInitialFrameState(0, Dst, Src);
		return MAI;
}
static MCCodeGenInfo *createLC3bMCCodeGenInfo(StringRef TT, Reloc::Model RM,CodeModel::Model CM, CodeGenOpt::Level OL) {
		MCCodeGenInfo *X = new MCCodeGenInfo();
		if (CM == CodeModel::JITDefault)
		RM = Reloc::Static;
		else if (RM == Reloc::Default)
		RM = Reloc::PIC_;
		X->InitMCCodeGenInfo(RM, CM, OL); // defined in lib/MC/MCCodeGenInfo.cpp
		return X;
}
static MCInstPrinter *createLC3bMCInstPrinter(const Target &T,
unsigned SyntaxVariant,
const MCAsmInfo &MAI,
const MCInstrInfo &MII,
const MCRegisterInfo &MRI,
const MCSubtargetInfo &STI) {
		return new LC3bInstPrinter(MAI, MII, MRI);
}
extern "C" void LLVMInitializeLC3bTargetMC() {
		// Register the MC asm info.
		RegisterMCAsmInfoFn X(TheLC3bTarget, createLC3bMCAsmInfo);
		RegisterMCAsmInfoFn Y(TheLC3belTarget, createLC3bMCAsmInfo);

		// Register the MC codegen info.
		TargetRegistry::RegisterMCCodeGenInfo(TheLC3bTarget,
		createLC3bMCCodeGenInfo);
		TargetRegistry::RegisterMCCodeGenInfo(TheLC3belTarget,
		createLC3bMCCodeGenInfo);
		// Register the MC instruction info.
		TargetRegistry::RegisterMCInstrInfo(TheLC3bTarget, createLC3bMCInstrInfo);
		TargetRegistry::RegisterMCInstrInfo(TheLC3belTarget, createLC3bMCInstrInfo);
		// Register the MC register info.
		TargetRegistry::RegisterMCRegInfo(TheLC3bTarget, createLC3bMCRegisterInfo);
		TargetRegistry::RegisterMCRegInfo(TheLC3belTarget, createLC3bMCRegisterInfo);
		// Register the MC subtarget info.
		TargetRegistry::RegisterMCSubtargetInfo(TheLC3bTarget,
		createLC3bMCSubtargetInfo);
		TargetRegistry::RegisterMCSubtargetInfo(TheLC3belTarget,
		createLC3bMCSubtargetInfo);
		// Register the MCInstPrinter.
		TargetRegistry::RegisterMCInstPrinter(TheLC3bTarget,
		createLC3bMCInstPrinter);
		TargetRegistry::RegisterMCInstPrinter(TheLC3belTarget,
		createLC3bMCInstPrinter);
}

