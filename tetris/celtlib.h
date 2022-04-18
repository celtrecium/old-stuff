#ifndef CELTLIB_H
#define CELTLIB_H

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

enum CL_CURSOR{
    CL_SHOW,
    CL_HIDE
};
void cl_cursor_mode(enum CL_CURSOR cur);
void cl_echo();
void cl_noecho();
int cl_kbhit();
void cl_term_size(int *width, int *height);
void *cl_malloc_check(size_t size);
void *cl_calloc_check(size_t number, size_t size);
void *cl_realloc_check(void *ptrmem, size_t size);
char *cl_itoa(int num);
int cl_strlen(const char *string);
char *cl_strcat(char *string, char *string_1);
/* char *cl_strcomb(int args, char *string, ...); */

#endif /* CELTLIB_H */
