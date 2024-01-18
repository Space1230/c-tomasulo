#include "RS.h"

unsigned char next_add_sub_RS_index = 0;
unsigned char next_mult_RS_index = 0;
unsigned char next_div_RS_index = 0;

unsigned char prev_add_sub_EXUnit = 1;
unsigned char prev_mult_EXUnit = 1;
unsigned char prev_div_EXUnit = 1;

void add_item(struct command *command, unsigned char ROB_output) {
    // ROB_output is our ROB_entry_num
    // get previous EXunit
    // take care of value stuff
    enum instruction instruction = command->instruction;
    struct RS_entry new_entry = {.EX_unit = 1, .busy = true, .operation = instruction, .val1 = 2, .val2 = 3, .ROB1 = 0, .ROB2 = 0, .ROB_dest = ROB_output};
}

unsigned char newEXUnit(enum instruction instruction) {
    unsigned char maxEX;
    unsigned char *prevEX;
    switch (instruction) {
        case ADD:
        case SUB:
            maxEX = getMaxEXUnit(RS_ADD_SUB);
            prevEX = &prev_add_sub_EXUnit;
            break;
        case MULT:
            maxEX = getMaxEXUnit(RS_MULT);
            prevEX = &prev_mult_EXUnit;
            break;
        case DIV:
            maxEX = getMaxEXUnit(RS_DIV);
            prevEX = &prev_div_EXUnit;
            break;
        default:
            fprintf(stderr, "ERROR: newEXUnit");
            maxEX = 1;
            break;
    }

    if (maxEX == 1) { return 1; }

    if (*prevEX < maxEX) {
        *prevEX += 1;
    } else {
        *prevEX = 1;
    }
    return *prevEX;

}

void print_all_RS() {
    print_RS(add_sub_RS, RS_ADD_SUB, next_add_sub_RS_index);
    print_RS(mult_RS, RS_MULT, next_mult_RS_index);
    print_RS(div_RS, RS_DIV, next_div_RS_index);
}

void print_RS(struct RS_entry *RS, enum RS_type type, unsigned char next_RS_index) {
    // TODO register printing
    bool shouldFreeV1, shouldFreeV2, shouldFreeR1, shouldFreeR2, shouldFreeRd;
    bool printHeader = true;
    for (int i = 0; i < next_RS_index; i++) {
        if (printHeader) {
            printf("%s RS\t\tEX unit\t\tbusy\toperation\tval 1\tval 2\tROB 1\tROB 2\tROB destination\n", getRS_typeString(type));
            printHeader = false;
        }

    shouldFreeV1 = shouldFreeV2 = shouldFreeR1 = shouldFreeR2 = shouldFreeRd = false;

    char* EX = getEXUnitString(RS[i].EX_unit, getMaxEXUnit(type), type);
    char* V1 = getValString(&RS[i].val1, &shouldFreeV1);
    char* V2 = getValString(&RS[i].val2, &shouldFreeV2);
    char* R1 = RS_getROBString(RS[i].ROB1, &shouldFreeR1);
    char* R2 = RS_getROBString(RS[i].ROB2, &shouldFreeR2);
    char* RD = RS_getROBString(RS[i].ROB_dest, &shouldFreeRd);

    printf("\t\t%s\t\t%s\t%s\t\t%s\t%s\t%s\t%s\t%s\n", EX, getBoolString(RS[i].busy), getInstructionString(RS->operation), V1, V2, R1, R2, RD);

    free(EX);
    optFree(V1, shouldFreeV1);
    optFree(V2, shouldFreeV2);
    optFree(R1, shouldFreeR1);
    optFree(R2, shouldFreeR2);
    optFree(RD, shouldFreeRd);
    }
    return;
}

unsigned char getMaxEXUnit (enum RS_type type) {
    switch (type) {
        case RS_ADD_SUB:
            return ADD_SUB_EXUNIT_MAX;
            break;
        case RS_MULT:
            return MULT_EXUNIT_MAX;
            break;
        case RS_DIV:
            return DIV_EXUNIT_MAX;
            break;
    }
    return 1;
}

char *getEXUnitString(unsigned char exunit_num, unsigned char max_exunit, enum RS_type type) {
    char *exName;
    switch (type) {
        case RS_ADD_SUB:
            exName = "AS";
            break;
        case RS_MULT:
            exName = "mul";
            break;
        case RS_DIV:
            exName = "div";
            break;
        default:
            exName = "ERR";
    }

    char* string = malloc(sizeof(exName) + 3); // the three is for the max num 255
    if (max_exunit == 1) {
        strcpy(string, exName);
    } else {
        sprintf(string, "%s%d", exName, exunit_num);
    }
    return string;
}

char *getRS_typeString(enum RS_type type) {
    switch(type) {
        case RS_ADD_SUB:
            return "ADD/SUB";
            break;
        case RS_MULT:
            return "MUL";
            break;
        case RS_DIV:
            return "DIV";
            break;
    }
    return "ERR";
}

char *getValString(struct RS_val* val, bool *shouldFree) {

    if (val->val == 0) {
        return "-";
    }

    *shouldFree = true;
    char* valString = malloc(sizeof("[newR255]"));

    char* new;
    if (val->isNew) {
        new = "new";
    } else {
        new = "";
    }

    sprintf(valString, "[%sR%d]", new, val->val);
    return valString;
}

char *RS_getROBString(unsigned char ROB_num, bool *shouldFree) {
    if (ROB_num == 0) { return "-"; }

    *shouldFree = true;
    char *str = malloc(sizeof("#255"));

    sprintf(str, "#%d", ROB_num);
    return str;
}
