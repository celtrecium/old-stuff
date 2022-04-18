#include <codl.h>

int
hid_input_form (codl_window *win, char **str, int pos_x, int pos_y,
                 size_t size)
{
  codl_window *inp_win = NULL;
  char **inp_str       = NULL;
  int count            = 0;
  int count_1          = 0;
  int pos              = 0;
  size_t len           = 1;
  unsigned int key     = 0;
    
  if (str)
    {
      free (*str);
      *str = NULL;
    }

  inp_str = codl_malloc_check (size * sizeof (char*));

  for (count = 0; (size_t)count < size; ++count)
    {
      inp_str[count] = codl_malloc_check(4 * sizeof (char));
      codl_memset (inp_str[count], 4, 0, 4);
    }

  inp_win = codl_create_window (win, win->layer + 1, pos_x, pos_y,
                                (int)size, 1);
  codl_set_colour (inp_win, win->colour_bg, win->colour_fg);
  codl_rectangle (inp_win, 0, 0, (int)size, 1, " ");
  codl_write (inp_win, "█");

  codl_display ();
    
  while (key != CODL_KEY_RETURN)
    {
      key = codl_get_key ();

      switch (key)
        {
        case 0:
          break;

        case CODL_KEY_UNICODE:
          codl_memcpy (inp_str[pos], 4, codl_get_stored_key(), 4);
          codl_set_cursor_position (inp_win, pos, 0);
          codl_write (inp_win, "*");
          pos += (size_t)pos < size - 1 ? 1 : 0;
          codl_set_cursor_position (inp_win, pos, 0);
          codl_write (inp_win, "█");
          codl_display ();

          break;

        case CODL_KEY_BACKSPACE:
          codl_set_cursor_position (inp_win, pos, 0);
          codl_write (inp_win, " ");
          pos -= pos ? 1 : 0;
          codl_set_cursor_position (inp_win, pos, 0);
          codl_write (inp_win, "█");
          codl_memset (inp_str[pos], 4, 0, 4);
          codl_display ();
                
          break;

        default:
          if (key < ' ' || key > '~') break;
          codl_memset (inp_str[pos], 4, 0, 4);
          *inp_str[pos] = (char)key;
          codl_set_cursor_position (inp_win, pos, 0);
          codl_write (inp_win, "*");
          pos += (size_t)pos < size - 1 ? 1 : 0;
          codl_set_cursor_position (inp_win, pos, 0);
          codl_write (inp_win, "█");
          codl_display ();
                
          break;
        }
    }

  for (count = 0, pos = 0; (size_t)count < size; ++count) 
    for (count_1 = 0; count_1 < 4 && inp_str[count][count_1]; ++count_1,
           ++len) 
      *str = codl_malloc_check (len * sizeof(char));
    
  codl_memset (*str, len, 0, len);

  for (count = 0, pos = 0; (size_t)count < size; ++count)
    {
      for (count_1 = 0; count_1 < 4 && inp_str[count][count_1];
           ++count_1, ++pos) 
        (*str)[pos] = inp_str[count][count_1];
        
      free (inp_str[count]);
    }
    
  free (inp_str);

  codl_destroy_window (inp_win);

  return 1;
}


int
menu_function (codl_window *win, int colour_bg_act, int colour_fg_act,
               int colour_bg, int colour_fg, char *list)
{
  int count;
  int len = 0;
  int list_pos = 0;
  int redraw = 1;
  unsigned int key = 0;
  struct timespec interrupt = {0, 10000000};
  struct timespec ret;

  for ((void)(count = 0); list[count]; ++count)
    {
      if ((list[count] & 0xC0) == 0xC0) count += 2;
      else if ((list[count] & 0xE0) == 0xE0) count += 3;
      else if ((list[count] & 0xF0) == 0xF0) count += 4;

      if (list[count] == '\n') ++len;
    }


  if (win->height < len)
    codl_resize_window(win, win->width, len);

  codl_set_cursor_position (win, 0, 0);
  codl_write (win, list);

  for(;;)
    {
      nanosleep (&interrupt, &ret);

      key = codl_get_key ();

      if (codl_resize_term ())
        {
          codl_set_window_position (win->parent_win, 
                                    (codl_get_term ()->width  / 2) -
                                    (win->parent_win->width  / 2),
                                    (codl_get_term ()->height / 2) -
                                    (win->parent_win->height / 2));
          codl_redraw ();
          redraw = 1;
        }

      if (redraw)
        {
          codl_set_colour (win, colour_bg, colour_fg);
          codl_replace_attributes (win, 0, 0, win->width, win->height);
          codl_set_colour (win, colour_bg_act, colour_fg_act);
          codl_replace_attributes (win, 0, list_pos, win->width, list_pos);

          codl_display ();

          redraw = 0;
        }

      if (key)
        {
          codl_set_colour (win, colour_bg, colour_fg);
          codl_replace_attributes (win, 0, 0, win->width, win->height);
          codl_set_colour (win, colour_bg_act, colour_fg_act);
          codl_replace_attributes (win, 0, list_pos, win->width, list_pos);
			
          switch (key)
            {
            case CODL_KEY_DOWN:
              if (list_pos < len) ++list_pos;
              redraw = 1;

              break;
            case CODL_KEY_UP:
              if (list_pos > 0) --list_pos;
              redraw = 1;

              break;
            case CODL_KEY_RETURN:
              return list_pos;
            }
        }
    }
}

char*
passwd (void)
{
  char *ret = NULL;
  codl_window *pwd_win = codl_create_window (NULL, codl_get_num_of_wins () + 1,
                                            (codl_get_term ()->width - 19) / 2,
                                            (codl_get_term ()->height - 5) / 2,
                                             21, 6);

  codl_set_alpha (pwd_win, CODL_ENABLE);

  codl_set_colour (pwd_win, 8, CODL_WHITE);
  codl_rectangle (pwd_win, 2, 1, pwd_win->width, pwd_win->height, " ");
  
  codl_set_colour (pwd_win, CODL_BRIGHT_RED, CODL_BRIGHT_WHITE);
  codl_rectangle (pwd_win, 0, 0, pwd_win->width - 2, pwd_win->height - 1, " ");
  codl_set_frame_colours (15, 0, 15, 0, 15, 0, 15, 0);

  codl_frame (pwd_win, 0, 0, pwd_win->width - 2, pwd_win->height - 1);

  codl_set_colour (pwd_win, CODL_BRIGHT_RED, CODL_BRIGHT_WHITE);
  codl_set_cursor_position (pwd_win, 2, 1);
  codl_write (pwd_win, "Enter password:");
  
  codl_set_colour (pwd_win, CODL_BLACK, CODL_BRIGHT_WHITE);
  hid_input_form (pwd_win, &ret, 2, 3, 15);

  codl_destroy_window (pwd_win);
  
  return ret;
}

int
Update (char *passwd)
{
  char comm[1024];
  FILE* stream = NULL;
  
  sprintf (comm, "echo -en \"%s\\ny\" | su -c \"pacman -Syu\"", passwd);

  stream = popen (comm, "r");
  pclose (stream);
  
  return EXIT_SUCCESS;
}

int
main (void)
{
  const int WIDTH  = 51;
  const int HEIGHT = 10;
  
  codl_window *main_win = NULL;
  codl_window *main_text = NULL;
  codl_window *main_menu = NULL;

  int exit_stat = 0;
  
  codl_initialize ();
  main_win  = codl_create_window (NULL, 1,
                                 (codl_get_term ()->width - WIDTH) / 2,
                                 (codl_get_term ()->height - HEIGHT) / 2,
                                 WIDTH, HEIGHT);
  
  main_text = codl_create_window(main_win, 2, 2, 1, main_win->width - 4, 2);
  main_menu = codl_create_window (main_win, 3, 3, main_win->height - 6,
                                  main_win->width - 6, 4);

  codl_set_frame_colours (15, 0, 15, 0, 15, 0, 15, 0);
  codl_set_colour (main_win, CODL_WHITE, CODL_BLACK);
  codl_set_colour (main_text, CODL_WHITE, CODL_BLACK);

  codl_rectangle (main_win, 0, 0, main_win->width, main_win->height, " ");
  codl_rectangle (main_text, 0, 0, main_text->width, main_text->height, " ");

  codl_frame (main_win, 0, 0, main_win->width, main_win->height);

  codl_set_frame_colours (0, 15, 0, 15, 0, 15, 0, 15);
  codl_frame(main_win, 1, main_menu->ref_y_position - 1, main_win->width - 1,
             main_menu->height + main_menu->ref_y_position + 1);

  codl_set_cursor_position (main_win, 17, 0);
  codl_set_colour (main_win, CODL_WHITE, CODL_BLACK);

  codl_write (main_win, " pacfront v0.1-a1 ");
  codl_write (main_text, "Arrow keys navigate the menu. "
              "\e[1m<Enter>\e[0m selects\nsubmenus.");
  
  while (exit_stat == 0)
    {
      switch (menu_function (main_menu, CODL_BLUE, CODL_BRIGHT_WHITE, CODL_WHITE,
                             CODL_BLACK,
                             "\e[18CInstall\n"
                             "\e[18CUpdate\n"
                             "\e[18CRemove\n"
                             "\e[18CExit"))
        {
        case 0:
          break;
        case 1:
          Update (passwd ());
          break;
        case 2:
          break;
        case 3:
          exit_stat = 1;
          break;
        }

      codl_display ();
    }
  
  codl_end ();
  
  return EXIT_SUCCESS;
}
