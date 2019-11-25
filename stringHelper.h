#ifndef STRING_HELPER
#define STRING_HELPER

const char delimeters[] = { ' ', ',', '\n', '\t', '.', '-', ':', ';', '!', '?' };
const int delimeterCount = (sizeof(delimeters) / sizeof(delimeters[0]));

void strtoupper(char* sourceStr, int count, char* destStr);
void nullStr(char* str, int count);

bool isDelimeter(char symbol);
bool isStringEmpty(char* str);
bool isWord(char* str, int count);
bool isSameWord(char* firstWord, const char* secondWord);
#endif