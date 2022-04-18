#include "../include/hashtable.h"
#include "../include/lexer.h"
#include "../include/stack.h"
#include <sbvector.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/interpreter.h"

int
main (void)
{
  stack_t code;
  char buffer[1024] = { 0 };
  interpreter_t intrp = new_interpreter ();

  puts ("TurnsOutPL v0.1\n"
        "---");

  while (true)
    {
      fputs ("\033[34m>>> \033[m", stdout);
      fgets (buffer, 1024, stdin);

      code = tokenize (buffer);

      interpret (&intrp, code);

      free_stack (&code);
    }

  free_stack (&code);
  interpreter_free (&intrp);

  return EXIT_SUCCESS;
}
