#include "../include/freasp.h"

int
copytok (token *dest, token *src)
{
  size_t i = strlen (src->data);

  dest->type = src->type;
  dest->level = src->level;
  dest->data = calloc (i + 1, sizeof (char));
  strcpy (dest->data, src->data);
  
  return EXIT_SUCCESS;
}

int
push_new_token (tokenlist *tl, char *data, TYPE type, int level)
{
  token *ptr = NULL;
  
  tl->list = realloc (tl->list, ++tl->size * sizeof (token));
  ptr = &tl->list[tl->size - 1];
  
  ptr->data =  calloc (strlen (data) + 1, sizeof (char));
  strcpy (ptr->data, data);

  ptr->type  = type;
  ptr->level = level;
  ptr->label = 0;
  
  return EXIT_SUCCESS;
}

int
push_tok (tokenlist *tl, token tok)
{
  tl->list = realloc (tl->list, ++tl->size * sizeof (token));
  
  tl->list[tl->size - 1].data = calloc (strlen (tok.data) + 1, sizeof (char));
  strcpy (tl->list[tl->size - 1].data, tok.data);
  tl->list[tl->size - 1].level = tok.level;
  tl->list[tl->size - 1].type  = tok.type;
  tl->list[tl->size - 1].label = tok.label;
  
  return EXIT_SUCCESS;
}

void
ftoklist (tokenlist *__tlist)
{
  size_t i = 0;

  for (i = 0; i < __tlist->size; ++i)
    free (__tlist->list[i].data);

  free (__tlist->list);
  __tlist->list = NULL;
  __tlist->size = 0;

  free (__tlist);

  return;
}
