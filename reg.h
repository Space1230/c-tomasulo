#ifndef REG_H_
#define REG_H_

#include "instruction.h"
#include "main.h"

// Reg stuff
typedef struct {
    int ROB_number;
    bool new;
} Reg_entry;

typedef struct {
    Reg_entry entries[REG_SIZE];
} Reg;

void Reg_print(Reg *Reg) {
    char reg_value_allocated_space[20];
    printf("Registers\n");

    for (int reg_num = 0; reg_num < REG_SIZE; reg_num++) {
        Reg_entry Reg_entry = Reg->entries[reg_num];
        if (!Reg_entry.ROB_number) { // if there is no entry, just skip
            continue;
        }

        if (Reg_entry.new == true) {
            sprintf(reg_value_allocated_space, "[newR%d]", reg_num);
        } else {
            sprintf(reg_value_allocated_space, "ROB #%d", Reg_entry.ROB_number);
        }
        printf("\tR%d, %s", reg_num, reg_value_allocated_space);
    }
    printf("\n\n");
}

#endif // REG_H_
