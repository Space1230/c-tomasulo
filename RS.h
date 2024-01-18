#include "commands.h"
#include "utils.h"

#ifndef RS_H_
#define RS_H_

struct RS_val {
    unsigned char val;
    bool isNew;
};

struct RS_entry {
    unsigned char EX_unit;
    bool busy;
    enum instruction operation;
    struct RS_val val1; // 0 = no value, reg num
    struct RS_val val2; // same as 1
    unsigned char ROB1; // 0 = no value, corresponds to rob entry
    unsigned char ROB2; // same as 1
    unsigned char ROB_dest; // same as 1
};

enum RS_type {RS_ADD_SUB, RS_MULT, RS_DIV};


void print_RS(struct RS_entry *RS, enum RS_type type, unsigned char next_RS_index);
// these functions are dependencies of print_RS
char *getRS_typeString(enum RS_type type);
unsigned char getMaxEXUnit (enum RS_type type);
char *getEXUnitString(unsigned char exunit_num, unsigned char max_exunit, enum RS_type type);
char *getValString(struct RS_val* val, bool *shouldFree);
char *RS_getROBString(unsigned char ROB_num, bool *shouldFree);

#endif // RS_H_
