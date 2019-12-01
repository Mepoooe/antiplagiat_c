#ifndef FILE_HELPER
#define FILE_HELPER
#include <iostream>
#include <windows.h>

const char searchable_files[] = "*.txt";
const char current_dir[] = ".";
const char parent_dir[] = "..";


char* concat(const char* s1, const char* s2);
void show_all_files(char* path);
void show_all_text_files_in_all_dirs(char* path);
void show_file(char* path, WIN32_FIND_DATAA* FileFind);

bool is_dir(DWORD FileAttr);
bool is_parent_dir(DWORD FileAttr);
bool is_current_dir(DWORD FileAttr);

int initFileHelper(char *dirName);

#endif