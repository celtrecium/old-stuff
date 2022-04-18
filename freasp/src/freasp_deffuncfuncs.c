#include "../include/freasp.h"

func_data*
create_fdata (void)
{
  func_data *fdat = malloc (sizeof (func_data));
  
  fdat->pd = malloc (sizeof (prog_data**));
  fdat->vl = malloc (sizeof (varlist*));
  fdat->tl = malloc (sizeof (tokenlist));
  fdat->fnames = malloc (sizeof (char*));
  
  fdat->size = 0;
  fdat->pdsz = 0;
  
  return fdat;
}

int
clear_fdata (func_data *fd)
{
  size_t i;
  
  for (i = 0; i < fd->size; ++i)
    {
      clear_varlist (fd->vl[i]);
      free (fd->fnames[i]);

      ftoklist (fd->tl[i]);
    }
  
  free (fd->pd);
  free (fd->vl);
  free (fd->fnames);
  free (fd->tl);
  
  return EXIT_SUCCESS;
}

size_t
deffunct (tokenlist *tl, size_t pos, func_data *fdat)
{
  int lvl = tl->list[pos].level;
  varlist *tmpvl = NULL;
  size_t tmpvlsz = 0;
  
  pos += 1;

  fdat->fnames = realloc (fdat->fnames, ++fdat->size * sizeof (char*));
  fdat->fnames[fdat->size - 1] = calloc (strlen (tl->list[pos].data) + 1,
                                       sizeof (char));
  strcpy (fdat->fnames[fdat->size - 1], tl->list[pos].data);

  pos += 2;
  
  fdat->vl = realloc (fdat->vl, fdat->size * sizeof (varlist*));
  fdat->tl = realloc (fdat->tl, fdat->size * sizeof (tokenlist*));
  fdat->tl[fdat->size - 1] = malloc (sizeof (tokenlist));
  
  fdat->vl[fdat->size - 1] = create_varlist ();

  fdat->tl[fdat->size - 1]->list = malloc (sizeof (token));
  fdat->tl[fdat->size - 1]->size = 0;

  while (tl->list[pos].level != lvl && tl->list[pos].type != C_BRACKET)
    {      
      tmpvlsz = ++fdat->vl[fdat->size - 1]->size;
      tmpvl = fdat->vl[fdat->size - 1];

      tmpvl->list = realloc (tmpvl->list, tmpvlsz * sizeof (variable*));
      tmpvl->list[tmpvlsz - 1] = malloc (sizeof (variable));

      tmpvl->list[tmpvlsz - 1]->name = calloc (strlen (tl->list[pos].data) + 1,
                sizeof (char));
      
      strcpy (tmpvl->list[tmpvlsz - 1]->name, tl->list[pos].data);

      tmpvl->list[tmpvlsz - 1]->value = calloc (2, sizeof (char));
      
      tmpvl->list[tmpvlsz - 1]->value[0] = '0';
      tmpvl->list[tmpvlsz - 1]->type = INTEGER;

      ++pos;
    }
    
  do
    {
      ++pos;
      push_tok (fdat->tl[fdat->size - 1], tl->list[pos]);
    }
  while (tl->list[pos].level != lvl + 1 || tl->list[pos].type != C_BRACKET);
  
  while (tl->list[++pos].level != lvl || tl->list[pos].type != C_BRACKET);

  return pos - 1;
}
