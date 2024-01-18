#include "RS.h"
#include "utils.h"
#include "main.h"

unsigned char next_add_sub_RS_index = 0;
unsigned char next_mult_RS_index = 0;
unsigned char next_div_RS_index = 0;

void print_RS(struct RS_entry *RS, enum RS_type type, unsigned char next_RS_index) {
    // TODO register printing
    bool shouldFreeV1, shouldFreeV2, shouldFreeR1, shouldFreeR2, shouldFreeRd;
    bool printHeader = true;
    for (int i = 0; i < next_RS_index; i++) {
        if (printHeader) {
            printf("%s RS\t\tEX unit\t\tbusy\toperation\tval 1\tval 2\tROB 1\tROB 2\tROB destination\n", getRS_typeString(type));
            printHeader = false;
        }
        else {
            RS++;
        }

    shouldFreeV1 = shouldFreeV2 = shouldFreeR1 = shouldFreeR2 = shouldFreeRd = false;

    char* EX = getEXUnitString(RS->EX_unit, getMaxEXUnit(type), type);
    char* V1 = getValString(&RS->val1, &shouldFreeV1);
    char* V2 = getValString(&RS->val2, &shouldFreeV2);
    char* R1 = RS_getROBString(RS->ROB1, &shouldFreeR1);
    char* R2 = RS_getROBString(RS->ROB2, &shouldFreeR2);
    char* RD = RS_getROBString(RS->ROB_dest, &shouldFreeRd);

    printf("\t\t%s\t\t%s\t%s\t\t%s\t%s\t%s\t%s\t%s\n", EX, getBoolString(RS->busy), getInstructionString(RS->operation), V1, V2, R1, R2, RD);

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
