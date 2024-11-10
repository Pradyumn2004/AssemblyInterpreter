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
    map<string, int> memory; // Memory storage for addresses
    map<string, int> registers = { {"R1", 0}, {"R2", 0}, {"R3", 0}, {"RA", 0}, {"ACC",0}}; // Registers
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
        if(PC >= numInstructions) {
            cout << "Program Completed\n";
            toQuit = true;
            return;
        }
        Instruction currentInstruction = parser.getInstruction(PC);
        const auto& opcode = currentInstruction.opcode;
        if(opcode == "ADD") {
            registers["ACC"] += registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "SUB") {
            registers["ACC"] -= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "MUL") {
            registers["ACC"] *= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "DIV") {
            registers["ACC"] /= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "MOD") {
            registers["ACC"] %= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "AND") {
            registers["ACC"] &= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "OR") {
            registers["ACC"] |= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "XOR") {
            registers["ACC"] ^= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "LSL") {
            registers["ACC"] <<= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "LSR") {
            registers["ACC"] = ((unsigned int)registers["ACC"]) >> registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "ASR") {
            registers["ACC"] >>= registers[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "NOT") {
            registers["ACC"] = ~registers["ACC"];
            PC++;
        }
        else if(opcode == "LD") {
            registers["ACC"] = memory[currentInstruction.operand1];
            PC++;
        }
        else if(opcode == "ST") {
            memory[currentInstruction.operand1] = registers["ACC"];
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
            stk.push(registers["ACC"]);
        }
        else if(opcode == "POP") {
            registers["ACC"] = stk.top();
            stk.pop();
        }
        else if(opcode == "CALL") {
            stk.push(PC);
            PC = labels[currentInstruction.operand1];
        }
        else if(opcode == "RET") {
            PC = stk.top();
            stk.pop();
        }
        else if(opcode == "HLT") {
            toQuit = true;
        }
        else if(opcode == "IN"){
            cout << "Enter a number: ";
            cin >> registers["ACC"];
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            PC++;
        }
        else if(opcode == "OUT"){
            cout << "Output: " << registers["ACC"] << endl;
            PC++;
        }
        else if(opcode == "LABEL"){
            PC++;
        }
        else {
            throw(1);
        }
    }

    void output() {
        cout << "ACC: " << registers["ACC"] << endl;
        cout << "Memory: " << endl;
        for(auto& [key, value] : memory) {
            cout << key << ": " << value << endl;
        }
        cout << "Registers: " << endl;
        for(auto& [key, value] : registers) {
            cout << key << ": " << value << endl;
        }
        cout << "Stack: " << endl;
        auto temp = stk;
        while(!temp.empty()) {
            cout << temp.top() << endl;
            temp.pop();
        }
    }

    void insertString(string* out, string str, int i, int j) {
        for(int k = 0; k < str.size() && j+k < 80; k++) {
            out[i][j+k] = str[k];
        }
    }

    void interactiveOutput() {
        string out[24];
        for(int i = 0; i < 24; i++) {
            out[i].assign(80,' ');
        }
    }

    string encodeInstruction(Instruction inst) {
        string ans;
        const set<string> opcode0 = {"HLT", "NOP", "NOT", "PUSH", "POP", "OUT", "IN", "RET", "CMP"};
        const set<string> opcode1 = {"ADD", "SUB", "MUL", "DIV", "MOD", // Arithmetic operations
                                     "AND", "OR", "XOR",                // Logical operations
                                     "LSL", "LSR", "ASR",               // Shift operations
                                     "LD", "ST"};
        const set<string> branchOpcodes = {"JMP", "JEQ", "JGT", "JNE", "JLT", "CALL"};
        const set<string> opcode2 = {"MOV", "SWAP"};
        const set<string> immxOpcode = {"MOVI"};

        if(opcode0.find(inst.opcode) != opcode0.end()) {
            ans = inst.opcode;
        }
        else if(opcode1.find(inst.opcode) != opcode1.end()) {
            ans = inst.opcode + " " + inst.operand1;
        }
        else if(branchOpcodes.find(inst.opcode) != branchOpcodes.end()) {
            ans = inst.opcode + " " + inst.operand1;
        }
        else if(opcode2.find(inst.opcode) != opcode2.end()) {
            ans = inst.opcode + " " + inst.operand1 + ", " + inst.operand2;
        }
        else if(immxOpcode.find(inst.opcode) != immxOpcode.end()) {
            ans = inst.opcode + " " + inst.operand1 + ", " + to_string(inst.immediate);
        }
        return ans;
    }

    void run() {
        while(!toQuit) {
            executeInstruction();
            output();    
            getchar();
        }
    }
};

#endif