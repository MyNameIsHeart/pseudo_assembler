#include "second_pass.h"

/* second pass of the assembler over the assembly code */
int second_pass(FILE *file_pointer, Machine_code_image *machine_codeP, Label *head, int code_counter_final, int data_counter_final, int errors_flag, char *file_name)
{
	int exit_status = 0;
	int data_counter =0;	
	int j;
	int i;
	int extern_flag = False;				/* flag for .extern directives */
	int entry_flag = False;					/* flag for .entry directives */
	char *new_file_name;
	Command command_table[NUM_OF_COMMANDS]; 
	FILE *new_file_pointer;
	int lengths[MAX_IN_LINE];
	int starts[MAX_IN_LINE];
	char line[MAX_IN_LINE];
	char current_word[MAX_IN_LINE];
	int token_counter;						/* token counter */
	int program_counter;					/* program counter */
	int token_index = 0;
	int line_counter = 0;
	int source_addressing;					/* source operand addressing method */
	int dest_addressing;					/* destination operand addressing method */
	int machine_code_index =100;
	rewind(file_pointer);
	create_command_table(command_table);	/* create the command table */
	if(errors_flag == True)
	{
		/* first pass detected errors */		
		free_list(&head);
		fclose(file_pointer);		
		printf("\nerrors in first pass, file: %s\n", file_name);
		exit_status = _EXIT_FAILURE;
		return exit_status;
  	}
	
	while(fgets(line, MAX_IN_LINE, file_pointer)) /* run over the lines in the source code */
	{
		token_index = 0;		
		line_counter++;	       	
		token_counter = parsing(line, starts, lengths); /* get line's amount of tokens */
	        if(token_counter == 0)  /* encountered an empty line */
    		{
    			continue;
    		}
    		copy_n_str(line+starts[token_index],current_word,lengths[token_index]); /* isolate the line's first word */
    		if(current_word[0] == ';') /* encounterd a comment */
    		{
    			continue;
    		}	
		if(is_label(current_word)) /* label */
    	{
		    token_index++; 
			if(token_index < token_counter) /* there is more to the line */
			{
					copy_n_str(line+starts[token_index],current_word,lengths[token_index]); 
			}
			else
			{
				continue; /* the label has nothing after it */
			}
    	}
					
		if(strcmp(current_word,".data") == 0 || strcmp(current_word,".string") == 0) /* .data / .string */
		{
			/* update the machine's index, skip the data word */			
			while(machine_code_index< MAX_MACHINE_CODE_LINES &&((machine_codeP+machine_code_index)->D_or_I == directive)) 
			{
				machine_code_index++;
				data_counter++;	
			}
			continue;
		}

		if(strcmp(current_word,".extern") == 0) /* .extern */
		{
				continue;
		}
    	if(strcmp(current_word,".entry") == 0) /* .entry */
    	{
			entry_flag= True;
			token_index++;			
			if(token_index<token_counter) /* check for empty .entry directives */
			{
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]); /* scan the next word*/
				update_entry(head, current_word,&errors_flag, line_counter);
			}
			else /* error */
			{
				printf("\nerror, .entry directive without any label afterwards - line %d\n",line_counter);
				continue;
			}    		    
    	}
		else
    	{
			find_addressing(command_table, &source_addressing, &dest_addressing, line, starts, lengths, token_index, token_counter, line_counter, &errors_flag);
			program_counter = machine_code_index - data_counter;			
			machine_code_index++;

			/* handle no param instructions */
			if(source_addressing == none && dest_addressing == none)
			{				
				continue;
			}
	
			/* handle one param instructions (direct param) */
			else if ((source_addressing == none) && (dest_addressing == direct))
			{
				/* scan the next word */
				token_index++;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]);				
				if(find_label_value(head,current_word) == -1)
				{
						printf("\nerror, tried to encode a label which do not exist - line %d\n",line_counter);
						errors_flag = True;
				}
				/* insert data to machine code image */
				((machine_codeP + machine_code_index)-> machine_code) = find_label_value(head,current_word); 

				((machine_codeP + machine_code_index)-> ARE) = find_label_ARE(head,current_word);

				if(find_label_ARE(head,current_word) == 'E')
				{			
					extern_flag = True;
					copy_n_str(current_word,(machine_codeP + machine_code_index)-> externLabel, strlen(current_word));
				}

				/* insert data to machine code image */
				((machine_codeP + machine_code_index)-> D_or_I) = instruction;
				machine_code_index++;
				continue;
			}
			
			
			/* handle one param instructions (relative param) */
			else if ((source_addressing == none) && (dest_addressing == relative))
			{
				/* scan the next word*/				
				token_index++;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
				
				if(find_label_value(head,current_word) == -1)
				{
					printf("\nerror,a try to encode a label which do not exist - line %d\n",line_counter);
					errors_flag = True;
				}
				/* insert data to machine code image */
				((machine_codeP + machine_code_index)-> machine_code) = find_label_value(head,current_word) - (program_counter+1);
				((machine_codeP + machine_code_index)-> ARE) = find_label_ARE(head,current_word);
			
				if(find_label_ARE(head,current_word) == 'E')
				{
					printf("\nerror, can not excute a relative addressing instruction on extern label. line %d\n", line_counter);
					errors_flag = True;
				} 
				((machine_codeP + machine_code_index)-> D_or_I) = instruction;
				machine_code_index++;
				continue;
			}
			else if((source_addressing == none) && (dest_addressing == immediate || dest_addressing == register_direct))
			{
				machine_code_index++;
				continue;
			}

			/* handle two param instructions */
			else if((source_addressing != none) && (dest_addressing != none))
			{
				/* scann the next word */
				token_index++;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
				if(source_addressing == direct)
				{

					if(find_label_value(head,current_word) == -1)
					{
						printf("\nerror,a try to encode a label which do not exist - line %d\n",line_counter);
						errors_flag = True;
					}					
					((machine_codeP + machine_code_index)-> machine_code) = find_label_value(head,current_word); 
					((machine_codeP + machine_code_index)-> ARE) = find_label_ARE(head,current_word);
					if(find_label_ARE(head,current_word) == 'E')
					{
						extern_flag = True;
						copy_n_str(current_word,(machine_codeP + machine_code_index)-> externLabel, strlen(current_word));

					}
					((machine_codeP + machine_code_index)-> D_or_I) = instruction;
				}
				else if(source_addressing == relative)
				{
					if(find_label_value(head,current_word) == -1)
					{
						printf("\nerror, tried to encode a label which does not exist - line %d\n",line_counter);
						errors_flag = True;
					}					
					((machine_codeP + machine_code_index)-> machine_code) = find_label_value(head,current_word) - (program_counter+1);
					((machine_codeP + machine_code_index)-> ARE) = find_label_ARE(head,current_word);
					if(find_label_ARE(head,current_word) == 'E')
					{
						printf("\nerror, can not excute a relative addressing instruction on extern label. line %d\n", line_counter);
						errors_flag = True;
					}  
					((machine_codeP + machine_code_index)-> D_or_I) = instruction;
				}
				
				token_index+=2;
				copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
				machine_code_index++;	
				
				if(dest_addressing == direct)
				{

					if(find_label_value(head,current_word) == -1)
					{
						printf("\nerror, tried to encode a label which does not exist - line %d\n",line_counter);
						errors_flag = True;
					}
					((machine_codeP + machine_code_index)-> machine_code) = find_label_value(head,current_word); 
					((machine_codeP + machine_code_index)-> ARE) = find_label_ARE(head,current_word);
					if(find_label_ARE(head,current_word) == 'E')
					{
						extern_flag = True;
						copy_n_str(current_word,(machine_codeP + machine_code_index)-> externLabel, strlen(current_word));
					}
					((machine_codeP + machine_code_index)-> D_or_I) = instruction;
				} else if(dest_addressing == relative)
				{
					if(find_label_value(head,current_word) == -1)
					{
						printf("\nerror, tried to encode a label which does not exist - line %d\n",line_counter);
						errors_flag = True;
					}
					((machine_codeP + machine_code_index)-> machine_code) = find_label_value(head,current_word) - (program_counter+1);
					((machine_codeP + machine_code_index)-> ARE) = find_label_ARE(head,current_word); 
					if(find_label_ARE(head,current_word) == 'E')
					{
						printf("\nerror, can not excute a relative addressing instruction on extern label. line %d\n", line_counter);
						errors_flag = True;
					} 
					((machine_codeP + machine_code_index)-> D_or_I) = instruction;
				}
				machine_code_index++;
			}   
    	}
	}

	fclose(file_pointer);
	if(errors_flag == False) /* verify that there are no errors before creating the output files */
	{
		/* create the object file */		
		new_file_name = (char *) malloc(sizeof(char)*(strlen(file_name)));
		copy_n_str(file_name,new_file_name , strlen(file_name)-4); /* copy the name of the file without the extension */
		strcat(new_file_name,".o"); /* add the extension */
		new_file_pointer = fopen(new_file_name,"w");
		free(new_file_name);
		j=100;		
		i= 100;
		fprintf(new_file_pointer,"     %d %d",code_counter_final-100,data_counter_final);
		for(;i < machine_code_index;i++)
		{
			if((machine_codeP+i)->D_or_I == instruction)
			{				
				fprintf( new_file_pointer,"\n%04d\t%03X   %c\n",j ,((machine_codeP+i)->machine_code) & 0xfff, (machine_codeP+i)->ARE);
				j++;
			}
		}
		i = 100;		
		for(;i<machine_code_index;i++)
		{
			if((machine_codeP+i)->D_or_I == directive)
			{
				fprintf( new_file_pointer,"\n%04d\t%03X   %c\n",j ,((machine_codeP+i)->machine_code) & 0xfff, (machine_codeP+i)->ARE);
				j++;
			}
		
		}
		fclose(new_file_pointer);

		printf("\entry flag = %d\n",entry_flag);
		/* create the .ent file */
		if(entry_flag == True)
		{
			new_file_name = (char *) malloc(sizeof(char)*(strlen(file_name)));
			copy_n_str(file_name,new_file_name , strlen(file_name)-4); /* copy the name of the file without the extension */
			strcat(new_file_name,".ent"); /* add the extension */
			new_file_pointer = fopen(new_file_name,"w");
			free(new_file_name);
			print_entry(new_file_pointer,head);
			fclose(new_file_pointer);
		}

		printf("\nextern flag = %d\n",extern_flag);
		/* create the .ext file */
		if(extern_flag == True)
		{
			new_file_name = (char *) malloc(sizeof(char)*(strlen(file_name)));
			copy_n_str(file_name,new_file_name , strlen(file_name)-4); /* copy the name of the file without the extension */
			strcat(new_file_name,".ext"); /* add the extension */
			new_file_pointer = fopen(new_file_name,"w");
			free(new_file_name);
			i =100;
			data_counter = 0;
			for(; i < machine_code_index;i++)
			{				
				if((machine_codeP+i)->D_or_I == directive)
				{
					data_counter++;
				}
				if((machine_codeP+i)->ARE == 'E')
				{
					fprintf(new_file_pointer,"\n%s %04d\n",(machine_codeP + i)-> externLabel , i-data_counter);
				}
			}
			fclose(new_file_pointer);
		}
		free_list(&head);
	}
	return exit_status;
}