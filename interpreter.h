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
    vector<string> program;           // Holds the program lines
    int PC = 0;                      // Program counter

    string filePath;

    InstructionParser parser;

    // Helper to parse line into opcode and operand

    // Load a value from memory or register
    int loadValue(const string &address) {
        ACC = memory[address];
    }

    // Store value in memory or register
    void storeValue(const string &address) {
        memory[address] = ACC;
    }

public:
    void setFilePath(string path) {
        filePath = path;
    }

    void init() {

    }

    void run() {
        
    }
};

#endif