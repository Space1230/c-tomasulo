#ifndef C_TOMASULO_H_
#define C_TOMASULO_H_

#include "instruction.h"
#include "rob.h"
#include "rs.h"
#include "reg.h"

#define gt_Reg(Reg) REG_SIZE < Reg

void issue_instruction(instruction instruction, ROB* ROB, RS** RS_array, Reg* Reg) {
    RS* RS = RS_array[RS_opperation_to_index(instruction.operation)];

    // check if ROB has room
    if (ROB_SIZE < ROB->size + 1) {
        fprintf(stderr, "ROB out of space\n");
        return;
    }

    // check if registration station has room
    if (RS->max_size < RS->size + 1) {
        fprintf(stderr, "RS out of space\n");
        return;
    }

    // check if register has room
    if (gt_Reg(instruction.destination_register) ||
        gt_Reg(instruction.opperand1) || gt_Reg(instruction.opperand2)) {
        fprintf(stderr, "Reg space is too small\n");
        return;
    }

    // add entry to ROB
    ROB_entry new_ROB_entry = {ROB->size + 1, true, instruction.operation, instruction.destination_register, {-1, false}, {ISSUE, -1}};
    ROB->entry[ROB->size++] = new_ROB_entry;


    // add entry to RS
    // picks the exunit
    int new_exunit_num = 1;
    if (RS->size > 0) {
        RS_entry old_entry = RS->entries[RS->size - 1];
        if (old_entry.ex_unit != RS->exunit_max) {
            new_exunit_num = old_entry.ex_unit + 1;
        }
    }

    // look through Reg to see if we are waiting on a ROB result
    value_register value1 = {instruction.opperand1, false};
    int ROB1 = -1;
    Reg_entry val1_Reg_entry = Reg->entries[instruction.opperand1];
    if (val1_Reg_entry.ROB_number) { // if entry exists
        value1.register_number = -1;
        ROB1 = val1_Reg_entry.ROB_number;
    }

    value_register value2 = {instruction.opperand2, false};
    int ROB2 = -1;
    Reg_entry val2_Reg_entry = Reg->entries[instruction.opperand2];
    if (val2_Reg_entry.ROB_number) {
        value2.register_number = -1;
        ROB2 = val2_Reg_entry.ROB_number;
    }

    RS_entry new_RS_entry = {new_exunit_num, true, instruction.operation, value1, value2, ROB1, ROB2, new_ROB_entry.number};
    RS->entries[RS->size++] = new_RS_entry;

    // add entry to Reg
    // update existing if found
    Reg_entry* dest_Reg_entry = &Reg->entries[instruction.destination_register];
    if (dest_Reg_entry->ROB_number) {
        dest_Reg_entry->ROB_number = new_ROB_entry.number;
        dest_Reg_entry->new = false;
    } else { // create new Reg
        Reg_entry new_Reg_entry = {new_ROB_entry.number, false};
        Reg->entries[instruction.destination_register] = new_Reg_entry;
    }
}

void process_previous_instructions(ROB* ROB, RS** RS_array, Reg* Reg) {
    //This function will basically handle all the stuff in the issue state
    //This may extend to just processing every old instruction IDK

    // to move to execute, all the values must be present.
    // so, we iterate through all RS entries and change it to execute

    for (int j = 0; j < 3; j++) {
        RS* RS = RS_array[j];

        for (int i = 0; i < RS->size; i++) {
            RS_entry RS_entry = RS->entries[i];
            int ROB_index = RS_entry.ROB_dest - 1;

            if (ROB->entry[ROB_index].state.name == ISSUE) {

                if (RS_entry.value1.register_number != -1 &&
                    RS_entry.value2.register_number != -1 &&
                    !RS->exunit_busy[RS_entry.ex_unit - 1]) {

                    if (RS_entry.ROB_dest > ROB->size) {
                        fprintf(stderr, "ROB_dest too large");
                        continue;
                    }

                    // change to execution
                    state *ROB_state = &ROB->entry[RS_entry.ROB_dest - 1].state;
                    ROB_state->name = EXECUTE;
                    ROB_state->step = 1;
                    RS->exunit_busy[RS_entry.ex_unit - 1] = true;
                }
            }
        }
    }
}

#endif // C_TOMASULO_H_
