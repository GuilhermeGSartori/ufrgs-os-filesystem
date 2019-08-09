/**
 * Change current working directory.
 * 
 * @author Guilherme Sartori
 */
#include "../include/constants.h"
#include "../include/t2fs.h"
#include "../include/support.h"
#include <stdlib.h>
#include <string.h>

int chdir2(char *pathname)
{
	extern _Bool __boot_init; 
	extern char working_dir_path[10000];
	extern unsigned int root_block;
	extern WORD working_dir_block, temp_dir_block;
	WORD block;
	int path_size, i, j;

	if(check_if_root(pathname) == 0)
	{
		block = root_block;
		working_dir_path[0] = '.';
		working_dir_path[1] = '/';
		working_dir_path[2] = '\0';
		working_dir_block = block;
	}

	else
	{
		if(__boot_init == 0)
			boot2();

		string *path = parse_path(pathname, &path_size);
		if(path[0][0] != '.')
		{
			printf("path invalido, comece com './'!\n");
			return -1;
		}
		temp_dir_block = working_dir_block;
		for(i = 1; i < path_size; i++)
		{
			if(find_target_dir(path[i]) != 0)
			{
				printf("path invalido '%s' nao existe!\n", path[i]);
				return -1;
			}

		}
		working_dir_block = temp_dir_block;

		for(j = 1; j < path_size; j++)
		{
			path[j][strlen(path[j])] = '\0';
			strcat(working_dir_path, path[j]);
		}
	}


    return 0;
}
