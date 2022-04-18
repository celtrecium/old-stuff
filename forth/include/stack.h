#ifndef STACK_H
#define STACK_H

#include <sbvector.h>
#include "word.h"

typedef sbvector_t stack_t;

stack_t create_stack (void);
bool push (stack_t *s, word_t w);
word_t *pop (stack_t *s);
word_t *peek (stack_t s, size_t offset);
bool free_stack (stack_t *s);
bool clear_stack (stack_t *s);

#endif /* STACK_H */
