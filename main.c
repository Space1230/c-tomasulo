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
    ADD,
    SUB,
    MULT,
    DIV
} operation;

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
    } else if (value_register.new) {
        sprintf(allocated_space, "[newR%d]", value_register.register_number);
        value = allocated_space;
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
    printf("\n");
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

char* ROB_value_string(int ROB_value, char* allocated_space) {
    if (ROB_value == -1) {
        return "-";
    }

    sprintf(allocated_space, "#%d", ROB_value);
    return allocated_space;
}

typedef struct {
    char* name;
    RS_entry* entries;
    int size;
    int max_size;
    char* exunit_name;
    int exunit_max;
} RS;

int opperation_to_RS(operation operation) {
    switch (operation) {
        case ADD:
        case SUB:
            return 0;
        case MULT:
            return 1;
        case DIV:
            return 2;
    }
}

void print_RS(RS** RS_array) {
    // allocating for strings
    char value1_allocated_space[10];
    char value2_allocated_space[10];
    char ROB1_allocated_space[10];
    char ROB2_allocated_space[10];
    char ROB_dest_allocated_space[10];

    for (int i = 0; i < 3; i++) {
        RS* RS = RS_array[i];
        // add padding if needed
        char* padding = "";
        if (i != 0) {padding = "\t";}

        printf("%s%s\tEx unit\tbusy\toperation\tval1\tval2\tROB 1\tROB 2\tROB dest\n", RS->name, padding);

        bool print_name_and_number = false;
        if (RS->exunit_max != 1) {
            print_name_and_number = true;
        }

        //print out every item
        //TODO replace 1
        for (int j = 0; j < RS->size; j++) {
            RS_entry entry = RS->entries[j];
            // print exunit
            if (print_name_and_number) {
                printf("\t\t%s%d\t", RS->exunit_name, entry.ex_unit);
            } else {
                printf("\t\t%s\t", RS->exunit_name);
            }
            // print rest
            char* busy = bool_string(entry.busy);
            char* operation = operation_string(entry.operation);
            char* value1 = value_register_string(entry.value1, value1_allocated_space);
            char* value2 = value_register_string(entry.value2, value2_allocated_space);
            char* ROB1 = ROB_value_string(entry.ROB1, ROB1_allocated_space);
            char* ROB2 = ROB_value_string(entry.ROB2, ROB2_allocated_space);
            char* ROB_dest = ROB_value_string(entry.ROB_dest, ROB_dest_allocated_space);

            printf("%s\t%s\t\t%s\t%s\t%s\t%s\t%s\n", busy, operation, value1, value2, ROB1, ROB2, ROB_dest);
        }
        printf("\n");
    }
}

// Reg stuff
typedef struct {
    int ROB_number;
    bool new;
} Reg_entry;

typedef struct {
    Reg_entry entries[REG_SIZE];
} Reg;

void print_Reg(Reg *Reg) {
    char reg_value_allocated_space[20];
    printf("Registers\n");

    for (int reg_num = 0; reg_num < REG_SIZE; reg_num++) {
        Reg_entry Reg_entry = Reg->entries[reg_num];
        if (!Reg_entry.ROB_number) { // if there is no entry, just skip
            continue;
        }

        if (Reg_entry.new == true) {
            sprintf(reg_value_allocated_space, "[newR%d]", reg_num);
        } else {
            sprintf(reg_value_allocated_space, "ROB #%d", Reg_entry.ROB_number);
        }
        printf("\tR%d, %s", reg_num, reg_value_allocated_space);
    }
    printf("\n\n");
}


#define gt_Reg(Reg) REG_SIZE < Reg

void issue_instruction(instruction instruction, ROB* ROB, RS** RS_array, Reg* Reg) {
    RS* RS = RS_array[opperation_to_RS(instruction.operation)];

    // check if ROB has room
    if (ROB_SIZE < ROB->size + 1) {
        fprintf(stderr, "ROB out of space\n");
        return;
    }

    // check if registration station has room
    if (RS->max_size < RS->size + 1) {
        fprintf(stderr, "RS out of space\n");
        return;
    }

    // check if register has room
    if (gt_Reg(instruction.destination_register) ||
        gt_Reg(instruction.opperand1) || gt_Reg(instruction.opperand2)) {
        fprintf(stderr, "Reg space is too small\n");
        return;
    }

    // add entry to ROB
    ROB_entry new_ROB_entry = {ROB->size + 1, true, instruction.operation, instruction.destination_register, {-1, false}, {ISSUE, -1}};
    ROB->entry[ROB->size++] = new_ROB_entry;


    // add entry to RS
    // picks the exunit
    int new_exunit_num = 1;
    if (RS->size > 0) {
        RS_entry old_entry = RS->entries[RS->size - 1];
        if (old_entry.ex_unit != RS->exunit_max) {
            new_exunit_num = old_entry.ex_unit + 1;
        }
    }

    // look through Reg to see if we are waiting on a ROB result
    value_register value1 = {instruction.opperand1, false};
    int ROB1 = -1;
    Reg_entry val1_Reg_entry = Reg->entries[instruction.opperand1];
    if (val1_Reg_entry.ROB_number) { // if entry exists
        value1.register_number = -1;
        ROB1 = val1_Reg_entry.ROB_number;
    }

    value_register value2 = {instruction.opperand2, false};
    int ROB2 = -1;
    Reg_entry val2_Reg_entry = Reg->entries[instruction.opperand2];
    if (val2_Reg_entry.ROB_number) {
        value2.register_number = -1;
        ROB2 = val2_Reg_entry.ROB_number;
    }

    RS_entry new_RS_entry = {new_exunit_num, true, instruction.operation, value1, value2, ROB1, ROB2, new_ROB_entry.number};
    RS->entries[RS->size++] = new_RS_entry;

    // add entry to Reg
    // update existing if found
    Reg_entry* dest_Reg_entry = &Reg->entries[instruction.destination_register];
    if (dest_Reg_entry->ROB_number) {
        dest_Reg_entry->ROB_number = new_ROB_entry.number;
        dest_Reg_entry->new = false;
    } else { // create new Reg
        Reg_entry new_Reg_entry = {new_ROB_entry.number, false};
        Reg->entries[instruction.destination_register] = new_Reg_entry;
    }
}


void process_issued_instructions(ROB* ROB, RS** RS_array, Reg* Reg) {
    //This function will basically handle all the stuff in the issue state
    //This may extend to just processing every old instruction IDK

    // to move to execute, all the values must be present.

    // it seems the best action will be iterating through the reservation stations
    for (int j = 0; j < 3; j++) {
        RS* RS = RS_array[j];
        for (int i = 0; i < RS->size; i++) {
            RS_entry RS_entry = RS->entries[i];
        }
    }
}

#define RS_init(name, max, exunit_name, exunit_max) {name, malloc(sizeof(RS_entry) * max), 0, max, exunit_name, exunit_max}

int main() {

    ROB ROB = {};

    RS add_sub_rs = RS_init("ADD/SUB RS", ADD_SUB_RS_SIZE, "AS", ADD_SUB_EXUNIT_MAX);
    RS mult_rs = RS_init("MULT RS", MULT_RS_SIZE, "mul", MULT_EXUNIT_MAX);
    RS div_rs = RS_init("DIV RS", DIV_RS_SIZE, "div", DIV_EXUNIT_MAX);
    RS* RS[] = {&add_sub_rs, &mult_rs, &div_rs};

    Reg Reg = {};

    int instruction_size = sizeof(instructions) / sizeof(instruction);
    for (int i = 0; i < instruction_size; i++) {
        issue_instruction(instructions[i], &ROB, RS, &Reg);
    }

    print_ROB(&ROB);
    print_RS(RS);
    print_Reg(&Reg);
}
