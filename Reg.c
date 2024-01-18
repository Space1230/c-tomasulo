#include "Reg.h"

unsigned char next_reg_index = 0;

// returns an entry if already seen it, or new entry if not, or NULL if the registers is full
struct Reg_entry *getRegEntry(unsigned char reg_num) {
    for (int i = 0; i < next_reg_index; i++) {
        if (registers[i].reg_num == reg_num) {
            return &registers[i];
        }
    }

    if (next_reg_index <= REG_SIZE) {
        return &registers[next_reg_index++];
    }

    fprintf(stderr, "ERROR: Reg buffer too small");
    return NULL;
}

void print_Reg() {
    printf("Registers\n");

    for(int i = 0; i < next_reg_index; i++) {
    char* newReg = getNewRegValue(registers[i].new_num, registers[i].new_or_ROB);
    printf("\tR%d, %s", registers[i].reg_num, newReg);

    free(newReg);
    }

    printf("\n");
}

char *getNewRegValue (unsigned char new_num, bool new_or_ROB) {
    char *string;

    if (new_or_ROB) {
        string = malloc(sizeof("[newR255]"));
        sprintf(string, "[newR%d]", new_num);
    } else {
        string = malloc(sizeof("ROB #255"));
        sprintf(string, "ROB #%d", new_num);
    }

    return string;
}
