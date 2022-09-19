all: compilation_message compile clean

compilation_message:
	@echo "\ncompiling...\n"

compile : command_table.o first_pass.o second_pass.o main.o symbol_table.o utils.o
	gcc -g -Wall -ansi -pedantic command_table.o first_pass.o second_pass.o main.o symbol_table.o utils.o -o assembler.out

main.o : main.c main.h libs.h exit_codes.h
	gcc -c -Wall -ansi -pedantic main.c -o main.o

command_table.o : command_table.c command_table.h libs.h command_table_types.h utils_types.h
	gcc -c -Wall -ansi -pedantic command_table.c -o command_table.o

first_pass.o : first_pass.c first_pass.h libs.h symbol_table_types.h utils_types.h command_table_types.h exit_codes.h
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o

second_pass.o : second_pass.c second_pass.h libs.h symbol_table_types.h utils_types.h command_table_types.h exit_codes.h
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o

symbol_table.o : symbol_table.c symbol_table.h libs.h symbol_table_types.h
	gcc -c -Wall -ansi -pedantic symbol_table.c -o symbol_table.o

utils.o : utils.c libs.h utils_types.h command_table_types.h
	gcc -c -Wall -ansi -pedantic utils.c -o utils.o

clean:
	@echo "\ncleaning up (object files)...\n"
	rm *.o 

rm:
	rm assembler.out
