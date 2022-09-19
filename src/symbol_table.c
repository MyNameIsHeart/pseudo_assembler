/* a linked-list implementation of the symbol table */

#include "symbol_table.h"
#include "utils.h"

/* create a new node (label) based on the given parameters */
Label * create_label(char *name, int value, int attribute)
{
	Label* new = (Label*) malloc(sizeof(Label));
	strcpy(new->symbol, name);
	new->value = value;
	new->attribute = attribute;
	new->next = NULL;

	return new;
}

/* function connect a new node (label) to the linked list */
void add_label(Label* newLabel, Label **head, int line_counter, int *errors_flag)
{
	Label *temp = *head;

	while (temp->next) /* go over the symbols list */
	{
		temp = temp->next;
		if (strcmp((temp->symbol),(newLabel->symbol)) == 0)
		{
			printf("\nError in line %d - label already exists\n", line_counter);
			*errors_flag = True;
			return;
		}
	}

	newLabel->next = NULL;
	temp->next = newLabel;
	return;
}

/* update the value of every data label by adding the final value of code_counter to it */
void update_data_value(Label* head,int code_counter_final)
{
	Label* curr = head;

	while(curr)
	{
		if((curr->attribute) == data)
		{
			(curr->value) += code_counter_final;
			
		}
	
		curr = curr->next;
		
	}

}

/* print the linked list (the symbol table) */
void print_list(Label* head)
{
	Label* curr = head;
	int i = 1;
	while (curr) {
		printf("node %d:\n",i);
		printf("Symbol\t%s\n", curr->symbol);
		printf("Value (decimal)\t%d\n", curr->value);
		switch ((curr->attribute)) /* print the correct attribute */
		{
			case code:
				printf("Attributes\tcode\n");
				break;
			case data:
				printf("Attributes\tdata\n");
				break;
			case code_and_entry:
				printf("Attributes\tcode, entry\n");
				break;
			case data_and_entry:
				printf("Attributes\tdata, entry\n");
				break;
			case external:
				printf("Attributes\texternal\n");
				break;
		}
		curr = curr->next;
		i++;
	}
}

/* free all of the list's memory */
void free_list(Label **head)
{
	Label* curr = *head;
	while (curr)
	{
		Label* next = curr->next;
		free(curr);
		curr = next;
	}
	return;
}

/* find the value of a given label */
int find_label_value(Label * head ,char * name)
{
	char temp[MAX_IN_LINE];
	Label* curr = head;
	
	if(name[0] == '%')
	{
		copy_n_str(name+1,temp,strlen(name)-1);
	}
	else
	{
		copy_n_str(name,temp,strlen(name));
	}
	
	while(curr)
	{
		if(strcmp(curr->symbol,temp) == 0)
		{
						
			return curr->value;
			
		}
		curr = curr->next;
	}

	return -1;
}

/* find the ARE value of a given label */
char find_label_ARE(Label * head ,char * name)
{

	Label* curr = head;
	char temp[LABEL_LENGTH];
	if(name[0] == '%')
	{
		copy_n_str(name+1,temp,strlen(name) - 1);
	}
	else
	{
		copy_n_str(name,temp,strlen(name));	
	}
	while(curr)
	{
		if(strcmp(curr->symbol ,temp) == 0)
		{
			
			if(name[0] == '%' && curr-> attribute != external )
			{
				return 'A';
			}

			if(curr-> attribute == external)
			{
				
				return 'E';
			}
			else
			{
				
				return 'R';
			}
			
		}
		curr = curr->next;
	}

	return -1;
}

void print_entry(FILE * new_file_pointer,Label * head)
{
	Label * curr = head;
	while(curr)
	{
		if(curr->attribute == data_and_entry || curr->attribute == code_and_entry)
		{
						
			fprintf(new_file_pointer,"\n%s %04d\n",curr->symbol,curr->value);
			
		}
	
		curr = curr->next;
	}
}

/* update entry (when you get tired of documentation) */
void update_entry(Label* head ,char * label,int *errors_flag, int line_counter)
{
	int error = 1;
	Label* curr = head;
	while(curr)
	{
		if(strcmp(curr->symbol , label) == 0)
		{
			
			if(curr->attribute == data)
			{
				(curr-> attribute) = data_and_entry;
				error = 0;
			}
			else if((curr-> attribute) == code)
			{
				(curr-> attribute) = code_and_entry;
				error = 0;
			}
			else if((curr-> attribute) == external)
			{
				error =0;
				printf("\nerror,attempt to declare a label entry and extern, the label's name: %s, line  %d\n",label ,line_counter);
				*(errors_flag) = 1;		
			}
		}
		curr = curr->next;
	}
	if(error)
	{
		*(errors_flag) = 1;		
		printf("\nerror, an .entry directive on a label which does not exist line %d\n", line_counter);
	}
}

