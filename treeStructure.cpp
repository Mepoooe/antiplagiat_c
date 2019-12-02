#include "pch.h"
#include <iostream>
#include "constants.cpp"
#include "antiplagiatModel.h"
#include "treeStructure.h"
#include "lineStructure.h"
#include "stringHelper.h"
#include <windows.h>

void initTreeStructure() {
	char* fileName = (char*)malloc(MAX_PATH * sizeof(char));
	scanf_s("%s", fileName, MAX_PATH);

	printf("Start load words from file: \n");

	textTree* sourceTextTree = loadTextTreeFromFile(NULL, fileName);
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
					root = addWordInTree(root, buffer, NULL);
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
		root = addWordInTree(root, buffer, NULL);
	}

	free(buffer);
	return root;
}

textTree* addWordInTree(textTree* node, char* word, textTree* sourceNode) {
	char *wordInUpperCase = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
	strcpy_s(wordInUpperCase, MAX_WORD_SIZE, word);
	strtoupper(wordInUpperCase, MAX_WORD_SIZE, wordInUpperCase);

	if (NULL == node) {
		// allocate memory for the structure
		node = (textTree*)malloc(sizeof(struct textTree));
		node->word = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
		strcpy_s(node->word, MAX_WORD_SIZE, word);

		node->wordInUpperCase = (char*)malloc(MAX_WORD_SIZE * sizeof(char));
		strcpy_s(node->wordInUpperCase, MAX_WORD_SIZE, word);
		strtoupper(node->wordInUpperCase, MAX_WORD_SIZE, node->wordInUpperCase);

		if (NULL != sourceNode)
		{
			node->count = sourceNode->count;
		}
		else {
			node->count = 1;
		}

		node->left = NULL;
		node->right = NULL;
	}
	else {
		if (0 < strcmp(node->wordInUpperCase, wordInUpperCase)) {
			node->left = addWordInTree(node->left, word, sourceNode);
		}
		else if (0 > strcmp(node->wordInUpperCase, wordInUpperCase)) {
			node->right = addWordInTree(node->right, word, sourceNode);
		}
		else {
			node->count++;
		}
	}
	free(wordInUpperCase);
	return node;
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

int wordCountInTree(textTree* root) {
	int wordCount = 1;
	if (NULL == root)
	{
		return 0;
	}
	else {
		wordCount += wordCountInTree(root->left);
		wordCount += wordCountInTree(root->right);
		return wordCount++;
	}

	return wordCount;
};

textTree* printTextTree(textTree* root) {
	if (NULL == root)
	{
		return NULL;
	}
	printTextTree(root->left);
	printTextTree(root->right);
	printf("-%s; count - %d\n", root->word, root->count);
};

textTree* findWordInOriginTree(textTree* sourceTextTree, textTree* originalTextTree, textTree* treeWithComparedWords) {

	if (NULL != originalTextTree)
	{
		if (0 == strcmp(sourceTextTree->wordInUpperCase, originalTextTree->wordInUpperCase)) {
			float percentOfRepetitionRate = culcPercentOfRepetitionRate(sourceTextTree->count, originalTextTree->count);
			bool isHightPercentOfRepetitionRate = (percentOfRepetitionRate >= 70);

			if (false == isWordExistInTree(treeWithComparedWords, sourceTextTree->word) && isHightPercentOfRepetitionRate) {
				treeWithComparedWords = addWordInTree(treeWithComparedWords, sourceTextTree->word, sourceTextTree);
			}
		}
		else if (0 < strcmp(sourceTextTree->wordInUpperCase, originalTextTree->wordInUpperCase)) {
			findWordInOriginTree(sourceTextTree, originalTextTree->right, treeWithComparedWords);
		}
		else if (0 > strcmp(sourceTextTree->wordInUpperCase, originalTextTree->wordInUpperCase)) {
			findWordInOriginTree(sourceTextTree, originalTextTree->left, treeWithComparedWords);
		}

	}

	return treeWithComparedWords;
};

textTree* itarateTree(textTree* sourceTextTree, textTree* originalTextTree, textTree* treeWithComparedWords) {
	if (NULL != sourceTextTree) {
		treeWithComparedWords = findWordInOriginTree(sourceTextTree, originalTextTree, treeWithComparedWords);
		itarateTree(sourceTextTree->left, originalTextTree, treeWithComparedWords);
		itarateTree(sourceTextTree->right, originalTextTree, treeWithComparedWords);
	}

	return treeWithComparedWords;
};

textTree* findWordsWichComparedTrees(textTree* sourceTextTree, textTree* originalTextTree) {

	return itarateTree(sourceTextTree, originalTextTree, NULL);
};

