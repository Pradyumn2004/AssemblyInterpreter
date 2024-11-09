#include <bits/stdc++.h>
#include "interpreter.h"

using namespace std;


int main() {
    Interpreter interpreter;

    // Sample program demonstrating various instructions
    vector<string> program = {
        "LD 10",        // Load memory location 10 into ACC
        "ADD 20",       // Add memory location 20 to ACC
        "OUT",          // Output result (should be 10 + 20)
        "PUSH",         // Push ACC to stack
        "LD 5",         // Load 5 into ACC
        "POP",          // Pop top of stack into ACC
        "OUT",          // Output result (should be 30)
        "HLT"           // Halt execution
    };

    // Load the program and run the interpreter
    interpreter.loadProgram(program);
    interpreter.run();

    return 0;
}
