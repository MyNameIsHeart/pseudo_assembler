#include "first_pass.h"

/* first pass of the assembler over the assembly code */
int first_pass(char *s)
{

	/* <variables> */

	int exit_status = 1;	
	int data_counter = 0;				/* counter for data image lines */
	int code_counter = 100;				/* counter for code image lines */
	int data_counter_final;				/* final value of data_counter */ 
	int code_counter_final;				/* final value of code_counter */ 
	int machine_code_index = 100;		/* index for working with the machine code array */
	int line_counter = 0;				/* counter for the original program's lines */
	int instruction_line_counter = 0;	/* counter for machine code lines of the current instruction. Also used to store the current command's index. */
	int temp = 0;						/* used to update data_counter and machine_code_index */
	int source_addressing;				/* addressing method of the source operand */
	int dest_addressing;				/* addressing method of the destination operand */

	/* flags */
	int errors_flag = False;			/* flag for errors in the asm program */
	int defining_label = False;			/* flag for label definitions */
	int is_head = True;					/* flag to determine if there is a head to the symbol table  */
	
	/* struct pointers */
	FILE *file_pointer; /* pointer to the current file */
	Label *head = NULL; /* head of the symbols linked list */
	Label *curr; 		/* for creating new symbols */

	/* command table */
	Command command_table[NUM_OF_COMMANDS];

	/* machine code image */
	Machine_code_image machine_code[MAX_MACHINE_CODE_LINES];

	/* variables for files and line parsing */
	char line[MAX_IN_LINE]; 		/* line */	
	int starts[MAX_IN_LINE];		/* tokens */
	int lengths[MAX_IN_LINE];		/* token's length */
	char current_word[MAX_IN_LINE];	/* current word */
	int token_index;				/* used to run over the tokens */
	int token_counter;				/* amount of tokens in current line */

	/* </variables> */

	file_pointer = open_file(s, "r");				/* open the given file with function "open_file" (from utils) */
	
	if(file_pointer == NULL)						/* file didn't open */
	{
		exit_status = _EXIT_FILES_FAILURE;
		return exit_status;
	}

	create_command_table(command_table);			/* create the command table using the array command_table */
	
	while(fgets(line, MAX_IN_LINE, file_pointer))	/* go over the file one line at a time */
	{
		/* resetting variables */
		defining_label = False;
		token_index = 0;

		line_counter++; /* count another line */

		token_counter = parsing(line,starts,lengths); /* parse the line */

		if(token_counter == 0)	/* empty line */
			continue;			/* the line is empty - go to the next line */

		copy_n_str(line+starts[token_index],current_word,lengths[token_index]); /* get the line's first word */

		if(current_word[0] == ';')	/* comment */
			continue;				/* the line is a comment - go to the next line */

		if(is_label(current_word)) /* the first word of the line is a label: */
		{
			if(is_valid_label(current_word, command_table)) /* verify that the label is valid */
			{
				defining_label = True; /* set the label definition flag */
				token_index++;
				if (token_index < token_counter) /* check if there are more words to the line */
					copy_n_str(line+starts[token_index],current_word,lengths[token_index]); /* get the next word */
				else
				{
					printf("\nLabel definition is wrong, error in line %d\n",line_counter); /* label definition is wrong */
					printf("\nA label needs to get defined at the beginning of an instruction or at the beginning of a .data or a .string directive\n");
					errors_flag = True; /* set the error flag */
					continue;
				}
			}
			else
			{
				printf("\nLabel is not valid, error in line %d\n",line_counter); /* label is invalid */
				errors_flag = True; /* set the error flag */
				continue;
			}			
			
		}

		/* .data or .string */

		if ((strcmp(current_word,".data") == 0) || (strcmp(current_word, ".string") == 0)) /* .data / .string directive */
		{
			if (defining_label == True) /* label definition */
			{
				/* get the label's name */
				token_index--;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]-1);

				/* define the label */
				curr = create_label(current_word,data_counter,data);

				/* add label to linked list */
				if (is_head == True)
				{
					head = curr;
					is_head = False;
				}			
				else
				{
					add_label(curr,&head,line_counter,&errors_flag);
				}
				
				token_index++;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
				
			}
			

			temp = add_data_value(line,starts,lengths,token_counter,token_index, machine_code_index, machine_code, line_counter, &errors_flag); /* temp gets the number of words that .data or .string takes */

			data_counter += temp; /* update data_counter */
			machine_code_index += temp; /* update machine_code_index */
			continue;				
		}

		/* .entry */

		if (strcmp(current_word, ".entry") == 0)
		{
			
			if (defining_label == True) /* a label definition before .entry is meaningless */
				printf("\nWarning: labels before .entry statements are meaningless (line %d)\n",line_counter);	
			continue; /* the second pass handles these */
		}

		/* .extern */				

		if (strcmp(current_word,".extern") == 0)
		{
			if (defining_label == True) /* a label definition before .extern is meaningless */
				printf("\nWarning: labels before .extern statements are meaningless (line %d)\n",line_counter);

			token_index++;
			if (token_index < token_counter) /* check if there are more words to the line */
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]); /* get the next word */
			else /* .extern without a parameter */
			{
				printf("\nSyntax error in line %d - .extern directive doesn't get a parameter\n",line_counter);
				errors_flag = True; /* set the error flag */
				continue;
			}

			if(is_valid_label(current_word, command_table)) /* verify that the label is valid */
			{
				/* define the label */
				curr = create_label(current_word,0,external);

				/* add label to linked list */
				if (is_head == True)
				{
					head = curr;
					is_head = False;
				}			
				else
				{
					add_label(curr,&head,line_counter,&errors_flag);

				}
				continue;
			}
			else
			{
				printf("\nLabel is not valid, error in line %d\n",line_counter); /* label is invalid */
				errors_flag = True; /* set the error flag */
				continue;
			}	
			
		}
		
		/* Command */

		instruction_line_counter = find_instruction_index(command_table, current_word); /* find the command's index */
		if (instruction_line_counter != NOT_VIABLE) /* verify that the command is viable */
		{
			if (defining_label == True) /* set the label definition flag */
			{
				/* get the label's name */
				token_index--;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]-1);

				/* define label */
				curr = create_label(current_word,code_counter,code);

				/* add label to linked list */
				if (is_head == True)
				{
					head = curr;
					is_head = False;
				}			
				else
				{
					add_label(curr,&head,line_counter,&errors_flag);
				}

				token_index++;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
				
			}

			find_addressing(command_table, &source_addressing, &dest_addressing,line,starts, lengths,token_index , token_counter ,line_counter, &errors_flag); /* find the addressing options in use */

			if ((check_addressing_options(command_table, instruction_line_counter, source_addressing, dest_addressing)) == NOT_VIABLE) /* verify that  the addressing options are viable */
			{
				printf("\nUnviable addressing options (in line %d)\n",line_counter); /* invalid addressing options */
				errors_flag = True; /* set the error flag */
				continue;
			}

			/* encode the instruction's first word */
			encode_first_word(command_table, instruction_line_counter, source_addressing, dest_addressing, machine_code, machine_code_index);
			machine_code_index++;

			instruction_line_counter = ((command_table+instruction_line_counter)->amount_param) + 1; /* get the number of memory (words) that the command needs */
			code_counter += instruction_line_counter; /* update code_counter */

			/* update code_counter */
			if ((source_addressing == none) && (dest_addressing == none))
				continue;

			/* handle instructions with one parameter */
			if ((source_addressing == none) && (dest_addressing == immediate || dest_addressing == register_direct))
			{
				
				
				token_index++;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]);

				/* encode the parameter of the instruction */
				encode_instruction(machine_code, machine_code_index, dest_addressing, current_word);
				machine_code_index++;
				continue;
			}

			/* create space for words encoded by the second pass */
			else if((source_addressing == none) && (dest_addressing == direct || dest_addressing == relative))
			{
				machine_code_index++;
				continue;
			}

			/* handle instructions with two parameters */
			if (source_addressing != none && dest_addressing != none)
			{				
				token_index++;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
				if(source_addressing == immediate || source_addressing == register_direct)
				{
					/* encode the instruction's first parameter */				
					encode_instruction(machine_code, machine_code_index, source_addressing, current_word);
					machine_code_index++;
				}
				else
				{
					/* create space for words encoded by the second pass */
					machine_code_index++;
				}
				
				token_index+=2;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
				if(dest_addressing == immediate || dest_addressing == register_direct)
				{
					/* encode the instruction's second parameter */
					encode_instruction(machine_code, machine_code_index, dest_addressing, current_word);
					machine_code_index++;
				}
				else
				{
					/* create space for words encoded by the second pass */
					machine_code_index++;

				}
				continue;
			}
			continue;
		}

		/* syntax error */

		printf("\nSyntax error in line %d\n",line_counter);
		errors_flag = True;

	}
	
	data_counter_final = data_counter; /* set the final value of data_counter */
	code_counter_final = code_counter; /* set the final value of code_counter */

	update_data_value(head,code_counter_final); /* update the value of every data label by adding the final value of code_counter to it */

	exit_status = second_pass(file_pointer, machine_code, head , code_counter_final, data_counter_final, errors_flag,s); /* call the second pass of the assembler */
	
	return exit_status;

}



