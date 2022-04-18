#include "../include/word.h"
#include <sbvector.h>
#include <u8string.h>

word_t
copy_word (word_t word)
{
  word_t ret;

  ret.word = sbvector_from_array (word.word.vector, word.word.length,
                                  word.word._type_size);
  ret.type = word.type;

  return ret;
}

word_t
word (sbvector_t word, word_type_t type)
{
  return (word_t){ word, type };
}

bool
word_free (word_t *word)
{
  if (!word)
    return false;

  sbv_free (&word->word);

  return true;
}
