#include "pch.h"
#include <iostream>
#include "constants.cpp"
#include "antiplagiatModel.h"
#include "stringHelper.h"
#include <windows.h>

void initAntoplagiat(bool isUseTree) {
	printf("Please entere path to file. For example 'd:\\sometext.txt'\n");
	printf("Be attention, we work only with files which consists russian symbols.\n");

	if (isUseTree)
	{
		useTree();
	} else {
		useLine();
	}
}

void useTree() {
	char* fileName = (char*)malloc(MAX_PATH * sizeof(char));
	scanf_s("%s", fileName, MAX_PATH);

	printf("Start load words from file: \n");

	textTree* sourceTextTree = loadTextTreeFromFile(NULL, fileName);
	//printTextTree(sourceTextTree);
	int wordCountInSourceTextTree = wordCountInTree(sourceTextTree);

	printf("Was loaded %d words \n", wordCountInSourceTextTree);

	if (wordCountInSourceTextTree <= 0)
	{
		printf("Please check file, or load other file. \n");
		free(fileName);
		clearTree(sourceTextTree);
	}
	else {
		free(fileName);

		printf("Please enter path to file for compare (original file). For example 'd:\\some_original_text.txt' \n");

		fileName = (char*)malloc(MAX_PATH * sizeof(char));
		scanf_s("%s", fileName, MAX_PATH);

		textTree* originalTextTree = loadTextTreeFromFile(NULL, fileName);
		int wordCountInOriginalTextTree = wordCountInTree(originalTextTree);

		if (wordCountInOriginalTextTree <= 0)
		{
			printf("Was loaded %d words \n", wordCountInOriginalTextTree);
			printf("Please check file, or load other file. \n");
			free(fileName);
			clearTree(sourceTextTree);
			clearTree(originalTextTree);
		}
		else {
			textTree* listWithComparedWords = findWordsWichComparedTrees(sourceTextTree, originalTextTree);
			int countReparidWords = wordCountInTree(listWithComparedWords);
			int percentOfRepetitionRate = culcPercentOfRepetitionRate((float)wordCountInSourceTextTree, (float)countReparidWords);

			if (percentOfRepetitionRate >= 51)
			{
				printf("\n This text is plagiat, percent of repetition rate - %d %", percentOfRepetitionRate);
			}
			else {
				printf("\n This text is not plagiat, percent of repetition rate - %d %", percentOfRepetitionRate);
			}

			printf("\n List of words that were repeated: \n");
			printTextTree(listWithComparedWords);

			free(fileName);
			clearTree(sourceTextTree);
			clearTree(originalTextTree);
			clearTree(listWithComparedWords);
		}
	}
}

void useLine() {
	char* fileName = (char*)malloc(MAX_PATH * sizeof(char));
	scanf_s("%s", fileName, MAX_PATH);

	printf("Start load words from file: \n");

	listOfText* sourceList = loadTextFromFile(NULL, fileName);
	int wordCountInSourceList = wordCount(sourceList);
	printf("Was loaded %d words \n", wordCountInSourceList);

	if (wordCountInSourceList <= 0)
	{
		printf("Please check file, or load other file. \n");
		free(fileName);
		clearList(sourceList);
	}
	else {
		free(fileName);

		printf("Please enter path to file for compare (original file). For example 'd:\\some_original_text.txt' \n");

		fileName = (char*)malloc(MAX_PATH * sizeof(char));
		scanf_s("%s", fileName, MAX_PATH);

		listOfText* originalList = loadTextFromFile(NULL, fileName);
		int wordCountInOriginalList = wordCount(originalList);

		if (wordCountInOriginalList <= 0)
		{
			printf("Was loaded %d words \n", wordCountInOriginalList);
			printf("Please check file, or load other file. \n");
			free(fileName);
			clearList(sourceList);
			clearList(originalList);
		}
		else {
			listOfText* listWithComparedWords = findWordsWichCompared(sourceList, originalList);
			int countReparidWords = wordCount(listWithComparedWords);
			int percentOfRepetitionRate = culcPercentOfRepetitionRate((float)wordCountInSourceList, (float)countReparidWords);

			if (percentOfRepetitionRate >= 51)
			{
				printf("\n This text is plagiat, percent of repetition rate - %d %", percentOfRepetitionRate);
			}
			else {
				printf("\n This text is not plagiat, percent of repetition rate - %d %", percentOfRepetitionRate);
			}

			printf("\n List of words that were repeated: \n");
			printTextList(listWithComparedWords);

			free(fileName);
			clearList(sourceList);
			clearList(originalList);
			clearList(listWithComparedWords);
		}
	}
}

void clearList(listOfText* list) {
	list = toFirstWord(list);
	listOfText* prev = NULL;

	while (NULL != list) {
		deleteWord(list->next);
		list = list->next;
	}

	deleteWord(list);
}

void clearTree(textTree* root) {
	if (NULL != root)
	{
		clearTree(root->left);
		clearTree(root->right);
		free(root->word);
		free(root->wordInUpperCase);
		free(root);
	}
}

void deleteWord(listOfText* item) {
	listOfText* nxt, * prv;
	if (NULL != item) {
		nxt = item->next;
		prv = item->pred;
		if (NULL != nxt) {
			nxt->pred = item->pred;
		}
		if (NULL != prv) {
			prv->next = item->next;
		}
		free(item->word);
		free(item->wordInUpperCase);
		free(item);
	}
}

listOfText* toFirstWord(listOfText* item) {
	listOfText* result = item;
	if (NULL != result) {
		while (NULL != result->pred) {
			result = result->pred;
		}
	}
	return result;
}

listOfText* toLastWord(listOfText* item) {
	listOfText* result = item;
	if (NULL != result) {
		while (NULL != result->next) {
			result = result->next;
		}
	}
	return result;
}

listOfText* initWord(const char* word) {
	listOfText* result = (listOfText*)malloc(sizeof(struct listOfText));

	result->word = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
	strcpy_s(result->word, MAX_WORD_SIZE, word);

	result->wordInUpperCase = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
	strcpy_s(result->wordInUpperCase, MAX_WORD_SIZE, word);
	strtoupper(result->wordInUpperCase, MAX_WORD_SIZE, result->wordInUpperCase);

	result->next = NULL;
	result->pred = NULL;
	return result;
}

listOfText* insertWordInList(listOfText* pred, const char* word) {
	listOfText* result = initWord(word);
	result->count = 1;

	result->pred = pred;
	if (NULL != pred) {
		result->next = pred->next;
		if (NULL != pred->next) {
			pred->next->pred = result;
		}
		pred->next = result;
	}
	return (result);
}

textTree* loadTextTreeFromFile(textTree* root, const char* fileName) {
	FILE* fileToLoad;

	// buffer for read words from file 
	char* buffer = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
	char* parser = buffer;
	char symbol;
	int err = fopen_s(&fileToLoad, fileName, "r");

	nullStr(buffer, MAX_WORD_SIZE);
	if (0 == err) {
		symbol = getc(fileToLoad);
		while (!feof(fileToLoad)) {
			if (isDelimeter(symbol)) {
				buffer = parser;

				if (isWord(buffer, MAX_WORD_SIZE))
				{
					root = addWordInTree(root, buffer);
				}

				nullStr(buffer, MAX_WORD_SIZE);
			}
			else {
				//@todo if symbols more than 100
				if (symbol == ' ') continue;
				*buffer = symbol;
				buffer++;
			}

			symbol = getc(fileToLoad);
		}
		fclose(fileToLoad);
	}
	else {
		printf("File not loaded, something was wrong..\n");
	}
	buffer = parser;

	if (FALSE == isStringEmpty(buffer) && isWord(buffer, MAX_WORD_SIZE))
	{
		root = addWordInTree(root, buffer);
	}

	free(buffer);
	return root;
}

listOfText* loadTextFromFile(listOfText* item, const char* fileName) {
	FILE* fileToLoad;
	listOfText* list = toLastWord(item);

	// buffer for read words from file 
	char* buffer = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
	char* parser = buffer;
	char symbol;
	int err = fopen_s(&fileToLoad, fileName, "r");

	nullStr(buffer, MAX_WORD_SIZE);
	if (0 == err) {
		symbol = getc(fileToLoad);
		while (!feof(fileToLoad)) {
			if (isDelimeter(symbol)) {
				buffer = parser;

				if (isWord(buffer, MAX_WORD_SIZE))
				{
					if (isWordExistInList(list, buffer))
					{
						list = markSameWord(list, buffer);
					}
					else {
						list = insertWordInList(list, buffer);
					}
				}

				nullStr(buffer, MAX_WORD_SIZE);
			}
			else {
				//@todo if symbols more than 100
				if (symbol == ' ') continue;
				*buffer = symbol;
				buffer++;
			}

			symbol = getc(fileToLoad);
		}
		fclose(fileToLoad);
	}
	else {
		printf("File not loaded, something was wrong..\n");
	}
	buffer = parser;

	if (FALSE == isStringEmpty(buffer) && isWord(buffer, MAX_WORD_SIZE))
	{
		list = insertWordInList(list, buffer);
	}

	free(buffer);
	return list;
}

listOfText* findWordInList(listOfText* list, const char* word) {
	listOfText* sourceListPointer = list;
	sourceListPointer = toFirstWord(sourceListPointer);

	while (NULL != sourceListPointer) {
		if (0 == strcmp(sourceListPointer->word, word))
		{
			return sourceListPointer;
		}
		sourceListPointer = sourceListPointer->next;
	}

	return NULL;
}

textTree* addWordInTree(textTree* node, char* word) {
	if (NULL == node) {
		// allocate memory for the structure
		node = (textTree*)malloc(sizeof(struct textTree));
		node->word = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
		strcpy_s(node->word, MAX_WORD_SIZE, word);

		node->wordInUpperCase = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
		strcpy_s(node->wordInUpperCase, MAX_WORD_SIZE, word);
		strtoupper(node->wordInUpperCase, MAX_WORD_SIZE, node->wordInUpperCase);

		node->count = 0;
		node->left = NULL;
		node->right = NULL;
	}
	else {
		strtoupper(word, MAX_WORD_SIZE, word);
		if (0 < strcmp(node->wordInUpperCase, word)) {
			node->left = addWordInTree(node->left, word);
		}
		else if (0 > strcmp(node->wordInUpperCase, word)) {
			node->right = addWordInTree(node->right, word);
		}
		else {
			node->count++;
		}
	}
	return node;
}

listOfText* markSameWord(listOfText* list, const char* word) {
	listOfText* currentWord = list;
	list = toFirstWord(list);

	while (NULL != list) {
		if (isSameWord(list->word, word))
		{
			list->count++;
		}
		list = list->next;
	}

	list = currentWord;
	return list;
}

bool isWordExistInList(listOfText* list, char* str) {
	list = toFirstWord(list);

	while (NULL != list) {
		if (isSameWord(list->word, str))
		{
			return true;
		}
		list = list->next;
	}

	return false;
}

bool isWordExistInTree(textTree* node, char* str) {
	if (NULL == node)
	{
		return false;
	}

	if (isSameWord(node->word, str))
	{
		return true;
	}

	if (isWordExistInTree(node->left, str) || isWordExistInTree(node->right, str))
	{
		return true;
	}

	return false;
}

void printWord(listOfText* item) {
	printf("-%s- count- %d\n", item->word, item->count);
}

void printTextList(listOfText* list) {
	list = toFirstWord(list);
	while (NULL != list) {
		printWord(list);
		list = list->next;
	}
};

textTree* printTextTree(textTree* root) {
	if (NULL == root)
	{
		return NULL;
	}
	printTextTree(root->left);
	printTextTree(root->right);
	printf("-%s- count- %d\n", root->word, root->count);
};

int wordCount(listOfText* list) {
	int wordCount = 0;
	list = toFirstWord(list);
	while (NULL != list) {
		wordCount++;
		list = list->next;
	}

	return wordCount;
};

int wordCountInTree(textTree* root) {
	int wordCount = 1;
	if (NULL == root)
	{
		return 0;
	} else {
		wordCount += wordCountInTree(root->left);
		wordCount += wordCountInTree(root->right);
		return wordCount++;
	}

	return wordCount;
};

float culcPercentOfRepetitionRate(float firsInteger, float secondInteger) {
	float result = ((secondInteger / firsInteger) * 100);
	return result;
};

listOfText* findWordsWichCompared(listOfText* sourceList, listOfText* originalList) {
	listOfText* listWithComparedWords = NULL;

	listOfText* sourceListPoiner = toFirstWord(sourceList);
	listOfText* originalListPointer = NULL;

	while (NULL != sourceListPoiner) {
		originalListPointer = toFirstWord(originalList);
		while (NULL != originalListPointer) {
			if (0 == strcmp(sourceListPoiner->wordInUpperCase, originalListPointer->wordInUpperCase)) {
				float percentOfRepetitionRate = culcPercentOfRepetitionRate(sourceListPoiner->count, originalListPointer->count);
				bool isHightPercentOfRepetitionRate = (percentOfRepetitionRate >= 70);

				if (false == isWordExistInList(listWithComparedWords, sourceListPoiner->word) && isHightPercentOfRepetitionRate) {
					listWithComparedWords = insertWordInList(listWithComparedWords, sourceListPoiner->word);
					listWithComparedWords->count = sourceListPoiner->count;
				}
			}

			originalListPointer = originalListPointer->next;
		}
		sourceListPoiner = sourceListPoiner->next;
	}

	return listWithComparedWords;
};

textTree* itarateTwoTree(textTree* sourceTextTree, textTree* originalTextTree, textTree* treeWithComparedWords) {
	while (NULL != sourceTextTree) {
		while (NULL != originalTextTree) {
			if (0 == strcmp(sourceTextTree->wordInUpperCase, originalTextTree->wordInUpperCase)) {
				float percentOfRepetitionRate = culcPercentOfRepetitionRate(sourceTextTree->count, originalTextTree->count);
				bool isHightPercentOfRepetitionRate = (percentOfRepetitionRate >= 70);

				if (false == isWordExistInTree(treeWithComparedWords, sourceTextTree->word) && isHightPercentOfRepetitionRate) {
					treeWithComparedWords = addWordInTree(treeWithComparedWords, sourceTextTree->word);
					treeWithComparedWords->count = sourceTextTree->count;
				}
			}

			itarateTwoTree(sourceTextTree, originalTextTree->left, treeWithComparedWords);
			itarateTwoTree(sourceTextTree, originalTextTree->right, treeWithComparedWords);
		}
		itarateTwoTree(sourceTextTree->left, originalTextTree, treeWithComparedWords);
		itarateTwoTree(sourceTextTree->right, originalTextTree, treeWithComparedWords);
	}

	return treeWithComparedWords;
};

textTree* findWordsWichComparedTrees(textTree* sourceTextTree, textTree* originalTextTree) {
	textTree* treeWithComparedWords = NULL;

	textTree* sourceTextTreePoiner = sourceTextTree;
	textTree* originalTextTreePointer = NULL;

	return itarateTwoTree(sourceTextTree, originalTextTree, treeWithComparedWords);
};

