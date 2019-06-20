#include "../include/t2fs.h"
#include "../include/apidisk.h"
#include "../include/support.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	extern WORD working_dir_block;
	extern _Bool __boot_init, __root_done;
	int dirhandler;
	char name[30];
	DIRENT2 entry;

	format2(2);
	getcwd2(name, 30);

	printf("MAIN: working dir: %s\n", name);
	printf("MAIN: workind dir block: %d\n", working_dir_block);
	printf("MAIN: boot init: %d\n", __boot_init);
	printf("MAIN: root done: %d\n", __root_done);
	printf("\n\n\n");

	mkdir2("./newdir");
	mkdir2("./otherdir");
	mkdir2("./otherdir1");
	mkdir2("./otherdir2");
	mkdir2("./otherdir3");
	mkdir2("./otherdir4");
	mkdir2("./otherdir5");
	mkdir2("./otherdir6");
	mkdir2("./otherdir7");
	mkdir2("./otherdir8");
	mkdir2("./otherdir9");
	mkdir2("./otherdir10");
	mkdir2("./otherdir11");
	mkdir2("./otherdir12");
	mkdir2("./otherdir13");
	mkdir2("./otherdir14");
	mkdir2("./otherdir15");
	mkdir2("./otherdir16");
	mkdir2("./newdir/oi");
	chdir2("./newdir");

	getcwd2(name, 30);
	printf("MAIN: working dir: %s\n", name);
	printf("MAIN: workind dir block: %d\n", working_dir_block);
	mkdir2("./WeMadeIt");
	mkdir2("./itsover");

	chdir2("./..");
	getcwd2(name, 30);
	printf("MAIN: workind dir block: %d\n", working_dir_block);
	printf("MAIN: working dir: %s\n", name);
	chdir2("./newdir/oi");
	printf("MAIN: workind dir block: %d\n", working_dir_block);
	printf("MAIN: working dir: %s\n", name);
	mkdir2("./final");


	dirhandler = opendir2("./../../.");

	printf("\nMAIN: handler %d\n", dirhandler);

	printf("\nMAIN: ls:\n");
	while(readdir2(dirhandler, &entry) == 0)
	{
		printf("\n");
		printf("MAIN: nome do arquivo: %s\n", entry.name);
		printf("MAIN: tipo do arquivo: %d\n", entry.fileType);
		printf("MAIN: bloco do arquivo: %d\n", entry.indexBlock);
		if((strcmp(entry.name, "..\0") != 0) && entry.fileType == 3)
			printf("MAIN: numero de entradas: %d\n", entry.numberOfEntries);
		printf("\n");
	}

	closedir2(dirhandler);

	if(readdir2(dirhandler, &entry) != 0)
		printf("MAIN: arquivo fechado\n");

	return 0;
}
