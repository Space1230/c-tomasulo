#ifndef INSTUCTION_H_
#define INSTUCTION_H_

#include "main.h"
#include <stdbool.h>
#include <stdio.h>

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

int operation_get_max_execution(operation operation) {
    switch (operation) {
    case ADD:
    case SUB:
        return ADD_SUB_TIME;
    case MULT:
        return MUL_TIME;
    case DIV:
        return DIV_TIME;
    }
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

#endif // INSTUCTION_H_
