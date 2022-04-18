#include "../include/stdproc.h"
#include "../include/word.h"
#include "interpreter.h"
#include "lexer.h"
#include "stack.h"
#include <sbvector.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static long int
_get_file_size(const char* file_name)
{
  long int _file_size = 0;
  FILE* fd = fopen (file_name, "rb");

  if(!fd)
    _file_size = -1;
  else
    {
      fseek (fd, 0, SEEK_END);
      _file_size = ftell (fd);
      fclose (fd);
    }

  return _file_size;
}

intrp_signal_t
stdproc_swap_words (interpreter_t *intrp)
{
  word_t w1;
  word_t w2;

  if (intrp->stack.length < 2)
    return ISIG_FAIL;
  
  w1 = *pop (&intrp->stack);
  w2 = *pop (&intrp->stack);

  push (&intrp->stack, w1);
  push (&intrp->stack, w2);

  return ISIG_OK;
}

intrp_signal_t
stdproc_show_stack (interpreter_t *intrp)
{
  size_t i = 0;

  printf ("Stack <%zu>: ", intrp->stack.length);
  
  for (i = 0; i < intrp->stack.length; ++i)
    printf ("%s ", (char *)sbv_get(&intrp->stack, word_t, i)->word.vector);

  putchar (0x0A);

  return ISIG_OK;
}

/* Function for basic math operations */
intrp_signal_t
_std_math_op (interpreter_t *intrp, double (*op)(double, double))
{
  word_t x;
  word_t y;
  sbvector_t result = sbvector (sizeof (char));

  if (intrp->stack.length < 2 || peek (intrp->stack, 0)->type == WT_STRING
      || peek (intrp->stack, 0)->type == WT_STRING)
      return ISIG_FAIL;
  
  y = *pop (&intrp->stack);
  x = *pop (&intrp->stack);

  sbv_resize (&result, 20);

  if ((x.type == WT_FLOAT || x.type == WT_INTEGER)
      && (y.type == WT_FLOAT || y.type == WT_INTEGER))
    sprintf (result.vector, "%g",
             op (strtod (x.word.vector, NULL), strtod (y.word.vector, NULL)));

  word_free (&x);
  word_free (&y);

  push (&intrp->stack,
        word (result, strchr (result.vector, '.') ? WT_FLOAT : WT_INTEGER));

  return ISIG_OK;
}

#define def_stdproc(name, op)                                                 \
  double _##name (double x, double y) { return x op y; }                      \
  intrp_signal_t stdproc_##name (interpreter_t *intrp)                        \
  {                                                                           \
    return _std_math_op (intrp, _##name);                                     \
  }

def_stdproc (add, +)
def_stdproc (sub, -)
def_stdproc (mul, *)
def_stdproc (div, /)
def_stdproc (more, >)
def_stdproc (less, <)
def_stdproc (equ, ==)
def_stdproc (not_equ, !=)
def_stdproc (or, ||)
def_stdproc (and, &&)

intrp_signal_t
stdproc_exit (interpreter_t *intrp)
{
  (void)intrp;
  return ISIG_EXIT;
}
  
intrp_signal_t
stdproc_defword (interpreter_t *intrp)
{
  (void)intrp;
  return ISIG_DEFWORD;
}

intrp_signal_t
stdproc_dup (interpreter_t *intrp)
{
  if (intrp->stack.length == 0)
    return ISIG_FAIL;

  push (&intrp->stack, copy_word (*peek (intrp->stack, 0)));

  return ISIG_OK;
}

intrp_signal_t
stdproc_pop (interpreter_t *intrp)
{
  if (intrp->stack.length == 0)
    return ISIG_FAIL;

  word_free (pop (&intrp->stack));

  return ISIG_OK;
}

intrp_signal_t
stdproc_print (interpreter_t *intrp)
{
  word_t w;
  
  if (intrp->stack.length == 0)
    return ISIG_FAIL;

  w = *peek (intrp->stack, 0);

  fputs (w.word.vector, stdout);

  stdproc_pop (intrp);
  
  return ISIG_OK;
}

intrp_signal_t
stdproc_new_line (interpreter_t *intrp)
{
  putchar ('\n');
  (void)intrp;

  return ISIG_OK;
}

intrp_signal_t
stdproc_var (interpreter_t *intrp)
{
  (void)intrp;
  return ISIG_VAR;
}

intrp_signal_t
stdproc_var_set (interpreter_t *intrp)
{
  word_t *w;
  
  if (intrp->stack.length < 2 && peek(intrp->stack, 0)->type != WT_IDENTIFIER)
    return ISIG_FAIL;

  w = ht_get (intrp->variables, word_t, peek (intrp->stack, 0)->word.vector);
  sbv_clear (&w->word);

  word_free (pop (&intrp->stack));

  sbv_copy (&w->word, &peek (intrp->stack, 0)->word);
  w->type = peek (intrp->stack, 0)->type;

  word_free (pop (&intrp->stack));

  return ISIG_OK;
}

intrp_signal_t
stdproc_var_get (interpreter_t *intrp)
{
  word_t var_value;
  
  if (intrp->stack.length < 1 || peek(intrp->stack, 0)->type != WT_IDENTIFIER)
    return ISIG_FAIL;

  var_value = *ht_get (intrp->variables, word_t,
                       peek (intrp->stack, 0)->word.vector);

  word_free (pop (&intrp->stack));
  push (&intrp->stack, copy_word (var_value));

  return ISIG_OK;
}

intrp_signal_t
stdproc_if (interpreter_t *intrp)
{
  word_t *w;
  intrp_signal_t res = ISIG_IF_TRUE;
  
  if (intrp->stack.length < 1)
    return ISIG_FAIL;

  w = peek (intrp->stack, 0);

  if (w->type == WT_INTEGER || w->type == WT_FLOAT)
    res = strtol (w->word.vector, NULL, 10) != 0 ? ISIG_IF_TRUE
                                                 : ISIG_IF_FALSE;

  word_free (pop (&intrp->stack));

  return res;
}

intrp_signal_t
stdproc_endif (interpreter_t *intrp)
{
  (void)intrp;
  return ISIG_OK;
}

intrp_signal_t
stdproc_else (interpreter_t *intrp)
{
  (void)intrp;
  return ISIG_SKIP_ELSE;
}

intrp_signal_t
stdproc_import (interpreter_t *intrp)
{
  FILE *file = NULL;
  sbvector_t buff;
  size_t fsize;
  stack_t code;
  
  if (!intrp->stack.length || peek(intrp->stack, 0)->type != WT_STRING)
    return ISIG_FAIL;

  file = fopen (peek (intrp->stack, 0)->word.vector, "r");

  if (!file)
    return ISIG_FAIL;

  buff = sbvector (sizeof (char));
  fsize = (size_t) _get_file_size (peek (intrp->stack, 0)->word.vector);

  sbv_resize (&buff, fsize);
  sbv_push (&buff, char, 0);

  fread (buff.vector, sizeof (char), fsize, file);
  fclose (file);
  
  code = tokenize (buff.vector);
  sbv_free (&buff);

  word_free (pop (&intrp->stack));

  interpret (intrp, code);
  free_stack (&code);
  
  return ISIG_OK;
}

intrp_signal_t
stdproc_cat (interpreter_t *intrp)
{
  word_t *w1;
  word_t *w2;
  
  if (intrp->stack.length < 2)
    return ISIG_FAIL;

  w2 = pop (&intrp->stack);
  w1 = pop (&intrp->stack);

  sbv_resize (&w1->word, w2->word.length + w1->word.length - 1);

  strcat (w1->word.vector, w2->word.vector);
  
  word_free (w2);
  push (&intrp->stack, *w1);
  
  return ISIG_OK;
}

intrp_signal_t
stdproc_while (interpreter_t *intrp)
{
  sbv_push (&intrp->cycles, size_t, intrp->code_pos);

  return ISIG_OK;
}

intrp_signal_t
stdproc_do (interpreter_t *intrp)
{
  if (stdproc_if (intrp) == ISIG_IF_TRUE)
    return ISIG_OK;

  sbv_pop (&intrp->cycles, size_t);
  
  return ISIG_SKIP_CYCLE;
}

intrp_signal_t
stdproc_loop (interpreter_t *intrp)
{
  if (!intrp->cycles.length)
    return ISIG_OK;

  intrp->code_pos =
    *sbv_get (&intrp->cycles, size_t, intrp->cycles.length - 1);

  return ISIG_OK;
}

intrp_signal_t
stdproc_alloc (interpreter_t *intrp)
{
  size_t sz, i;
  sbvector_t vec;
  word_t *w;
  word_t blank = word (sbvector (sizeof (char)), WT_STRING);
  
  if (!intrp->stack.length || peek (intrp->stack, 0)->type != WT_INTEGER)
    return ISIG_FAIL;

  w = peek (intrp->stack, 0);
  sz = strtoul (w->word.vector, NULL, 10);
  vec = sbvector (sizeof (word_t));

  sbv_resize (&vec, sz);

  for (i = 0; i < vec.length; ++i)
    sbv_set (&vec, word_t, i, copy_word (blank));
  
  sbv_push (&intrp->arrays, sbvector_t, vec);

  sbv_resize (&w->word, sizeof (size_t));

  *(size_t*) w->word.vector = intrp->arrays.length - 1;
  sbv_push (&w->word, char, 0);
  w->type = WT_BIN_DATA;
  
  return ISIG_OK;
}

static word_t*
_get_arr_element (interpreter_t *intrp)
{
  word_t *w1, *w2, *res;
  
  w1 = pop (&intrp->stack);
  w2 = pop (&intrp->stack);

  res = sbv_get (sbv_get (&intrp->arrays,
                          sbvector_t,
                          *(size_t*)w1->word.vector),
                 word_t,
                 strtoul (w2->word.vector, NULL, 10));

  word_free (w1);
  word_free (w2);
  
  return res;
}

intrp_signal_t
stdproc_set_el (interpreter_t *intrp)
{
  word_t *wvar;
  
  if (intrp->stack.length < 2)
    return ISIG_FAIL;

  if (stdproc_var_get (intrp) == ISIG_FAIL)
    return ISIG_FAIL;

  wvar = _get_arr_element (intrp);

  if (!wvar)
    return ISIG_FAIL;
  
  *wvar = *pop (&intrp->stack);

  return ISIG_OK;
}

intrp_signal_t
_get_string_sym (interpreter_t *intrp)
{
  word_t w = word (sbvector (sizeof (char)), WT_STRING);
  word_t str = *pop (&intrp->stack);
  word_t index = *pop (&intrp->stack);
  
  sbv_push (&w.word, char, *sbv_get (&str.word, char,
                                     strtoul (index.word.vector, NULL, 10)));
  sbv_push (&w.word, char, 0);

  push (&intrp->stack, w);
  
  word_free (&index);
  word_free (&str);
  
  return ISIG_OK;
}

intrp_signal_t
stdproc_get_el (interpreter_t *intrp)
{

  if (intrp->stack.length < 2)
    return ISIG_FAIL;

  if (peek (intrp->stack, 0)->type == WT_STRING)
    return _get_string_sym (intrp);
  
  if (stdproc_var_get (intrp) == ISIG_FAIL)
    return ISIG_FAIL;

  push (&intrp->stack, copy_word (*_get_arr_element (intrp)));
  
  return ISIG_OK;
}

intrp_signal_t
stdproc_input (interpreter_t *intrp)
{
  sbvector_t text = sbvector (sizeof (char));
  char ch = (char)getchar ();

  while (ch != '\n')
    {
      sbv_push (&text, char, ch);
      ch = (char)getchar ();
    }

  push (&intrp->stack, word (text, WT_STRING));
  
  return ISIG_OK;
}

intrp_signal_t
stdproc_cmp (interpreter_t *intrp)
{
  word_t w1, w2;
  sbvector_t res;
  
  if (intrp->stack.length < 2)
    return ISIG_FAIL;

  w2 = *pop (&intrp->stack);
  w1 = *pop (&intrp->stack);

  res = sbvector (sizeof (char));

  sbv_push (&res, char, !strcmp (w1.word.vector, w2.word.vector) ? '1' : '0');
  push (&intrp->stack, word (res, WT_INTEGER));

  word_free (&w1);
  word_free (&w2);
  
  return ISIG_OK;
}

intrp_signal_t
stdproc_int (interpreter_t *intrp)
{
  word_t *w;
  
  if (intrp->stack.length < 1)
    return ISIG_FAIL;

  w = peek (intrp->stack, 0);
  
  if (!(isdigit (*(char*)w->word.vector) ||
        (*(char*)w->word.vector == '-'
         && isdigit (((char*)w->word.vector)[1]))))
    {
      sbv_clear (&w->word);
      sbv_push (&w->word, char, '0');
      sbv_push (&w->word, char, 0);
    }

  w->type = WT_INTEGER;
  
  return ISIG_OK;
}

intrp_signal_t
stdproc_rand (interpreter_t *intrp)
{
  word_t w = word (sbvector (sizeof (char)), WT_INTEGER);
  
  sprintf (w.word.vector, "%d", rand ());
  sbv_resize (&w.word, strlen (w.word.vector));
  push (&intrp->stack, w);
  
  return ISIG_OK;
}

intrp_signal_t
stdproc_mod (interpreter_t *intrp)
{
  word_t x;
  word_t y;
  sbvector_t result = sbvector (sizeof (char));

  if (intrp->stack.length < 2 || peek (intrp->stack, 0)->type != WT_INTEGER
      || peek (intrp->stack, 1)->type != WT_INTEGER)
      return ISIG_FAIL;
  
  y = *pop (&intrp->stack);
  x = *pop (&intrp->stack);

  sbv_resize (&result, 20);

  if ((x.type == WT_FLOAT || x.type == WT_INTEGER)
      && (y.type == WT_FLOAT || y.type == WT_INTEGER))
    sprintf (result.vector, "%lu",
             strtol (x.word.vector, NULL, 10) %
             strtol (y.word.vector, NULL, 10));

  word_free (&x);
  word_free (&y);

  push (&intrp->stack, word (result, WT_INTEGER));

  return ISIG_OK;
}

static intrp_signal_t (*stdprocs[])(interpreter_t*) =
  {
    stdproc_swap_words,
    stdproc_show_stack,
    stdproc_add,
    stdproc_sub,
    stdproc_mul,
    stdproc_div,
    stdproc_exit,
    stdproc_defword,
    stdproc_dup,
    stdproc_pop,
    stdproc_print,
    stdproc_new_line,
    stdproc_var,
    stdproc_var_set,
    stdproc_var_get,
    stdproc_if,
    stdproc_endif,
    stdproc_else,
    stdproc_import,
    stdproc_more,
    stdproc_less,
    stdproc_equ,
    stdproc_not_equ,
    stdproc_or,
    stdproc_and,
    stdproc_cat,
    stdproc_while,
    stdproc_do,
    stdproc_loop,
    stdproc_alloc,
    stdproc_get_el,
    stdproc_set_el,
    stdproc_input,
    stdproc_cmp,
    stdproc_int,
    stdproc_mod,
    stdproc_rand
  };

static char *stdproc_names[] =
  {
    "swap",
    ".s",
    "+", "-", "*", "/",
    "exit",
    ":",
    "dup",
    "drop",
    "print",
    "nl",
    "var",
    "!",
    "@",
    "if",
    "then",
    "else",
    "import",
    ">", "<", "=", "!=", "or", "and",
    "cat",
    "while",
    "do",
    "loop",
    "alloc", "@@", "!!",
    "input",
    "cmp",
    "integer",
    "mod",
    "rand"
  };

void
stdproc_initialize (hashtable_t *ht)
{
  size_t i = 0;
  _std_func_t func;

  srand ((unsigned int) time (NULL));
  
  for (i = 0; i < sizeof (stdproc_names) / sizeof (char *); ++i)
    {
      func.func = stdprocs[i];
      ht_update (ht, _std_func_t, stdproc_names[i], func);
    }
}
