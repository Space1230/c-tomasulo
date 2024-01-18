#include "utils.h"
#include "main.h"

#ifndef REG_H_
#define REG_H_

struct Reg_entry {
    unsigned char reg_num;
    unsigned char new_num;
    bool new_or_ROB;
};


struct Reg_entry registers[REG_SIZE];
unsigned char next_reg_index;

struct Reg_entry *getRegEntry(unsigned char reg_num);

void print_Reg(struct Reg_entry *registers, unsigned char next_reg_index);
// these functions are dependencies of print_Reg
char *getNewRegValue (unsigned char new_num, bool new_or_ROB);

#endif // REG_H_
