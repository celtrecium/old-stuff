#include "../include/freasp.h"

long int
get_file_size(const char* file_name)
{
  long int _file_size = 0;
  FILE* fd = fopen (file_name, "rb");

  if(fd == NULL)
    {
      _file_size = -1;
    }
  else
    {
      fseek (fd, 0, SEEK_END);
      _file_size = ftell (fd);
      fclose (fd);
    }
  return _file_size;
}

long int
get_file_size_ff(FILE* __fd)
{
  long int _file_size = 0;

  if(__fd == NULL)
    {
      _file_size = -1;
    }
  else
    {
      fseek (__fd, 0, SEEK_END);
      _file_size = ftell (__fd);
      fseek (__fd, 0, SEEK_SET);
    }
  
  return _file_size;
}
