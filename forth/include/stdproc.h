#ifndef STDPROC_H
#define STDPROC_H

#include "../include/hashtable.h"
#include "../include/word.h"
#include "../include/stack.h"
#include "../include/interpreter.h"

typedef struct _std_proc
{
  intrp_signal_t (*func)(interpreter_t*);
} _std_func_t;

intrp_signal_t stdproc_swap_words (interpreter_t *intrp);
intrp_signal_t stdproc_show_stack (interpreter_t *intrp);
intrp_signal_t stdproc_add (interpreter_t *intrp);

void stdproc_initialize (hashtable_t *ht);

#endif /* STDPROC_H */
