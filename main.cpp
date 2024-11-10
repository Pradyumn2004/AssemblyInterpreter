#include <bits/stdc++.h>
#include "interpreter.h"

using namespace std;


// int main(int argc, char *argv[]) {
//     Interpreter interpreter;

//     if(argc == 1) {
//         cout << "Usage: " << argv[0] << " <file>" << endl;
//         return 1;
//     }

//     interpreter.setFilePath(argv[1]);
//     interpreter.init();
    
//     // Load the program and run the interpreter
//     interpreter.run();

//     return 0;
// }

int main() {
    Interpreter interpreter;

    interpreter.setFilePath("Program.txt");
    interpreter.init();
    
    // Load the program and run the interpreter
    interpreter.run();
    return 0;
}
