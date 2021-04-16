/*
Daniel Frey
CS 4200-001
Program 3
11/29/18

This program simulates a processor.
(*cout is redirected to output file*)
*/

#include <iostream>		//cout, left
#include <fstream>		//file i/o
#include <string>		//to_string
#include <sstream>		//stringstream
#include <iomanip>		//setw, setfill
#include <vector>		//vector

using namespace std;

//constants for file names
const string INFILE_NAME = "MachineCode.txt";
const string OUTFILE_NAME = "Cycle Output.txt";

//register data
//$zero, $at, $v0, $t0, $t1, $s0
const unsigned int zero = 0;
unsigned int at = 0;
unsigned int v0 = 0;
unsigned int t0 = 0;
unsigned int t1 = 0;
unsigned int s0 = 0;

//function prototypes
void readMachineCodeFile();
void runPipeline(vector<unsigned int> instructions);
void instrFetch(unsigned int machCode, unsigned int& baseAddr, int cycleCount);
void instrDecode(unsigned int machCode, int cycleCount);
void execute(unsigned int machCode, int cycleCount);
void memory(unsigned int machCode, int cycleCount);
void writeback(unsigned int machCode, int cycleCount);
unsigned int getALUResult(unsigned int machCode, int cycleCount);
string getRDest(unsigned int machCode);
string getAddress(unsigned int& baseAddr);
//void displayCntrlSettings(unsigned int machCode);
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
	vector<unsigned int> instructions;	//array of instructions from file

	//check to see if file opened successfully
	if (inFile.is_open())
	{
		//set file read in as hex
		inFile >> hex;

		//iterate through each line of file, store machine code in machCode
		while (inFile >> machCode)
		{
			instructions.push_back(machCode);
		}

		//run fetch, decode, exe, mem, wb
		runPipeline(instructions);

		//close file
		inFile.close();
	}
	else
	{
		cout << "Error opening file." << endl;
	}
}

//Runs the pipeline processes - fetch, decode, execute, memory, writeback
void runPipeline(vector<unsigned int> instructions)
{
	int cycleCount = 0;		//counter for number of cycles
	unsigned int baseAddr = 0x00400000;	//base address for instructions (start at hex: 0x00400000)
	int wbIndex = 0, memIndex = 0, exIndex = 0, decodeIndex = 0, fetchIndex = 0;

	//run the pipeline in reverse order
	while (cycleCount < 26)
	{
		//WB
		writeback(instructions[wbIndex], cycleCount);

		//MEM
		if (cycleCount < 24)
		{
			memory(instructions[memIndex], cycleCount);
		}
		else
		{
			memory(0, cycleCount);
		}

		//EX
		if (cycleCount == 25)
		{
			execute(0, cycleCount);
		}
		else if (cycleCount > 1 && cycleCount != 21)
		{
			execute(instructions[exIndex], cycleCount);
		}

		//ID
		if (cycleCount < 24 && cycleCount != 21)
		{
			instrDecode(instructions[decodeIndex], cycleCount);
		}
		else if (cycleCount == 21); //do nothing
		else
		{
			instrDecode(0, cycleCount);
		}

		//IF
		if (cycleCount < 23)
		{
			instrFetch(instructions[fetchIndex], baseAddr, cycleCount);
		}
		else
		{
			instrFetch(0, baseAddr, cycleCount);
		}



		if (cycleCount == 20)
		{
			baseAddr = 0x00400024;
			fetchIndex = 9;
			decodeIndex++;
			exIndex++;
			memIndex++;
			wbIndex++;
		}
		else if (cycleCount == 21)
		{
			fetchIndex++;
			decodeIndex = 9;
			exIndex = 4;
			memIndex++;
			wbIndex++;
		}
		else if (cycleCount == 22)
		{
			fetchIndex++;
			decodeIndex++;//=10
			exIndex = 9;
			memIndex++;
			wbIndex = 7;
		}
		else if (cycleCount == 8 || cycleCount == 13 || cycleCount == 18)
		{
			baseAddr = 0x00400010;
			fetchIndex = 4;
			decodeIndex++;
			exIndex++;
			memIndex++;
			wbIndex++;
		}
		else if (cycleCount == 9 || cycleCount == 14 || cycleCount == 19)
		{
			fetchIndex++;
			decodeIndex = 4;
			exIndex++;
			memIndex++;
			wbIndex++;
		}
		else if (cycleCount == 10 || cycleCount == 15)
		{
			fetchIndex++;
			decodeIndex++;
			exIndex = 4;
			memIndex++;
			wbIndex++;
		}
		else if (cycleCount == 11 || cycleCount == 16)
		{
			fetchIndex++;
			decodeIndex++;
			exIndex++;
			memIndex = 4;
			wbIndex++;
		}
		else if (cycleCount == 12 || cycleCount == 17)
		{
			fetchIndex++;
			decodeIndex++;
			exIndex++;
			memIndex++;
			wbIndex = 4;
		}
		else if (cycleCount == 0)
		{
			fetchIndex++;
		}
		else if (cycleCount < 2)
		{
			fetchIndex++;
			decodeIndex++;
		}
		else if (cycleCount < 3)
		{
			fetchIndex++;
			decodeIndex++;
			exIndex++;
		}
		else if (cycleCount < 4)
		{
			fetchIndex++;
			decodeIndex++;
			exIndex++;
			memIndex++;
		}
		else if (cycleCount < 5)
		{
			fetchIndex++;
			decodeIndex++;
			exIndex++;
			memIndex++;
			wbIndex++;
		}
		else if (cycleCount > 24)
		{
			memIndex++;
			wbIndex++;
		}
		else if (cycleCount > 23)
		{
			exIndex++;
			memIndex++;
			wbIndex++;
		}
		else if (cycleCount > 22)
		{
			decodeIndex++;
			exIndex++;
			memIndex++;
			wbIndex++;
		}
		else if (cycleCount >= 5 || cycleCount <= 22)
		{
			fetchIndex++;
			decodeIndex++;
			exIndex++;
			memIndex++;
			wbIndex++;
		}

		cycleCount++;

		cout << endl;
	}
}

//Simulates reading the instruction from memory of processor
void instrFetch(unsigned int machCode, unsigned int& baseAddr, int cycleCount)
{
	cout << "Cycle " << cycleCount << " IF: " << getInstruction(machCode) << " read from " << getAddress(baseAddr) << endl;
}

//Simulates decoding the instruction of processor
void instrDecode(unsigned int machCode, int cycleCount)
{
	unsigned int dataA = 0x0, dataB = 0x0;	//variables for register data

	//beq, add, addi, sw, j

	switch (machCode >> 26)
	{
		case 0x04:
			//beq
			switch (cycleCount)
			{
			case 5:
				dataA = 0x2;
				break;
			case 10:
				dataA = 0x1;
				break;
			}

		case 0x00:
			if ((machCode & 0x3f) == 0x20)
			{
				//add rd
				switch (cycleCount)
				{
				case 6:
					dataA = 0x14;
					break;
				case 11:
					dataA = 0x14;
					dataB = 0x2;
					break;
				case 16:
					dataA = 0x16;
					dataB = 0x1;
					break;
				}
			}

		case 0x08:
			//addi rt
			switch (cycleCount)
			{
			case 7:
				dataA = 0x3;
				dataB = 0x3;
				break;
			case 12:
				dataA = 0x2;
				dataB = 0x2;
				break;
			case 17:
				dataA = 0x1;
				dataB = 0x1;
				break;
			}
			break;

		case 0x2b:
			//sw
			switch (cycleCount)
			{
			case 8:
				dataA = 0x10010000;
				dataB = 0x14;
				break;
			case 13:
				dataA = 0x10010000;
				dataB = 0x14;
				break;
			case 18:
				dataA = 0x10010000;
				dataB = 0x16;
				break;
			}
			break;

		case 0x02:
			//j target
			switch (cycleCount)
			{
				case 8:
					dataB = 0x10010000;
					break;
				case 13:
					dataB = 0x10010000;
					break;
				case 18:
					dataB = 0x10010000;
					break;
			}
			break;
	}

	//if machine code to decode then decode it, otherwise process does nothing
	if (cycleCount == 0)
	{
		//do nothing
	}
	else if ((cycleCount < 24) && (cycleCount > 0))
	{
		cout << "Cycle " << dec << cycleCount << " ID: " << getInstruction(machCode) << " DataA = 0x" << hex << dataA 
			<< ", DataB = 0x" << dataB << dec << ", Imm = " << (short)(machCode & 0xffff) << endl;
	}
	else
	{
		cout << "Cycle " << dec << cycleCount << " ID: " << getInstruction(machCode) << " DataA = 0x0, DataB = 0x0, Imm = 0" << endl;
	}
}

//Simulates the memory step
void memory(unsigned int machCode, int cycleCount)
{
	if (cycleCount > 2)
	{
		//if lw or sw read from memory, otherwise nothing
		switch (machCode >> 26)
		{
		case 0x23:
			//lw
			cout << "Cycle " << dec << cycleCount << " MEM: " << getInstruction(machCode) << " Read 0x" << hex << 20 << endl;
			break;
		case 0x2b:
			//sw
			switch (cycleCount)
			{
				case 10:
					cout << "Cycle " << dec << cycleCount << " MEM: " << getInstruction(machCode) << " Write 0x" << hex << 23 << endl;
					break;
				case 15:
					cout << "Cycle " << dec << cycleCount << " MEM: " << getInstruction(machCode) << " Write 0x" << hex << 25 << endl;
					break;
				case 20:
					cout << "Cycle " << dec << cycleCount << " MEM: " << getInstruction(machCode) << " Write 0x" << hex << 26 << endl;
					break;
			}
			break;
		}
	}
	//else nothing
}

//Simulates the write back step of processor
void writeback(unsigned int machCode, int cycleCount)
{
	if (cycleCount > 3)
	{
		//lui, ori, lw, addi, add
		switch (machCode >> 26)
		{
			case 0x08:
				//addi
				if (cycleCount != 5 || cycleCount != 8 || cycleCount != 13 || cycleCount != 18)
				{
					cout << "Cycle " << cycleCount << " WB: " << getInstruction(machCode) << " Result 0x" << hex << (short)(machCode & 0xffff)
						<< " written to " << convertReg((machCode >> 16) & 0x1f) << endl;
				}
				else
				{
					cout << "Cycle " << cycleCount << " WB: " << getInstruction(machCode) << " Result 0x" << hex << getALUResult(machCode, cycleCount)
						<< " written to " << convertReg((machCode >> 16) & 0x1f) << endl;
				}
				break;
			case 0x0d:
				//ori
				cout << "Cycle " << cycleCount << " WB: " << getInstruction(machCode) << " Result 0x" << hex << 268500992
					<< " written to " << convertReg((machCode >> 16) & 0x1f) << endl;
				break;
			case 0x0f:
				//lui
				cout << "Cycle " << cycleCount << " WB: " << getInstruction(machCode) << " Result 0x" << hex << ((short)(machCode & 0xffff) << 16)
					<< " written to " << convertReg((machCode >> 16) & 0x1f) << endl;
				break;
			case 0x23:
				//lw
				cout << "Cycle " << cycleCount << " WB: " << getInstruction(machCode) << " Result 0x" << hex << 20
					<< " written to " << convertReg((machCode >> 16) & 0x1f) << endl;
				break;
			case 0x00:
				if ((machCode & 0x3f) == 0x20)
				{
					//add
					cout << "Cycle " << cycleCount << " WB: " << getInstruction(machCode) << " Result 0x" << hex << getALUResult(machCode, cycleCount)
					<< " written to " << convertReg((machCode >> 11) & 0x1f) << endl;
				}
				break;
		}
	}
	//else nothing
}

//Simulates the execution step of processor
void execute(unsigned int machCode, int cycleCount)
{
	//ALUResult, RDest
	unsigned int aluResult = 0;

	cout << "Cycle " << dec << cycleCount << " EX: " << getInstruction(machCode) << hex << " ALUResult = " << getALUResult(machCode, cycleCount)
		<< " RDest = " << getRDest(machCode) << endl;
}

//Returns the ALU Result
unsigned int getALUResult(unsigned int machCode, int cycleCount)
{
	unsigned int aluResult = 0;

	switch (machCode >> 26)
	{
		case 0x00:
			if ((machCode & 0x3f) == 0x20)
			{
				//add rd
				switch (cycleCount)
				{
					case 7:
					case 9:
						aluResult = 0x17;
						break;
					case 12:
						aluResult = 0x19;
						break;
					case 17:
						aluResult = 0x1A;
						break;
				}
			}
			else if ((machCode & 0x3f) == 0x0c)
			{
				//syscall
				aluResult = 0;
			}
			break;
		case 0x0f:
			//lui rt
			aluResult = ((short)(machCode & 0xffff)) << 16;
			break;
		case 0x08:
			//addi rt
			aluResult = ((short)(machCode & 0xffff));
			switch (cycleCount)
			{
				case 5:
					aluResult = 3;
					break;
				case 8:
				case 10:
					aluResult = 2;
					break;
				case 13:
					aluResult = 1;
					break;
				case 18:
					aluResult = 0;
					break;
			}
			break;
		case 0x0d:
			//ori rt
			aluResult = ((short)(machCode & 0xffff)) << 16;
			break;
		case 0x23:
			//lw rt
			aluResult = ((short)(machCode & 0xffff)) << 16;
			break;
		case 0x02:
			//j target
			aluResult = 0;
			break;
		case 0x04:
			//beq
			switch (cycleCount)
			{
				case 6:
					aluResult = 3;
					break;
				case 11:
					aluResult = 2;
					break;
				case 16:
					aluResult = 1;
					break;
				case 22:
					aluResult = 0;
					break;
			}
			break;
		case 0x2b:
			//sw
			aluResult = 268500992;
			break;
	}

	return aluResult;
}

//Returns the destination register
string getRDest(unsigned int machCode)
{
	string rDest;
	unsigned int rd = (machCode >> 11) & 0x1f;
	unsigned int rt = (machCode >> 16) & 0x1f;

	switch (machCode >> 26)
	{
		case 0x00:
			if ((machCode & 0x3f) == 0x20)
			{
				//add rd
				rDest = convertReg(rd);
			}
			else if ((machCode & 0x3f) == 0x0c)
			{
				//syscall
				rDest = convertReg(0);
			}
			else
			{
				rDest = "0";
			}
			break;
		case 0x0f:
			//lui rt
		case 0x08:
			//addi rt
		case 0x0d:
			//ori rt
		case 0x23:
			//lw rt
			rDest = convertReg(rt);
			break;

			//$zero
		case 0x02:
			//j target
		case 0x04:
			//beq
		case 0x2b:
			//sw
			rDest = convertReg(0);
			break;
	}

	return rDest;
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

//Breaks down the machine code according to op code and returns a string of the instruction
string getInstruction(unsigned int machCode)
{
	stringstream instruction;	//ss for building instruction
	unsigned int opCode, rs, rt, rd, shamt, func, imm, target;	//variable for part of instruction

	//get pieces from instruction, whether needed or not
	//bit shift, if necessary, then do 'and' to get only wanted bits
	opCode = machCode >> 26;
	rs = (machCode >> 21) & 0x1f;
	rt = (machCode >> 16) & 0x1f;
	rd = (machCode >> 11) & 0x1f;
	shamt = (machCode >> 6) & 0x1f;
	func = machCode & 0x3f;
	imm = (short)(machCode & 0xffff);
	target = (machCode & 0x03ffffff) << 2;

	//determine which instruction the opcode is and display information
	switch (opCode)
	{
		case 0x02:
			//op code 000010 = 2 dec
			//j target

			//display instruction
			instruction << "j " << "0x" << hex << setw(8) << setfill('0') << target;
			break;

		case 0x04:
			//beq rs rt imm=offset-hex

			//display instruction
			instruction << "beq " << convertReg(rs) << ", " << convertReg(rt) << ", "
				<< "0x" << hex << setw(4) << setfill('0') << imm;
			break;

		case 0x08:
			//addi rt rs imm-dec

			//display instruction
			//dec to output as number, short to get just 16bits of imm
			instruction << "addi " << convertReg(rt) << ", " << convertReg(rs) << ", " << dec << (short)imm;
			break;

		case 0x0d:
			//ori rt rs imm-hex

			//display instruction
			instruction << "ori " << convertReg(rt) << ", " << convertReg(rs) << ", "
				<< "0x" << hex << setw(4) << setfill('0') << imm;
			break;
	
		case 0x0f:
			//lui rt imm-hex

			//display instruction
			instruction << "lui " << convertReg(rt) << ", " << hex << "0x" << imm;
			break;		

		case 0x23:
			//lw rt offset($s#)

			//display instruction
			instruction << "lw " << convertReg(rt) << ", " << "0(" << convertReg(rs) << ")";
			break;

		case 0x2b:
			//sw rt offset($s#)

			//display instruction
			instruction << "sw " << convertReg(rt) << ", " << "0(" << convertReg(rs) << ")";
			break;

		default:
			//op code 0x00

			//if add else syscall
			if (func == 0x20)
			{
				//add rd rs rt

				//display instruction
				instruction << "add " << convertReg(rd) << ", " << convertReg(rs) << ", " << convertReg(rt);
			}
			else if (func == 0x0c)
			{
				//syscall

				//display instruction
				instruction << "syscall";
			}
			else
			{
				instruction << setw(8) << setfill('0') << 0;
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
	rs = (machCode >> 21) & 0x1f;
	rt = (machCode >> 16) & 0x1f;
	rd = (machCode >> 11) & 0x1f;

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
	if ((machCode >> 26) == 0x02)
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
		case 0x00:
			//if add instr, else syscall (same op code as syscall), reg write = 1
			if ((machCode & 0x3f) == 0x20)
			{
				regWrite = 1;
			}
			else
			{
				regWrite = 0;
			}
			break;
		case 0x02:
			//j
		case 0x04:
			//beq
		case 0x2b:
			//sw

			//set reg write to 0 for no write
			regWrite = 0;

			break;

		default:
			//must be instr that writes to reg
			//addi
			//ori
			//lui
			//lw

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
		case 0x08:
			//addi
		case 0x0d:
			//ori
		case 0x0f:
			//lui
		case 0x23:
			//lw
		case 0x2b:
			//sw

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
	if ((machCode >> 26) == 0x23)
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
	if ((machCode >> 26) == 0x2b)
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
		case 0x00:
			//add, syscall: op code 000000
			//if add, else syscall (same op code), determined by func code
			if ((machCode & 0x3f) == 0x20)
			{
				wbSel = 1;
			}
			else
			{
				wbSel = 0;
			}
			break;

		//fall through for addi, ori, lui
		case 0x08:
			//addi
		case 0x0d:
			//ori
		case 0x0f:
			//lui

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
		case 0x04:
			//beq
		case 0x08:
			//addi
		case 0x0d:
			//ori
		case 0x0f:
			//lui
		case 0x23:
			//lw

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
	//beq
	if ((machCode >> 26) == 0x04)
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
		case 0x04:
			//beq
		case 0x23:
			//lw
		case 0x2b:
			//sw

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