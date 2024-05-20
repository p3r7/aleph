#include "fat_filelib_comp.h"

FILE *my_fopen(const char *filename, const char *mode) {
    return (FILE *)fl_fopen(filename, mode);
}

int my_fclose(FILE *stream) {
    fl_fclose((FL_FILE *)stream);
    return 0;
}

int my_fflush(FILE *stream) {
    return fl_fflush((FL_FILE *)stream);
}

int my_fgetc(FILE *stream) {
    return fl_fgetc((FL_FILE *)stream);
}

char *my_fgets(char *str, int num, FILE *stream) {
    return fl_fgets(str, num, (FL_FILE *)stream);
}

int my_fputc(int c, FILE *stream) {
    return fl_fputc(c, (FL_FILE *)stream);
}

int my_fputs(const char *str, FILE *stream) {
    return fl_fputs(str, (FL_FILE *)stream);
}

size_t my_fwrite(const void *ptr, size_t size, size_t count, FILE *stream) {
    return fl_fwrite(ptr, size, count, (FL_FILE *)stream);
}

size_t my_fread(void *ptr, size_t size, size_t count, FILE *stream) {
    return fl_fread(ptr, size, count, (FL_FILE *)stream);
}

int my_fseek(FILE *stream, long int offset, int whence) {
    return fl_fseek((FL_FILE *)stream, offset, whence);
}

int my_fgetpos(FILE *stream, fpos_t *pos) {
    // This might require additional handling if `fpos_t` is not directly compatible.
    return fl_fgetpos((FL_FILE *)stream, (unsigned long *)pos);
}

long int my_ftell(FILE *stream) {
    return fl_ftell((FL_FILE *)stream);
}

int my_feof(FILE *stream) {
    return fl_feof((FL_FILE *)stream);
}

int my_remove(const char *filename) {
    return fl_remove(filename);
}

int my_mkdir(const char *pathname) {
    return fl_createdirectory(pathname);
}

int my_rmdir(const char *pathname) {
    // Assuming you always return 0 for remove directory as per your list.
    return 0;
}
