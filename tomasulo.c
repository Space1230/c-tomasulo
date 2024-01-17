
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// to make storing the data simple, lets define a struct that holds commands

enum instruction {ADD, SUB, MULT, DIV};

struct command {
    enum instruction instruction;
    unsigned char destination_reg;
    unsigned char first_reg;
    unsigned char second_reg;
};

// there is 8 items
struct command commands[] = { {MULT, 1, 2, 3},
                              {ADD, 5, 1, 4},
                              {SUB, 6, 1, 5},
                              {DIV, 3, 2, 3},
                              {MULT, 6, 1, 2},
                              {ADD, 2, 6, 5},
                              {SUB, 6, 2, 5},
                              {ADD, 1, 5, 2} };

#define COMMANDS_SIZE 8;

enum stage {ISSUE, EXECUTE, WRITE, COMMIT};

struct state { // this has to be in a struct because we must keep track
               // of the different amount of time the instruction takes
               // to complete
    enum stage stage;
    unsigned char execute_num;
};

struct ROB_entry {
    // # will be filled in by printing logic
    bool busy;
    enum instruction instruction;
    unsigned char destination; // 0 = no value, this is the register number
    unsigned char value; // same as dest
    struct state state;
};

struct RS_val {
    unsigned char val;
    bool isNew;
};

struct RS_entry {
    unsigned char EX_unit;
    bool busy;
    enum instruction operation;
    struct RS_val val1; // 0 = no value, reg num
    struct RS_val val2; // same as 1
    unsigned char ROB1; // 0 = no value, corresponds to rob entry
    unsigned char ROB2; // same as 1
    unsigned char ROB_dest; // same as 1
};

enum RS_type {RS_ADD_SUB, RS_MULT, RS_DIV};

struct Reg_entry {
    unsigned char reg_num;
    unsigned char ROB_num;
};

void print_ROB(struct ROB_entry *ROB, unsigned char next_ROB_index);
// these instructions are dependencies of print_ROB
char* getStateString(struct state *state, bool *shouldFree); // in the case of an execute,
                                                             // you need to free the generated string
char* getValueString(unsigned char value, bool *shouldFree);


void print_RS(struct RS_entry *RS, enum RS_type type, unsigned char next_RS_index);
// these instructions are dependencies of print_RS
char *getRS_typeString(enum RS_type type);
char *getValString(struct RS_val* val, bool *shouldFree);
char *RS_getROBString(unsigned char ROB_num, bool *shouldFree);

// these are generally useful print functions
char* getInstructionString(enum instruction instruction);
char* getBoolString(bool boolean);

// utils
void optFree(void *item, bool shouldFree);

#define ROB_SIZE 8
#define ADD_SUB_RS_SIZE 4
#define MULT_RS_SIZE 2
#define DIV_RS_SIZE 2
#define REG_SIZE 10

int main() {
    struct ROB_entry ROB[ROB_SIZE];
    unsigned char next_ROB_index = 0;
    struct RS_entry add_sub_RS[ADD_SUB_RS_SIZE];
    unsigned char next_add_sub_RS_index = 0;
    struct RS_entry mult_RS[MULT_RS_SIZE];
    unsigned char next_mult_RS_index = 0;
    struct RS_entry div_RS[DIV_RS_SIZE];
    unsigned char next_div_RS_index = 0;
    struct Reg_entry registers[REG_SIZE];
    unsigned char next_reg_index = 0;

    struct ROB_entry my_entry = {.busy = true, .instruction = MULT, .destination = 1, .value = 0, .state = {EXECUTE, 1}};
    struct ROB_entry my_entry2 = {.busy = true, .instruction = ADD, .destination = 5, .value = 0, .state = {ISSUE, 0}};
    ROB[next_ROB_index++] = my_entry;
    ROB[next_ROB_index++] = my_entry2;

    struct RS_entry my_rs_entry = {.EX_unit = 1, .busy = true, .operation = MULT, .val1 = {2, false}, .val2 = {3, false}, .ROB1 = 0, .ROB2 = 0, .ROB_dest = 1};
    mult_RS[next_mult_RS_index++] = my_rs_entry;

    struct Reg_entry my_reg = {.reg_num = 1, .ROB_num = 1};
    registers[next_reg_index++] = my_reg;

    //print_ROB(ROB, next_ROB_index);
    print_RS(mult_RS, RS_MULT, next_mult_RS_index);


}

void optFree(void *item, bool shouldFree) {
    if (shouldFree) { free(item); }
}

void print(struct ROB_entry *ROB,
           unsigned char next_ROB_index,
           struct RS_entry *add_sub_RS,
           unsigned char next_add_sub_RS_index,
           struct RS_entry *mult_RS,
           unsigned char next_mult_RS_index,
           struct RS_entry *div_RS,
           unsigned char next_div_RS_index,
           struct Reg_entry *registers,
           unsigned char next_reg_index) {
    printf("Time 1\t\tinstruction - MULT R1, R2, R3\n");
}

void print_RS(struct RS_entry *RS, enum RS_type type, unsigned char next_RS_index) {
    // TODO programmatically gen EX units
    // TODO wrap stuff in loop
    // TODO register printing
    bool shouldFreeV1, shouldFreeV2, shouldFreeR1, shouldFreeR2, shouldFreeRd;

    printf("%s RS\t\tEX unit\t\tbusy\toperation\tval 1\tval 2\tROB 1\tROB 2\tROB destination\n", getRS_typeString(type));

    shouldFreeV1 = shouldFreeV2 = shouldFreeR1 = shouldFreeR2 = shouldFreeRd = false;

    char* V1 = getValString(&RS->val1, &shouldFreeV1);
    char* V2 = getValString(&RS->val2, &shouldFreeV2);
    char* R1 = RS_getROBString(RS->ROB1, &shouldFreeR1);
    char* R2 = RS_getROBString(RS->ROB2, &shouldFreeR2);
    char* RD = RS_getROBString(RS->ROB_dest, &shouldFreeRd);

    printf("\t\t%s\t\t%s\t%s\t\t%s\t%s\t%s\t%s\t%s\n", "mul", getBoolString(RS->busy), getInstructionString(RS->operation), V1, V2, R1, R2, RD);

    optFree(V1, shouldFreeV1);
    optFree(V2, shouldFreeV2);
    optFree(R1, shouldFreeR1);
    optFree(R2, shouldFreeR2);
    optFree(RD, shouldFreeRd);

    return;
}

char *getRS_typeString(enum RS_type type) {
    switch(type) {
        case RS_ADD_SUB:
            return "ADD/SUB";
            break;
        case RS_MULT:
            return "MUL";
            break;
        case RS_DIV:
            return "DIV";
            break;
    }
    return "ERR";
}

char *getValString(struct RS_val* val, bool *shouldFree) {

    if (val->val == 0) {
        return "-";
    }

    *shouldFree = true;
    char* valString = malloc(sizeof("[newR255]"));

    char* new;
    if (val->isNew) {
        new = "new";
    } else {
        new = "";
    }

    sprintf(valString, "[%sR%d]", new, val->val);
    return valString;
}

char *RS_getROBString(unsigned char ROB_num, bool *shouldFree) {
    if (ROB_num == 0) { return "-"; }

    *shouldFree = true;
    char *str = malloc(sizeof("#255"));

    sprintf(str, "#%d", ROB_num);
    return str;
}

void print_ROB(struct ROB_entry *ROB, unsigned char next_ROB_index) {
    bool printHeader = true;
    bool stateShouldFree;
    bool valueShouldFree;

    for (int i = 0; i < next_ROB_index; i++) {
        if (printHeader) {
            printf("ROB\t#\tbusy\tinstruction\tdestination\tvalue\tstate\n"); // ROB header
            printHeader = false;
        }

        stateShouldFree = false;
        char* stateString = getStateString(&ROB[i].state, &stateShouldFree);

        valueShouldFree = false;
        char* valueString = getValueString(ROB[i].value, &valueShouldFree);

        printf("\t%d\t%s\t%s\t\tR%d\t\t%s\t%s\n", i+1, getBoolString(ROB[i].busy), getInstructionString(ROB[i].instruction),
               ROB[i].destination, valueString, stateString);

        optFree(stateString, stateShouldFree);
        optFree(valueString, valueShouldFree);
    }
    return;
}

char* getBoolString(bool boolean) {
    if (boolean) {
        return "yes";
    }
    return "no";
}

char* getInstructionString(enum instruction instruction) {
    switch (instruction) {
        case ADD:
            return "ADD";
            break;
        case SUB:
            return "SUB";
            break;
        case MULT:
            return "MUL";
            break;
        case DIV:
            return "DIV";
            break;
    }
    return "ERR";
}

char* getValueString(unsigned char value, bool *shouldFree) {
    if (value == 0) { return "-"; }

    *shouldFree = true;

    // "[newR]" takes up 6 chars + 3 for values of value + 1 for null
    // = 10 chars
    char *valueString = malloc(10 * sizeof(char));
    sprintf(valueString, "[newR%d]", value);
    return valueString;
}

char* getStateString(struct state *state, bool *shouldFree) {
    if (state->stage == EXECUTE) {
        *shouldFree = true;
        // "execute ()" takes 10 chars, since the execution
        // number is an unsigned char, it takes up to 3 chars,
        // so total number of chars needed is 14 (with null)
        char* string = malloc(14 * sizeof(char));
        sprintf(string, "execute (%d)", state->execute_num);
        return string;
    }

    switch (state->stage) {
        case ISSUE:
            return "issue";
            break;
        case EXECUTE:
            // this logic is done above since this is a
            // special case
            // if we actually reach down here, it is an error
            return "ERR";
            break;
        case WRITE:
            return "write";
        case COMMIT:
            return "commit";
    }
    return "ERR";
}
