#include "commands.h"

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
