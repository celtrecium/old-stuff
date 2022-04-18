#include "internal.h"

char
__strsseq (char __ch)
{
  switch (__ch)
    {
    case 't':
      return '\t';
    case 'n':
      return '\n';
    case 'e':
      return '\033';
    case '"':
      return '"';
    case '\\':
      return '\\';
    }

  return 0;
}

TYPE
__dettype (const char *__ch)
{
  size_t len = strlen (__ch);
  size_t i;
  
  switch (*__ch)
    {
    case '"':
      return STRING;
    case '(':
      return O_BRACKET;
    case ')':
      return C_BRACKET;
    case ';':
      return COMMENT;
    }

  if (*__ch >= '0' && *__ch <= '9')
    return INTEGER;

  
  if ((*__ch == '-' || *__ch == '+') && __ch[1] >= '0' && __ch[1] <= '9')
    {
      for (i = 0; i < len &&
             (__ch[i] >= '0' || __ch[i] <= '9' ||
              __ch[i] == '-' || __ch[i] == '+'); ++i)
        if (__ch[i] == '.' || __ch[i] == ',')
          return FLOAT;

      return INTEGER;
    }
  
  return IDENTIFIER;
}
