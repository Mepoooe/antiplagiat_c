#include "pch.h"
#include "fileHelper.h"


// check the file that it is a directory
bool is_dir(DWORD FileAttr)
{
	return 0 != (FileAttr & FILE_ATTRIBUTE_DIRECTORY);
}

// check the file that it is a path to parent ".."
bool is_parent_dir(char* file_name)
{
	return 0 == strcmp(file_name, parent_dir);
}

// check the file that it is a path to parent "."
bool is_current_dir(char* file_name)
{
	return 0 == strcmp(file_name, current_dir);
}

// concat two string. 
// for example: concat("Misha ", "Nikula") == "Misha Nikula"
// don't forget call free(result) after use var
char* concat(const char* s1, const char* s2)
{
	int buffer_size = (strlen(s1) + strlen(s2) + 1); // +1 for "\0"
	char* result = (char*)malloc(strlen(s1) + strlen(s2) + buffer_size);

	strcpy_s(result, buffer_size, s1);
	strcat_s(result, buffer_size, s2);

	return result;
}

// print file
// if it is not a file call functioni show_all_files() with new path
void show_file(char* path, WIN32_FIND_DATAA* FileFind)
{
	// check the file that it is a directory
	if (true == is_dir(FileFind->dwFileAttributes))
	{
		// check the directory that it is not a ".." or "."
		// I init var "is_real_dir" and add to if for better readability
		bool is_real_dir = (!is_parent_dir(FileFind->cFileName)) && (!is_current_dir(FileFind->cFileName));

		if (true == is_real_dir)
		{
			// create new path
			char* temp_path = concat(FileFind->cFileName, "/");
			char* new_mask = concat(path, temp_path);
			free(temp_path);

			// and call show_all_files for repeat functionality
			show_all_files(new_mask);
			//don't forget clean after self
			free(new_mask);
		}
	}
	else {
		//print file name
		printf(" |%s|, full path - %s%s \n", FileFind->cFileName, path, FileFind->cFileName);
	}
}

// find and show all directories in all dirs
void show_all_text_files_in_all_dirs(char* path)
{
	show_all_files(path);
	// init winAPI for search files
	WIN32_FIND_DATAA* FileFind = (WIN32_FIND_DATAA*)malloc(sizeof(WIN32_FIND_DATAA));
	HANDLE hFoundFiles;
	const char searchable_all[] = "*.*";

	// make mask for search
	char* mask = concat(path, searchable_all);
	hFoundFiles = FindFirstFileA(mask, FileFind);

	if (INVALID_HANDLE_VALUE != hFoundFiles)
	{
		// show file
		if (is_dir(FileFind->dwFileAttributes))
		{
			show_file(path, FileFind);
		}

		while (FindNextFileA(hFoundFiles, FileFind)) {
			if (is_dir(FileFind->dwFileAttributes))
			{
				show_file(path, FileFind);
			}
		}
		// clean memory
		free(mask);
		FindClose(hFoundFiles);
	}

	free(FileFind);
}

// find and show all files in dir
void show_all_files(char* path)
{
	// init winAPI for search files
	WIN32_FIND_DATAA* FileFind = (WIN32_FIND_DATAA*)malloc(sizeof(WIN32_FIND_DATAA));
	HANDLE hFoundFiles;

	// make mask for search
	char* mask = concat(path, searchable_files);

	hFoundFiles = FindFirstFileA(mask, FileFind);

	if (INVALID_HANDLE_VALUE != hFoundFiles)
	{
		// show file
		show_file(path, FileFind);

		while (FindNextFileA(hFoundFiles, FileFind)) {
			show_file(path, FileFind);
		}

	}

	// clean memory
	free(mask);
	FindClose(hFoundFiles);
	free(FileFind);
}

int initFileHelper(char* main_path)
{
	// start path
	//char main_path[] = "d:\\";

	printf("=============================================\n");
	printf("==============START==========================\n");
	printf("=============================================\n");
	printf("\n");

	// call function for print all files
	show_all_text_files_in_all_dirs(main_path);

	printf("\n");
	printf("=============================================\n");
	printf("==============END============================\n");
	printf("=============================================\n");
	return (0);
}