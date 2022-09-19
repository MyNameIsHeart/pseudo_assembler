/* header to utils.c */

#ifndef UTILS_H /* Include guard */

    #define UTILS_H

    #include "libs.h"                   /* include C standard libraries */
    #include "utils_types.h"            /* include utils' new types */
    #include "command_table_types.h"    /* include command table's new types */

    /* function prototypes */

    /* open the given file and run several verification tests (such as that the extension is ".asm") */
    FILE *open_file(char *, char *);

    /* check if a given string is a number */
    int is_num(char *);

    /* divide a string into tokens by keeping a record of indexes and lengths. return the amount of tokens */
    int parsing(char [], int [],int []);

    /* check if a label is valid */
    int is_valid_label(char *, Command *);

    /* check if a string is supposed to represent a label */
    int is_label(char *);

    /* encode the first word of an instruction */
    void encode_first_word(Command *, int, int, int, Machine_code_image *, int);

    /* return the correct value of each register */
    int register_val(char *);

    /* encodes an instruction's operand */
    void encode_instruction(Machine_code_image *, int, int, char *);

    /* copy an integer to a specific place in a different integer */
    void copy_set_bits(unsigned *, unsigned, int, int);

    /* encode a .data / .string directive */
    int add_data_value(char *,int [],int [],int,int,int,Machine_code_image *, int, int *);

    /* check if a given string is a register */
    int is_register(char *);

    /* find addressing options */
    void find_addressing(Command *, int *, int *, char[], int[], int[], int, int, int, int *);

    /* print machine code */
    void print_machine_code(Machine_code_image *,int,int, int);

    /* copy a specific part of a string to another string and add '\0' at the end. */
    void copy_n_str(char *, char *, int);

    /* functions in use from command_table.c */
    int find_instruction_index(Command *, char *);

#endif















