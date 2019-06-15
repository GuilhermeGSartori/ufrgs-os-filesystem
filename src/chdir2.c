/**
 * Change current working directory.
 * 
 * @author Guilherme Sartori
 */
#include "../include/constants.h"
#include "../include/t2fs.h"
#include "../include/support.h"
#include <stdlib.h>

int chdir2(char *pathname)
{
	extern char working_dir_path[10000];
	extern unsigned int root_block;
	extern WORD working_dir_block;
	WORD block;

	if(check_if_root(pathname) == 0)
	{
		block = root_block;
		working_dir_path[0] = '/';
		working_dir_path[1] = '\0';
		working_dir_block = block;
	}

	else
		printf("not implemented yet :)\n");


    return 0;
}
