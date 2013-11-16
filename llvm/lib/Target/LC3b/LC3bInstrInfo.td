//===- LC3bInstrInfo.td - Target Description for LC3b Target -*- tablegen -*-=//
//
//
//						The LLVM Compiler Infrastructure
//
// This file is a project of EE382V, Code Generation and Optimization course,
// at the University of Texas at Austin
//
//===----------------------------------------------------------------------===//
//
// This file contains the LC3b implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//
include "LC3bInstrFormats.td"

//===----------------------------------------------------------------------===//
// LC3b profiles and nodes
//===----------------------------------------------------------------------===//
def SDT_LC3bRet : SDTypeProfile< 0, 1, [SDTCisInt<0>] >;
// Return
def LC3bRet : SDNode<"LC3bISD::Ret", SDT_LC3bRet, [SDNPHasChain, SDNPOptInGlue]>;

//===----------------------------------------------------------------------===//
// LC3b Operand, Complex Patterns and Transformations Definitions.
//===----------------------------------------------------------------------===//
// Signed Operand
def simm4 : Operand<i16> {
	let DecoderMethod= "DecodeSimm4";
}

def simm5 : Operand<i16> {
	let DecoderMethod= "DecodeSimm5";
}

def simm6 : Operand<i16> {
	let DecoderMethod= "DecodeSimm6";
}




// Address operand (PatFrag)
def mem : Operand<i16> {
	let PrintMethod 	= "printMemOperand";
	let MIOperandInfo 	= (ops CPURegs, simm6);
	let EncoderMethod 	= "getMemEncoding";
}

// Node immediate fits as 4-bit sign extended on target immediate.
// SHF
def immSExt4 : PatLeaf<(imm), [{ return isInt<4>(N->getSExtValue()); }]>;

// Node immediate fits as 5-bit sign extended on target immediate.
// ADD, AND, XOR
def immSExt5 : PatLeaf<(imm), [{ return isInt<5>(N->getSExtValue()); }]>;

// Node immediate fits as 6-bit sign extended on target immediate.
// LDW, LDB, STW, STB
def immSExt6 : PatLeaf<(imm), [{ return isInt<6>(N->getSExtValue()); }]>;


// LC3b Address Mode! SDNode frameindex could possibily be a match
// since load and store instructions from stack used it.
def addr : ComplexPattern< iPTR, 2, "SelectAddr", [frameindex], [SDNPWantParent] >;

//===----------------------------------------------------------------------===//
// Pattern fragment for load/store
//===----------------------------------------------------------------------===//
class AlignedLoad<PatFrag Node> :
	PatFrag <(ops node:$ptr), (Node node:$ptr), 
				[{ 	
					LoadSDNode *LD = cast<LoadSDNode>(N);
					return LD->getMemoryVT().getSizeInBits()/8 <= LD->getAlignment();
				}]
			>;

class AlignedStore<PatFrag Node> :
	PatFrag <(ops node:$val, node:$ptr), (Node node:$val, node:$ptr), 
				[{
					StoreSDNode *SD = cast<StoreSDNode>(N);
					return SD->getMemoryVT().getSizeInBits()/8 <= SD->getAlignment();
				}]
			>;

// Load/Store PatFrags.
def load_a  : AlignedLoad<load>;
def store_a : AlignedStore<store>;

//===----------------------------------------------------------------------===//
// Instructions specific format
//===----------------------------------------------------------------------===//
// Arithmetic and logical instructions with 2 register operands.
<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmA>


class FAR< bit<4> op, (outs InstReg(outs IntRegs:$dst), (ins IntRegs:$b, IntRegs:$c), "xnor $b, $c, $dst", [(set i32:$dst, (not (xor i32:$b, i32:$c)))] >

multiclass FARITH < string OpcStr, bits<4> op, SDNode OpNode >




multiclass F3_12 <string OpcStr, bits<6> Op3Val, SDNode OpNode> {
  def rr  : F3_1 <2, Op3Val,
                 (outs IntRegs:$dst), (ins IntRegs:$b, IntRegs:$c),
                 !strconcat(OpcStr, " $b, $c, $dst"),
                 [(set i32:$dst, (OpNode i32:$b, i32:$c))]>;
  def ri  : F3_2 <2, Op3Val,
                 (outs IntRegs:$dst), (ins IntRegs:$b, i32imm:$c),
                 !strconcat(OpcStr, " $b, $c, $dst"),
                 [(set i32:$dst, (OpNode i32:$b, simm13:$c))]>;
}


class FAM < bits<4> op, (outs RC:$ra), (ins RC




def ADD : ArithLogicI<0x09, "addiu", add, simm16, immSExt16, CPURegs>;
/// Arithmetic and logical instructions with 2 register operands.


class FAM< bits<4> op, string instr_asm, SDNode OpNode, Operand Od, PatLeaf imm_type, RegisterClass RC> :
	FA<op, (outs RC:$ra), (ins RC:$rb, Od:$imm16), !strconcat(instr_asm, "\t$ra, $rb, $imm16"), [(set RC:$ra, (OpNode RC:$rb, imm_type:$imm16))], IIAlu> {
	let isReMaterializable = 1;
}

def LDW : FAM<0x5, "ldw", load, simm6, immSExt16, CPUReg


///////////////////////////////////////////////////////////////////////////////////////////////

class FMem< bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin >
: FA<op, outs, ins, asmstr, pattern, itin> {
	bits<6> addr;
//	let Inst{8-6}=rb;
	let Inst{5-0} = addr{5-0};
	let DecoderMethod = "DecodeMem";
}

// Memory Load/Store 
// FIXME - For the Opnode is it set to "add" explicitly here ( maybe because baseReg would be added with offset? )
// load_a, store_a -> PatFrag object here
// what is ops??
//class AlignedLoad<PatFrag Node> :
//	PatFrag <(ops node:$ptr), (Node node:$ptr), 
//				[{ 	
//					LoadSDNode *LD = cast<LoadSDNode>(N);
//					return LD->getMemoryVT().getSizeInBits()/8 <= LD->getAlignment();
//				}]
//			>;

//class AlignedStore<PatFrag Node> :
//	PatFrag <(ops node:$val, node:$ptr), (Node node:$val, node:$ptr), 
//				[{
//					StoreSDNode *SD = cast<StoreSDNode>(N);
//					return SD->getMemoryVT().getSizeInBits()/8 <= SD->getAlignment();
//				}]
//			>;

let canFoldAsLoad = 1 in
class LoadM< bits<4> op, string instr_asm, PatFrag OpNode, RegisterClass RC, Operand MemOpnd, bit Pseudo >
: FMem<op, (outs RC:$ra), (ins RC:$rb, MemOpnd:$addr), !strconcat(instr_asm, "\t$ra, $rb, $addr"), [(set RC:$ra, OpNode (add RC:$rb, MemOpnd:$addr))], FrmA>{
//	let Inst[5-0]= addr;
	let isPseudo = Pseudo;
}

class StoreM<bits<4> op, string instr_asm, PatFrag OpNode, RegisterClass RC, Operand MemOpnd, bit Pseudo>
: FMem<op, (outs), (ins RC:$ra, RC:$rb, MemOpnd:$addr), !strconcat(instr_asm, "\t$ra, $rb, $addr"), [(OpNode RC:$ra, (add RC:$rb, MemOpnd:$addr))], FrmA> {
	
	let isPseudo = Pseudo;
}

// LDW
multiclass LoadM16< bits<4> op, string instr_asm, PatFrag OpNode, bit Pseudo = 0> {
	def W : LoadM<op, instr_asm, OpNode, CPURegs, mem, Pseudo>;
}

// STW
multiclass StoreM16< bits<4> op, string instr_asm, PatFrag OpNode, bit Pseudo = 0> {
	def W : StoreM<op, instr_asm, OpNode, CPURegs, mem, Pseudo>;
}


// LDB
multiclass LoadM8< bits<4> op, string instr_asm, PatFrag OpNode, bit Pseudo = 0> {
	def B : LoadM<op, instr_asm, OpNode, CPURegs, mem, Pseudo>;
}

// STB
multiclass StoreM8< bits<4> op, string instr_asm, PatFrag OpNode, bit Pseudo = 0> {
	def B : StoreM<op, instr_asm, OpNode, CPURegs, mem, Pseudo>;
}
//===----------------------------------------------------------------------===//
// Instruction definition
//===----------------------------------------------------------------------===//



//===----------------------------------------------------------------------===//
// LC3b Instructions
//===----------------------------------------------------------------------===//
/// Load and Store Instructions ////////////////////////////////////////////////////
/// aligned
defm LDW : LoadM16<  0x6, "ldw", load_a  >; 
defm STW : StoreM16< 0x7, "stw", store_a >;
defm LDB : LoadM8<   0x2, "ldw", load_a  >; 
defm STB : StoreM8<  0x3, "stw", store_a >;


////////////////////////////////////////////////////////////////////////////////////

/// Arithmetic Instructions (ALU Immediate)/////////////////////////////////////////
// IR "add" defined in include/llvm/Target/TargetSelectionDAG.td, line 315 (def add).
def ADDI : ArithLogic_I<0x1, "add", add, simm5, immSExt5, CPURegs>;
def ANDI : ArithLogic_I<0x5, "and", and, simm5, immSExt5, CPURegs>;
def XORI : ArithLogic_I<0x9, "xor", xor, simm5, immSExt5, CPURegs>; 

def ADD : ArithLogic_R<0x1, "add", add, simm5, immSExt5, CPURegs>;
def AND : ArithLogic_R<0x5, "and", and, simm5, immSExt5, CPURegs>;
def XOR : ArithLogic_R<0x9, "xor", xor, simm5, immSExt5, CPURegs>; 

/// Arithmetic and logical instructions with 2 register operands.
//class ArithLogicI<bits<4> op, string instr_asm, SDNode OpNode, Operand Od, PatLeaf imm_type, RegisterClass RC> :
//FL<op, (outs RC:$ra), (ins RC:$rb, Od:$imm5), !strconcat(instr_asm, "\t$ra, $rb, $imm5"),
//[(set RC:$ra, (OpNode RC:$rb, imm_type:$imm16))], FrmA> {
//	let isReMaterializable = 1;
//}

class ArithLogic_I< bits<4> op, string instr_asm, SDNode OpNode, Operand Od, PatLeaf imm_type, RegisterClass RC > :
	FA<op, (outs RC:$ra), (ins RC:$rb, Od:$imm5), !strconcat(instr_asm, "\t$ra, $rb, $imm5"),
	[(set RC:$ra, (OpNode RC:$rb, imm_type:$imm5))], FrmA> {
		let isReMaterializable = 1;
		let Inst{5} = 1;
		let Inst{4-0} = imm5;
	}
class ArithLogic_R< bits<4> op, string instr_asm, SDNode OpNode, Operand Od, PatLeaf imm_type, RegisterClass RC > :
	FA<op, (outs RC:$ra), (ins RC:$rb, Od:$imm5), !strconcat(instr_asm, "\t$ra, $rb, $rc"),
	[(set RC:$ra, (OpNode RC:$rb, RC:$rc))], FrmA> {
		let isReMaterializable = 1;
		let Inst{5-3} = 0;
		let Inst{2-0} = rc;
	}

//FA<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmA>

///////////////////////////////////////////////////////////////////////////////////

//op = 0x09
//instr_asm = “addiu”
//OpNode = add
//Od = simm16
//imm_type = immSExt16
//RC = CPURegs

/// Shift Instructions ////////////////////////////////////////////////////////////
def LSHF  : ArithLogicII<0x1, "add", add, simm4, immSExt4, CPURegs>;
def RSHFL : ArithLogicII<0x5, "and", and, simm4, immSExt4, CPURegs>;
def RSHFA : ArithLogicI<0x9,  "xor", xor, simm4, immSExt4, CPURegs>; 
/// Arithmetic and logical instructions with 2 register operands.
class ShiftLogic<bits<4> op, string instr_asm, SDNode OpNode, Operand Od, PatLeaf imm_type, RegisterClass RC> :
FL<op, (outs RC:$ra), (ins RC:$rb, Od:$imm4), !strconcat(instr_asm, "\t$ra, $rb, $imm4"),
[(set RC:$ra, (OpNode RC:$rb, imm_type:$imm4))], FrmSHF> {
	let isReMaterializable = 1;
}
///////////////////////////////////////////////////////////////////////////////////

def RET : ArithLogicII<0x5, "and", and, simm4, immSExt4, CPURegs>;
def RSHFA : ArithLogicI<0x9,  "xor", xor, simm4, immSExt4, CPURegs>; 
/// Arithmetic and logical instructions with 2 register operands.
class ShiftLogic<bits<4> op, string instr_asm, SDNode OpNode, Operand Od, PatLeaf imm_type, RegisterClass RC> :
FL<op, (outs RC:$ra), (ins RC:$rb, Od:$imm4), !strconcat(instr_asm, "\t$ra, $rb, $imm4"),
[(set RC:$ra, (OpNode RC:$rb, imm_type:$imm4))], FrmSHF> {
	let isReMaterializable = 1;
}
///////////////////////////////////////////////////////////////////////////////////


/// JUMP Instructions /////////////////////////////////////////////////////////////
def JMP : FJ <0xc, (outs), (ins CPURegs:$target), "ret\t$target", [(LC3bRet CPURegs:$target)], FrmJ>;
def JSRR: FJ <0xc, (outs), (ins CPURegs:$target), "ret\t$target", [(LC3bRet CPURegs:$target)], FrmJ>;
let isReturn=1, isTerminator=1, hasDelaySlot=1, isCodeGenOnly=1, isBarrier=1, hasCtrlDep=1 in
def RET : FJ <0xc, (outs), (ins CPURegs:$target), "ret\t$target", [(LC3bRet CPURegs:$target)], FrmJ>;
///////////////////////////////////////////////////////////////////////////////////


/// JSR Instructions /////////////////////////////////////////////////////////////
def JMP : FJ <0xc, (outs), (ins CPURegs:$target), "ret\t$target", [(LC3bRet CPURegs:$target)], FrmJ>;
let isReturn=1, isTerminator=1, hasDelaySlot=1, isCodeGenOnly=1, isBarrier=1, hasCtrlDep=1 in
def RET : FJ <0xc, (outs), (ins CPURegs:$target), "ret\t$target", [(LC3bRet CPURegs:$target)], FrmJ>;
///////////////////////////////////////////////////////////////////////////////////

/// BR Instructions /////////////////////////////////////////////////////////////
def BR  : FJ <0xc, (outs), (ins CPURegs:$target), "ret\t$target", [(LC3bRet CPURegs:$target)], FrmJ>;


///////////////////////////////////////////////////////////////////////////////////

// FIXME, for the offset, $target is correct?
def JSR	: FC< 0x4, "jsr",  	(outs), (ins CPURegs:$target), "jsr\t$target", 	[(LC3bRet CPURegs:$target)], FrmC >;


/*
def TRAP: FD< 0xf, "trap", 	(outs), (ins CPURegs:$target), "trap\t$target", [(LC3bRet CPURegs:$target)], FrmD >;
def BR	: FE< 0x0, "br",	(outs), (ins CPURegs:$target), "br\t$target", 	[(LC3bRet CPURegs:$target)], FrmE >;
def LEA	: FE< 0xe, "lea",	(outs RC:$ra), (ins CPURegs:$target), "lea\t$ra, $target", [(LC3bRet CPURegs:$target)], FrmE >;
*/


///////////////////////////////////////////////////////////////////////////////////

//===----------------------------------------------------------------------===//
// Arbitrary patterns that map to one or more instructions
//===----------------------------------------------------------------------===//
// Small immediates
//def : Pat<(i16 immSExt8:$in), (ADD ZERO, imm:$in)>;


////////////////////////////////////////////////////////////////////////////////


