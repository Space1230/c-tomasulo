#ifndef ROB_H_
#define ROB_H_

#include "instruction.h"
#include "main.h"

typedef enum {
    ISSUE,
    EXECUTE,
    COMMIT,
    WRITE,
    DONE
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
        case DONE: state_string = "";
    }
    return state_string;
}

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

void ROB_print(ROB* ROB) {
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

char* ROB_value_string(int ROB_value, char* allocated_space) {
    if (ROB_value == -1) {
        return "-";
    }

    sprintf(allocated_space, "#%d", ROB_value);
    return allocated_space;
}

#endif // ROB_H_
