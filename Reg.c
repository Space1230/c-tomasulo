#include "Reg.h"

unsigned char next_reg_index = 0;

struct Reg_entry *getRegEntry(unsigned char reg_num) {

}

void print_Reg(struct Reg_entry *registers, unsigned char next_reg_index) {
    bool printHeader = true;
    for(int i = 0; i < next_reg_index; i++) {
        if (printHeader) {
            printf("Registers\n");
            printHeader = false;
        } else {
            registers++;
        }

    char* newReg = getNewRegValue(registers->new_num, registers->new_or_ROB);

    printf("\tR%d, %s", registers->reg_num, newReg);

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
