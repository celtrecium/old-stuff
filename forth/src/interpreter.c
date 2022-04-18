#include "../include/interpreter.h"
#include "../include/stdproc.h"
#include "hashtable.h"
#include <sbvector.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

intrp_signal_t
execute_word (interpreter_t *interp, word_t w)
{
  _std_func_t *func = ht_get (interp->std_words, _std_func_t, w.word.vector);
  stack_t *userfunc = ht_get (interp->user_words, stack_t, w.word.vector);

  if (func)
    return func->func (interp);
  else if (userfunc)
    {
      interpret (interp, *userfunc);

      return ISIG_OK;
    }
  else if (ht_get (interp->variables, word_t, w.word.vector))
    {
      push (&interp->stack, copy_word (w));
      
      return ISIG_OK;
    }

  return ISIG_UNKNOWN_WORD;
}

interpreter_t
new_interpreter (void)
{
  interpreter_t new;

  new.stack = create_stack ();
  new.std_words = hashtable (sizeof (_std_func_t));
  new.user_words = hashtable (sizeof (stack_t));
  new.variables = hashtable (sizeof (word_t));
  new.comp_flag = false;
  new.code_pos = 0;
  new.cycles = sbvector (sizeof (size_t));
  new.arrays = sbvector (sizeof (sbvector_t));

  stdproc_initialize (&new.std_words);

  return new;
}

bool
define_word (interpreter_t *intrp, stack_t tokens)
{
  stack_t new_w = create_stack ();
  word_t curr_w;
  word_t defword = *sbv_get (&tokens, word_t, ++intrp->code_pos);

  for (++intrp->code_pos; intrp->code_pos < tokens.length; ++intrp->code_pos)
    {
      curr_w = *sbv_get (&tokens, word_t, intrp->code_pos);

      if (!strcmp (curr_w.word.vector, ";"))
        {
          ht_update (&intrp->user_words, stack_t, (char *)defword.word.vector,
                     new_w);

          return true;
        }

      push (&new_w, copy_word (curr_w));
    }

  free_stack (&new_w);

  return false;
}

bool
define_variable (interpreter_t *intrp, word_t name)
{
  if (name.type != WT_IDENTIFIER)
    return false;

  ht_update (&intrp->variables, word_t, name.word.vector,
             word (sbvector (sizeof (char)), WT_STRING));

  return true;
}

bool
skip_if_statement (stack_t tokens, size_t *pos)
{
  size_t nested = 0;
  bool is_nested_if = false;

  for (++(*pos); *pos < tokens.length; ++(*pos))
    {
      if (!strcmp(sbv_get (&tokens, word_t, *pos)->word.vector, "if"))
        {
          ++nested;
          is_nested_if = true;
        }

      if (!strcmp(sbv_get (&tokens, word_t, *pos)->word.vector, "else") &&
          !is_nested_if)
        return true;
      
      if (!strcmp(sbv_get (&tokens, word_t, *pos)->word.vector, "then"))
        {
          if (!nested)
            return true;
          
          if (!(--nested))
            is_nested_if = false;
        }
    }

  return false;
}

bool
skip_block (stack_t tokens, char *begin, char *end, size_t *pos)
{
  size_t nested = 0;

  for (++*pos; *pos < tokens.length; ++*pos)
    {
      if (!strcmp (sbv_get (&tokens, word_t, *pos)->word.vector, begin))
        ++nested;

      if (!strcmp (sbv_get (&tokens, word_t, *pos)->word.vector, end))
        {
          if (!nested)
            return true;
          
          --nested;
        }
    }

  return false;
}

static bool
execute_word_processing (interpreter_t *intrp, stack_t tokens, word_t w)
{
  switch (execute_word (intrp, w))
    {
    case ISIG_UNKNOWN_WORD:
      fprintf (stderr, "Unknown word \"%s\"\n", (char *)w.word.vector);
      return false;
    case ISIG_FAIL:
      fprintf (stderr, "Error while executing the word \"%s\"\n",
               (char *)w.word.vector);
      return false;
    case ISIG_DEFWORD:
      if (!define_word (intrp, tokens))
        {
          fprintf (stderr, "Error while defining new word \"%s\"\n",
                   (char *)w.word.vector);

          return false;
        }
    case ISIG_OK:
      break;
    case ISIG_EXIT:
      return false;
    case ISIG_VAR:
      if (++intrp->code_pos < tokens.length
          && define_variable (intrp,
                              *sbv_get (&tokens, word_t, intrp->code_pos)))
        return true;

      fputs ("Error while defining the variable\n", stderr);

      return false;
    case ISIG_IF_TRUE:
      break;
    case ISIG_SKIP_ELSE:
      if (skip_block (tokens, "if", "then", &intrp->code_pos))
        return true;
      
      fputs ("Endless if statement\n", stderr);

      return false;
    case ISIG_IF_FALSE:
      if (skip_if_statement (tokens, &intrp->code_pos))
        return true;
      
      fputs ("Endless if statement\n", stderr);

      return false;
    case ISIG_SKIP_CYCLE:
      if (skip_block (tokens, "while", "loop", &intrp->code_pos))
        return true;
      
      fputs ("Endless cycle statement\n", stderr);

      return false;
    }
  
  return true;
}



void
interpret (interpreter_t *intrp, stack_t tokens)
{
  word_t word;
  size_t prev_pos = intrp->code_pos;

  for (intrp->code_pos = 0; intrp->code_pos < tokens.length; ++intrp->code_pos)
    {
      word = *sbv_get (&tokens, word_t, intrp->code_pos);

      if (word.type != WT_IDENTIFIER)
        push (&intrp->stack, copy_word (word));
      else if (!execute_word_processing (intrp, tokens, word))
        return;
    }

  intrp->code_pos = prev_pos;
}

bool
interpreter_free (interpreter_t *intrp)
{
  return intrp && free_stack (&intrp->stack)
    && ht_free (&intrp->std_words)
    && ht_free (&intrp->user_words)
    && sbv_free (&intrp->arrays);
}
