#include "../include/freasp.h"

varlist*
create_varlist (void)
{
  varlist *vlist = malloc (sizeof (varlist));
  vlist->list    = malloc (sizeof (variable*));
  
  vlist->size = 0;

  return vlist;
}


int
clear_varlist (varlist* vlist)
{
  size_t i = 0;

  for (i = 0; i < vlist->size; ++i)
    {
      free (vlist->list[i]->name);
      free (vlist->list[i]->value);
      free (vlist->list[i]);
    }

  free (vlist->list);
  free (vlist);

  return EXIT_SUCCESS;
}

vlstack*
create_vlstack (void)
{
  vlstack *vlst = malloc (sizeof (vlstack));
  vlst->list    = malloc (sizeof (varlist*));
  
  vlst->size = 0;

  return vlst;
}

int
clear_vlstack (vlstack* vlst)
{
  size_t i = 0;

  for (i = 0; i < vlst->size; ++i)
    {
      clear_varlist (vlst->list[i]);
      free (vlst->list[i]);
    }

  free (vlst->list);
  free (vlst);

  return EXIT_SUCCESS;
}

int
push_vlstack (vlstack *vlst)
{
  vlst->list = realloc (vlst->list, ++vlst->size * sizeof (varlist*));
  vlst->list[vlst->size - 1] = malloc (sizeof (varlist));
  vlst->list[vlst->size - 1] = create_varlist ();
  
  return EXIT_SUCCESS;
}

int
pop_vlstack (vlstack *vlst)
{
  clear_varlist (vlst->list[--vlst->size]);
  vlst->list = realloc (vlst->list, vlst->size * sizeof (varlist*));  
  
  return EXIT_SUCCESS;
}

int
push_varlist (varlist *vl, variable *vr)
{
  variable *tmp = NULL;
  
  vl->list = realloc (vl->list, ++vl->size * sizeof (variable*));
  vl->list[vl->size - 1] = malloc (sizeof (variable));

  tmp = vl->list[vl->size - 1];

  tmp->name = calloc (strlen (vr->name) + 1, sizeof (char));
  strcpy (tmp->name, vr->name);

  tmp->value = calloc (strlen (vr->value) + 1, sizeof (char));
  strcpy (tmp->value, vr->value);

  tmp->type = vr->type;
  
  return EXIT_SUCCESS;
}

int
copyvar (variable *dest, const variable *src)
{
  if (dest->name != NULL)
    free (dest->name);

  dest->name = calloc (strlen (src->name) + 1, sizeof (char));
  strcpy (dest->name, src->name);
  
  dest->value = calloc (strlen (src->value) + 1, sizeof (char));
  strcpy (dest->value, src->value);
  
  dest->type = src->type;
  
  return EXIT_SUCCESS;
}

tokenlist*
pvartls (tokenlist *tl, size_t beg)
{
  size_t i = beg;
  size_t j = 0;
  varlist *vlptr = NULL;
  
  for (i = beg; i < tl->size; ++i)
    {
      if (tl->list[i].type != IDENTIFIER)
        continue;

      vlptr = g_var;
      
      for (j = 0; j < g_var->size; ++j)
        {          
          if (strcmp (tl->list[i].data, vlptr->list[j]->name))
            continue;
          
          free (tl->list[i].data);
          tl->list[i].data = calloc (strlen (vlptr->list[j]->value) + 1,
                                     sizeof (char));
          strcpy (tl->list[i].data, vlptr->list[j]->value);

          tl->list[i].type = vlptr->list[j]->type;
          
          break;
        }

      if (g_vlst->size == 0)
        continue;

      vlptr = g_vlst->list[g_vlst->size - 1];

      for (j = 0; j < g_vlst->list[g_vlst->size - 1]->size; ++j)
        {
          if (strcmp (tl->list[i].data, vlptr->list[j]->name))
            continue;
          
          free (tl->list[i].data);
          tl->list[i].data = calloc (strlen (vlptr->list[j]->value) + 1,
                                     sizeof (char));
          strcpy (tl->list[i].data, vlptr->list[j]->value);

          tl->list[i].type = vlptr->list[j]->type;
          
          break;
        }
    }

  return tl;
}
