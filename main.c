// For now: focus on issue
//
// RS
// I want each RS to be in
// a RS station array, accessable
// by the opperation as an index

#include "main.h"
#include "c-tomasulo.h"
#include "instruction.h"
#include <stdlib.h>

instruction instructions[] = {{MULT, 1, 2, 3},
                              {ADD, 5, 1, 4},
                              {SUB, 6, 1, 5},
                              {DIV, 3, 2, 3},
                              {MULT, 6, 1, 2},
                              {ADD, 2, 6, 5},
                              {SUB, 6, 2, 5},
                              {ADD, 1, 5, 2}};

#define RS_init(name, max, exunit_name, exunit_max) {name, malloc(sizeof(RS_entry) * max), 0, max, exunit_name, exunit_max, malloc(exunit_max * sizeof(bool))}

int main() {

    ROB ROB = {};

    RS add_sub_rs = RS_init("ADD/SUB RS", ADD_SUB_RS_SIZE, "AS", ADD_SUB_EXUNIT_MAX);
    RS mult_rs = RS_init("MULT RS", MULT_RS_SIZE, "mul", MULT_EXUNIT_MAX);
    RS div_rs = RS_init("DIV RS", DIV_RS_SIZE, "div", DIV_EXUNIT_MAX);

    RS* RS[] = {&add_sub_rs, &mult_rs, &div_rs};

    Reg Reg = {};

    int instruction_size = sizeof(instructions) / sizeof(instruction);
    //instruction_size = 7;
    for (int i = 0; i < instruction_size; i++) {
        process_previous_instructions(&ROB, RS, &Reg);
        issue_instruction(instructions[i], &ROB, RS, &Reg);
    }

    ROB_print(&ROB);
    RS_print(RS);
    Reg_print(&Reg);

}
