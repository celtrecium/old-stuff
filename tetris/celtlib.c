#include "celtlib.h"

static struct termios stored_settings;

void cl_cursor_mode(enum CL_CURSOR cur){
    switch(cur){
        case CL_SHOW:
            printf("\033[?25h");
            break;

        case CL_HIDE:
            printf("\033[?25l");
            break;

    }
}

void cl_echo() {
	tcsetattr(0,TCSANOW,&stored_settings);
	
	return;
}

void cl_noecho() {
	struct termios new_settings;

	tcgetattr(0,&stored_settings);

	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON & ~ECHO);
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;

	tcsetattr(0,TCSANOW,&new_settings);
	
	return;
}

int cl_kbhit(void) {
	struct termios oldt, newt;
	
	int ch;
	int oldf;
	
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();
	
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	if(ch != EOF) {
		ungetc(ch, stdin);
		return(1);
	}
	
	return(0);
}

void cl_term_size(int *width, int *height) {
	struct winsize size;

	ioctl( 0, TIOCGWINSZ, (char *) &size );

	*height = size.ws_row;
	*width = size.ws_col;
}

void *cl_malloc_check(size_t size) {
    void *tmp;
    
    tmp = malloc(size);
    
    if(!tmp) {
        puts("Memory allocation error");
        
        cl_echo();
        cl_cursor_mode(CL_SHOW);

        exit(EXIT_FAILURE);
    }

    return(tmp);
}

void *cl_calloc_check(size_t number, size_t size) {
    void *tmp;
    
    tmp = calloc(number, size);
    
    if(!tmp) {
        puts("Memory allocation error");
        
        cl_echo();
        cl_cursor_mode(CL_SHOW);

        exit(EXIT_FAILURE);
    }

    return(tmp);
}

void *cl_realloc_check(void *ptrmem, size_t size) {
    void *tmp;
    
    tmp = realloc(ptrmem, size);
    
    if(!tmp) {
        puts("Memory allocation error");
        
        cl_echo();
        cl_cursor_mode(CL_SHOW);

        exit(EXIT_FAILURE);
    }

    return(tmp);
}

char *cl_itoa(int num) {
    int count = 0;
    int copy = num;
    char *result = cl_calloc_check(num + 1, 1);

    do {
        ++count;
    } while(copy /= 10);

    int i = count - 1;

    for(; i >= 0; --i) {
        result[i] = num % 10 + '0';
        num /= 10;
    }
    
    return(result);
}

int cl_strlen(const char *string) {
    int len = 0;
    
    for(; string[len]; ++len);

    return(len);
}

char *cl_strcat(char *string, char *string_1) {
    int str_len = cl_strlen(string);
    int str_1_len = cl_strlen(string_1);
    int count = 0;
    char *ptr = cl_calloc_check(str_len + str_1_len + 1, 1);

    for(; count <= str_len; ++count) {
        ptr[count] = string[count];
    }

    for(count = 0; count <= str_1_len; ++count) {
        ptr[str_len + count] = string_1[count];
    }
    
    return(ptr);
}

/*
char *cl_strcomb(int args, char *string, ...) {
    char **ptr = &string;
    int count = 0;
    int count_1 = 0;
    int count_2 = 0;
    int size = 0;

    for(; count < args; ++count, ++ptr) {
       size += cl_strlen(*ptr);
    }

    *ptr += 5;

    FILE *fptr = fopen("file.out", "w");
    if(fptr) {
        fprintf(fptr, "Size of all string: %d\nString: %s\n", size, *ptr);
        fclose(fptr);
    }

    char *result = cl_calloc_check(size + 1, 1);
    ptr = &string;

    for(count = 0; count < args; ++count) {
        for(count_1 = 0; ptr[count][count_1]; ++count_1) {
            result[count_2] = ptr[count][count_1];
            ++count_2;
        }
    }

    return(result);
} 
*/
