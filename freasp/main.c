#include <stdio.h>
#include "lexer.h"

char *typename[8] =
  { "NUMBER    ",
    "STRING    ",
    "IDENTIFIER",
    "O_BRACKET ",
    "C_BRACKET ",
    "COMMENT   ",
    "TOK_EOF   "
  };


int
main (void)
{
  char *code = "(hello \"Hello world!\" (goodbye 10))";
  
  tokenlist *data      = lexer (code);
  size_t     i         = 0;
  int        colors[7] = {4, 2, 14, 5, 5, 8};

  printf ("%s\n\n", code);
  
  for (i = 0; i < data->size; ++i)
    printf ("[ \033[1;38;5;%dm%d\033[0m ] [ \033[1;38;5;%dm%s\033[0m ]"
            "[ \033[1;38;5;%dm%s\033[0m ]\n",
            data->list[i].level + 1, data->list[i].level,
	    colors[data->list[i].type], typename[data->list[i].type],
	    colors[data->list[i].type], data->list[i].data);

            putc ('\n', stdout);
            
  /* for (i = 0; i < data->size; ++i)
    printf ("\033[1;38;5;%dm%s\033[0m",
    colors[data->list[i].type], data->list[i].data); */
  
  putc ('\n', stdout);
  ftoklist(data);
  
  return EXIT_SUCCESS;
}
