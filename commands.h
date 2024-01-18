#ifndef COMMANDS_H_
#define COMMANDS_H_

enum instruction {ADD, SUB, MULT, DIV};

struct command {
    enum instruction instruction;
    unsigned char destination_reg;
    unsigned char first_reg;
    unsigned char second_reg;
};



char* getInstructionString(enum instruction instruction);

#endif // COMMANDS_H_
