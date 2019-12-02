#include "pch.h"
#include "antiplagiatModel.h"
#include "constants.cpp"
#include <iostream>
#include "fileHelper.h"
#include <windows.h>
#include <locale.h>

int main()
{
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);


	int flag = 0;
	int keyPressed = 0;

	do {
		char* command = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
		printf("\n Please enter command. For example: 'run_line', 'run_tree', 'show_files', 'exit'\n");
		scanf_s("%s", command, MAX_WORD_SIZE);

		if (0 == strcmp("run_line", command))
		{
			initAntoplagiat(false);
		}
		else if ((0 == strcmp("run_tree", command))) {
			initAntoplagiat(true);
		}
		else if ((0 == strcmp("show_files", command))) {
			char* dirName = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
			printf("\n Please enter dir where start search text files\n");
			scanf_s("%s", dirName, MAX_WORD_SIZE);

			initFileHelper(dirName);
			free(dirName);
		}
		else if ((0 == strcmp("exit", command))) {
			flag = 1;
		}
		else {
			printf("\n You print wrong command, please enter right command. For example: 'run', 'exit'\n");
		}

		free(command);
	} while (0 == flag);

	return 0;
}

