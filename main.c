#include "utils.h"
#include "ROB.h"
#include "RS.h"
#include "Reg.h"
#include "main.h"

int main() {

    for (int i = 0; i < 8; i++) {
        process_ROB(&ROB[i], i+1, &commands[i]);
    }
    print_ROB(ROB, 8);
    /* struct ROB_entry my_entry = {.busy = true, .instruction = MULT, .destination = 1, .value = 0, .state = {EXECUTE, 1}}; */
    /* struct ROB_entry my_entry2 = {.busy = true, .instruction = ADD, .destination = 5, .value = 0, .state = {ISSUE, 0}}; */
    /* ROB[next_ROB_index++] = my_entry; */
    /* ROB[next_ROB_index++] = my_entry2; */

    /* struct RS_entry my_rs_entry = {.EX_unit = 1, .busy = true, .operation = MULT, .val1 = {2, false}, .val2 = {3, false}, .ROB1 = 0, .ROB2 = 0, .ROB_dest = 1}; */
    /* struct RS_entry my_rs_entry2 = {.EX_unit = 1, .busy = true, .operation = MULT, .val1 = {0, false}, .val2 = {2, false}, .ROB1 = 1, .ROB2 = 0, .ROB_dest = 5}; */
    /* mult_RS[next_mult_RS_index++] = my_rs_entry; */
    /* mult_RS[next_mult_RS_index++] = my_rs_entry2; */

    /* struct Reg_entry my_reg = {.reg_num = 1, .new_num = 1, .new_or_ROB = false}; */
    /* struct Reg_entry my_reg2 = {.reg_num = 5, .new_num = 2, .new_or_ROB = false}; */
    /* registers[next_reg_index++] = my_reg; */
    /* registers[next_reg_index++] = my_reg2; */

    //print_ROB(ROB, next_ROB_index);
    //print_RS(mult_RS, RS_MULT, next_mult_RS_index);
    //print_Reg(registers, next_reg_index);


}
