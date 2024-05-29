#ifndef __FAT_FILELIB_COMP_H__
#define __FAT_FILELIB_COMP_H__

#include <stdio.h>  // For FILE, size_t
#include "fat_filelib.h"
#include "types.h"

// File Operations
FILE *my_fopen(const char *filename, const char *mode);
int my_fclose(FILE *stream);
int my_fflush(FILE *stream);
int my_fgetc(FILE *stream);
char *my_fgets(char *str, int num, FILE *stream);
int my_fputc(int c, FILE *stream);
int my_fputs(const char *str, FILE *stream);
size_t my_fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
size_t my_fread(void *ptr, size_t size, size_t count, FILE *stream);
int my_fseek(FILE *stream, long int offset, int whence);
int my_fgetpos(FILE *stream, fpos_t *pos);
long int my_ftell(FILE *stream);
int my_feof(FILE *stream);
int my_remove(const char *filename);
int my_mkdir(const char *pathname);
int my_rmdir(const char *pathname);

#define fopen my_fopen
#define fclose my_fclose
#define fflush my_fflush
#define fgetc my_fgetc
#define fgets my_fgets
#define fputc my_fputc
#define fputs my_fputs
#define fwrite my_fwrite
#define fread my_fread
#define fseek my_fseek
#define fgetpos my_fgetpos
#define ftell my_ftell
#define feof my_feof
#define remove my_remove
#define mkdir my_mkdir
#define rmdir my_rmdir

#endif // __FAT_FILELIB_COMP_H__
