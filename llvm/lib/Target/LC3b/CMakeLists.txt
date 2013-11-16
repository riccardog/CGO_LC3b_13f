# CMakeLists.txt
# Our td all in LC3b.td, LC3bRegisterInfor.td and LC3bInstrInfo.td in LC3b.td

set (LLVM_TARGET_DEFINITIONS LC3b.td)

# Generate LC3bGenRegisterInfo.inc and LC3bGenInstrInfo.inc by your hand code C++ files.
# LC3bGenRegisterInfo.inc came from LC3bRegisterInfo.td, LC3bGenInstrInfo.inc came from LC3bInstrInfo.td.

tablegen (LLVM LC3bGenRegisterInfo.inc -gen-register-info)
tablegen (LLVM LC3bGenInstrInfo.inc -gen-instr-info)
tablegen (LLVM LC3bGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM  LC3bGenCodeEmitter.inc -gen-emitter)
tablegen(LLVM  LC3bGenMCCodeEmitter.inc -gen-emitter -mc-emitter)
tablegen(LLVM  LC3bGenAsmWriter.inc -gen-asm-writer)


# LC3bCommonTableGen must be defined
add_public_tablegen_target (LC3bCommonTableGen)


# cpuCodeGen Should match with LLVMBuild.tx LC3bCodeGen
add_llvm_target (LC3bCodeGen LC3bTargetMachine.cpp)

# should match with "subdirectories = MCTargetDesc TargetInfo" in LLVMBuild.txt
add_subdirectory (TargetInfo)
add_subdirectory (MCTargetDesc)
add_subdirectory (InstPrinter)
# LC3bCodeGen should match with LLVMBuild.txt LC3bCodeGen
add_llvm_target(LC3bCodeGen
				LC3bAsmPrinter.cpp
				LC3bMCInstLower.cpp
				LC3bISelLowering.cpp
				LC3bInstrInfo.cpp
				LC3bFrameLowering.cpp
				LC3bRegisterInfo.cpp
				LC3bSubtarget.cpp
				LC3bTargetMachine.cpp
				LC3bSelectionDAGInfo.cpp)

