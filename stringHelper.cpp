#include "pch.h"
#include "stringHelper.h"
#include "constants.cpp"
#include <iostream>

void strtoupper(char* sourceStr, int count, char* destStr) {
	char* source = sourceStr;
	char* destination = destStr;

	for (int i = 0; i < count; i++)
	{
		*destination = toupper(*source);
		source++;
		destination++;
	}
}

bool isDelimeter(char symbol) {
	for (int i = 0; i < delimeterCount; i++)
	{
		if (symbol == delimeters[i]) {
			return true;
		}
	}

	return false;
}

/*
* Parameter str must point on first symbol, and if it zero this is end of string
* @param char *str
* @return bool
*/
bool isStringEmpty(char* str) {
	return *str == 0;
}

void nullStr(char* str, int count) {
	char* wrk = str;
	for (int i = 0; i < count; i++) {
		*wrk = 0;
		wrk++;
	}
}

bool isWord(char* str, int count) {
	char* wrk = str;
	for (int i = 0; i < count; i++) {
		if (*wrk >= 'À' && *wrk <= 'ÿ')
		{
			return true;
		}
		wrk++;
	}

	return false;
}

bool isSameWord(char* firstWord, const char* secondWord) {
	char* firstWordInUpperCase = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
	strcpy_s(firstWordInUpperCase, MAX_WORD_SIZE, firstWord);
	strtoupper(firstWordInUpperCase, MAX_WORD_SIZE, firstWordInUpperCase);

	char* secondWordInUpperCase = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
	strcpy_s(secondWordInUpperCase, MAX_WORD_SIZE, secondWord);
	strtoupper(secondWordInUpperCase, MAX_WORD_SIZE, secondWordInUpperCase);

	if (0 == strcmp(firstWordInUpperCase, secondWordInUpperCase))
	{
		free(firstWordInUpperCase);
		free(secondWordInUpperCase);
		return true;
	}

	free(firstWordInUpperCase);
	free(secondWordInUpperCase);
	return false;
}