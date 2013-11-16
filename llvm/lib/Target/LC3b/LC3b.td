// This is top level entry for LC3b target

include "llvm/Target/Target.td"

// Register file, calling conv, instruction description
include "LC3bRegisterInfor.td"
include "LC3bSchedule.td"
include "LC3bInstrInfo.td"

def LC3bInstrInfo : InstrInfo;

// Without this will have error: ’cpu032’ is not a recognized processor for
// this target (ignoring processor)
//===----------------------------------------------------------------------===//
// LC3b Subtarget features
//
//===----------------------------------------------------------------------===//

def FeatureLC3b32 : SubtargetFeature<"cpu032", "LC3bArchVersion", "LC3b32", "LC3b32 ISA Support">;

//===----------------------------------------------------------------------===//
// LC3b processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features> : Processor<Name, LC3bGenericItineraries, Features>;
def : Proc<"cpu032", [FeatureLC3b32]>;
def LC3bAsmWriter : AsmWriter {
	string AsmWriterClassName = "InstPrinter";
	bit isMCAsmWriter = 1;
}

// Will generate LC3bGenAsmWrite.inc included by LC3bInstPrinter.cpp, contents
// as follows,
// void LC3bInstPrinter::printInstruction(const MCInst *MI, raw_ostream &O) {...}
// const char *LC3bInstPrinter::getRegisterName(unsigned RegNo) {...}


def LC3b : Target {

	let InstructionSet LC3bInstrInfo;
	let AssemblyWriters = [LC3bAsmWriter];

}

