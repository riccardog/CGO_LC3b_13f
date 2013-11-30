//===-- LC3bSchedule.td - LC3b Scheduling Definitions ------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Functional units across LC3b chips sets. Based on GCC/LC3b backend files.
//===----------------------------------------------------------------------===//
def ALU     : FuncUnit;
def SHF     : FuncUnit;
//FIXME : assuming reg->PC loads are through the adder sitting before PC, rather than A input of ALU
def MAR_PC_ADDER  : FuncUnit; //added because Load/Store/Branch/LEA do not use ALU for calculations

//===----------------------------------------------------------------------===//
// Instruction Itinerary classes used for LC3b
//===----------------------------------------------------------------------===//
def IIAlu              : InstrItinClass;
def IIShf              : InstrItinClass;
def IILea              : InstrItinClass;
def IILoad             : InstrItinClass;
def IIStore            : InstrItinClass;
def IIBranch           : InstrItinClass;
def IITrap             : InstrItinClass;
def IIPseudo           : InstrItinClass;

//===----------------------------------------------------------------------===//
// LC3b Generic instruction itineraries.
//===----------------------------------------------------------------------===//
def LC3bGenericItineraries : ProcessorItineraries<[ALU, SHF], [], [
  InstrItinData<IIAlu              , [InstrStage<9,  [ALU]>]>,
  InstrItinData<IIShf              , [InstrStage<9,  [SHF]>]>,
  InstrItinData<IILea              , [InstrStage<9,  [MAR_PC_ADDER]>]>,
  InstrItinData<IILoad             , [InstrStage<15,  [MAR_PC_ADDER]>]>,
  InstrItinData<IIStore            , [InstrStage<15,  [MAR_PC_ADDER]>]>,
  InstrItinData<IIBranch           , [InstrStage<10,  [MAR_PC_ADDER]>]>,
  InstrItinData<IITrap             , [InstrStage<15,  [MAR_PC_ADDER]>]>,
]>;
