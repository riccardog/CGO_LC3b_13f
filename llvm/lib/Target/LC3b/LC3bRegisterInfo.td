// Declaration that describe the LC3b register file


class LC3bReg<string n> : Register<n> {
	field bits <3> Num;
	let Namespace = "LC3b";
}

class LC3bGPRReg <bits<3> num, string n> : LC3bReg<n> {
	let Num=num;
}



//
// Registers
//

// Defining registers for LC3b architecture. 
// PSR and IR are not exposed to the code.
let Namespace = "LC3b" in {

	def R0 : LC3bGPRReg <0, "R0">, 	DwarfRegNum<[0]>;
	def R1 : LC3bGPRReg <1, "R1">, 	DwarfRegNum<[1]>;
	def R2 : LC3bGPRReg <2, "R2">, 	DwarfRegNum<[2]>;
	def R3 : LC3bGPRReg <3, "R3">, 	DwarfRegNum<[3]>;
	def R4 : LC3bGPRReg <4, "R4">, 	DwarfRegNum<[4]>;
	def R5 : LC3bGPRReg <5, "R5">, 	DwarfRegNum<[5]>;
	def R6 : LC3bGPRReg <6, "R6">, 	DwarfRegNum<[6]>;
	def R7 : LC3bGPRReg <7, "R7">, 	DwarfRegNum<[7]>;
	def PC : LC3bGPRReg <8, "PC">, 	DwarfRegNum<[9]>;
//	def PSR : LC3bGPRReg <9, "PSR">,DwarfRegNum<[8]>;
//	def IR : LC3bGPRReg <10, "IR">, DwarfRegNum<[10]>;

}



// Register Classes

/// check for third arguement 16
def LC3bRegs : RegisterClass <"LC3b", [i16], 16, 
	( add R0, R1, R2, R3, R4, R5,
		//not allocatable registers
	    R6, 	//Stack Pointer
	    R7, 	// Return Address
//	    PSR, 	// Status Register
//	    IR, 	// Instruction Register
	)>;
 
