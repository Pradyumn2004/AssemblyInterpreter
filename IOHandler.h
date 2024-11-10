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

        refresh();

        prompt = newwin(3, 80, 0, 0);
        box(prompt,'|','-');
        mvwprintw(prompt,1,1,"Execute Instruction: e, Memory: u/d, Quit: q");
        wrefresh(prompt);

        ins = newwin(21,16, 2, 2);
        box(ins,'|','-');
        wrefresh(ins);

        pcw = newwin(21,2,2,0);
        mvwprintw(pcw,1,0,"->");
        wrefresh(pcw);
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
            int res;
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
    WINDOW* io;
    WINDOW* flagsw;
    WINDOW* stackw;
    string output;
    int numInstructions;
    stack<int> stk;
};

#endif