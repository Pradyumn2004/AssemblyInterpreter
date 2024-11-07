#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <sstream>

using namespace std;

class Interpreter {
private:
    int ACC = 0;                     // Accumulator register
    map<string, int> memory; // Memory storage for addresses
    map<string, int> registers = { {"R1", 0}, {"R2", 0}, {"R3", 0} }; // Extra registers
    stack<int> stk;                  // Stack for PUSH and POP operations
    map<string, int> labels; // Labels for control flow (mapping label to line index)
    vector<string> program;           // Holds the program lines

    // Helper to parse line into opcode and operand
    pair<string, string> parseInstruction(const string &line) {
        
    }

    // Load a value from memory or register
    int loadValue(const string &operand) {
        
    }

    // Store value in memory or register
    void storeValue(const string &operand, int value) {
        
    }

public:
    void loadProgram(const vector<string> &prog) {
        
    }

    void run() {
        for (int i = 0; i < program.size(); ++i) {
            auto [opcode, operand] = parseInstruction(program[i]);

            // Ignore label lines
            if (opcode.back() == ':') continue;

            // Execute the instruction
            
        }
    }
};