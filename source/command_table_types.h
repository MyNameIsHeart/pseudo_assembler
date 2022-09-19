#ifndef COMMAND_TABLE_TYPES_H /* include guard */

    #define COMMAND_TABLE_TYPES_H

    /* defines */

    #define NUM_OF_COMMANDS 16

    /* new types */

    enum addressing_options {immediate, direct, relative, register_direct, none}; /* possible addressing options */

    /* datatype to represent a line in the command table */
    typedef struct
    {
            char name[5];       /* command's name */
            int amount_param;   /* amount of params the command receives */
            int opcode;         /* opcode */
            int funct;          /* funct */
            enum addressing_options source_addressing_options[3];   /* possible addressing options for the source operand */
        enum addressing_options dest_addressing_options[3];         /* possible addressing options for the destination operand */
    } Command;

#endif