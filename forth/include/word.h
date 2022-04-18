#ifndef WORD_H
#define WORD_H

#include <sbvector.h>
#include <u8string.h>

typedef enum word_type
{
  WT_IDENTIFIER,
  WT_INTEGER,
  WT_FLOAT,
  WT_STRING,
  WT_BIN_DATA
} word_type_t;

typedef struct word
{
  sbvector_t word;
  word_type_t type;
} word_t;

word_t copy_word (word_t word);
word_t word (sbvector_t word, word_type_t type);
bool word_free (word_t *word);

#endif  /* WORD_H */
