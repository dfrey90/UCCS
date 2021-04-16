/*
Daniel Frey
CS 4200-001
Program 1
9/11/18

This program takes in a specific file of machine code and translates/prints assembly language.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <iomanip>

using namespace std;

void readFile();
void processInstr(unsigned int instruction);
string convertReg(unsigned int reg);

//Read each line of the file and process it
void readFile()
{
	ifstream inFile;	//text file containing machine code
	unsigned int instruction;	//instruction from file

	//open file
	inFile.open("MachineCode.txt");

	//set file read in as hex and pad necessary 0's
	inFile >> hex;

	//iterate through each line of file, store line as instruction
	while (inFile >> instruction)
	{
		//deal with the instruction to determine how to break down
		processInstr(instruction);
	}

	//close file
	inFile.close();
}

//Breaks down the instruction according to the op code and outputs to screen
void processInstr(unsigned int instruction)
{
	unsigned int opCode, rs, rt, rd, shamt, func, imm, target;	//variables for parts of instructions
	
	//set op code, bit shift right 26 bits to right align op code
	opCode = instruction >> 26;

	//determine which instruction the opcode is
	switch (opCode)
	{
		case 15:
			//op code 001111 = 15 dec
			//lui rt imm-hex

			//bit shift, if necessary, then do 'and' to get only wanted bits
			rt = instruction >> 16 & 0x1f;
			imm = instruction & 0xffff;

			cout << "lui\t" << convertReg(rt) << ", " << hex << "0x" << imm << endl;
			break;

		case 13:
			//op code 001101 = 13 dec
			//ori rt rs imm-hex

			//bit shift, if necessary, then do 'and' to get only wanted bits
			rt = instruction >> 16 & 0x1f;
			rs = instruction >> 21 & 0x1f;
			imm = instruction & 0xffff;

			cout << "ori\t" << convertReg(rt) << ", " << convertReg(rs) << ", " 
				<< "0x" << hex << setw(4) << setfill('0') << imm << endl;
			break;

		case 35:
			//op code 100011 = 35 dec
			//lw rt offset($s#)

			//bit shift, if necessary, then do 'and' to get only wanted bits
			rs = instruction >> 21 & 0x1f;
			rt = instruction >> 16 & 0x1f;

			cout << "lw\t" << convertReg(rt) << ", " << "0(" << convertReg(rs)<< ")" << endl;
			break;

		case 8:
			//op code 001000 = 8 dec
			//addi rt rs imm-dec

			//bit shift, if necessary, then do 'and' to get only wanted bits
			rt = instruction >> 16 & 0x1f;
			rs = instruction >> 21 & 0x1f;
			imm = instruction & 0xffff;
			
			//dec to output as number, short to get just 16bits of imm
			cout << "addi\t" << convertReg(rt) << ", " << convertReg(rs) << ", " << dec << (short) imm << endl;
			break;

		case 4:
			//op code 000100 = 4 dec
			//beq rs rt imm=offset-hex

			//bit shift, if necessary, then do 'and' to get only wanted bits
			rt = instruction >> 16 & 0x1f;
			rs = instruction >> 21 & 0x1f;
			imm = instruction & 0xffff;

			cout << "beq\t" << convertReg(rs) << ", " << convertReg(rt) << ", "
				<< "0x" << hex << setw(4) << setfill('0') << imm << endl;
			break;

		case 43:
			//op code 101011 = 43 dec
			//sw rt offset($s#)

			//bit shift, if necessary, then do 'and' to get only wanted bits
			rs = instruction >> 21 & 0x1f;
			rt = instruction >> 16 & 0x1f;

			cout << "sw\t" << convertReg(rt) << ", " << "0(" << convertReg(rs) << ")" << endl;
			break;

		case 2:
			//op code 000010 = 2 dec
			//j target

			//bit shift, if necessary, then do 'and' to get only wanted bits
			target = instruction << 2 & 0xfffffff;
			
			cout << "j\t" << "0x" << hex << setw(8) << setfill('0') << target << endl;
			break;

		default:
			//op code 000000

			//get only the last 6 bits of function code, 'and' to make first 26 bits 0
			func = instruction & 0x3f;

			//if add else syscall
			if (func == 32)
			{
				//add rd rs rt

				//bit shift, if necessary, then do 'and' to get only wanted bits
				rs = instruction >> 21 & 0x1f;
				rt = instruction >> 16 & 0x1f;
				rd = instruction >> 11 & 0x1f;
				shamt = instruction >> 6 & 0x1f;

				cout << "add\t" << convertReg(rd) << ", " << convertReg(rs) << ", " << convertReg(rt) << endl;
			}
			else
			{
				//syscall
				cout << "syscall" << endl;
			}
	}//end switch
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

int main()
{
	//start reading the input file
	readFile();

	return 0;
}