#ifndef TREE_STRUCTURE
#define TREE_STRUCTURE

struct textTree {
	char* word;
	char* wordInUpperCase;
	int count;
	textTree* left;
	textTree* right;
};

void initTreeStructure();
void clearTree(textTree* root);

bool isWordExistInTree(textTree* node, char* str);

int wordCountInTree(textTree* root);

textTree* addWordInTree(textTree* root, char* word, textTree* sourceNode);
textTree* loadTextTreeFromFile(textTree* item, const char* fileName);
textTree* printTextTree(textTree* root);
textTree* findWordsWichComparedTrees(textTree* sourceTextTree, textTree* originalTextTree);
textTree* itarateTree(textTree* sourceTextTree, textTree* originalTextTree, textTree* treeWithComparedWords);
textTree* findWordInOriginTree(textTree* sourceTextTree, textTree* originalTextTree, textTree* treeWithComparedWords);

#endif