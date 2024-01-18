#include "ROB.h"

unsigned char next_ROB_index = 0;

void process_ROB(struct ROB_entry *ROB, unsigned char next_ROB_index, struct command *command) {
    if (next_ROB_index <= ROB_SIZE) { // default new entry
        struct ROB_entry entry = {.busy = true, .instruction = command->instruction, .destination = command->destination_reg, .value = 0, .state = {.stage = ISSUE, .execute_num = 0}};
        *ROB = entry;
    }

    // need to see if register has the dest. already there
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
