#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#define FUNCNUM 15

#define TRUE_BRANCH       2
#define FALSE_BRANCH      3
#define CYCLE_EXPRESSION  4
#define END_BRANCH       -1
#define EXPRESSION_TRUE  -2
#define EXPRESSION_FALSE -3
#define END_EXPRESSION   -4
#define END_IF           -5
#define END_CYCLE_BODY   -6
#define END_CYCLE        -7
#define RETURN           -8

token plus_f    (tokenlist *data);
token minus_f   (tokenlist *data);
token mult_f    (tokenlist *data);
token divide_f  (tokenlist *data);
token mod_f     (tokenlist *data);
token print_f   (tokenlist *data);
token more_f    (tokenlist *data);
token less_f    (tokenlist *data);
token equ_f     (tokenlist *data);
token exit_f    (tokenlist *data);
token defvar_f  (tokenlist *data);
token do_f      (tokenlist *data);
token set_f     (tokenlist *data);
token require_f (tokenlist *data);
token getchar_f (tokenlist *data);

typedef struct prog_data
{
  tokenlist **pfdata;
  size_t size;
} prog_data;

typedef struct variable
{
  char *name;
  char *value;
  TYPE type;
} variable;

typedef struct varlist
{
  variable **list;
  size_t size;
} varlist;

typedef struct vlstack
{
  varlist **list;
  size_t size;
} vlstack;

typedef struct func_data
{
  char **fnames;
  tokenlist **tl;
  prog_data ***pd;
  varlist **vl;
  size_t size;
  size_t pdsz;
} func_data;

extern char *funcs[FUNCNUM];
extern token (*pfuncs[]) (tokenlist *data);
extern prog_data *g_pfuncs;
extern prog_data *g_pd;
extern tokenlist *toklst;
extern varlist *g_var;
extern vlstack *g_vlst;
extern func_data *g_fdat;
extern int intrmode;
extern  struct termios stored_settings;

varlist *create_varlist (void);
int clear_varlist (varlist *vlist);
void em_exit (void);
tokenlist *pvartls (tokenlist *tl, size_t beg);
int push_pfdata (prog_data *pd);
int pop_pfdata (prog_data *pd);
prog_data *create_pfdata (void);
int clear_pfdata (prog_data *pd);
token cstmfunc (func_data *fd, size_t fnum, tokenlist *tl);
int chckf (prog_data *pd, tokenlist *tl);
int label_if (tokenlist *tl, int pos);
int label_while (tokenlist *tl, int pos);
size_t gotofwd (tokenlist *tl, int pos, int label);
size_t gotoback (tokenlist *tl, int pos, int label);
long int get_file_size (const char *file_name);
long int get_file_size_ff (FILE *__fd);
token interpret_code (prog_data **pd, tokenlist *tl);
func_data *create_fdata (void);
size_t deffunct (tokenlist *tl, size_t pos, func_data *fdat);
int clear_fdata (func_data *fd);
vlstack *create_vlstack (void);
int clear_vlstack (vlstack *vlst);
int push_vlstack (vlstack *vlst);
int pop_vlstack (vlstack *vlst);
int push_varlist (varlist *vl, variable *vr);
int copyvar (variable *dest, const variable *src);
void set_ret (tokenlist *tl, size_t pos);
