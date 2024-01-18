#include "main.h"
#include "commands.h"
#include "utils.h"

#ifndef ROB_H_
#define ROB_H_

enum stage {ISSUE, EXECUTE, WRITE, COMMIT};

struct state { // this has to be in a struct because we must keep track
               // of the different amount of time the instruction takes
               // to complete
    enum stage stage;
    unsigned char execute_num;
};

struct ROB_entry {
    unsigned char ROB_entry_num;
    bool busy;
    enum instruction instruction;
    unsigned char destination; // 0 = no value, this is the register number
    unsigned char value; // same as dest
    struct state state;
};


struct ROB_entry ROB[ROB_SIZE];
unsigned char next_ROB_index;

void process_ROB(struct command *command);

void print_ROB();
// these instructions are dependencies of print_ROB
// do the fact that we may or may not use malloc, shouldFree is passed in
char* getStateString(struct state *state, bool *shouldFree);
char* getValueString(unsigned char value, bool *shouldFree);


#endif // ROB_H_
