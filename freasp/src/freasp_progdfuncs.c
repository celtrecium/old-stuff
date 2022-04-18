#include "../include/freasp.h"

int
push_pfdata (prog_data *pd)
{
  pd->pfdata = realloc (pd->pfdata, ++pd->size * sizeof (tokenlist*));

  pd->pfdata[pd->size - 1] = malloc (sizeof (tokenlist));
  pd->pfdata[pd->size - 1]->list = malloc (sizeof (token));
  pd->pfdata[pd->size - 1]->size = 0;
  
  return EXIT_SUCCESS;
}

int
pop_pfdata (prog_data *pd)
{
  ftoklist(pd->pfdata[--pd->size]);
  
  pd->pfdata = realloc (pd->pfdata, pd->size * sizeof (tokenlist));
  
  return EXIT_SUCCESS;
}

prog_data*
create_pfdata (void)
{
  prog_data *pd = malloc (sizeof (prog_data));

  pd->size = 0;
  pd->pfdata = malloc (sizeof (tokenlist*));
  
  return pd;
}

int
clear_pfdata (prog_data *pd)
{
  size_t i = 0;

  for (i = 0; i < pd->size; ++i)
    ftoklist (pd->pfdata[i]);

  free (pd->pfdata);
  free (pd);
  
  return EXIT_SUCCESS;
}

token
cstmfunc (func_data *fd, size_t fnum, tokenlist *tl)
{
  token tkret;
  size_t i;
  variable *vptr = NULL;

  pvartls (tl, 1);
  
  fd->pd = realloc (fd->pd, ++fd->pdsz * sizeof (prog_data**));
  fd->pd[fd->pdsz - 1] = malloc (sizeof (prog_data*));
  fd->pd[fd->pdsz - 1][0] = create_pfdata ();
  
  push_vlstack (g_vlst);
  
  for (i = 0; i < fd->vl[fnum]->size; ++i)
    {
      push_varlist (g_vlst->list[g_vlst->size - 1], fd->vl[fnum]->list[i]);
      
      vptr = g_vlst->list[g_vlst->size - 1]->
         list[g_vlst->list[g_vlst->size - 1]->size - 1];

      free (vptr->value);
      vptr->value = calloc (strlen (tl->list[i + 1].data) + 1, sizeof (char));
      strcpy (vptr->value, tl->list[i + 1].data);
      
      vptr->type = tl->list[i + 1].type;
    }
  
  tkret = interpret_code(fd->pd[fd->pdsz - 1], fd->tl[fnum]);

  clear_pfdata (fd->pd[fd->pdsz - 1][0]);
  free (fd->pd[fd->pdsz - 1]);
  fd->pd = realloc (fd->pd, --fd->pdsz * sizeof (prog_data*));  
  
  pop_vlstack (g_vlst);
  
  return tkret;
}

int
chckf (prog_data *pd, tokenlist *tl)
{
  size_t i;
  token tmp = {NULL, 0, 0, 0};

  if (tl->size == 0)
    {
      tmp.data = calloc (2, sizeof (char));
      tmp.data[0] = '0';
      tmp.label = 0;
      tmp.level = 0;
      tmp.type = INTEGER;

      pop_pfdata (pd);
  
      if (pd->size != 0)
        push_tok(pd->pfdata[pd->size - 1], tmp);
            
      free (tmp.data);
  
      return EXIT_SUCCESS;
    }
  
  for (i = 0; i < FUNCNUM; ++i)
    {      
      if (!strcmp (tl->list[0].data, funcs[i]))
        {
          tmp = pfuncs[i] (tl);

          pop_pfdata (pd);

          if (pd->size != 0)
            push_tok (pd->pfdata[pd->size - 1], tmp);
            
          free (tmp.data);

          return EXIT_SUCCESS;          
        }
    }

  for (i = 0; i < g_fdat->size; ++i)
    {
      if (!strcmp (tl->list[0].data, g_fdat->fnames[i]))
        {
          tmp = cstmfunc (g_fdat, i, tl);

          pop_pfdata (pd);

          if (pd->size != 0)
            push_tok (pd->pfdata[pd->size - 1], tmp);
            
          free (tmp.data);

          return EXIT_SUCCESS;
        }
    }

  tmp.data = calloc (strlen (tl->list[tl->size - 1].data) + 1,
                     sizeof (char));
  strcpy (tmp.data, tl->list[tl->size - 1].data);
  tmp.label = tl->list[tl->size - 1].label;
  tmp.level = tl->list[tl->size - 1].level;
  tmp.type = tl->list[tl->size - 1].type;
  
  pop_pfdata (pd);
  
  if (pd->size != 0)
    push_tok (pd->pfdata[pd->size - 1], tmp);
            
  free (tmp.data);
  
  return EXIT_SUCCESS;
}
