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

size_t fake_fread(volatile u8* dst, u32 size, void* fp) {
    u8 c;
    size_t n = 0;
    while(n < size) {
        // *dst = fl_fgetc(fp);
        c = fl_fgetc(fp);
        if (c == EOF) {
            break;
        }
        *dst = c;
        n++;
         dst++;
     }
     return n;
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

/* size_t my_fread(void *ptr, size_t size, size_t count, FILE *stream) {
 *     // NB: buggy w/ files other a certain size
 *     // return fl_fread(ptr, size, count, (FL_FILE *)stream);
 *
 *     return fake_fread(ptr, count, (FL_FILE *)stream);
 * } */

size_t my_fread(void *ptr, size_t size, size_t count, FILE *stream) {
    size_t n = 0;
    size_t total_bytes = 0;
    u8 *dst = (u8*) ptr;

    for (size_t i = 0; i < count; i++) {
        size_t bytes_read_for_block = 0;

        for (size_t j = 0; j < size; j++) {
            int c = fl_fgetc(stream);
            // if (fl_feof(ptr) == EOF) {
            if (c == EOF) {
                return n;
            }
            *dst++ = (u8) c;
            bytes_read_for_block++;
        }

        if (bytes_read_for_block == size) {
            n++;
            total_bytes += bytes_read_for_block;
        } else {
            break;
        }
    }

    // nb blocks read
    return n;
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
