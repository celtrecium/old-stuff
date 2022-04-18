#include "../include/freasp.h"

int
label_if (tokenlist *tl, int pos)
{
  int levelif = tl->list[pos].level;
  int label = 1;
  
  while (tl->list[pos++].level >= levelif)
    {
      switch (tl->list[pos].type)
        {
        case O_BRACKET:
          if (tl->list[pos].level == levelif + 1)
            tl->list[pos].label = label <= FALSE_BRANCH ? label++ : 0;

          break;
        case C_BRACKET:
          if (tl->list[pos].level == levelif + 1 && label == TRUE_BRANCH)
            tl->list[pos].label = END_EXPRESSION;
          else if (tl->list[pos].level == levelif + 1)
            {
              tl->list[pos].label = END_BRANCH;
            }
          else if (tl->list[pos].level == levelif)
            tl->list[pos].label = END_IF;
        
          break;
        case INTEGER:
          if (label == 1 && tl->list[pos].level == levelif)
            tl->list[pos].label = strtol (tl->list[pos].data, NULL, 10) != 0 ?
              EXPRESSION_TRUE : EXPRESSION_FALSE;
              
              break;
        default:
          break;
        }
      
      if (tl->list[pos].label == -5)
        break;
    }

  return EXIT_SUCCESS;
}

int
label_while (tokenlist *tl, int pos)
{
  int levelif = tl->list[pos].level;
  int label = 1;
  
  while (tl->list[pos++].level >= levelif)
    {
      switch (tl->list[pos].type)
        {
        case O_BRACKET:
          if (tl->list[pos].level == levelif + 1)
            tl->list[pos].label =
              label == 1 ? (void) (++label), CYCLE_EXPRESSION :
              label == 2 ? label++ : 0;

          break;
        case C_BRACKET:
          if (tl->list[pos].level == levelif + 1 && label == 2)
            {
              tl->list[pos].label = END_EXPRESSION;
            }
          else if (tl->list[pos].level == levelif + 1)
            {
              tl->list[pos].label = END_CYCLE_BODY;
            }
          else if (tl->list[pos].level == levelif)
            tl->list[pos].label = END_CYCLE;
        
          break;
        case INTEGER:
          if (label == 1 && tl->list[pos].level == levelif)
            tl->list[pos].label = strtol (tl->list[pos].data, NULL, 10) != 0 ?
              EXPRESSION_TRUE : EXPRESSION_FALSE;
              
              break;
        default:
          break;
        }
      
      if (tl->list[pos].label == END_CYCLE)
        break;
    }

  return EXIT_SUCCESS;
}

void
set_ret (tokenlist *tl, size_t pos)
{
  int lvl = tl->list[pos].level;

  while (!(tl->list[++pos].level == lvl && tl->list[pos].type == C_BRACKET));
  
  tl->list[pos].label = RETURN;
}

size_t
gotofwd (tokenlist *tl, int pos, int label)
{
  size_t ret = (size_t) pos;
  int currlvl = tl->list[pos].level;
  
  while (1)
    if ((label == tl->list[++ret].label && tl->list[ret].level <= currlvl + 1)
        || ((tl->list[ret].label == END_IF || tl->list[ret].label == END_CYCLE)
            && tl->list[ret].level <= currlvl))
      break;
  
  return ret;
}

size_t
gotoback (tokenlist *tl, int pos, int label)
{
  size_t ret = (size_t) pos;
  int currlvl = tl->list[pos].level;
  
  while (1)
    if (tl->list[--ret].label == label && tl->list[ret].level <= currlvl + 1)
      break;
  
  return ret;
}
