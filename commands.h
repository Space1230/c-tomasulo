#ifndef COMMANDS_H_
#define COMMANDS_H_

enum instruction {ADD, SUB, MULT, DIV};

struct command {
    enum instruction instruction;
    unsigned char destination_reg;
    unsigned char first_reg;
    unsigned char second_reg;
};

#define COMMANDS_SIZE 8
struct command commands[COMMANDS_SIZE];

char* getInstructionString(enum instruction instruction);

#endif // COMMANDS_H_
