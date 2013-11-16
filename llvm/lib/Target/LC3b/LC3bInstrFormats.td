//===----------------------------------------------------------------------===//
// Describe LC3b instructions format
//
// CPU INSTRUCTION FORMATS
//
// opcode - operation code.
// ra - dst reg, only used on 3 regs instr.
// rb - src reg.
// rc - src reg (on a 3 reg instr).
// cx - immediate
//
//===----------------------------------------------------------------------===//
// Format specifies the encoding used by the instruction. This is part of the
// ad-hoc solution used to emit machine instruction encodings by our machine
// code emitter.


class Format<bits<4> val> {
	bits<4> Value =val;
}


def Pseudo 	: Format <0>;
def FrmA   	: Format <1>;	
def FrmB   	: Format <2>;
def FrmC   	: Format <3>;
def FrmD 	: Format <4>;
def FrmE 	: Format <5>; 	
//def FrmMEM : Format <6>; 	//load and store
//def FrmJSR : Format <7>; 
//def FrmLEA : Format <8>;
//def FrmTRAP: Format <9>;	//

// Generic LC3b Format
class LC3bInst<dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin, Format f>: Instruction
{
	field bits<16> Inst;
	Format Form = f;
	let Namespace = "LC3b";
	let Size = 2;
	bits<4> Opcode=0;

	// Top 4 bits are the ’opcode’ field
	let Inst{15-12} = Opcode;
	let OutOperandList = outs;
	let InOperandList = ins;
	let AsmString = asmstr;
	let Pattern = pattern;
	let Itinerary = itin;

	//
	// Attributes specific to LC3b instructions...
	//
	bits<3> FormBits = Form.Value;
	// TSFlags layout should be kept in sync with LC3bInstrInfo.h.
	let TSFlags{3-0} = FormBits;
	let DecoderNamespace = "LC3b";
	field bits<16> SoftFail = 0;
}

//===----------------------------------------------------------------------===//
// Format A instruction class in LC3b : <|opcode|ra|rb|Not Defined|>
//===----------------------------------------------------------------------===//

class FA<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
: LC3bInst<outs, ins, asmstr, pattern, itin, FrmA>
{
	bits<3> ra;
	bits<3> rb;
	let Opcode = op;
	let Inst{11-9}=ra;
	let Inst{8-6}=rb;
}

//===----------------------------------------------------------------------===//
// Format B instruction class in LC3b : <|opcode|000|ra|000000|>
//===----------------------------------------------------------------------===//
class FB<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
: LC3bInst<outs, ins, asmstr, pattern, itin, FrmB>
{
	bits<3> ra;
	let Opcode = op;
	let Inst{11-9}=0b000;
	let Inst{5-0}=0b000000;
}

//===----------------------------------------------------------------------===//
// Format C instruction class in LC3b : <|opcode|1|PC OFFSET 11 bits|>  -> JSRR
//===----------------------------------------------------------------------===//
class FC<bits<4> op, bits<11> addr, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
: LC3bInst<outs, ins, asmstr, pattern, itin, FrmC>
{
	let Opcode = op;
	let Inst{11}=1;
	let Inst{10-0}=addr;
}


class FD<bits<4> op, bits<8> trapvector, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
: LC3bInst<outs, ins, asmstr, pattern, itin, FrmD>
{
	let Opcode = op;
	let Inst{11-8}=0b0000;
	let Inst{7-0}=trapvector;
}


class FE<bits<4> op, bits<9> offset, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>
: LC3bInst<outs, ins, asmstr, pattern, itin, FrmE>
{
	bits<3> bcc;
	let Opcode = op;
	let Inst{11-8}=0b0000;
	let Inst{8-0}=offset;
}



//===----------------------------------------------------------------------===//
// Format L instruction class in LC3b : <|opcode|ra|rb|imm5|>
//===----------------------------------------------------------------------===//
//class FL<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern, InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmA>
//{
//	bits<3> ra;
//	bits<3> rb;
//	bits<5> imm5;
//	let Opcode = op;
//	let Inst{11-9} = ra;
//	let Inst{8-6} = rb;
//	let Inst{5}=1;
//	let Inst{4-0} = imm5;
//}


//===----------------------------------------------------------------------===//
// Format J instruction class in Cpu0 : <|opcode|1|address|>
//===----------------------------------------------------------------------===//
class FJ<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern,InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmJ>
{
	bits<3> base;
	let Opcode = op;
	let Inst{8-6} = base;
}


//===----------------------------------------------------------------------===//
// Format R instruction class in Cpu0 : <|opcode|000000000000|>
//===----------------------------------------------------------------------===//
class FR<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern,InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmRTI>
{
	let Opcode = op;
	let Inst{11-0} = 0b000000000000;
}


//===----------------------------------------------------------------------===//
// Format SHFT instruction class in Cpu0 : <|opcode|ra|rb|00|amnt4|>
//===----------------------------------------------------------------------===//

//FIXME : this format is not correct....10,00,11
class FS<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern,InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmSHF>
{
	bits<3> ra;
	bits<3> rb;
	bits<4> amnt4;
	let Opcode = op;
	let Inst{11-9} = ra;
	let Inst{8-6}  = rb;
	let Inst{3-0}  = amnt4;
}



//===----------------------------------------------------------------------===//
// Format MEM instruction class in Cpu0 : <|opcode|ra|rb|offset6|>
//===----------------------------------------------------------------------===//
class FM<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern,InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmMEM>
{
	bits<3> ra;
	bits<3> rb;
	bits<5> offt6;
	let Opcode = op;
	let Inst{11-9} = ra;
	let Inst{8-6} = rb;
	let Inst{5-0} = offt6;
}




//===----------------------------------------------------------------------===//
// Format JSR instruction class in Cpu0 : <|opcode|1|pcoffset11|>
//===----------------------------------------------------------------------===//
class FJSR<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern,InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmJSR>
{
	bits<11> ofst11;
	let Opcode = op;
	let Inst{11} = 1;
	let Inst{10-0} = ofst11;
}




//===----------------------------------------------------------------------===//
// Format LEA instruction class in Cpu0 : <|opcode|ra|pcoffset9|>
//===----------------------------------------------------------------------===//
class FLEA<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern,InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmLEA>
{
	bits<9> offt9;
	let Opcode = op;
	let Inst{8-0} = offt9;
}



//===----------------------------------------------------------------------===//
// Format TRAP instruction class in Cpu0 : <|opcode|0000|TrapVector|>
//===----------------------------------------------------------------------===//
class FT<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern,InstrItinClass itin>: LC3bInst<outs, ins, asmstr, pattern, itin, FrmTRAP>
{
	bits<8> offt8;
	let Opcode = op;
	let Inst{11-8} = 0;
	let Inst{7-0} = offt8;
}




