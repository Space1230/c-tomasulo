// For now: focus on issue

#include "main.h"
#include "stdbool.h"
#include "stdio.h"

typedef enum {
    ADD = 0,
    SUB = 1,
    MULT = 2,
    DIV = 3} operation;

typedef enum {
    ISSUE = 0,
    EXECUTE = 1,
    COMMIT = 2,
    WRITE = 3
} state_name;

typedef struct {
    state_name state_name;
    int state_step;
} state;

typedef struct {
    operation operation;
    int destination_register;
    int opperand1;
    int opperand2;
} instruction;

instruction instructions[] = {{MULT, 1, 2, 3},
                              {ADD, 5, 1, 4},
                              {SUB, 6, 1, 5},
                              {DIV, 3, 2, 3},
                              {MULT, 6, 1, 2},
                              {ADD, 2, 6, 5},
                              {SUB, 6, 2, 5},
                              {ADD, 1, 5, 2}};

// if value is -1, it currently doesn't exist
typedef struct {
    int number;
    bool busy;
    operation opperation;
    int destination;
    int value;
    state state;
    } ROB_entry;

typedef struct{
    ROB_entry entry[ROB_SIZE];
    int size;
}ROB;

void issue_instruction(instruction instruction, ROB* ROB) {
    // check if ROB has room
    if (ROB_SIZE < ROB->size + 1) {
        fprintf(stderr, "ROB out of space");
        return;
    }
    // check if registration station has room
    // check if register has room

    // add entry to ROB
    state new_state = {ISSUE, -1};
    ROB_entry new_entry = {ROB->size + 1, true, instruction.operation, instruction.destination_register, -1, new_state};
    ROB->entry[ROB->size++] = new_entry;
}

void print_ROB(ROB* ROB) {
    char value_space[4];
    printf("ROB\t#\tbusy\tinstruction\tdestination\tvalue\tstate\n");

    for (int i = 0; i < ROB->size; i++) {
        ROB_entry entry = ROB->entry[i];
// convert busy, instruction, and value into proper form
        char* busy = (entry.busy == true) ? "true" : "false";
        char* instruction;
        switch(entry.opperation) {
            case ADD: instruction = "ADD";
            break;
            case SUB: instruction = "SUB";
            break;
            case MULT: instruction = "MULT";
            break;
            case DIV: instruction = "DIV";
            break;
        }
        char* value;
        if (entry.value == -1) {
            value = "-";
        } else {
            sprintf(value_space, "%d", entry.value);
            value = value_space;
        }
        char* state;
        switch(entry.state.state_name) {
            case ISSUE: state = "issue";
            break;
            case EXECUTE: state = "execute";
            break;
            case COMMIT: state = "commit";
            break;
            case WRITE: state = "write";
            break;
        }

        printf("\t%d\t%s\t%s\t\tR%d\t\t%s\t%s\n", entry.number, busy, instruction, entry.destination, value, state);
    }
}

int main() {
    ROB ROB = {};

    int instruction_size = sizeof(instructions) / sizeof(instruction);
    for (int i = 0; i < instruction_size; i++) {
        issue_instruction(instructions[i], &ROB);
        ROB_entry entry = ROB.entry[i];
    }
    print_ROB(&ROB);
}
