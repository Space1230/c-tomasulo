#include "utils.h"
#include "ROB.h"
#include "RS.h"
#include "Reg.h"
#include "main.h"

int main() {

    for (int i = 0; i < 8; i++) {
        process_ROB(&commands[i]);
    }
    print_ROB();
    print_Reg();
}
