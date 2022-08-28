#include "main.h"

int main(int argc, char * argv[])
{
	int index = 1;
	int exit_status = 1;

	if(argc == 1) /* no files were inserted */
	{
		puts("\nNo files were inserted. Exiting...");
		return(_EXIT_NO_FILES);
	}	
	
	for(;index < argc ; index++) /* scan the argv array */
	{
		exit_status = first_pass(argv[index]); /* send the files to the assembler */

		if (exit_status == _EXIT_FILES_FAILURE)
			puts("\nSomething went wrong - didn't open file. Exiting...");

	}

	return exit_status;
}

