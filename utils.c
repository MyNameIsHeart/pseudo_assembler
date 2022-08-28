/* utility functions */

#include "utils.h"

/* open the given file and run several verification tests (such as that the extension is ".asm") */
FILE *open_file(char * s, char * mode)
{
	FILE *temp; /* a temporery pointer to open the file */
	char *str;
	char *extension;

	str = (char *) malloc(sizeof(char)* strlen(s));

	if(!str)
	{
		printf("\nDynamic allocation failed");
		return NULL;
	}

	strcpy(str,s);
	extension = strrchr(str,'.');

	if(extension == NULL || strcmp(extension,".asm") != 0)
	{
		printf("\nFile isn't of the correct format (.asm)"); /* file isn't an .asm file */
		free(str);
		return NULL;
	}

	temp = fopen(str,mode); /* use a library function to open the file in the requested mode */
	if(temp == NULL)
	{
		printf("\ncouldn't open requested file: %s", s); /* couldn't open the given file */
		free(str);
		return NULL;
	}
	free(str);
	return temp; /* success - return the pointer */
}

/* check if a given string is a number */
int is_num(char * str)
{
    int boolean = True;
    int i = 0;
    if(str[0] == '-' || str[0] == '+')
    {
        i++;
    }
    for(; i< strlen(str);i++)
    {
        if(!isdigit(str[i]))
        {
            boolean = False;
        }
    }
    return boolean;
}

/* divide a string into tokens by keeping a record of indexes and lengths. return the amount of tokens */
int parsing(char line[], int token_starts[],int lengths[])
{
	int token_counter = 0;
	int index=0;						/* an index to scan the string */
	int string_length = strlen(line);	/* the length of the string in the array */

	while(index < MAX_IN_LINE) /* Set strats and lengths to zero in each cell */
	{
	    lengths[index] = 0;
	    token_starts[index] = 0;
	    index++;
	}

	index = 0; /* reset index */

	while(index < string_length && line[index] != '\n' && line[index] != '\0') /* scan the string */
	{
		
		while((line[index]==' '|| line[index] == '\t')&& index < string_length && line[index] != '\n' && line[index] != '\0') /* ignore tabs and spaces */
		{
			index++;
		}
        	if(line[index] == ',') /* encountered a comma */
        	{
				token_starts[token_counter] = index; /* record the comma as a token in starts and set the length to 1 in lengths */
				lengths[token_counter] =1; 
				index++;
				token_counter++; /* move to the next index in the string and count another token */
        	}
        	else
        	{
           		if(line[index] != '\0' && line[index] != '\n')  /* check for end of string */
				{
					token_starts[token_counter] = index; /* record a token */
				}
				else
				{
					break;
				}

				/* go over the token and search for it's end. update the length of the token */
				while((line[index]!=' '&& line[index] != '\t' && line[index] != ',')&& index < string_length && line[index] != '\n' && line[index] != '\0')
				{
					lengths[token_counter]++;
					index++;
				}
				token_counter++; /* count another token */
        	}
	}
	return token_counter; /* return the amount of tokens */
}

/* copy a specific part of a string to another string and add '\0' at the end. */
void copy_n_str(char* line, char* word, int length)
{
    int index =0;
    for(;index <length;index++)		/* copy the chosen amount of chars */
    {
        word[index] = line[index];
    }
    word[index] = '\0'; 			/* add '\0' at the end */
    
}

/* check if a label is valid */
int is_valid_label(char * word,Command * arr)
{
    int i = 1;										/* an index to scan the string with */
    char temp[MAX_IN_LINE];
    if(word[strlen(word)-1] == ':')
    {
        
        copy_n_str(word, temp, strlen(word) - 1);	/* copy the given string without ':' using the "copy_n_str" function */ 
    }
    else
    {
        copy_n_str(word, temp, strlen(word));		/* Copy the given string */
    }
    
    if(!(isalpha(temp[0])))							/* check if the first character is in the alphabet */
    {
        return False;
    }
    
    
    if(strlen(temp)>MAX_IN_LABEL)					/* verify that the label is in the chosen length */
    {
        return False;
    }

    for(;i<strlen(temp);i++)						/* check if the other characters (excluding the first one) are digits \ alphabet characters */
    {
        if((!isalpha(temp[i])) && (!isdigit(temp[i])))
        {
            return False;
        }
    }
    i = 0;
    
    for(;i < 16;i++)								/* verify that the lable isn't a saved word (specifically an instruction) */
    {
       
        if(0 == strcmp(arr->name,temp))
        {
            return False;
        }
        arr++;
    }

    if(is_register(temp)) 							/* verify that the label isn't a register */
    {
        return False;
    }

    return True;									/* the label is valid - return True */
}

/* check if a string is supposed to represent a label */
int is_label(char * str)
{
    if(str[strlen(str)-1] == ':')
    {
        return True;
    }
    else
    {
        return False;
    }
    
}

/* encode the first word of an instruction */
void encode_first_word(Command *commands_arr, int commandIndex, int source, int dest, Machine_code_image *machine_code, int machine_code_index)
{
	copy_set_bits(&((machine_code+machine_code_index)->machine_code), (commands_arr+commandIndex)->opcode, 8, 11);	/* set the opcode */
	copy_set_bits(&((machine_code+machine_code_index)->machine_code), (commands_arr+commandIndex)->funct, 4, 7);	/* set the funct */
	
	if (source != none)
		copy_set_bits(&((machine_code+machine_code_index)->machine_code), source, 2, 3);							/* set the source addressing option */
	else
		copy_set_bits(&((machine_code+machine_code_index)->machine_code), 0, 2, 3);									/* no source operand */
		
	if (dest != none)
		copy_set_bits(&((machine_code+machine_code_index)->machine_code), dest, 0, 1);								/* set the destination addressing option */
	else
		copy_set_bits(&((machine_code+machine_code_index)->machine_code), 0, 0, 1);									/* no destination operand */
	copy_set_bits(&((machine_code+machine_code_index)->machine_code), 0, 12, (sizeof(int)-1));						/* turn off the rest of the bits */
	(machine_code+machine_code_index)->ARE = 'A';
	(machine_code+machine_code_index)->D_or_I = instruction;
}

/* return the correct value of each register */
int register_val(char * str)
{

    if(strcmp( str,"r0" ) ==0)
    {
        return r0_val;
    }
    else if(strcmp( str,"r1" ) ==0)
    {
        return r1_val;
    }
    else if(strcmp( str,"r2" ) ==0)
    {
        return r2_val;
    }
    else if(strcmp( str,"r3" ) ==0)
    {
        return r3_val;
    }
    else if(strcmp( str,"r4" ) ==0)
    {
        return r4_val;
    }
    else if(strcmp( str,"r5" ) ==0)
    {
        return r5_val;
    }
    else if(strcmp( str,"r6" ) ==0)
    {
        return r6_val;
    }
    else if(strcmp( str,"r7" ) ==0)
    {
        return r7_val;
    }
    return -1;

    


}

/* encodes an instruction's operand */
void encode_instruction(Machine_code_image *machine_code, int machine_code_index, int addressing_methud, char *operand)
{
	int num;

	char *temp;
	if (addressing_methud == register_direct)
	{	
			
		num = register_val(operand);	
		(machine_code+machine_code_index)->machine_code = num;
		
	}
	else
	{
		temp = (char *) malloc(sizeof(char)*(strlen(operand)));
		copy_n_str(operand+1, temp,strlen(operand)-1);
		(machine_code+machine_code_index)->machine_code = atoi(temp);
		free(temp);
	
	}	
	(machine_code+machine_code_index)->ARE = 'A';
	(machine_code+machine_code_index)->D_or_I = instruction;
	
}

/* copy an integer to a specific place in a different integer */
void copy_set_bits(unsigned *x, unsigned num, int start, int end)
{
	int unsigned bitStatus;
    	int index = 0;
    	int i;
	
	for (i = start; i <= end; i++)
	{
	    bitStatus = (num >> index) & 1;
	    
	    if (bitStatus == 0)
	        *x &= ~(1 << i);
	    else
	        *x |= 1 << i;
        
        index++;
	}
}

/* encode a .data / .string directive */
int add_data_value(char *line, int starts[], int lengths[], int token_counter, int token_index, int machineIndex, Machine_code_image *machine_codeP, int line_counter, int *error_flag)
{	
	char current_word[MAX_IN_LINE];	
	int dataOrComma = 1; /* for .data directives. makes sure there are commas seperating the strings */
	int i = 0;
	int counter = 0;
	int strLength;
	copy_n_str(line+starts[token_index],current_word,lengths[token_index]);

	if((strcmp(current_word,".data")) == 0)
	{
		token_index++;
		if(!(token_index < token_counter))
		{
			printf("\nError in line %d - a .data directive without any parameters\n", line_counter);
			(*error_flag) = True;
		}	
		for(;token_index<token_counter;token_index++)
		{
			copy_n_str(line+starts[token_index],current_word,lengths[token_index]);

			if(dataOrComma)
			{
				if(is_num(current_word))
				{
					(machine_codeP+machineIndex+i)->machine_code = atoi(current_word);
					(machine_codeP+machineIndex+i)->ARE = 'A';
					(machine_codeP+machineIndex+i)->D_or_I = directive;
					counter++;
					i++;
					dataOrComma = 0;
				}
				else
				{
					printf("\nError in line %d - input inserted into the .data directive is not a number\n", line_counter);
					(*error_flag) = True;
				}
			}
			else if(strcmp(current_word,",") == 0)
			{
				dataOrComma = 1;				
				continue;				
			}
			else
			{
				printf("\nError in line %d - the numbers in a .data directive need to be separated by a comma\n",line_counter);
				(*error_flag) = True;
			}
			
			

		}
		if(strcmp(current_word , ",") == 0)
		{
			printf("\nError in line %d - there can't a comma after the final number in a .data directive\n",line_counter);
			(*error_flag) = True;
		}		
	}
	else if((strcmp(current_word,".string")) == 0)
	{
	    i=0;
		if(token_index<token_counter)
		{
		    token_index++;
			if(*(line+starts[token_index]) != '"')
			{
				printf("\nError in line %d - A string has to have quotation marks\n",line_counter);
				(*error_flag) = True;
			}
			strLength = 1;
			while(*(line+starts[token_index]+ strLength) != '\n' && *(line+starts[token_index]+ strLength) != '\0')
			{
				
				if(*(line+starts[token_index]+ strLength)== '"')
				{
					strLength++;					
					break;
				}				
				strLength++;
			}			
			if(*(line+starts[token_index]+strLength-1) != '"')
			{
				printf("\nError in line %d - A string has to have quotation marks\n",line_counter);
		        	(*error_flag) = True;	
			}
			copy_n_str(line+starts[token_index],current_word, strLength);

			for(; i < strLength-2;i++)
			{
				(machine_codeP+i+machineIndex)->machine_code = current_word[i+1];
				(machine_codeP+i+machineIndex)->ARE = 'A';
				(machine_codeP+i+machineIndex)->D_or_I = directive;

				counter++;
			}

			(machine_codeP+i+machineIndex)->machine_code = '\0';
			(machine_codeP+i+machineIndex)->ARE = 'A';
			(machine_codeP+i+machineIndex)->D_or_I = directive;
			counter++;
		}
		else
		{
			printf("\nError in line %d - a .string directive without any parameters\n", line_counter);
			(*error_flag) = True;
		}		
	}
	else
	{
		printf("\nInternal (code) error\n");
		exit(2);
	}
	return counter;
}

/* check if a given string is a register */
int is_register(char * str)
{
    if(strcmp( str,"r0" ) ==0)
    {
        return True;
    }
    else if(strcmp( str,"r1" ) ==0)
    {
        return True;
    }
    else if(strcmp( str,"r2" ) ==0)
    {
        return True;
    }
    else if(strcmp( str,"r3" ) ==0)
    {
        return True;
    }
    else if(strcmp( str,"r4" ) ==0)
    {
        return True;
    }
    else if(strcmp( str,"r5" ) ==0)
    {
        return True;
    }
    else if(strcmp( str,"r6" ) ==0)
    {
        return True;
    }
    else if(strcmp( str,"r7" ) ==0)
    {
        return True;
    }
    else
    {
        return False;
    }
    
}

/* find addressing options */
void find_addressing(Command * command_table, int * source, int * dest,char line[], int starts[], int lengths[], int token_index, int token_counter, int line_counter, int *error_flag)
{
    int index;
    int temp;
    char current_word[MAX_IN_LINE];
    char str[MAX_IN_LINE];
    int amount_param;

    copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
    index = find_instruction_index(command_table, current_word);

    if(index == NOT_VIABLE)
    {
        printf("\nError in line %d - This function does not exist\n",line_counter);
        *error_flag = True;
        return;
    }

    amount_param = (command_table+ index)->amount_param; /* get the amount of the instruction's parameters*/

    if(amount_param == 2)
    {
        temp =1;
    }
    else
    {
        temp = 0;
    }
   
    if(token_counter-token_index == amount_param + 1 + temp)
{
		if(amount_param == 2)
		{
			token_index++;
			copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
			copy_n_str(current_word + 1,str,strlen(current_word) - 1);	
			if(is_register(current_word))
			{
				*source = register_direct;
			}
		else if(current_word[0] == '#' && is_num(str))
			{
				*source = immediate;
			}
			else if(is_valid_label(current_word,command_table) && !is_label(current_word))
			{
				*source = direct;
			}
			else if(current_word[0] == '%' && is_valid_label(str, command_table) && !is_label(str))
			{
				
				*source = relative;
			}
			else
			{
				
				printf("\nError in line %d - parameter syntax error\n", line_counter);
				*error_flag = True;
				return ;
			}
			token_index++;
			copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
			if(strcmp(current_word,",") != 0)
			{
				
				printf("\nError in line %d - no comma between the instruction's parameters\n", line_counter);
				*error_flag = True;
				return ;
			}
			token_index++;
			copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
			copy_n_str(current_word + 1,str,strlen(current_word) - 1);
			if(is_register(current_word))
			{
				*dest = register_direct;
			}
		else if(current_word[0] == '#' && is_num(str))
			{
				*dest = immediate;
			}
			else if(is_valid_label(current_word,command_table) && !is_label(current_word))
			{
				*dest = direct;
			}
			else if(current_word[0] == '%' && is_valid_label(str, command_table) && !is_label(str))
			{
				
				*dest = relative;
			}
			else
			{
				
				printf("\nError in line %d - parameter syntax error\n", line_counter);
				*error_flag = True;
				return ;
			}
			
			return;
			
			
		}
		else if(amount_param == 1)
		{
			token_index++;
			copy_n_str(line+starts[token_index],current_word,lengths[token_index]);
			copy_n_str(current_word + 1,str,strlen(current_word) - 1);	
			if(is_register(current_word))
			{
				*source = none;
				*dest = register_direct;
			}
		else if(current_word[0] == '#' && is_num(str))
			{
				*source = none;
				*dest = immediate;
			}
			else if(is_valid_label(current_word,command_table) && !is_label(current_word))
			{
				*source = none;
				*dest = direct;
			}
			else if(current_word[0] == '%' && is_valid_label(str, command_table) && !is_label(str))
			{
				
				*source = none;
				*dest = relative;
			}
			else
			{
				
				printf("\nError in line %d - parameter syntax error\n", line_counter);
				*error_flag = True;
				return ;
			}
			
			return;
			
		}
		else if(amount_param == 0)
		{
			*source = none;
			*dest = none;
			return;
		}

	}
	else if(token_counter-token_index < amount_param + 1 + temp)
	{
		printf("\nError in line %d - fewer parameters than needed or a missing a comma\n", line_counter);
		*error_flag = True;
	}
	else if(token_counter-token_index >amount_param + 1 + temp) 
	{
		printf("\nError in line %d - too many parameters\n", line_counter);
		*error_flag = True;
	}
}

/* print machine code */
void print_machine_code(Machine_code_image *machine_codeP, int lines_counted, int code_counter, int data_counter)
{
	int i =100;
	printf("\t%d %d",code_counter-100,data_counter);
	for(;i< lines_counted;i++)
	{
		printf("\n%04d\t%03X   %c   %d\n",i ,((machine_codeP+i)->machine_code) & 0xfff, (machine_codeP+i)->ARE, (machine_codeP+i)->D_or_I);
	}
	return;
}

