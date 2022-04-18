#include "../include/lexer.h"
#include <sbvector.h>
#include <stdbool.h>
#include <string.h>
#include <u8string.h>
#include "../include/word.h"
#include <ctype.h>

typedef enum _state
{
  STRING_BEGIN,
  STRING_END,
  STRING,
  WORD_END,
  WORD,
  END,
  SEQ_BEGIN,
  SEQUENCE
} _state_t;

typedef enum _signal
  {
    PUSH_CHAR,
    SKIP_CHAR,
    END_WORD,
    PUSH_SEQ
  } _signal_t;

/* Table of states and corresponding signals */
static const _signal_t signals_by_state[] = {
  [STRING_BEGIN] = SKIP_CHAR,
  [STRING_END]   = END_WORD,
  [STRING]       = PUSH_CHAR,
  [WORD]         = PUSH_CHAR,
  [WORD_END]     = END_WORD,
  [END]          = END_WORD,
  [SEQ_BEGIN]    = SKIP_CHAR,
  [SEQUENCE]     = PUSH_SEQ
};

/* Table of unprintable chars */
static const char unprintable_chars[256] = {
  ['n'] = '\n',
  ['t'] = '\t',
  ['"'] = '"',
  ['\\'] = '\\',
  ['e'] = '\033',
  ['r'] = '\r',
  ['b'] = '\b'
};

static _state_t
_get_word_state (char ch, _state_t curr_state)
{
  switch (ch)
    {
    case '\n':
    case ' ':
      if (curr_state == STRING_BEGIN
          || curr_state == STRING
          || curr_state == SEQUENCE)
        return STRING;
      else
        return WORD_END;

    case '"':
      if (curr_state == SEQ_BEGIN)
        return SEQUENCE;
      else if (curr_state == STRING
               || curr_state == SEQUENCE)
        return STRING_END;
      else
        return STRING_BEGIN;

    case '\0':
      return END;

    case '\\':
      if (curr_state == STRING_BEGIN || curr_state == STRING
          || curr_state == SEQUENCE)
        return SEQ_BEGIN;

      return WORD;
    default:
      if (curr_state == STRING_BEGIN || curr_state == STRING
          || curr_state == SEQUENCE)
        return STRING;
      else if (curr_state == SEQ_BEGIN)
        return SEQUENCE;
      else
        return WORD;
    }

  return WORD;
}

static word_type_t
_get_word_type (char *word)
{
  if (isdigit (*word) ||
      (*word == '-' && isdigit (word[1])))
    return strchr (word, '.') ? WT_FLOAT : WT_INTEGER;

  return WT_IDENTIFIER;
}

static void
_push_token (stack_t *tokens, sbvector_t w, _state_t state)
{
  push (tokens, word (w, (state == STRING_END ? WT_STRING
                                              : _get_word_type (w.vector))));
}

stack_t
tokenize (char *str)
{
  sbvector_t tmp_word = sbvector (sizeof (char));
  stack_t tokens = create_stack ();
  _state_t state = WORD;
  _signal_t prev_signal;

  for (; state != END; ++str)
    {
      state = _get_word_state (*str, state);
      
      switch (signals_by_state[state])
        {
        case SKIP_CHAR:
          break;

        case PUSH_CHAR:
          sbv_push (&tmp_word, char, *str);
          break;

        case END_WORD:
          if (prev_signal == END_WORD || !tmp_word.length)
            break;
          
          sbv_push (&tmp_word, char, 0);
          _push_token (&tokens, tmp_word, state);

          tmp_word = sbvector (sizeof (char));
          break;
        case PUSH_SEQ:
          sbv_push (&tmp_word, char, unprintable_chars[(int)*str]);
        }

      prev_signal = signals_by_state[state];
    }

  sbv_free (&tmp_word);

  return tokens;
}

