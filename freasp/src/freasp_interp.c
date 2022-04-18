#include "../include/freasp.h"
#include <string.h>

prog_data *g_pd   = NULL;
tokenlist *toklst = NULL;
varlist   *g_var  = NULL;
func_data *g_fdat = NULL;
vlstack   *g_vlst = NULL;
int        intrmode = 0;
struct termios stored_settings;

char *funcs[FUNCNUM] =
  {
    "+",
    "-",
    "*",
    "/",
    "mod",
    "print",
    ">",
    "<",
    "=",
    "exit",
    "defvar",
    "do",
    "set",
    "require",
    "getchar"
  };

token (*pfuncs[FUNCNUM]) (tokenlist *data) =
  {
    plus_f,
    minus_f,
    mult_f,
    divide_f,
    mod_f,
    print_f,
    more_f,
    less_f,
    equ_f,
    exit_f,
    defvar_f,
    do_f,
    set_f,
    require_f,
    getchar_f
  };

void
em_exit (void)
{
  ftoklist (toklst);
  clear_pfdata (g_pd);
  
  exit (EXIT_FAILURE);
}

token
interpret_code (prog_data **pd, tokenlist *tl)
{
  size_t i;
  token tr = {NULL, 0, 0, 0};

  int exit_stat = 0;
  
  push_pfdata (*pd);
  
  for (i = 0; i < tl->size && exit_stat == 0; ++i)
    {
      switch (tl->list[i].type)
        {
        case O_BRACKET:
          push_pfdata (*pd);
              
          break;
        case C_BRACKET:
          switch (tl->list[i].label)
            {
            case END_BRANCH:
              i = gotofwd (tl, (int) i, END_IF) - 1;
              break;
            case END_CYCLE_BODY:
              i = gotoback(tl, (int) i, 4) - 1;

              break;
            case END_IF:
            case END_CYCLE:
              pop_pfdata (*pd);

              continue;
            case RETURN:
              exit_stat = 1;

              pvartls ((*pd)->pfdata[(*pd)->size - 1], 1);
              copytok (&tr, &(*pd)->pfdata[(*pd)->size - 1]->list[1]);
              
              clear_pfdata (*pd);
              *pd = create_pfdata ();
                            
              continue;
            }
                        
          chckf (*pd, (*pd)->pfdata[(*pd)->size - 1]);

          if (tl->list[i].label == END_EXPRESSION)
            i = gotofwd (tl, (int) i,
                         strtol ((*pd)->pfdata[(*pd)->size - 1]->
                                 list[(*pd)->pfdata[(*pd)->size - 1]->size - 1].
                                 data, NULL, 10) != 0 ?
                         TRUE_BRANCH : FALSE_BRANCH) - 1;
              
          break;
        case IDENTIFIER:
          if (!strcmp (tl->list[i].data, "if"))
            label_if (tl, (int) i);
          else if (!strcmp (tl->list[i].data, "while"))
            label_while (tl, (int) i);
          else if (!strcmp (tl->list[i].data, "defun"))
            {
              i = deffunct (tl, i, g_fdat);
              
              continue;
            }
          else if (!strcmp (tl->list[i].data, "return"))
            set_ret (tl, i);
          
          push_tok ((*pd)->pfdata[(*pd)->size - 1], tl->list[i]);

          break;
        case TOK_EOF:
          break;
        default:
          push_tok ((*pd)->pfdata[(*pd)->size - 1], tl->list[i]);
          
          break;
        }
    }

  if (exit_stat == 0 && (*pd)->size != 0)
    {
      if ((*pd)->pfdata[0]->size != 0)
        copytok (&tr, &(*pd)->pfdata[0]->list[0]);
      
      pop_pfdata (*pd);
    }
  
  return tr;
}

int
main (int argc, char **argv)
{
  char *code = NULL;
  char shell[4096];
  long int fsize;
  FILE* file = NULL;
  token ext;

  g_pd = create_pfdata ();
  g_var = create_varlist ();
  g_fdat = create_fdata ();
  g_vlst = create_vlstack ();
  
  if (argc > 1)
    {
      if (!strcmp (argv[1], "-i") && argc > 2)
        {          
          toklst = lexer (argv[2]);

          ext = interpret_code (&g_pd, toklst);

          ftoklist (toklst);          
        }
      else
        {
          file = fopen (argv[1], "rb");

          fsize = get_file_size_ff (file);

          code = calloc ((size_t) fsize + 1, sizeof (char));
          fread (code, (size_t) fsize, (size_t) fsize, file);
          fclose (file);

          toklst = lexer (code);
          free (code);

          ext = interpret_code (&g_pd, toklst);

          ftoklist (toklst);
        }
    }
  else
    {
      intrmode = 1;
      
      puts ("🐬 Freasp 0.1-a1\n\n"
        "Freasp comes with ABSOLUTELY NO WARRANTY.\n"
        "This program is free software, and you are welcome to redistribute it\n"
        "under certain conditions.\n");
      
      while (1)
        {
          fputs ("\033[1;34m>>\033[0m ", stdout);
          fflush (stdout);
          fgets (shell, 4096, stdin);

          toklst = lexer (shell);

          ext = interpret_code (&g_pd, toklst);

          if (ext.data != NULL)
            switch (ext.type)
              {
              case INTEGER:
                printf ("\033[33m%s\033[0m\n", ext.data);
                break;
              case FLOAT:
                printf ("\033[36m%s\033[0m\n", ext.data);
                break;
              case STRING:
                printf ("\033[32m'%s'\033[0m\n", ext.data);
                break;
              default:
                printf ("\033[34m%s\033[0m\n", ext.data);
                break;
              }
          else
            puts ("\033[90m(null)\033[0m");

          ftoklist (toklst);  
        }
    }
  
  fflush (stdout);
  
  if (ext.data != NULL)
    free (ext.data);

  clear_pfdata (g_pd);
  
  clear_varlist (g_var);
  clear_fdata (g_fdat);

  clear_vlstack (g_vlst);
  
  free (g_fdat);
  
  return EXIT_SUCCESS;
}
