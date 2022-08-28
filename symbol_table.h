/* header to symbol_table.c */

#ifndef SYMBOL_TABLE_H 				/* include guard */

	#define SYMBOL_TABLE_H

	#include "libs.h"					/* include C standard libraries */
	#include "symbol_table_types.h"		/* include symbol table's new types */

	/* function prototypes */

	/* create a new node (label) based on the given parameters */
	Label *create_label(char *, int, int);

	/* connect a new node (label) to the linked list */
	void add_label(Label* newLabel, Label** head, int line_counter,int * errors_flag);

	/* update the value of every data label by adding the final value of code_counter to it */
	void update_data_value(Label *, int);

	/* print the linked list (the symbol table) */
	void print_list(Label*);

	/* free all of the list's memory */
	void free_list(Label **);

	/* find the value of a given label */
	int find_label_value(Label * ,char *);

	/* find the ARE value of a given label */ 
	char find_label_ARE(Label * ,char *);

	void print_entry(FILE *,Label *);

	void update_entry(Label* ,char *,int *, int);


#endif



















