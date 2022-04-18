#include "../include/internal.h"

tokenlist*
lexer (const char *__src)
{
  tokenlist *ret    = malloc (sizeof (tokenlist));
  char      *tokdata = NULL;
  char       delimeter_tok[2] = {0, 0};
  int        lvl    = 0;
  int        lvlprv = 0;
  size_t     tbeg   = 0;
  size_t     i      = 0;
  size_t     tokpos = 0;
  STATE      st     = S_D_START; 
  TYPE       tp     = IDENTIFIER;
  
  ret->list = malloc (sizeof (token));
  ret->size = 0;
    
  while (st != S_D_END)
    {
      st = stmachine (__src[i], st, delimeter_tok);

      switch (st)
        {
        case S_P_STRBEG:
          ++tbeg;
          ++tokpos;

          break;
        case S_P_STRSEQ:
        case S_P_WDELIM:
        case S_P_STRING:
        case S_P_WTOK:
          ++tokpos;
          
          break;
        case S_P_COMMBEG:
        case S_P_COMMENT:
          break;
        case S_P_EOS:
        case S_P_EOT:
        case S_D_END:
          if (tokpos != 0)
            {
              if (st == S_P_EOS)
                --tokpos;

              if (tp != STRING)
                tokdata = copy_string_fragment (__src, tbeg, tbeg + tokpos);
              else
                tokdata = copy_strfrag_seqreplace (__src, tbeg, tbeg + tokpos);
              
              push_new_token (ret, tokdata, tp, lvl);
              free (tokdata);
            }
          
          if (*delimeter_tok != 0)
            {
              if (*delimeter_tok == '(')
                lvlprv = ++lvl;
              else if (*delimeter_tok == ')')
                --lvl;

              tp = __dettype (delimeter_tok);

              push_new_token (ret, delimeter_tok, tp,
                              lvlprv != lvl ? lvlprv-- : lvl);
              
              delimeter_tok[0] = 0;
            }

          tbeg = i + 1;
          tokpos = 0;

          break;
        case S_P_WTYPE:
          tp = __dettype (__src + i);
          
          --i;
          
          break;
        default:
          break;
        }

      ++i;
    }

  push_new_token (ret, "", TOK_EOF, 0);
  
  return ret;
}
