/* header to command_table.c */

#ifndef COMMAND_TABLE_H /* include guard */

    #define COMMAND_TABLE_H

    #include "libs.h"                   /* include C standard libraries */
    #include "command_table_types.h"    /* include command table's new types */
    #include "utils_types.h"            /* include utils' new types */

    /* function prototypes */

    /* create a new line in the command table based on the given parameters, excluding the possible addressing options */
    void create_command_table_line(Command *, char *, int, int, int);

    /* set the possible addressing options of the source operand */
    void set_source_addressing_options(Command *, int, int, int);

    /* set the possible addressing options of the destination operand */
    void set_dest_addressing_options(Command *, int, int, int);

    /* create the command table using an empty array of commands */
    void create_command_table(Command *);

    /* print a line from the command table */
    void print_command_table_line(Command *);

    /* print the command table */
    void print_command_table(Command *);

    /* return the given command's index using the command table and the command's name */
    int find_instruction_index(Command *, char *);

    /* check if the given addressing options are valid for the given command */
    int check_addressing_options(Command *, int, int, int);

#endif

