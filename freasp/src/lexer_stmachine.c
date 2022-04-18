#include "../include/internal.h"

STATE
stmachine (char __ch, STATE __curr_state,  char *__delim)
{
  *__delim = 0;
  
  if (__ch == 0)
    return S_D_END;
  
  switch (__curr_state)
    {
    case S_P_EOT:
    case S_D_START:
      return S_P_WTYPE;

    case S_P_STRSEQ:
      return S_P_STRING;

    case S_P_STRING:
    case S_P_STRBEG:
    case S_P_WDELIM:
      if (__ch != '"' && __ch != '\\')
        return S_P_STRING;
      else if (__ch == '"')
        return S_P_EOS;
      else if (__ch == '\\')
        return S_P_STRSEQ;
          
      break;
    case S_P_COMMBEG:
      if (__ch == ';')
        return S_P_COMMENT;

      return S_P_WTOK;

    case S_P_COMMENT:
      if (__ch == '\n')
        return S_P_EOT;

      return S_P_COMMENT;
      
    default:
      break;
    }
  
  switch (__ch)
    {
    case '\\':
      if (__curr_state == S_P_STRING || __curr_state == S_P_STRBEG)
        return S_P_STRSEQ;

      break;
    case '"':
      if (__curr_state == S_P_STRING)
        return S_P_EOS;
      
      return S_P_STRBEG;

    case '(':
    case ')':
      *__delim = __ch;

      return S_P_EOT;

    case ' ':
    case '\t':
    case '\n':
      return S_P_EOT;
      
    case ';':
      return S_P_COMMBEG;
    }

  return S_P_WTOK;
}
