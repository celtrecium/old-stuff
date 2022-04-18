#include <sbvector.h>
#include <stdbool.h>
#include <u8string.h>
#include "../include/stack.h"
#include <stdio.h>

stack_t
create_stack (void)
{
  return sbvector (sizeof (word_t));
}

bool
push(stack_t *s, word_t w)
{
  if (!w.word.length)
    return false;

  sbv_push (s, word_t, w);
  
  return true;
}

word_t *
pop(stack_t *s)
{
  return sbv_pop (s, word_t);
}

word_t *
peek (stack_t s, size_t offset)
{
  offset = offset + 1;

  return sbv_get (&s, word_t, s.length - offset);
}

bool
clear_stack (stack_t *s)
{
  if (!s)
    return false;

  while (s->length)
    word_free (pop (s));

  return true;
}

bool
free_stack (stack_t *s)
{
  size_t i = 0;

  if (!s)
    return false;

  for (i = 0; i < s->length; ++i)
    sbv_free (&sbv_get (s, word_t, i)->word);

  sbv_free (s);

  return true;
}
