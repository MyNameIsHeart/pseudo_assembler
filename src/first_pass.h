/* header to first_pass.c */

#ifndef FIRST_PASS_H                    /* include guard */

    #define FIRST_PASS_H

    #include "libs.h"                   /* include C standard libraries */
    #include "symbol_table_types.h"	    /* include symbol table's new types */
    #include "utils_types.h"            /* include utils' new types */
    #include "command_table_types.h"    /* include command table's new types */
    #include "exit_codes.h"             /* include exit codes header */

    /* function prototypes */

    /* first pass of the assembler over the assembly code */
    int first_pass(char *);

    /* functions in use from symbol_table.c */
    Label * create_label(char *, int, int);
    void add_label(Label* newLabel, Label** head, int line_counter,int * errors_flag);
    void update_data_value(Label *, int);

    /* functions in use from second_pass.c */
    int second_pass(FILE *, Machine_code_image *, Label *, int, int, int, char *);

    /* functions in use from utils.c */
    FILE * open_file(char *, char *);
    int parsing(char [], int [],int []);
    int is_valid_label(char *,Command *);
    int is_label(char *);
    void encode_first_word(Command *, int, int, int, Machine_code_image *, int);
    void encode_instruction(Machine_code_image *, int, int, char *);
    int add_data_value(char *,int [],int [],int,int,int,Machine_code_image *, int, int *);
    void find_addressing(Command *,int *, int *,char[], int[], int[],int ,int ,int, int *);
    void copy_n_str(char *, char *, int);

    /* functions in use from command_table.c */
    void create_command_table(Command *);
    int find_instruction_index(Command *, char *);
    int check_addressing_options(Command *, int, int, int);

#endif
