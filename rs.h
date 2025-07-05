#ifndef RS_H_
#define RS_H_

#include "instruction.h"
#include "rob.h"

// RS STUFF
typedef struct {
    int ex_unit;
    bool busy;
    operation operation;
    value_register value1;
    value_register value2;
    int ROB1;
    int ROB2;
    int ROB_dest;
} RS_entry;


typedef struct {
    char* name;
    RS_entry* entries;
    int size;
    int max_size;
    char* exunit_name;
    int exunit_max;
    bool* exunit_busy; // this is the META exunit busy. Right now
                       // this causes duplicated logic, maybe we
                       // should drop this in the future and search
                       // through our RS entries for this info
} RS;

int RS_opperation_to_index(operation operation) {
    switch (operation) {
        case ADD:
        case SUB:
            return 0;
        case MULT:
            return 1;
        case DIV:
            return 2;
    }
}

void RS_print(RS** RS_array) {
    // allocating for strings
    char value1_allocated_space[10];
    char value2_allocated_space[10];
    char ROB1_allocated_space[10];
    char ROB2_allocated_space[10];
    char ROB_dest_allocated_space[10];

    for (int i = 0; i < 3; i++) {
        RS* RS = RS_array[i];
        // add padding if needed
        char* padding = "";
        if (i != 0) {padding = "\t";}

        printf("%s%s\tEx unit\tbusy\toperation\tval1\tval2\tROB 1\tROB 2\tROB dest\n", RS->name, padding);

        bool print_name_and_number = false;
        if (RS->exunit_max != 1) {
            print_name_and_number = true;
        }

        //print out every item
        //TODO replace 1
        for (int j = 0; j < RS->size; j++) {
            RS_entry entry = RS->entries[j];
            // print exunit
            if (print_name_and_number) {
                printf("\t\t%s%d\t", RS->exunit_name, entry.ex_unit);
            } else {
                printf("\t\t%s\t", RS->exunit_name);
            }
            // print rest
            char* busy = bool_string(entry.busy);
            char* operation = operation_string(entry.operation);
            char* value1 = value_register_string(entry.value1, value1_allocated_space);
            char* value2 = value_register_string(entry.value2, value2_allocated_space);
            char* ROB1 = ROB_value_string(entry.ROB1, ROB1_allocated_space);
            char* ROB2 = ROB_value_string(entry.ROB2, ROB2_allocated_space);
            char* ROB_dest = ROB_value_string(entry.ROB_dest, ROB_dest_allocated_space);

            printf("%s\t%s\t\t%s\t%s\t%s\t%s\t%s\n", busy, operation, value1, value2, ROB1, ROB2, ROB_dest);
        }
        printf("\n");
    }

}

void RS_print_exunit_avaliablity(RS RS) {
    printf("%s exunit avaliability\n", RS.exunit_name);
    for (int i = 0; i < RS.exunit_max - 1; i++) {
        printf("%s, ", bool_string(RS.exunit_busy[i]));
    }
    printf("%s\n", bool_string(RS.exunit_busy[RS.exunit_max - 1]));
}

#endif // RS_H_
