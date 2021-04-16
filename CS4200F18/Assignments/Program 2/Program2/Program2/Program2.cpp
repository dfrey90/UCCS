/*
Daniel Frey
CS 4200-001
Program 2
10/23/18

Single-Cycle Execution:
This program takes in a specific file of machine code and produces a table of control signal settings.
(*cout is redirected to output file*)
*/

#include <iostream>		//cout, left
#include <fstream>		//file i/o
#include <string>		//to_string
#include <sstream>		//stringstream
#include <iomanip>		//setw, setfill

using namespace std;

//constants for file names
const string INFILE_NAME = "MachineCode.txt";
const string OUTFILE_NAME = "Control Signal Settings.txt";

void readMachineCodeFile();
string getAddress(unsigned int& baseAddr);
void displayCntrlSettings(unsigned int machCode);
string getInstruction(unsigned int machCode);
string convertReg(unsigned int reg);
string getRegData(unsigned int machCode);
string getOpCode(unsigned int machCode);
string getFuncCode(unsigned int machCode);
unsigned int getJump(unsigned int machCode);
unsigned int getRegWrite(unsigned int machCode);
unsigned int getAluSrc(unsigned int machCode);
unsigned int getMemRd(unsigned int machCode);
unsigned int getMemWr(unsigned int machCode);
unsigned int getWbSel(unsigned int machCode);
unsigned int getRdRtSel(unsigned int machCode);
unsigned int getBeq(unsigned int machCode);
unsigned int getAlu(unsigned int machCode);

//Set up table and read each line of the file and process it
void readMachineCodeFile()
{
	ifstream inFile(INFILE_NAME);	//text file containing machine code
	unsigned int machCode;	//instruction from file
	unsigned int baseAddr = 4194304;	//base address for instructions (start at hex: 0x00400000)

	//check to see if file opened successfully
	if (inFile.is_open())
	{
		//output table header
		cout<<"Address\t\tMachine Code\tInstruction\t\t"
			"Rd\tRs\tRt\tOp\tFunc\tJump\t"
			"RegWrite\tALU Src\tMem Rd\tMem Wr\tWB Sel\tRd/Rt Sel\tbeq\tALU\t" << endl;

		//set file read in as hex
		inFile >> hex;

		//iterate through each line of file, store line as machCode
		while (inFile >> machCode)
		{
			//display address in hex with specific width padded with 0's
			cout << getAddress(baseAddr) << '\t';

			//display machine code from file
			cout << "0x" << hex << setw(8) << setfill('0') << hex << machCode << '\t';

			//deal with the machine code to display required info
			displayCntrlSettings(machCode);
		}

		//close file
		inFile.close();
	}
	else
	{
		cout << "Error opening file." << endl;
	}
}

//Returns the address for the instruction and increments for next
string getAddress(unsigned int& baseAddr)
{
	stringstream address;	//ss for address in hex

	//build address string
	address << "0x" << hex << setw(8) << setfill('0') << baseAddr;

	//increment the address
	baseAddr = baseAddr + 4;

	return address.str();
}

//Breaks down the machine code and displays instructions and control signals
void displayCntrlSettings(unsigned int machCode)
{
	//display instruction, align left where necessary
	//Rd	Rs	Rt	Op	Fct	Jump	RegWrite	ALU Src	Mem Rd	Mem Wr	WB Sel	Rd/Rt Sel	beq	ALU
	cout << left << setw(20) << setfill(' ') << getInstruction(machCode) << '\t';
	cout << getRegData(machCode) << '\t';
	cout << getOpCode(machCode) << '\t';
	cout << getFuncCode(machCode) << '\t';
	cout << getJump(machCode) << '\t';
	cout << getRegWrite(machCode) << "\t\t";
	cout << getAluSrc(machCode) << '\t';
	cout << getMemRd(machCode) << '\t';
	cout << getMemWr(machCode) << '\t';
	cout << getWbSel(machCode) << '\t';
	cout << getRdRtSel(machCode) << "\t\t";
	cout << getBeq(machCode) << '\t';
	cout << getAlu(machCode) << endl;
}

//Breaks down the machine code according to op code and returns a string of the instruction
string getInstruction(unsigned int machCode)
{
	stringstream instruction;	//ss for building instruction
	unsigned int opCode, rs, rt, rd, shamt, func, imm, target;	//variable for part of instruction

	//get pieces from instruction, whether needed or not
	//set op code, bit shift right 26 bits to right align op code
	opCode = machCode >> 26;
	//bit shift, if necessary, then do 'and' to get only wanted bits
	rs = machCode >> 21 & 0x1f;
	rt = machCode >> 16 & 0x1f;
	rd = machCode >> 11 & 0x1f;
	shamt = machCode >> 6 & 0x1f;
	func = machCode & 0x3f;
	imm = machCode & 0xffff;
	target = machCode << 2 & 0xfffffff;

	//determine which instruction the opcode is and display information
	switch (opCode)
	{
		case 15:
			//op code 001111 = 15 dec
			//lui rt imm-hex

			//display instruction
			instruction << "Lui " << convertReg(rt) << ", " << hex << "0x" << imm;
			break;

		case 13:
			//op code 001101 = 13 dec
			//ori rt rs imm-hex

			//display instruction
			instruction << "Ori " << convertReg(rt) << ", " << convertReg(rs) << ", "
				<< "0x" << hex << setw(4) << setfill('0') << imm;
			break;

		case 35:
			//op code 100011 = 35 dec
			//lw rt offset($s#)

			//display instruction
			instruction << "Lw " << convertReg(rt) << ", " << "0(" << convertReg(rs) << ")";
			break;

		case 8:
			//op code 001000 = 8 dec
			//addi rt rs imm-dec

			//display instruction
			//dec to output as number, short to get just 16bits of imm
			instruction << "Addi " << convertReg(rt) << ", " << convertReg(rs) << ", " << dec << (short)imm;
			break;

		case 4:
			//op code 000100 = 4 dec
			//beq rs rt imm=offset-hex

			//display instruction
			instruction << "Beq " << convertReg(rs) << ", " << convertReg(rt) << ", "
				<< "0x" << hex << setw(4) << setfill('0') << imm;
			break;

		case 43:
			//op code 101011 = 43 dec
			//sw rt offset($s#)

			//display instruction
			instruction << "Sw " << convertReg(rt) << ", " << "0(" << convertReg(rs) << ")";
			break;

		case 2:
			//op code 000010 = 2 dec
			//j target

			//display instruction
			instruction << "J " << "0x" << hex << setw(8) << setfill('0') << target;
			break;

		default:
			//op code 000000

			//if add else syscall
			if (func == 32)
			{
				//add rd rs rt

				//display instruction
				instruction << "Add " << convertReg(rd) << ", " << convertReg(rs) << ", " << convertReg(rt);
			}
			else
			{
				//syscall
				//display instruction
				instruction << "Syscall";
			}
	}//end switch

	return instruction.str();
}

//Change registers rs or rt to the correct register form/number
string convertReg(unsigned int reg)
{
	int regNum;	//used to hold the correct register number

				//determine which register category it belongs to
	if (reg == 0)
	{
		return "$zero";
	}
	else if (reg == 1)
	{
		return "$at";
	}
	else if (2 <= reg && reg <= 3)
	{
		regNum = reg - 2;
		return "$v" + to_string(regNum);
	}
	else if (8 <= reg && reg <= 15)
	{
		regNum = reg - 8;
		return "$t" + to_string(regNum);
	}
	else
		//else if (16 <= reg && reg <= 23)
	{
		regNum = reg - 16;
		return "$s" + to_string(regNum);
	}
}

//Returns register data: Rd, Rs, Rt
string getRegData(unsigned int machCode)
{
	stringstream regData;	//ss to contain each register data
	unsigned int rs, rt, rd;	//register data variable

	//break down machine code
	//bit shift, if necessary, then do 'and' to get only wanted bits
	rs = machCode >> 21 & 0x1f;
	rt = machCode >> 16 & 0x1f;
	rd = machCode >> 11 & 0x1f;

	//feed rs, rt, rd into ss
	regData << rd << '\t' << rs << '\t' << rt;

	return regData.str();
}

//Returns op code in hex
string getOpCode(unsigned int machCode)
{
	unsigned int opCode;	//variable to hold op code
	stringstream opCodeStr;	//ss for op code

	//set op code, bit shift right 26 bits to right align op code
	opCode = machCode >> 26;

	opCodeStr << "0x" << setw(2) << setfill('0') << hex << opCode;

	return opCodeStr.str();
}

//Returns function code in hex
string getFuncCode(unsigned int machCode)
{
	stringstream funcCode;	//ss for building function code
	unsigned int func;	//variable for holding calculated func code from machine code

	//if instruction from machine code has function code, build it (if add or syscall)
	//test op code == 0
	if ((machCode >> 26) == 0)
	{
		//'and' to get rid of unnecessary bits
		func = machCode & 0x3f;

		//build func code
		funcCode << "0x" << setw(2) << setfill('0') << hex << func;
	}
	else
	{
		funcCode << ' ';
	}

	return funcCode.str();
}

//Returns the jump value 0,1
unsigned int getJump(unsigned int machCode)
{
	unsigned int jump;	//variable for setting for jump

	//if jump instruction, assign 1, else 0
	if ((machCode >> 26) == 2)
	{
		jump = 1;
	}
	else
	{
		jump = 0;
	}

	return jump;
}

//Returns 0,1 value if instruction writes to register
unsigned int getRegWrite(unsigned int machCode)
{
	unsigned int regWrite;	//variable for register write setting

	//determine instr by opcode (machCode bit-shifted 26 right), and set register write setting
	//beq, sw, j, syscall don't write to register
	switch (machCode >> 26)
	{
		//determine if instr has no reg write, otherwise is reg write
		case 0:
			//if add instr, else syscall (same op code as syscall), reg write = 1
			if ((machCode & 0x3f) == 32)
			{
				regWrite = 1;
			}
			else
			{
				regWrite = 0;
			}
			break;
		case 2:
			//op code 000010 = 2 dec
			//j
		case 4:
			//op code 000100 = 4 dec
			//beq
		case 43:
			//op code 101011 = 43 dec
			//sw

			//set reg write to 0 for no write
			regWrite = 0;

			break;

		default:
			//must be instr that writes to reg
			//addi: op code 001000 = 8 dec
			//ori: op code 001101 = 13 dec
			//lui: op code 001111 = 15 dec
			//lw: op code 100011 = 35 dec

			//set reg write
			regWrite = 1;
	}//end switch

	return regWrite;
}

//Returns ALU Src value, 0 if reg reads data B, or 1 for sign extender
unsigned int getAluSrc(unsigned int machCode)
{
	unsigned int aluSrc;	//variable hold alu src value

	//addi, lui, lw, ori, sw = 1
	//add, beq = 0

	//determine type of instruction by op code
	switch (machCode >> 26)
	{
		//fall through for addi, lui, lw, ori, sw
		case 8:
			//addi: op code 001000 = 8 dec
		case 13:
			//ori: op code 001101 = 13 dec
		case 15:
			//lui: op code 001111 = 15 dec
		case 35:
			//lw: op code 100011 = 35 dec
		case 43:
			//sw: op code 101011 = 43 dec

			aluSrc = 1;
			break;
		default:
			aluSrc = 0;
	}

	return aluSrc;
}

//Returns 1 if load instruction, otherwise returns 0
unsigned int getMemRd(unsigned int machCode)
{
	unsigned int memRd;	//variable for read from memory

	//if load instruction, else not load instruction
	//lw: op code 100011 = 35 dec
	if ((machCode >> 26) == 35)
	{
		memRd = 1;
	}
	else
	{
		memRd = 0;
	}

	return memRd;
}

//Returns 1 if store instruction, otherwise returns 0
unsigned int getMemWr(unsigned int machCode)
{
	unsigned int memWr;	//variable for write to memory

	//if store instruction, else not store instruction
	//sw: op code 101011 = 43 dec
	if ((machCode >> 26) == 43)
	{
		memWr = 1;
	}
	else
	{
		memWr = 0;
	}

	return memWr;
}

//Returns where write back occurs from, 0 if from ALU, 1 if from memory
unsigned int getWbSel(unsigned int machCode)
{
	unsigned int wbSel;	//variable for where write back occurs from

	//determine type of instr by opcode and set write back selector
	switch (machCode >> 26)
	{
		//beq, sw, j, sys
		//lw = 0
		//add, addi, lui, ori = 1
		case 0:
			//add, syscall: op code 000000
			//if add, else sys call (same op code) 
			if ((machCode & 0x3f) == 32)
			{
				wbSel = 1;
			}
			else
			{
				wbSel = 0;
			}
			break;

		//fall through for addi, ori, lui
		case 8:
			//addi: op code 001000 = 8 dec
		case 13:
			//ori: op code 001101 = 13 dec
		case 15:
			//lui: op code 001111 = 15 dec

			wbSel = 1;
			break;

		default:
			wbSel = 0;
	}

	return wbSel;
}

//Returns 0 if R-type instruction, otherwise returns 1 for not R-type
unsigned int getRdRtSel(unsigned int machCode)
{
	unsigned int rdRtSel;	//variable for R-type instr or not

	//determine instr type by opcode
	switch (machCode >> 26)
	{
		//add=0
		//fall through for:
		//addi, beq, lui, lw, ori = 1
		case 4:
			//beq: op code 000100 = 4 dec
		case 8:
			//addi: op code 001000 = 8 dec
		case 13:
			//ori: op code 001101 = 13 dec
		case 15:
			//lui: op code 001111 = 15 dec
		case 35:
			//lw: op code 100011 = 35 dec

			rdRtSel = 1;
			break;

		default:
			rdRtSel = 0;
	}

	return rdRtSel;
}

//Returns 1 if branch instruction, otherwise returns 0
unsigned int getBeq(unsigned int machCode)
{
	unsigned int branch;	//variable to tell if branch

	//if branch instruction, else not branch instruction
	//beq: op code 000100 = 4 dec
	if ((machCode >> 26) == 4)
	{
		branch = 1;
	}
	else
	{
		branch = 0;
	}

	return branch;
}

//Returns the arithmetic/logic unit value, 0=add, 1=subtract, 2=<<16 imm, 3=or imm
unsigned int getAlu(unsigned int machCode)
{
	unsigned int alu;	//variable for ALU

	//add, addi = 0
	//beq, lw, sw = 1

	switch (machCode >> 26)
	{
		//fall through for beq, lw, sw
		case 4:
			//beq: op code 000100 = 4 dec
		case 35:
			//lw: op code 100011 = 35 dec
		case 43:
			//sw: op code 101011 = 43 dec

			alu = 1;
			break;
		default:
			alu = 0;
	}

	return alu;
}

int main()
{
	ofstream outFile(OUTFILE_NAME);	//output file variable
	auto coutBuf = cout.rdbuf(outFile.rdbuf()); //save and redirect cout to file
	
	//start reading the input file
	readMachineCodeFile();

	//close output file
	outFile.close();

	return 0;
}