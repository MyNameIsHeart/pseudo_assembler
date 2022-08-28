/* functions related to the command table */

#include "command_table.h"

/* create a new line in the command table based on the given parameters, excluding the possible addressing options */
void create_command_table_line(Command *command, char *name, int amount_param, int opcode, int funct)
{
	strcpy(command->name, name); 			/* set the command's name */
	command->amount_param = amount_param;	/* set the amount of params the command receives */
	command->opcode = opcode;				/* set the command's opcode */
	command->funct = funct; 				/* 					 funct */
}

/* set the possible addressing options (immediate, direct, relative, register_direct, none) of the source operand */
void set_source_addressing_options(Command *command, int source0, int source1, int source2)
{
    command->source_addressing_options[0] = source0;
	command->source_addressing_options[1] = source1;
	command->source_addressing_options[2] = source2;
}

/* set the possible addressing options (immediate, direct, relative, register_direct, none) of the destination operand */
void set_dest_addressing_options(Command *command, int dest0, int dest1, int dest2)
{
    command->dest_addressing_options[0] = dest0;
	command->dest_addressing_options[1] = dest1;
	command->dest_addressing_options[2] = dest2;
}

/* create the command table using an empty array of commands */
void create_command_table(Command *commands_arr)
{
	/* 0 */ /* mov */
	create_command_table_line(commands_arr, "mov", 2, 0, 0);
	set_source_addressing_options(commands_arr, 0, 1, 3);
	set_dest_addressing_options(commands_arr, 1, 3, none);
	
	commands_arr++; /* 1 */ /* cmp */
	create_command_table_line(commands_arr, "cmp", 2, 1, 0);
	set_source_addressing_options(commands_arr, 0, 1, 3);
	set_dest_addressing_options(commands_arr, 0, 1, 3);
	
	commands_arr++; /* 2 */ /* add */
	create_command_table_line(commands_arr, "add", 2, 2, 10);
	set_source_addressing_options(commands_arr, 0, 1, 3);
	set_dest_addressing_options(commands_arr, 1, 3, none);
	
	commands_arr++; /* 3 */ /* sub */
	create_command_table_line(commands_arr, "sub", 2, 2, 11);
	set_source_addressing_options(commands_arr, 0, 1, 3);
	set_dest_addressing_options(commands_arr, 1, 3, none);
	
	commands_arr++; /* 4 */ /* lea */
	create_command_table_line(commands_arr, "lea", 2, 4, 0);
	set_source_addressing_options(commands_arr, 1, none, none);
	set_dest_addressing_options(commands_arr, 1, 3, none);
	
	commands_arr++; /* 5 */ /* clr */
	create_command_table_line(commands_arr, "clr", 1, 5, 10);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, 1, 3, none);
	
	commands_arr++; /* 6 */ /* not */
	create_command_table_line(commands_arr, "not", 1, 5, 11);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, 1, 3, none);
	
	commands_arr++; /* 7 */ /* inc */
	create_command_table_line(commands_arr, "inc", 1, 5, 12);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, 1, 3, none);
	
	commands_arr++; /* 8 */ /* dec */
	create_command_table_line(commands_arr, "dec", 1, 5, 13);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, 1, 3, none);
	
	commands_arr++; /* 9 */ /* jmp */
	create_command_table_line(commands_arr, "jmp", 1, 9, 10);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, 1, 2, none);
	
	commands_arr++; /* 10 */ /* bne */
	create_command_table_line(commands_arr, "bne", 1, 9, 11);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, 1, 2, none);
	
	commands_arr++; /* 11 */ /* jsr */
	create_command_table_line(commands_arr, "jsr", 1, 9, 12);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, 1, 2, none);
	
	commands_arr++; /* 12 */ /* red */
	create_command_table_line(commands_arr, "red", 1, 12, 0);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, 1, 3, none);
	
	commands_arr++; /* 13 */ /* rts */
	create_command_table_line(commands_arr, "rts", 0, 13, 0);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, none, none, none);
	
	commands_arr++; /* 14 */ /* print */
	create_command_table_line(commands_arr, "print", 1, 14, 0);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, 0, 1, 3);

	commands_arr++; /* 15 */ /* stop */
	create_command_table_line(commands_arr, "stop", 0, 15, 0);
	set_source_addressing_options(commands_arr, none, none, none);
	set_dest_addressing_options(commands_arr, none, none, none);
}

/* print a line from the command table */
void print_command_table_line(Command *command)
{
	int i;
	printf("command: %s\n", command->name);  /* print the command's name */
	printf("amount of parameters the command receives: %d\n", command->amount_param); /* print the amount of params the command receives */
	printf("opcode: %d\n", command->opcode); /* print the command's opcode */
	printf("funct: %d\n", command->funct);	/* 					   	funct */
	printf("possible source addressing options:\n"); /* print possible source addressing options */
	for (i=0; i<3; i++)
	{
		switch (command->source_addressing_options[i])
		{
		    case none:
		        break;
			case immediate:
				printf("immediate\t");
				break;
			case direct:
				printf("direct\t");
				break;
			case relative:
				printf("relative\t");
				break;
			case register_direct:
				printf("register direct\t");
				break;
		}
	}
	printf("\npossible destination addressing options:\n"); /* print possible destination addressing options */
	for (i=0; i<3; i++)
	{
		switch (command->dest_addressing_options[i])
		{
		    case none:
		        break;
			case immediate:
				printf("immediate\t");
				break;
			case direct:
				printf("direct\t");
				break;
			case relative:
				printf("relative\t");
				break;
			case register_direct:
				printf("register direct\t");
				break;
		}
	}
	printf("\n");
}

/* print the command table ( using print_command_table_line() ) */
void print_command_table(Command *commands_arr)
{
    int i;
    for (i=0; i<NUM_OF_COMMANDS; i++) /* go over the command table and print each line */
    {
        print_command_table_line(commands_arr);
        printf("\n");
        commands_arr++;
    }
}

/* return the given command's index using the command table and the command's name */
int find_instruction_index(Command *commands_arr, char *name)
{
    int i;
    for (i=0; i<NUM_OF_COMMANDS; i++)
    {
        if (strcmp((commands_arr->name),(name)) == 0)
            return i;
        commands_arr++;
    }
    return NOT_VIABLE; /* command not found */
}

/* check if the given addressing options are valid for the given command */
int check_addressing_options(Command *commands_arr, int index, int source_addressing, int dest_addressing)
{
	int i;
	int check = NOT_VIABLE;
	for (i = 0; i < 3; i++)
	{
		if ((commands_arr+index)->source_addressing_options[i] == source_addressing)
			check = True;
	}
	if (check == NOT_VIABLE)
		return check;
	for (i = 0; i < 3; i++)
	{
		if ((commands_arr+index)->dest_addressing_options[i] == dest_addressing)
			check = True;
	}
	return check;
}
