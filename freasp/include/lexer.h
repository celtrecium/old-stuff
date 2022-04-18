#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOK 1024

typedef enum TYPE
  {
    INTEGER,
    FLOAT,
    STRING,
    IDENTIFIER,
    O_BRACKET,
    C_BRACKET,
    COMMENT,
    TOK_EOF
  } TYPE;

typedef enum STATE
  {
    S_D_START,
    S_P_WTOK,
    S_P_WTYPE,
    S_P_SKIP,
    S_P_COMMBEG,
    S_P_COMMENT,
    S_P_STRBEG,
    S_P_STRING,
    S_P_STRSEQ,
    S_P_WDELIM,
    S_P_EOT,
    S_P_EOS,
    S_D_END
  } STATE;

typedef struct token
{
  char *data;
  TYPE  type;
  int   level;
  int   label;
} token;

typedef struct tokenlist
{
  token  *list;
  size_t  size;
} tokenlist;

void ftoklist (tokenlist *__tlist);
tokenlist *lexer(const char *__src);
int copytok (token *dest, token *src);
int push_new_token (tokenlist *tl, char *data, TYPE type, int level);
int push_tok (tokenlist *tl, token tok);
char *copy_string_fragment (const char* __str, size_t __beg, size_t __end);
STATE stmachine (char __ch, STATE __curr_state,  char *__delim);
char* copy_strfrag_seqreplace (const char* __str, size_t __beg, size_t __end);
