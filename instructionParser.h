#ifndef INSTRUCTIONPARSER_H
#define INSTRUCTIONPARSER_H

#include "instruction.h"
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <ctype.h>
using namespace std;
class InstructionParser
{
public:
    int loadProgram(string filepath, map<string, int> &labels)
    {
        ifstream file(filepath);
        if (!file.is_open())
        {
            cerr << "Unable to open file" << endl;
            exit(1);
        }
        string line;
        while (getline(file, line))
        {
            Instruction instruction = parseInstruction(line);
            if (instruction.opcode == "LABEL")
            {
                labels[instruction.operand1] = instructions.size();
                instructions.push_back(instruction);
            }
            else if (instruction.opcode == "INVALID") 
            {
                continue;
            }
            else
            {
                instructions.push_back(instruction);
            }
        }
        return instructions.size();
    }

    Instruction getInstruction(int index)
    {
        return instructions[index];
    }

    string getEncodedInstruction(int index) {
        return encodeInstruction(instructions[index]);
    }

    int getNumInstructions() {
        return instructions.size();
    }


private:
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
        else if(inst.opcode == "LABEL") {
            ans = "." + inst.operand1;
        }
        return ans;
    }
    
    vector<Instruction> instructions;

    Instruction parseInstruction(string line)
    {
        if (line.empty())
            return {"INVALID", "", "", 0};
        if (line[0] == '@')
            return {"INVALID", "", "", 0};
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == '@')
            {
                line = line.substr(0, i);
                break;
            }
        }
        while (line.back() == ' ')
            line.pop_back();
        while (line.front() == ' ')
            line = line.substr(1);
        if (line.empty())
            return {"INVALID", "", "", 0};
        if (line[0] == '.')
        {
            line = line.substr(1);
            while (line.back() == ' ')
                line.pop_back();
            while (line.front() == ' ')
                line = line.substr(1);
            return {"LABEL", line, "", 0};
        }
        string opcode;
        string operand1;
        string operand2;
        int immediate = 0;
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == ' ')
            {
                opcode = line.substr(0, i);
                for (int i = 0; i < opcode.size(); i++)
                    if (islower(opcode[i]))
                        opcode[i] = toupper(opcode[i]);
                line = line.substr(i + 1);
                break;
            }
            else {
                opcode = line;
            }
        }
        const set<string> opcode0 = {"HLT", "NOP", "NOT", "PUSH", "POP", "OUT", "IN", "RET", "CMP"};
        const set<string> opcode1 = {"ADD", "SUB", "MUL", "DIV", "MOD", // Arithmetic operations
                                     "AND", "OR", "XOR",                // Logical operations
                                     "LSL", "LSR", "ASR",               // Shift operations
                                     "LD", "ST"};
        const set<string> branchOpcodes = {"JMP", "JEQ", "JGT", "JNE", "JLT", "CALL"};
        const set<string> opcode2 = {"MOV", "SWAP"};
        const set<string> immxOpcode = {"MOVI"};
        if (opcode0.find(opcode) != opcode0.end())
        {
            return {opcode, "", "", 0};
        }
        if (opcode1.find(opcode) != opcode1.end())
        {
            operand1 = line;
            return {opcode, operand1, "", 0};
        }
        if (branchOpcodes.find(opcode) != branchOpcodes.end())
        {
            operand1 = line;
            return {opcode, operand1, "", 0};
        }
        if (opcode2.find(opcode) != opcode2.end())
        {
            int comma = -1;
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == ',')
                {
                    comma = i;
                }
            }
            for (int i = 0; i < comma; i++)
            {
                if (line[i] != ' ')
                    operand1 += line[i];
                else
                    break;
            }
            int i = comma + 1;
            while (line[i] == ' ')
            {
                i++;
            }
            while (i < line.size())
            {
                operand2 += line[i];
                i++;
            }
            return {opcode, operand1, operand2, 0};
        }
        if (immxOpcode.find(opcode) != immxOpcode.end())
        {
            int comma = -1;
            for (int i = 0; i < line.size(); i++)
            {
                if (line[i] == ',')
                {
                    comma = i;
                }
            }
            for (int i = 0; i < comma; i++)
            {
                if (line[i] != ' ')
                    operand1 += line[i];
                else
                    break;
            }
            int i = comma + 1;
            string s;
            while (line[i] == ' ')
            {
                i++;
            }
            while (i < line.size())
            {
                s += line[i];
                i++;
            }
            immediate = stoi(s);
            return { opcode, operand1, "", immediate };
        }
        return {"INVALID", "", "", 0};
    }
};

#endif
