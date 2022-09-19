#ifndef UTILS_TYPES_H /* include guard */

    #define UTILS_TYPES_H

    /* defines */

    #define MAX_IN_LABEL 31
    #define MAX_MACHINE_CODE_LINES 4096
    #define MAX_IN_LINE 82
    #define NOT_VIABLE -1

    /* new types */

    enum boolean {False, True};

    enum registers{r0_val=1, r1_val=2, r2_val=4,r3_val= 8 ,r4_val=16 ,r5_val=32 ,r6_val=64 ,r7_val=128};

    enum directive_or_instruction {directive = 1, instruction = 2}; /* to distinguish between directives and instructions */

    /* datatype to represent a line in the machine code image */
    typedef struct
    {
            unsigned int machine_code;
            char ARE;
            char externLabel[MAX_IN_LABEL +1];
            enum directive_or_instruction D_or_I;
    } Machine_code_image;

#endif