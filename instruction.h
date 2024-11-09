#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
using namespace std;

struct Instruction
{
    string opcode;
    string operand1;
    string operand2;
    int immediate;
};

#endif
