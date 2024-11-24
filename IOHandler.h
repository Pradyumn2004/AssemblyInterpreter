#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <ncurses.h>
#include <map>
#include <vector>
#include <stack>
#include "instruction.h"

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

    void setNumInstructions(int num) {
        numInstructions = num;
    }

    void updateRegister(string index, int value){
        registers[index] = value;
    }

    void updateFlags(bool gt, bool eq) {
        greater = gt;
        equal = eq;
    }

    void updateMemory(string index, int value){
        memory[index] = value;
    }

    void updatePC(int value){
        PC = value;
    }    

    void updateOutput(string output) {
        this->output = output;
        wclear(outp);
        mvwprintw(outp,0,0,"%s",output.c_str());
        wrefresh(outp);
    }

    void pushIntoStack(int value) {
        stk.push(value);
    }

    void popFromStack() {
        stk.pop();
    }

    void init() {
        initscr();
        noecho();
        cbreak();

        box(stdscr,0,0);

        refresh();

        

        prompt = newwin(3, 78, 1, 1);
        // wborder(prompt,'*','*','*','*','*','*','*','*');
        box(prompt,0,0);
        mvwprintw(prompt,1,1,"Execute Instruction: e, Memory: u/d, Quit: q");
        wrefresh(prompt);

        ins = newwin(19,22, 4, 3);
        box(ins,0,0);
        // wborder(ins,'*','*','*','*','*','*','*','*');
        wrefresh(ins);

        pcw = newwin(17,2,5,1);
        mvwprintw(pcw,0,0,"->");
        wrefresh(pcw);

        outp = newwin(3,36,16,43);
        // wborder(outp,'*','*','*','*','*','*','*','*');
        box(outp,0,0);
        mvwprintw(outp,1,1,"OUTPUT:");
        mvwprintw(outp,2,1,"%s",output.c_str());
        wrefresh(outp);

        inp = newwin(4, 12, 19, 43);
        // wborder(inp,'*','*','*','*','*','*','*','*');
        box(inp,0,0);
        mvwprintw(inp,1,1,"INPUT:");
        wrefresh(inp);

        r1 = newwin(3, 16, 4, 26);
        // wborder(r1,'*','*','*','*','*','*','*','*');
        box(r1,0,0);
        mvwprintw(r1,1,1,"R1: %d",registers["R1"]);
        wrefresh(r1);

        r2 = newwin(3, 16, 7, 26);
        // wborder(r2,'*','*','*','*','*','*','*','*');
        box(r2,0,0);
        mvwprintw(r2,1,1,"R2: %d",registers["R2"]);
        wrefresh(r2);

        r3 = newwin(3, 16, 10, 26);
        // wborder(r3,'*','*','*','*','*','*','*','*');
        box(r3,0,0);
        mvwprintw(r3,1,1,"R3: %d",registers["R3"]);
        wrefresh(r3);

        ra = newwin(3, 16, 13, 26);
        // wborder(ra,'*','*','*','*','*','*','*','*');
        box(ra,0,0);
        mvwprintw(ra,1,1,"RA: %d",registers["RA"]);
        wrefresh(ra);

        acc = newwin(3, 16, 16, 26);
        // wborder(acc,'*','*','*','*','*','*','*','*');
        box(acc,0,0);
        mvwprintw(acc,1,1,"ACC: %d",registers["ACC"]);
        wrefresh(acc);

        mem = newwin(12, 24, 4, 43);
        // wborder(mem,'*','*','*','*','*','*','*','*');
        box(mem,0,0);
        mvwprintw(mem,1,1,"MEMORY");
        wrefresh(mem);

        flagsw = newwin(4, 7, 19, 30);
        // wborder(flagsw,'*','*','*','*','*','*','*','*');
        box(flagsw,0,0);
        mvwprintw(flagsw,1,1,"GT: %d",greater);
        mvwprintw(flagsw,2,1,"EQ: %d",equal);
        wrefresh(flagsw);

        stackw = newwin(12, 12, 4, 67);
        // wborder(stackw,'*','*','*','*','*','*','*','*');
        box(stackw,0,0);
        mvwprintw(stackw,1,1,"STACK");
        wrefresh(stackw);


    }


    int takeInput(bool isChar) {
        if(isChar) {
            char a;
            while(true) {
                a = getch();
                if(a == 'e' || a == 'u' || a == 'd' || a == 'q') {
                    updateOutput("");
                    return a;
                }
                else {
                    updateOutput("INVALID INPUT!");
                }
            }
        }
        else {
            char a;
            int res = 0;
            while(true) {
                a = getch();
                if(a == '\n'){
                    updateOutput("");
                    return res;
                }
                else if(a < '0' || a > '9') {
                    updateOutput("INVALID INPUT!");
                }
                else {
                    res = res*10 + a - '0';
                    updateOutput("");
                    // TODO: UPDATE IN INPUT WINDOW
                }
            }
        }
    }

private:
    map<string,int> registers = { {"R1", 0}, {"R2", 0}, {"R3", 0}, {"RA", 0}, {"ACC",0}};
    map<string,int> memory;
    bool greater = false;
    bool equal = false;
    vector<Instruction> instructions;
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
    stack<int> stk;
};

#endif