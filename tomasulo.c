
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
    unsigned char value; // 0 = no value, this is the register number
    struct state state;
};

struct RS_entry {
    unsigned char EX_unit;
    bool busy;
    enum instruction operation;
    unsigned char val1; // 0 = no value, reg num
    unsigned char val2; // same as 1
    unsigned char ROB1; // 0 = no value, corresponds to rob entry
    unsigned char ROB2; // same as 1
    unsigned char ROB_dest; // same as 1
};

struct Reg_entry {
    unsigned char reg_num;
    unsigned char ROB_num;
};

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

    /*
     * ROB_entry
    number will be filled in by printing logic
    bool busy;
    enum instruction instruction;
    unsigned char value; // 0 = no value, this is the register number
    struct state state;
     */

    struct ROB_entry my_entry = {.busy = true, .instruction = MULT, .value = 0, .state = {ISSUE, 0}};
    ROB[next_ROB_index++] = my_entry;

   /*
    * RS_entry
    unsigned char EX_unit;
    bool busy;
    enum instruction operation;
    unsigned char val1; // 0 = no value, reg num
    unsigned char val2; // same as 1
    unsigned char ROB1; // 0 = no value, corresponds to rob entry
    unsigned char ROB2; // same as 1
    unsigned char ROB_dest; // same as 1
    */

    struct RS_entry my_rs_entry = {.EX_unit = 1, .busy = true, .operation = MULT, .val1 = 2, .val2 = 3, .ROB1 = 0, .ROB2 = 0, .ROB_dest = 1};
    mult_RS[next_mult_RS_index++] = my_rs_entry;


    struct Reg_entry my_reg = {.reg_num = 1, .ROB_num = 1};
    registers[next_reg_index++] = my_reg;

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


// note string will always be 2 characters long
// this is also allocated using malloc, so we must
// free it as well
char* getRegString(unsigned char reg_num) {
    char* str = malloc(3 * sizeof(char));
    str[0] = 'R';
    str[1] = reg_num + 48;
    str[2] = '\0';

    return str;
}

char* getInstructionString(enum instruction instruction) {
    char* str = malloc(4 * sizeof(char));
    switch (instruction) {
        case ADD:
            strcpy(str, "ADD");
            break;
        case SUB:
            strcpy(str, "SUB");
            break;
        case MULT:
            strcpy(str, "MUL");
            break;
        case DIV:
            strcpy(str, "DIV");
            break;
    }
    return str;
}
