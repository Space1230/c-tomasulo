// For now: focus on issue
//
// RS
// I want each RS to be in
// a RS station array, accessable
// by the opperation as an index

#include "main.h"
#include "stdbool.h"
#include "stdio.h"

#define bool_string(b) (b == true) ? "true" : "false"

typedef enum {
    ADD = 0,
    SUB = 1,
    MULT = 2,
    DIV = 3} operation;

char* operation_string(operation operation) {
    char* operation_string;
    switch(operation) {
        case ADD: operation_string = "ADD";
        break;
        case SUB: operation_string = "SUB";
        break;
        case MULT: operation_string = "MULT";
        break;
        case DIV: operation_string = "DIV";
        break;
    }
    return operation_string;
}

typedef enum {
    ISSUE = 0,
    EXECUTE = 1,
    COMMIT = 2,
    WRITE = 3
} state_name;

typedef struct {
    state_name name;
    int step;
} state;

char* state_string(state state, char* allocated_space) {
    char* state_string;
    switch(state.name) {
        case ISSUE: state_string = "issue";
        break;
        case EXECUTE:
            if (state.step > 0) {
                sprintf(allocated_space, "execute (%d)", state.step);
                state_string = allocated_space;
            } else {
                state_string = "execute";
            }
        break;
        case COMMIT: state_string = "commit";
        break;
        case WRITE: state_string = "write";
        break;
    }
    return state_string;
}

typedef struct {
    int register_number;
    bool new;
} value_register;

char* value_register_string(value_register value_register, char* allocated_space) {
    char* value;

    // no item
    if (value_register.register_number == -1) {
        value = "-";
    // item, but new
    } else if (value_register.new == true) {
            sprintf(allocated_space, "[newR%d]", value_register.register_number);
    // item
    } else {
        sprintf(allocated_space, "[R%d]", value_register.register_number);
        value = allocated_space;
    }

    return value;
}

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

// ROB STUFF
// if value is -1, it currently doesn't exist
typedef struct {
    int number;
    bool busy;
    operation opperation;
    int destination;
    value_register value;
    state state;
    } ROB_entry;

typedef struct{
    ROB_entry entry[ROB_SIZE];
    int size;
}ROB;

// RS STUFF
typedef struct {
    int ex_unit;
    bool busy;
    operation operation;


} RS_entry;

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
    char value_space[10];
    char state_space[100];
    printf("ROB\t#\tbusy\tinstruction\tdestination\tvalue\tstate\n"); // header

    for (int i = 0; i < ROB->size; i++) {
        ROB_entry entry = ROB->entry[i];

        char* busy = bool_string(entry.busy);
        char* instruction = operation_string(entry.opperation);
        char* value = value_register_string(entry.value, value_space);
        char* state = state_string(entry.state, state_space);

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
