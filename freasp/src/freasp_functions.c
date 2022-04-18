#include "../include/freasp.h"

static int
__isfloat (const char* str)
{
  size_t i;

  for (i = 0; str[i] != 0; ++i)
    if (str[i] == '.')
      return 0;

  return -1;
}

static double __fmult  (double val, char *str) {return val * atof (str);}
static double __fdiv   (double val, char *str) {return val / atof (str);}
static double __fminus (double val, char *str) {return val - atof (str);}
static double __fplus  (double val, char *str) {return val + atof (str);}

static double
__fmod   (double val, char *str)
{
  return (double) ((int) val % atoi(str));
}

static double (*operat[]) (double val, char *str) =
  {__fmult, __fdiv, __fminus, __fplus, __fmod};


static token
__master (tokenlist *data, int operation)
{
  double fret = 0;
  size_t i = 0;
  char numbuf[32] = {0};
  token tr;

  if (data->size < (operation == 1 || operation == 2 ? 3 : 2))
    {
      printf ("\033[31mError: "
              "not enough arguments for function \"%s\"\033[0m\n",
              data->list[0].data);

      em_exit ();
    }
    
  data = pvartls (data, 1);

  switch (operation)
    {
    case 0:
      fret = 1;
      i = 1;
      break;
    case 3:
      i = 1;
      break;
    case 1:
    case 2:
    case 4:
      fret = atof (data->list[1].data);
      i = 2;
      break;
    }
  
  tr.type = FLOAT;

  for (; i < data->size; ++i)
    if (data->list[i].type == INTEGER || data->list[i].type == FLOAT)
      fret = operat[operation] (fret, data->list[i].data);
      
  sprintf (numbuf, "%g", fret);

  if (__isfloat (numbuf) != 0)
    tr.type = INTEGER;
  
  tr.data = calloc (strlen (numbuf) + 1, sizeof (char));
  strcpy (tr.data, numbuf);
  
  return tr;  
}

token mult_f   (tokenlist *data) {return __master (data, 0);}
token divide_f (tokenlist *data) {return __master (data, 1);}
token minus_f  (tokenlist *data) {return __master (data, 2);}
token mod_f    (tokenlist *data) {return __master (data, 4);}

token
plus_f  (tokenlist *data)
{
  double fret = 0;
  size_t i = 0;
  size_t j = 0;
  size_t len = 0;
  char numbuf[32] = {0};
  token tr;

  if (data->size < 2)
    {
      printf ("\033[31mError: "
              "not enough arguments for function \"%s\"\033[0m\n",
              data->list[0].data);

      em_exit ();
    }
    
  data = pvartls (data, 1);
  
  tr.type = FLOAT;

  for (i = 1; i < data->size; ++i)
    if (data->list[i].type == INTEGER || data->list[i].type == FLOAT)
      fret += atof (data->list[i].data);
    else
      break;
      
  sprintf (numbuf, "%g", fret);

  if (__isfloat (numbuf) != 0)
    tr.type = INTEGER;
  
  tr.data = calloc (strlen (numbuf) + 1, sizeof (char));
  strcpy (tr.data, numbuf);

  if (i < data->size)
    {
      tr.type = STRING;
      
      if (i == 1)
        tr.data[0] = 0;

      len = strlen (tr.data);
      
      for (j = i; j < data->size; ++j)
        len += strlen (data->list[j].data);

      tr.data = realloc(tr.data, (len + 1) * sizeof (char));

      for (j = i; j < data->size; ++j)
        strcat (tr.data, data->list[j].data);
    }
  
  return tr;  
}


token
print_f (tokenlist *data)
{
  size_t i;
  token tr;

  if (data->size < 2)
    {
      printf ("\033[31mError: "
              "not enough arguments for function \"%s\"\033[0m\n",
              data->list[0].data);
      em_exit ();
    }

  data = pvartls (data, 1);
  
  tr.type = INTEGER;
  tr.data = calloc (2, sizeof (char));
  tr.data[0] = '0';
  
  for (i = 1; i < data->size; ++i)
    printf ("%s", data->list[i].data);

  /* if (intrmode != 0)
     putchar (0x0A); */

  fflush (stdout);
  
  return tr;
}

token
more_f (tokenlist *data)
{
  token tr;

  data = pvartls (data, 1);
  
  tr.type = INTEGER;
  tr.data = calloc (2, sizeof (char));
  strcpy (tr.data, atof (data->list[1].data) >
          atof (data->list[2].data) ? "1" : "0");
    
  return tr;
}

token
less_f (tokenlist *data)
{
  token tr;

  data = pvartls (data, 1);
  
  tr.type = INTEGER;
  tr.data = calloc (2, sizeof (char));
  strcpy (tr.data, atof (data->list[1].data) <
          atof (data->list[2].data) ? "1" : "0");
    
  return tr;
}

token
equ_f (tokenlist *data)
{
  token tr;

  data = pvartls (data, 1);
    
  tr.type = INTEGER;
  tr.data = calloc (2, sizeof (char));
  
  if (data->list[1].type == STRING && data->list[2].type == STRING)
    strcpy (tr.data, !strcmp (data->list[1].data, data->list[2].data)
            ? "1" : "0");
  else
    strcpy (tr.data, strtol (data->list[1].data, NULL, 10) ==
            strtol (data->list[2].data, NULL, 10) ? "1" : "0");
    
  return tr;
}

token
exit_f (tokenlist *data)
{
  token tr;
  
  int exitstat = (int) strtol (data->list[1].data, NULL, 10);

  clear_pfdata (g_pd);
  clear_varlist (g_var);
  clear_fdata (g_fdat);
  clear_vlstack (g_vlst);
  ftoklist (toklst);
  
  free (g_fdat);
    
  exit (exitstat);

  return tr;
}

token
defvar_f (tokenlist *data)
{
  token tr;
  size_t i = 0;
  varlist *vlptr = g_vlst->size == 0 ? g_var : g_vlst->list[g_vlst->size - 1];
  
  for (i = 0; i < vlptr->size; ++i)
    if (!strcmp (vlptr->list[i]->name, data->list[1].data))
      return set_f (data);
  
  data = pvartls (data, 2);
  
  vlptr->list = realloc (vlptr->list, ++vlptr->size * sizeof (variable*));
  vlptr->list[vlptr->size - 1] = malloc (sizeof (variable));

  vlptr->list[vlptr->size - 1]->name = calloc (strlen (data->list[1].data) + 1,
                                              sizeof (char));
  strcpy (vlptr->list[vlptr->size - 1]->name, data->list[1].data);

  if (data->size > 2)
    {
      vlptr->list[vlptr->size - 1]->value =
        calloc (strlen (data->list[2].data) + 1, sizeof (char));
      
      strcpy (vlptr->list[vlptr->size - 1]->value, data->list[2].data);
      vlptr->list[vlptr->size - 1]->type = data->list[2].type;
    }
  else
    {
      vlptr->list[vlptr->size - 1]->value = calloc (2, sizeof (char));
      vlptr->list[vlptr->size - 1]->value[0] = '0';
      vlptr->list[vlptr->size - 1]->type = INTEGER;
    }
  
  tr.data = calloc (2, sizeof (char));
  tr.data[0] = '0';
  tr.type = INTEGER;
  
  return tr;
} 

token
do_f (tokenlist *data)
{
  token tr;
  (void) data;

  if (data->size > 1)
    copytok (&tr, &data->list[data->size - 1]);
  else
    {
      tr.type = INTEGER;
      tr.data = calloc (2, sizeof (char));
      tr.data[0] = '0';
    }
  
  return tr;
}

token
set_f (tokenlist *data)
{
  token tr;
  int i;
  varlist *vlptr = g_var;
  
  data = pvartls (data, 2);

  for (i = 0; (size_t) i <= vlptr->size; ++i)
    {
      if ((size_t) i == vlptr->size)
        {
          if (g_vlst->size == 0)
            break;

          /* if (vlptr == g_vlst->list[g_vlst->size - 1])
             break; */
          
          vlptr = g_vlst->list[g_vlst->size - 1];
          i = -1;

          continue;
        }
  
      if (!strcmp (vlptr->list[i]->name, data->list[1].data))
        {
          free (vlptr->list[i]->value);

          vlptr->list[i]->value = calloc (strlen (data->list[2].data) + 1,
                                          sizeof (char));
          strcpy (vlptr->list[i]->value, data->list[2].data);

          vlptr->list[i]->type = data->list[2].type;

          break;
        }  
    }
    
  tr.type = data->list[2].type;
  tr.data = calloc (strlen (data->list[2].data) + 1, sizeof (char));
  strcpy (tr.data, data->list[2].data);

  return tr;
}

token
require_f (tokenlist *data)
{
  token tr;
  long fsize = 0;
  FILE *file = NULL;
  char *code = NULL;
  tokenlist* tmptl = NULL;
  prog_data* tmppd = NULL;

  tmppd = create_pfdata ();
  
  data = pvartls (data, 1);

  fsize = get_file_size (data->list[1].data);
  file = fopen (data->list[1].data, "r");
  
  code = calloc ((size_t) fsize + 1, sizeof (char));
  fread (code, (size_t) fsize, (size_t) fsize, file);
  fclose (file);

  tmptl = lexer (code);
  free (code);
  
  interpret_code (&g_pd, tmptl);

  tr.data = calloc (2, sizeof (char));
  strcpy (tr.data, "0");
  tr.label = 0;
  tr.level = 0;
  tr.type =  INTEGER;
  
  clear_pfdata (tmppd);
  ftoklist (tmptl);
    
  return tr;
}

static void
__no_echo (void)
{
  struct termios new_settings;

  tcgetattr (0, &stored_settings);
  new_settings = stored_settings;

  new_settings.c_lflag &= (unsigned int) (~ICANON & ~ECHO);
  new_settings.c_cc[VTIME] = 0;
  new_settings.c_cc[VMIN] = 1;

  tcsetattr (0, TCSANOW, &new_settings);
}

static void
__echo (void)
{
  tcsetattr (0, TCSANOW, &stored_settings);
}

token
getchar_f (tokenlist *data)
{
  token tr;
  (void) data;

  __no_echo ();
  
  tr.type = STRING;
  tr.data = calloc (2, sizeof (char));
  tr.data[0] = (char) getchar ();

  fflush (stdin);
  
  __echo ();
  
  return tr;
}
