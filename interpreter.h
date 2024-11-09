#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <sstream>
#include "instructionParser.h"

using namespace std;

class Interpreter {
private:
    int ACC = 0;                     // Accumulator register
    map<string, int> memory; // Memory storage for addresses
    map<string, int> registers = { {"R1", 0}, {"R2", 0}, {"R3", 0}, {"RA", 0} }; // Extra registers
    stack<int> stk;                  // Stack for PUSH and POP operations
    map<string, int> labels; // Labels for control flow (mapping label to line index)
    int PC = 0;                      // Program counter
    int numInstructions;

    //flags registers
    bool greater;
    bool equal;

    bool toQuit = false;

    string filePath;

    InstructionParser parser;

public:
    void setFilePath(string path) {
        filePath = path;
    }

    void init() {
        numInstructions = parser.loadProgram(filePath, labels);
    }


    //executes the instruction and updates PC, flags, memory and registers
    void executeInstruction() {
        Instruction currentInstruction = parser.getInstruction(PC);
        const auto& opcode = currentInstruction.opcode;
        if(opcode == "ADD") {
            ACC += registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "SUB") {
            ACC -= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "MUL") {
            ACC *= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "DIV") {
            ACC /= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "MOD") {
            ACC %= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "AND") {
            ACC &= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "OR") {
            ACC |= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "XOR") {
            ACC ^= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "LSL") {
            ACC <<= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "LSR") {
            ACC = ((unsigned int)ACC) >> registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "ASR") {
            ACC >>= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "NOT") {
            ACC = ~ACC;
            PC++;
        }
        else if(opcode == "LD") {
            ACC = memory[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "ST") {
            memory[currentInstruction.operand1] = ACC;
            PC++;
        }
        else if(opcode == "MOV") {
            registers[currentInstruction.operand1] = registers[currentInstruction.operand2];
            PC++;
        }
        else if(opcode == "MOVI") {
            registers[currentInstruction.operand1] = currentInstruction.immediate;
            PC++;
        }
        else if(opcode == "SWAP") {
            int temp = registers[currentInstruction.operand1];
            registers[currentInstruction.operand1] = registers[currentInstruction.operand2];
            registers[currentInstruction.operand2] = temp;
            PC++;
        }
        else if(opcode == "CMP") {
            if(ACC > 0) {
                greater = true;
            }
            else {
                greater = false;
            }
            if(ACC == 0) {
                equal = true;
            }
            else {
                equal = false;
            }
            PC++;
        }
        else if(opcode == "JMP") {
            PC = labels[currentInstruction.operand1];
        }
        else if(opcode == "JEQ") {
            if(equal) {
                PC = labels[currentInstruction.operand1];
            }
        }
        else if(opcode == "JGT") {
            if(greater) {
                PC = labels[currentInstruction.operand1];
            }
        }
        else if(opcode == "JNE") {
            if(!equal) {
                PC = labels[currentInstruction.operand1];
            }
        }
        else if(opcode == "JLT") {
            if(!greater && !equal) {
                PC = labels[currentInstruction.operand1];
            }
        }
        else if(opcode == "PUSH") {
            stk.push(ACC);
        }
    }

    void run() {
        while(!toQuit) {
            executeInstruction();
            //The above function is not complete yet
            //Add input output logic here
        }
    }
};

#endif