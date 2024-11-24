#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <sstream>
#include "instructionParser.h"
#include "IOHandler.h"

using namespace std;

class Interpreter {
private:
    map<string, int> memory; // Memory storage for addresses
    map<string, int> registers = { {"R1", 0}, {"R2", 0}, {"R3", 0}, {"RA", 0}, {"ACC",0}}; // Registers
    stack<int> stk;                  // Stack for PUSH and POP operations
    map<string, int> labels; // Labels for control flow (mapping label to line index)
    int PC = 0;                      // Program counter
    int numInstructions;

    IOHandler ioHandler;

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
        ioHandler.setInstructionParser(&parser);
        ioHandler.init();
        ioHandler.updatePC(PC);
    }


    //executes the instruction and updates PC, flags, memory and registers
    void executeInstruction() {
        if(PC >= numInstructions) {
            ioHandler.updateOutput("Program Finished");
            toQuit = true;
            return;
        }
        Instruction currentInstruction = parser.getInstruction(PC);
        const auto& opcode = currentInstruction.opcode;
        if(opcode == "ADD") {
            registers["ACC"] += registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "SUB") {
            registers["ACC"] -= registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "MUL") {
            registers["ACC"] *= registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "DIV") {
            registers["ACC"] /= registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "MOD") {
            registers["ACC"] %= registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "AND") {
            registers["ACC"] &= registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "OR") {
            registers["ACC"] |= registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "XOR") {
            registers["ACC"] ^= registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "LSL") {
            registers["ACC"] <<= registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "LSR") {
            registers["ACC"] = ((unsigned int)registers["ACC"]) >> registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "ASR") {
            registers["ACC"] >>= registers[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "NOT") {
            registers["ACC"] = ~registers["ACC"];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "LD") {
            registers["ACC"] = memory[currentInstruction.operand1];
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "ST") {
            memory[currentInstruction.operand1] = registers["ACC"];
            ioHandler.updateMemory(currentInstruction.operand1, memory[currentInstruction.operand1]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "MOV") {
            registers[currentInstruction.operand1] = registers[currentInstruction.operand2];
            ioHandler.updateRegister(currentInstruction.operand1, registers[currentInstruction.operand1]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "MOVI") {
            registers[currentInstruction.operand1] = currentInstruction.immediate;
            ioHandler.updateRegister(currentInstruction.operand1, registers[currentInstruction.operand1]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "SWAP") {
            int temp = registers[currentInstruction.operand1];
            registers[currentInstruction.operand1] = registers[currentInstruction.operand2];
            registers[currentInstruction.operand2] = temp;
            ioHandler.updateRegister(currentInstruction.operand1, registers[currentInstruction.operand1]);
            ioHandler.updateRegister(currentInstruction.operand2, registers[currentInstruction.operand2]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "CMP") {
            if(registers["ACC"] > 0) {
                greater = true;
            }
            else {
                greater = false;
            }
            if(registers["ACC"] == 0) {
                equal = true;
            }
            else {
                equal = false;
            }
            ioHandler.updateFlags(greater, equal);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "JMP") {
            PC = labels[currentInstruction.operand1];
            ioHandler.updatePC(PC);
        }
        else if(opcode == "JEQ") {
            if(equal) {
                PC = labels[currentInstruction.operand1];
                ioHandler.updatePC(PC);
            }
        }
        else if(opcode == "JGT") {
            if(greater) {
                PC = labels[currentInstruction.operand1];
                ioHandler.updatePC(PC);
            }
        }
        else if(opcode == "JNE") {
            if(!equal) {
                PC = labels[currentInstruction.operand1];
                ioHandler.updatePC(PC);
            }
        }
        else if(opcode == "JLT") {
            if(!greater && !equal) {
                PC = labels[currentInstruction.operand1];
                ioHandler.updatePC(PC);
            }
        }
        else if(opcode == "PUSH") {
            stk.push(registers["ACC"]);
            ioHandler.pushIntoStack(registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "POP") {
            registers["ACC"] = stk.top();
            stk.pop();
            ioHandler.popFromStack();
            ioHandler.updateRegister("ACC", registers["ACC"]);
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "CALL") {
            registers["RA"] = PC+1;
            ioHandler.updateRegister("RA", registers["RA"]);
            PC = labels[currentInstruction.operand1];
            ioHandler.updatePC(PC);
        }
        else if(opcode == "RET") {
            PC = registers["RA"];
            ioHandler.updatePC(PC);
        }
        else if(opcode == "HLT") {
            toQuit = true;
        }
        else if(opcode == "IN"){
            ioHandler.updateOutput("Enter the input");
            registers["ACC"] = ioHandler.takeInput(false);
            PC++;
            ioHandler.updateRegister("ACC", registers["ACC"]);
            ioHandler.updatePC(PC);
        }
        else if(opcode == "OUT"){
            ioHandler.updateOutput(to_string(registers["ACC"]));
            PC++;
            ioHandler.updatePC(PC);
        }
        else if(opcode == "LABEL"){
            PC++;
            ioHandler.updatePC(PC);
        }
        else {
            throw(1);
        }
    }

    void run() {
        bool quit = false;
        while(!toQuit) {
            char inp = ioHandler.takeInput(true);
            ioHandler.updateOutput("");
            switch(inp) {
                case 'e':
                    executeInstruction();
                    break;
                case 'u':
                    break;
                case 'd':
                    break;
                case 'q':
                    toQuit = true;
                    quit = true;
                    break;                
            }
            if(toQuit) break;
        }

        if(!quit) getch();
    }
};

#endif