#include "../include/internal.h"

char*
copy_string_fragment (const char* __str, size_t __beg, size_t __end)
{
  char *ret = calloc (__beg + __end + 1, sizeof (char));
  size_t i = 0;
  
  for (i = __beg; i < __end; ++i)
    ret[i - __beg] = __str[i];

  return ret;
}

char*
copy_strfrag_seqreplace (const char* __str, size_t __beg, size_t __end)
{
  char *ret = calloc (__beg + __end + 1, sizeof (char));
  size_t i = 0;
  
  for (i = __beg; i < __end; ++i)
    {
      if (__str[i] == '\\')
        {
          ret[i - __beg] = __strsseq (__str[i + 1]);
          ++i;
          ++__beg;

          continue;
        }
      
      ret[i - __beg] = __str[i];
    }

  return ret;
}
