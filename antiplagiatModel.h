#ifndef ANTIPLAGIAT_MODEL
#define ANTIPLAGIAT_MODEL

struct listOfText {
	char* word;
	char* wordInUpperCase;
	int count;
	listOfText* next;
	listOfText* pred;
};

struct textTree {
	char* word;
	char* wordInUpperCase;
	int count;
	textTree* left;
	textTree* right;
};

void initAntoplagiat(bool isUseTree);
void printTextList(listOfText *root);
void printWord(listOfText *word);
void clearList(listOfText *list);
void clearTree(textTree *root);
void deleteWord(listOfText *item);
void useTree();
void useLine();

bool isWordExistInTree(textTree* node, char* str);
bool isWordExistInList(listOfText *list, char* str);

int wordCount(listOfText* list);
int wordCountInTree(textTree* root);

float culcPercentOfRepetitionRate(float firsInteger, float secondInteger);

listOfText* findWordsWichCompared(listOfText *sourceList, listOfText *originalList);
listOfText* findWordInList(listOfText* list, const char* word);
listOfText* loadTextFromFile(listOfText* item, const char* fileName);
listOfText* toLastWord(listOfText* item);
listOfText* toFirstWord(listOfText* item);
listOfText* markSameWord(listOfText* list, const char* word);

textTree* addWordInTree(textTree* root, char* word);
textTree* loadTextTreeFromFile(textTree* item, const char* fileName);
textTree *printTextTree(textTree *root);
textTree* findWordsWichComparedTrees(textTree *sourceTextTree, textTree *originalTextTree);

#endif