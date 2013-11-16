#ifndef LC3bMCTARGETDESC_H
#define LC3bMCTARGETDESC_H
namespace llvm {
		class Target;
		extern Target TheLC3bTarget;
		extern Target TheLC3belTarget;
} // End llvm namespace


// Defines symbolic names for LC3bregisters. This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "LC3bGenRegisterInfo.inc"
// Defines symbolic names for the LC3binstructions.
#define GET_INSTRINFO_ENUM
#include "LC3bGenInstrInfo.inc"
#define GET_SUBTARGETINFO_ENUM
#include "LC3bGenSubtargetInfo.inc"
#endif

