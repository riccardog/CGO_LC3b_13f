//===----------------------------------------------------------------------===//
//
// Implements the info about LC3b target spec.
//
//===----------------------------------------------------------------------===//
#include "LC3bTargetMachine.h"
#include "LC3b.h"
#include "llvm/PassManager.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;
extern "C" void LLVMInitializeLC3bTarget() {
	// Register the target.
	//- Big endian Target Machine
	RegisterTargetMachine<LC3bebTargetMachine> X(TheLC3bTarget);
	//- Little endian Target Machine
	RegisterTargetMachine<LC3belTargetMachine> Y(TheLC3belTarget); ///FIXME LITTLE Endian
}
// DataLayout --> Big-endian, 32-bit pointer/ABI/alignment
// The stack is always 8 byte aligned
// On function prologue, the stack is created by decrementing
// its pointer. Once decremented, all references are done with positive
// offset from the stack/frame pointer, using StackGrowsUp enables
// an easier handling.
// Using CodeModel::Large enables different CALL behavior.
LC3bTargetMachine::LC3bTargetMachine(	const Target &T, StringRef TT, StringRef CPU, StringRef FS, 
										const TargetOptions &Options, Reloc::Model RM, 
										CodeModel::Model CM, CodeGenOpt::Level OL, bool isLittle)
			//- Default is big endian -> FIXME
	: 	LLVMTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL), 
		Subtarget(TT, CPU, FS, isLittle), 
		DL(isLittle ?
		("e-p:32:32:32-i8:8:32-i16:16:32-i64:64:64-n32") :
		("E-p:32:32:32-i8:8:32-i16:16:32-i64:64:64-n32")),
		InstrInfo(*this), 
		FrameLowering(Subtarget), 
		TLInfo(*this), 
		TSInfo(*this) 
{}

void LC3bebTargetMachine::anchor() { }
LC3bebTargetMachine::LC3bebTargetMachine(const Target &T, StringRef TT, StringRef CPU, StringRef FS, 
										 const TargetOptions &Options, Reloc::Model RM, CodeModel::Model CM, CodeGenOpt::Level OL)
	: LC3bTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, false) {}

///FIXME LITTLE Endian
void LC3belTargetMachine::anchor() { }
LC3belTargetMachine::LC3belTargetMachine(const Target &T, StringRef TT, StringRef CPU, StringRef FS, 
										 const TargetOptions &Options, Reloc::Model RM, CodeModel::Model CM, CodeGenOpt::Level OL)
	: LC3bTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, true) {}

namespace {
/// LC3b Code Generator Pass Configuration Options.
class LC3bPassConfig : public TargetPassConfig {
public:
	LC3bPassConfig(LC3bTargetMachine *TM, PassManagerBase &PM)
		: TargetPassConfig(TM, PM) {}
	LC3bTargetMachine &getLC3bTargetMachine() const {
		return getTM<LC3bTargetMachine>();
	}
	const LC3bSubtarget &getLC3bSubtarget() const {
		return *getLC3bTargetMachine().getSubtargetImpl();
	}
};

} // namespace

TargetPassConfig *LC3bTargetMachine::createPassConfig(PassManagerBase &PM) {
	return new LC3bPassConfig(this, PM);
}

