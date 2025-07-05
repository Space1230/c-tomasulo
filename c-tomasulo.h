#ifndef C_TOMASULO_H_
#define C_TOMASULO_H_

#include "instruction.h"
#include "rob.h"
#include "rs.h"
#include "reg.h"
#include <stdio.h>

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
            RS_entry *RS_entry = &RS->entries[i];
            int ROB_index = RS_entry->ROB_dest - 1;
            state *ROB_state = &ROB->entry[ROB_index].state;
            state_name current_state = ROB->entry[ROB_index].state.name;
            ROB_entry *ROB_entry = &ROB->entry[ROB_index];

            if (current_state == ISSUE) {

                // check to see if the ROB entries we're waiting for have a value
                //
                // NOTE: a "bug" is that all the previously scanned RS entries
                // won't be updated right when the state is changed to WRITE,
                // but honestly I think it is a fair "problem", cause this
                // doesn't actually happen in hardware
                if (RS_entry->ROB1 != -1) {
                    value_register ROB_result_value = ROB->entry[RS_entry->ROB1 - 1].value;
                    // copy the ROB result over if we have a new value
                    if (ROB_result_value.new == true) {
                        value_register *value1_reg = &RS_entry->value1;
                        value1_reg->new = true;
                        value1_reg->register_number = ROB_result_value.register_number;
                        RS_entry->ROB1 = -1;
                    }
                }
                if (RS_entry->ROB2 != -1) {
                    value_register ROB_result_value = ROB->entry[RS_entry->ROB2 - 1].value;
                    // copy the ROB result over if we have a new value
                    if (ROB_result_value.new == true) {
                        value_register *value2_reg = &RS_entry->value2;
                        value2_reg->new = true;
                        value2_reg->register_number = ROB_result_value.register_number;
                        RS_entry->ROB2 = -1;
                    }
                }


                // see if we have items already in the ROB
                if (RS_entry->value1.register_number != -1 &&
                    RS_entry->value2.register_number != -1 &&
                    !RS->exunit_busy[RS_entry->ex_unit - 1]) {

                    if (RS_entry->ROB_dest > ROB->size) {
                        fprintf(stderr, "ROB_dest too large");
                        continue;
                    }

                    // change to execution
                    state *ROB_state = &ROB->entry[RS_entry->ROB_dest - 1].state;
                    ROB_state->name = EXECUTE;
                    ROB_state->step = 1;
                    RS->exunit_busy[ROB_index] = true;
                    RS_entry->busy = true;
                }

            } else if (current_state == EXECUTE) {

                // check if we'd hit the max number needed for calculations
                int max_execution = operation_get_max_execution(RS_entry->operation);
                if (ROB_state->step == max_execution) {
                    // changing the state and copying our result to the ROB
                    ROB_state->name = WRITE;
                    ROB_state->step = -1;

                    value_register *ROB_value = &ROB_entry->value;
                    ROB_value->register_number = ROB_entry->destination;
                    ROB_value->new = true;

                    // updating ex unit status and clearing our RS
                    RS->exunit_busy[ROB_index] = false;
                    RS_entry->busy = false;
                    value_register *value1_reg = &RS_entry->value1;
                    value1_reg->register_number = -1;
                    value1_reg->new = false;
                    value_register *value2_reg = &RS_entry->value2;
                    value2_reg->register_number = -1;
                    value2_reg->new = false;

                    // removing our register from the register's list
                    Reg_entry *reg = &Reg->entries[ROB_value->register_number];
                    reg->new = false;
                    reg->ROB_number = 0;

                    continue;
                }

                ROB_state->step++;

            } else if (current_state == WRITE) {
                // ensures that we commit in order
                if (ROB_index == 0 || ROB->entry[ROB_index - 1].busy == false) {
                    ROB_state->name = COMMIT;
                }

            } else if (current_state == COMMIT) {
                ROB_state->name = DONE;
                ROB_entry->busy = false;
            }
        }
    }
}

#endif // C_TOMASULO_H_
