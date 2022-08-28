/* header to second_pass.c */

#ifndef SECOND_PASS_H /* include guard */

    #define SECOND_PASS_H

    #include "libs.h"                   /* include C standard libraries */
    #include "exit_codes.h"             /* include exit codes header */
    #include "symbol_table_types.h"	    /* include symbol table's new types */
    #include "utils_types.h"            /* include utils' new types */
    #include "command_table_types.h"    /* include command table's new types */

    /* function prototypes */

    /* second pass of the assembler over the assembly code */
    int second_pass(FILE *, Machine_code_image *, Label *, int, int, int, char *);

    /* functions in use from symbol_table.c */
    void free_list(Label **);

    /* functions in use from command_table.c */
    void create_command_table(Command *);
    int find_label_value(Label * ,char *);
    char find_label_ARE(Label * ,char *);
    void print_entry(FILE *,Label *);
    void update_entry(Label* ,char *,int *, int);

    /* functions in use from utils.c */
    int parsing(char [], int [],int []);
    int is_label(char *);
    void find_addressing(Command *, int *, int *, char[], int[], int[], int, int, int, int *);
    void copy_n_str(char *, char *, int);

#endif
