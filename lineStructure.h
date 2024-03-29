#ifndef LINE_STRUCTURE
#define LINE_STRUCTURE

struct listOfText {
	char* word;
	char* wordInUpperCase;
	int count;
	listOfText* next;
	listOfText* pred;
};

void initLineStructure();
void printTextList(listOfText* root);
void printWord(listOfText* word);
void clearList(listOfText* list);
void deleteWord(listOfText* item);

bool isWordExistInList(listOfText* list, char* str);

int wordCount(listOfText* list);

listOfText* findWordsWichCompared(listOfText* sourceList, listOfText* originalList);
listOfText* findWordInList(listOfText* list, const char* word);
listOfText* loadTextFromFile(listOfText* item, const char* fileName);
listOfText* toLastWord(listOfText* item);
listOfText* toFirstWord(listOfText* item);
listOfText* markSameWord(listOfText* list, const char* word);

#endif