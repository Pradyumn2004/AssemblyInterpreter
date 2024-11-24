#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <ncurses.h>
#include <map>
#include <vector>
#include <stack>
#include "instruction.h"
#include "instructionParser.h"

using namespace std;

class IOHandler {
public:

    ~IOHandler() {
        // delwin(r1);
        // delwin(r2);
        // delwin(r3);
        // delwin(acc);
        // delwin(ra);
        // delwin(ins);
        // delwin(pcw);
        // delwin(mem);
        // delwin(prompt);
        // delwin(io);
        endwin();
    }

    void updateRegister(string index, int value){
        registers[index] = value;
        if(index == "R1") {
            wclear(r1);
            box(r1,0,0);
            mvwprintw(r1,1,1,"R1: %d",value);
            wrefresh(r1);
        }
        else if(index == "R2") {
            wclear(r2);
            box(r2,0,0);
            mvwprintw(r2,1,1,"R2: %d",value);
            wrefresh(r2);
        }
        else if(index == "R3") {
            wclear(r3);
            box(r3,0,0);
            mvwprintw(r3,1,1,"R3: %d",value);
            wrefresh(r3);
        }
        else if(index == "RA") {
            wclear(ra);
            box(ra,0,0);
            mvwprintw(ra,1,1,"RA: %d",value);
            wrefresh(ra);
        }
        else if(index == "ACC") {
            wclear(acc);
            box(acc,0,0);
            mvwprintw(acc,1,1,"ACC: %d",value);
            wrefresh(acc);
        }
    }

    void updateFlags(bool gt, bool eq) {
        greater = gt;
        equal = eq;
        wclear(flagsw);
        box(flagsw,0,0);
        mvwprintw(flagsw,1,1,"GT: %d",greater);
        mvwprintw(flagsw,2,1,"EQ: %d",equal);
        wrefresh(flagsw);
    }

    void updateMemory(string index, int value){
        if(memory.find(index) == memory.end()) {
            if(memoryPointer == -1) {
                memoryPointer = 0;
            }
        }
        int cnt = 0;
        memory[index] = value;
        auto start = memory.begin();
        if(memoryPointer) {
            start = next(memory.begin(), memoryPointer);            
        }
        wclear(mem);
        
        box(mem,0,0);

        mvwprintw(mem,1,1,"MEMORY");

        for(auto it = start; it != memory.end() && cnt < 8; it++) {
            mvwprintw(mem,++cnt,1,"%s:%d",it->first.c_str(), it->second);
        }

        wrefresh(mem);
    }

    void updateMemoryPointer(int delta) {
        if(memoryPointer == -1) return;
        memoryPointer += delta;
        if(memoryPointer < 0) memoryPointer = 0;
        else updateMemory(memory.begin()->first, memory.begin()->second);
    }

    void updatePC(int value){
        PC = value;
        wclear(pcw);
        wclear(ins);
        box(ins,0,0);

        int start, end;

        int l = parser->getNumInstructions();

        if(PC < 9) {
            start = 0;
            end = min(16, (int)l - 1);
        }
        else {
            if(PC + 8 < l) {
                start = PC - 8;
                end = PC + 8;
            }
            else {
                if(l >  16) {
                    start = l - 17;
                    end = l - 1;
                }
                else {
                    start = 0;
                    end = l - 1;
                }
            }
        }

        mvwprintw(pcw,PC-start,0,"->");
        wrefresh(pcw);

        for(int i = start; i <= end; i++) {
            string inst = parser->getEncodedInstruction(i);
            mvwprintw(ins,i-start+1,1,"%s",inst.c_str());
        }
        wrefresh(ins);

    }    

    void updateOutput(string output) {
        this->output = output;
        wclear(outp);
        box(outp,0,0);
        mvwprintw(outp,1,1,"OUTPUT:");
        mvwprintw(outp,2,1,"%s",output.c_str());
        wrefresh(outp);
    }

    void updateInput(string input) {
        wclear(inp);
        box(inp,0,0);
        mvwprintw(inp,0,0,"INPUT:");
        mvwprintw(inp,0,0,"%s",input.c_str());
        wrefresh(inp);
    }

    void pushIntoStack(int value) {
        stk.push(value);
        stack<int> temp = stk;
        wclear(stackw);
        box(stackw,0,0);
        mvwprintw(stackw,1,1,"STACK");
        int i = 2;
        while(!temp.empty() && i < 10) {
            mvwprintw(stackw,i++,1,"%d",temp.top());
            temp.pop();
        }
        wrefresh(stackw);
    }

    void popFromStack() {
        stk.pop();
        stack<int> temp = stk;
        wclear(stackw);
        box(stackw,0,0);
        mvwprintw(stackw,1,1,"STACK");
        int i = 2;
        while(!temp.empty() && i < 10) {
            mvwprintw(stackw,i++,1,"%d",temp.top());
            temp.pop();
        }
        wrefresh(stackw);
    }

    void init() {
        initscr();
        noecho();
        cbreak();

        box(stdscr,0,0);

        refresh();

        prompt = newwin(3, 78, 1, 1);
        box(prompt,0,0);
        mvwprintw(prompt,1,1,"Execute Instruction: e, Memory: u/d, Quit: q");
        wrefresh(prompt);

        ins = newwin(19,22, 4, 3);
        box(ins,0,0);
        wrefresh(ins);

        pcw = newwin(17,2,5,1);
        mvwprintw(pcw,0,0,"->");
        wrefresh(pcw);

        outp = newwin(4,37,15,42);
        box(outp,0,0);
        mvwprintw(outp,1,1,"OUTPUT:");
        mvwprintw(outp,2,1,"%s",output.c_str());
        wrefresh(outp);

        inp = newwin(4, 13, 19, 42);
        box(inp,0,0);
        mvwprintw(inp,1,1,"INPUT:");
        wrefresh(inp);

        r1 = newwin(3, 17, 4, 25);
        box(r1,0,0);
        mvwprintw(r1,1,1,"R1: %d",registers["R1"]);
        wrefresh(r1);

        r2 = newwin(3, 17, 7, 25);
        box(r2,0,0);
        mvwprintw(r2,1,1,"R2: %d",registers["R2"]);
        wrefresh(r2);

        r3 = newwin(3, 17, 10, 25);
        box(r3,0,0);
        mvwprintw(r3,1,1,"R3: %d",registers["R3"]);
        wrefresh(r3);

        ra = newwin(3, 17, 13, 25);
        box(ra,0,0);
        mvwprintw(ra,1,1,"RA: %d",registers["RA"]);
        wrefresh(ra);

        acc = newwin(3, 17, 16, 25);
        box(acc,0,0);
        mvwprintw(acc,1,1,"ACC: %d",registers["ACC"]);
        wrefresh(acc);

        mem = newwin(11, 24, 4, 42);
        box(mem,0,0);
        mvwprintw(mem,1,1,"MEMORY");
        wrefresh(mem);

        flagsw = newwin(4, 7, 19, 30);
        box(flagsw,0,0);
        mvwprintw(flagsw,1,1,"GT: %d",greater);
        mvwprintw(flagsw,2,1,"EQ: %d",equal);
        wrefresh(flagsw);

        stackw = newwin(11, 13, 4, 66);
        box(stackw,0,0);
        mvwprintw(stackw,1,1,"STACK");
        wrefresh(stackw);


    }


    pair<int,bool> takeInput(bool isChar) {
        if(isChar) {
            char a;
            while(true) {
                a = getch();
                if(a == 'e' || a == 'u' || a == 'd' || a == 'q') {
                    updateOutput("");
                    return make_pair(a,false);
                }
                else {
                    updateOutput("INVALID INPUT!");
                }
            }
        }
        else {
            char a;
            string res;
            while(true) {
                a = getch();
                if(a == '\n'){
                    if(res.empty()) {
                        updateOutput("INVALID INPUT!");
                        continue;
                    }
                    updateOutput("");
                    int result;
                    if(res[0] == '-') {
                        result = -stoi(res.substr(1));
                    }
                    else {
                        result = stoi(res);
                    }
                    return make_pair(result,false);
                }
                else if(a == 'q') {
                    return make_pair(-1,true);
                }
                else if(a == '-') {
                    if(res.empty()) {
                        res += a;
                        updateOutput("");
                    }
                    else{
                        updateOutput("INVALID INPUT!");
                    }
                }
                else if(a < '0' || a > '9') {
                    updateOutput("INVALID INPUT!");
                }
                else {
                    res += a;
                    updateOutput("");
                    updateInput(res);
                }
            }
        }
    }

    void setInstructionParser(InstructionParser* parser) {
        this->parser = parser;
    }

private:
    map<string,int> registers = { {"R1", 0}, {"R2", 0}, {"R3", 0}, {"RA", 0}, {"ACC",0}};
    map<string,int> memory;
    bool greater = false;
    bool equal = false;
    int PC = 0;
    WINDOW* r1;
    WINDOW* r2;
    WINDOW* r3;
    WINDOW* acc;
    WINDOW* ra;
    WINDOW* ins;
    WINDOW* pcw;
    WINDOW* mem;
    WINDOW* prompt;
    WINDOW* inp;
    WINDOW* outp;
    WINDOW* flagsw;
    WINDOW* stackw;
    string output;
    int numInstructions;
    int memoryPointer = -1;
    stack<int> stk;
    InstructionParser* parser;
};

#endif