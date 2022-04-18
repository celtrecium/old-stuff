#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../include/hashtable.h"
#include "../include/word.h"
#include "../include/stack.h"
#include <sbvector.h>

typedef enum intrp_signal
  {
    ISIG_EXIT,
    ISIG_DEFWORD,
    ISIG_VAR,
    ISIG_IF_TRUE,
    ISIG_IF_FALSE,
    ISIG_SKIP_ELSE,
    ISIG_OK,
    ISIG_FAIL,
    ISIG_UNKNOWN_WORD,
    ISIG_SKIP_CYCLE
  } intrp_signal_t;

typedef struct interpreter
{
  stack_t stack;
  bool comp_flag;
  hashtable_t std_words;
  hashtable_t user_words;
  hashtable_t variables;
  size_t code_pos;
  sbvector_t cycles;
  sbvector_t arrays;
} interpreter_t;

intrp_signal_t execute_word (interpreter_t *interp, word_t w);
interpreter_t new_interpreter (void);
void interpret (interpreter_t *intrp, stack_t tokens);
bool interpreter_free (interpreter_t *intrp);

#endif /* INTERPRETER_H */
