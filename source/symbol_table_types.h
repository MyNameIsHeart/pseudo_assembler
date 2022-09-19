#ifndef SYMBOL_TABLE_TYPES_H /* include guard */

    #define SYMBOL_TABLE_TYPES_H

    /* defines */

	#define LABEL_LENGTH 33 			/* 33 = max length (31) + ':' + end-of-string char */

    /* new types */

    /* attribute options */
    enum attributes {code, data, code_and_entry, data_and_entry, external};

    /* datatype to represent a line in the symbol table */
    typedef struct Label
    {
        char symbol[LABEL_LENGTH];      /* label's name */
        int value;                      /* address/value of the label (decimal) */
        enum attributes attribute;      /* additional attributes */
        struct Label *next;             /* linked list */
    } Label;

#endif