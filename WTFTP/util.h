#ifndef UTIL_H
#define UTIL_H

#include "pre_defination.h"

#define LF '\n'
#define CR '\r'
#define NUL '\0'
#define PLATFORM_WINDOWS 1
#define PLATFORM_LINUX 2

#define FULLSTRLEN(str) (strlen(str) + 1)
#define FULLNUMLEN(num) (number_length(num) + 1)

int append_Message(char* buf, rsize_t bufferCount, int* offset, const char* format, ...);
int get_Message(char* buf, rsize_t bufferCount, const char* source, int* offset);
int get_filesize(const char* fileName);
int number_length(int num);

int encodeNetascii(const char *fileName);
int decodeNetascii(const char *fileName, int platform);

#endif // UTIL_H
