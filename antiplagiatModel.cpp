#include "pch.h"
#include <iostream>
#include "constants.cpp"
#include "antiplagiatModel.h"
#include "treeStructure.h"
#include "lineStructure.h"
#include "stringHelper.h"
#include <windows.h>

void initAntoplagiat(bool isUseTree) {
	printf("Please entere path to file. For example 'd:\\sometext.txt'\n");
	printf("Be attention, we work only with files which consists russian symbols.\n");

	if (isUseTree)
	{
		initTreeStructure();
	}
	else {
		initLineStructure();
	}
}

float culcPercentOfRepetitionRate(float firsInteger, float secondInteger) {
	float result = ((secondInteger / firsInteger) * 100);
	return result;
};