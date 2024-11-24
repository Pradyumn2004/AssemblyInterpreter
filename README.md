# AssemblyInterpreter

## Overview
This project is an **Assembly Language Interpreter** written in C++ that simulates an assembly-like instruction set for a hypothetical machine with basic registers, flags, and operations. The interpreter provides a means to execute assembly instructions sequentially, mimicking the behavior of low-level hardware processing, useful for learning assembly concepts and testing simple assembly programs.

### What is an Assembly Interpreter?
An assembly interpreter translates and executes each instruction written in a low-level, human-readable assembly language, simulating CPU operations. This project includes a limited Instruction Set Architecture (ISA) for a custom-designed virtual machine with key components such as an accumulator (single register), arithmetic and logic operations, and basic flow control.

## Instruction Set Architecture (ISA)

### Registers
The machine consists of several registers:
- **ACC**: Accumulator, the primary register used for most operations.
- **R1, R2, R3**: General-purpose registers.
- **RA**: Return Address register, used to store return addresses for subroutine calls.

### Flags
The machine includes two flags, set after certain comparison operations:
- **Flags.GT**: Set when a comparison result is greater than 0.
- **Flags.EQ**: Set when a comparison result is equal to 0.

### Instructions
This interpreter supports various types of instructions for arithmetic, logic, memory operations, control flow, and I/O.

#### Arithmetic Operations
- `ADD X`: Add the value in register `X` to `ACC`.
- `SUB X`: Subtract the value in register `X` from `ACC`.
- `MUL X`: Multiply `ACC` by the value in register `X`.
- `DIV X`: Divide `ACC` by the value in register `X`.
- `MOD X`: Set `ACC` to the remainder when divided by the value in register `X`.

#### Logical Operations
- `AND X`: Perform bitwise AND between `ACC` and register `X`.
- `OR X`: Perform bitwise OR between `ACC` and register `X`.
- `XOR X`: Perform bitwise XOR between `ACC` and register `X`.
- `LSL X`: Logical left shift `ACC` by `X` bits.
- `LSR X`: Logical right shift `ACC` by `X` bits.
- `ASR X`: Arithmetic right shift `ACC` by `X` bits.
- `NOT`: Perform bitwise NOT on `ACC`.

#### Memory Operations
- `LD X`: Load the value from memory address `X` into `ACC`.
- `ST X`: Store the value of `ACC` at memory address `X`.
- `MOV R, X`: Move the value from register `X` to register `R`.
- `MOVI R, immx`: Move immediate value `immx` to register `R`.
- `SWAP R1, R2`: Swap the values in registers `R1` and `R2`.

#### Comparison and Flag Setting
- `CMP`: Compare `ACC` with 0 and set `Flags.GT` if greater, and `Flags.EQ` if equal.

#### Control Flow
- `JMP LABEL`: Unconditional jump to `LABEL`.
- `JEQ LABEL`: Jump to `LABEL` if `Flags.EQ` is set.
- `JGT LABEL`: Jump to `LABEL` if `Flags.GT` is set.
- `JNE LABEL`: Jump to `LABEL` if `Flags.EQ` is not set.
- `JLT LABEL`: Jump to `LABEL` if `Flags.GT` is not set.

#### Stack Operations
- `PUSH`: Push the value of `ACC` onto the stack.
- `POP`: Pop the top value from the stack into `ACC`.

#### Subroutine Control
- `CALL LABEL`: Call a subroutine at `LABEL` and store the return address in `RA`.
- `RET`: Return from subroutine to the address in `RA`.

#### Input/Output Operations
- `IN`: Read a value from the user into `ACC`.
- `OUT`: Output the value of `ACC` to the display.

#### Miscellaneous
- `HLT`: Halt the execution.
- `NOP`: No operation, continue to the next instruction.

## Project Structure
#### Instruction Parsing
- The InstructionParser class is a C++ utility for parsing and managing instructions for an assembly-like language. It processes program files to load instructions, identify labels, and store them in an internal list. The class provides functionality to retrieve instructions by index and encode them into a specific string format for further processing or display. 
- It categorizes instructions based on predefined opcode sets, such as zero-operand (HLT, NOP), one-operand (ADD, SUB), two-operand (MOV, SWAP), branch operations (JMP, CALL), and immediate-value operations (MOVI). The parseInstruction method processes lines of code, discarding comments and whitespace, and validates the syntax to classify the instruction. Labels are treated as special instructions to map their names to respective line numbers. Invalid or unsupported instructions are ignored. The encodeInstruction method converts parsed instructions back into an easily readable string format. 
- Overall, the InstructionParser serves as a foundation for handling assembly-like programs by offering structured parsing, encoding, and label mapping capabilities.

#### Interpreter : Core Execution
- The Interpreter class is the central execution engine of the assembly language simulator, responsible for processing and executing instructions parsed from an input program. It integrates closely with the InstructionParser for fetching instructions and the IOHandler for user interaction and system updates. The class maintains essential components like a program counter (PC), registers (ACC, R1, R2, etc.), memory, flags (greater/equal for comparisons), and a stack for operations like PUSH and POP.
- It implements the logic for various opcodes, handling arithmetic (ADD, SUB, etc.), logical (AND, XOR), memory access (LD, ST), control flow (JMP, CALL, etc.), and I/O (IN, OUT) operations.



#### IOHandler
- The IOHandler class creates a terminal-based graphical interface using the ncurses library, designed to visualize the state of an assembly interpreter in real-time. 
- The interface is divided into several sections, each represented by a distinct window. The Instructions Window displays the list of assembly instructions, while the Register Windows (R1, R2, R3, RA, and ACC) show the current values of registers. The Flags Window indicates the status of the GT and EQ flags, and the Stack Window displays the stack contents. Additionally, the Memory Window lists memory addresses with their corresponding values, and the Output Window shows the result of executed instructions. User input is handled via the Input Window, and prompts like "Execute Instruction: e, Memory: u/d, Quit: q" are displayed in the Prompt Window. 
- This organized layout provides a clear and interactive way to debug and monitor the state of the interpreter, making it user-friendly and efficient for analyzing program execution.

## Future Enhancements
Potential improvements to expand functionality:
- Add support for additional logical operations.
- Extend memory management with dynamic allocation and freeing.
- Implement multi-threaded instruction handling.

## Usage

To run the interpreter:
1. Write the assembly program in a `.txt` file with supported instructions.
2. Install the ncurses library by running `sudo apt-get install libncurses-dev`
3. Build the interpreter by running `g++ main.cpp -lncurses -o main`
4. Run the binary by `./main {Program filepath}`

(the above steps are for unix based operating systems, if you're on windows, install ncurses library, compile and link ncurses and run the exe file with the cli argument as the filepath of the assembly program.)
