/*
 * Copyright (c) 1994 1995 1996 ... 2023 2024
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * rtw_j.c -- routines from lib j_lib2
 */

#ifndef _MSDOS
#include <sys/param.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "rtw.h"

#ifdef _AIX
#define HAS_GETLINE 1
#endif
#ifdef __NetBSD_Version__
#define HAS_GETLINE 1
#endif
#ifdef OpenBSD
#define HAS_GETLINE 1
#endif
#ifdef __FreeBSD__
#define HAS_GETLINE 1
#endif
#ifdef linux
#define HAS_GETLINE 1
#endif

#define SIZE_GETLINE_BUF 256

/*
 * j2_f_exist() -- determines if a file exists
 */
int j2_f_exist(char *file_name)

{
  if (file_name == (char *) NULL)
    return((int) FALSE);

#ifdef _MSDOS
  if (access(file_name, 00) == -1)
    return (FALSE);
  else
    return (TRUE);
#else
  struct stat file_info;
  if (stat(file_name, &file_info) == 0)
    return (TRUE);
  else
    return (FALSE);
#endif

} /* j2_f_exist() */

/*
 * j2_clr_str() -- Clears a string with a char & ensure it ends with NULL
 */
long int j2_clr_str(char *s, char c, int size)
  
{
  if (s == (char *) NULL)
    return(0L);
  
  memset(s, (int) c, (size_t) size);
  s[size - 1 ] = JLIB2_CHAR_NULL;

  return((long int) strlen(s));

} /* j2_clr_str() */

/*
 * j2_getline() -- A front end to getline(3) or a hack for
 *                 systems without getline(3)
 */
SSIZE_T j2_getline(char **buf, size_t *n, FILE *fp)
{
#ifdef HAS_GETLINE
  return(getline(buf, n, fp));
#else

  if ((*buf) == (char *) NULL)
    {
      (*n) = SIZE_GETLINE_BUF + 1;
      (*buf) = (char *) malloc(((*n) * sizeof(char)));
      if ((*buf) == (char *) NULL)
	return(-1);
      j2_clr_str((*buf), (*n), JLIB2_CHAR_NULL);
    }

  if (fgets((*buf), (*n), fp) == (char *) NULL)
    return(-1);
  return((SSIZE_T) strlen((*buf)));

#endif

} /* j2_getline() */

/*
 * j2_get_prgname() -- return filename or default name.
 *
 *                     basename() is not used.  Some OS Systems do not
 *                     have basename() so this can be a basename()
 *                     replacement
 */
char *j2_get_prgname(char *argv_0, char *default_name)

{
  int i = 0, j = 0;

  if (argv_0 == (char *) NULL)
    {
      if (default_name == (char *) NULL)
	return((char *) NULL);
      else
	return(strdup(default_name));
    }

  /* basename(3) not in Coherent or MS-DOS */

  for (i = 0, j = 0; argv_0[i] != JLIB2_CHAR_NULL; i++)
    {
      if (argv_0[i] == '/')
	j = i + 1;
      else
	{
	  if (argv_0[i] == '\\')
	    j = i + 1;
	}
    }

  if (argv_0[j] == JLIB2_CHAR_NULL)
    if (default_name == (char *) NULL)
      return((char *) NULL);
    else
      return(strdup(default_name));
  else
    return(strdup(&(argv_0[j])));

} /* j2_get_prgname() */

/*
 * j2_justleft() -- left justifies a string (removes beginning white
 *                  space) returns -1 on failure, strlen on success
 */
long int j2_justleft(char *s)

{
  char *temp_s;
  int i;
#ifdef OpenBSD
  size_t len;
#endif
  
  if (s == (char *) NULL)
    return(0L); /* NULL pointer, pretend 0 */

  if (strlen(s) < 2)
    {
      if (isspace((*s)))
	{
	  (*s) = JLIB2_CHAR_NULL;
	  return(0L);
	}
      else
	return((long int) strlen(s));
    }
  
#ifdef OpenBSD
  len = strlen(s) + 1;
  temp_s = strndup(s, len);
#else
  temp_s = strdup(s);
#endif

  if ( temp_s == (char *) NULL )
    return(-1L); /* not enough memory */
  
  for (i=0; temp_s[i] != JLIB2_CHAR_NULL; i++)
    {
      if (! isspace((int) temp_s[i]) )
	break;
    }
 
#ifdef OpenBSD
  strlcpy(s,&(temp_s[i]),len);
#else
  strcpy(s,&(temp_s[i]));
#endif

  free(temp_s);
  
  return((long int) strlen(s));

} /* j2_justleft() */

/*
 * j2_rtw() -- removes trailing white space
 */
long int j2_rtw(char *buffer)

{
  char *last_non_space;
  char *b = buffer;

  if (buffer == (char *) NULL)
    return(0L); /* NULL pointer */

  last_non_space = buffer;

  for ( ; (*buffer) != JLIB2_CHAR_NULL; buffer++)
    {
      if ( ! isspace((int)(*buffer)) )
	last_non_space = buffer;
    }

  if ( ! isspace ((int) *last_non_space) )
    last_non_space++;

  (*last_non_space) = JLIB2_CHAR_NULL;

  return((long int) strlen(b));

} /* j2_rtw() */
