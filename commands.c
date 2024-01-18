#include "commands.h"

struct command commands[COMMANDS_SIZE] = { {MULT, 1, 2, 3},
                                           {ADD, 5, 1, 4},
                                           {SUB, 6, 1, 5},
                                           {DIV, 3, 2, 3},
                                           {MULT, 6, 1, 2},
                                           {ADD, 2, 6, 5},
                                           {SUB, 6, 2, 5},
                                           {ADD, 1, 5, 2} };

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
