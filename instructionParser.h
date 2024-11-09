#ifndef INSTRUCTIONPARSER_H
#define INSTRUCTIONPARSER_H

#include <instruction.h>
#include <vector>
#include <map>

class InstructionParser {
    public:
        void loadProgram(const vector<string> &prog, map<string, int> &labels) {
            
        }

        Instruction getInstruction(int index) {
            return instructions[index];
        }

    private:
        vector<Instruction> instructions;

        Instruction parseInstruction(const string &line) {
            
        }

};

#endif
