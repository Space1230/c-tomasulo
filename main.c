// For now: focus on issue
//
// RS
// I want each RS to be in
// a RS station array, accessable
// by the opperation as an index

#include "main.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

void print_ROB(ROB* ROB) {
    char value_space[10];
    char state_space[20];
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

// RS STUFF
typedef struct {
    int ex_unit;
    bool busy;
    operation operation;
    value_register value1;
    value_register value2;
    int ROB1;
    int ROB2;
    int ROB_dest;
} RS_entry;

typedef struct {
    RS_entry* entries;
    int size;
    int max_size;
    int exunit_max;
} RS;

void issue_instruction(instruction instruction, ROB* ROB, RS* RS_array) {
    RS RS = RS_array[instruction.operation];

    // check if ROB has room
    if (ROB_SIZE < ROB->size + 1) {
        fprintf(stderr, "ROB out of space");
        return;
    }

    // check if registration station has room
    if (RS.max_size < RS.size + 1) {
        fprintf(stderr, "RS out of space");
        return;
    }
    // check if register has room

    // add entry to ROB
    state new_state = {ISSUE, -1};
    value_register new_value = {-1, false};
    ROB_entry new_ROB_entry = {ROB->size + 1, true, instruction.operation, instruction.destination_register, new_value, new_state};
    ROB->entry[ROB->size++] = new_ROB_entry;

    // add entry to RS
    // picks the exunit
    RS_entry old_RS_entry = RS.entries[RS.size - 1];
    int new_exunit_num;
    if (old_RS_entry.ex_unit == RS.exunit_max) {
        new_exunit_num = 1;
    } else {
        new_exunit_num  = old_RS_entry.ex_unit + 1;
    }
    // TODO ROB and value logic
    // requires the registers
    // to look through
    RS_entry new_RS_entry = {new_exunit_num, true, instruction.operation, -1, -1, -1, -1, new_ROB_entry.number};
    RS.entries[RS.size++] = new_RS_entry;
   
    // add entry to Reg

}

#define RS_init(max, exunit_max) {malloc(sizeof(RS_entry) * max), 0, max, exunit_max}

int main() {
    ROB ROB = {};

    RS add_rs = RS_init(ADD_SUB_RS_SIZE, ADD_SUB_EXUNIT_MAX);
    RS sub_rs = RS_init(ADD_SUB_RS_SIZE, ADD_SUB_EXUNIT_MAX);
    RS mult_rs = RS_init(MULT_RS_SIZE, MULT_EXUNIT_MAX);
    RS div_rs = RS_init(DIV_RS_SIZE, DIV_EXUNIT_MAX);
    RS RS[] = {add_rs, sub_rs, mult_rs, div_rs};

    int instruction_size = sizeof(instructions) / sizeof(instruction);
    for (int i = 0; i < instruction_size; i++) {
        issue_instruction(instructions[i], &ROB, RS);
        ROB_entry entry = ROB.entry[i];
    }
    print_ROB(&ROB);
}
