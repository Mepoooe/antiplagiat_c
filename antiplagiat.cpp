#include "pch.h"
#include "antiplagiatModel.h"
#include "constants.cpp"
#include <iostream>
#include <windows.h>
#include <locale.h>

int main()
{
	setlocale(LC_ALL, ".1251");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);


	int flag = 0;
	int keyPressed = 0;
	char* command = (char*)malloc(MAX_WORD_SIZE * sizeof(char));

	do {
		printf("\n Please enter command. For example: 'run', 'exit'\n");
		scanf_s("%s", command, MAX_WORD_SIZE);

		if (0 == strcmp("run", command))
		{
			initAntoplagiat();
		} else if ((0 == strcmp("exit", command))) {
			flag = 1;
		}
		else {
			printf("\n You print wrong command, please enter right command. For example: 'run', 'exit'\n");
		}


	} while (0 == flag);
	free(command);
	return 0;
}

