/*
 * Copyright (c) 2009 ...  2021 2022
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

#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#ifdef OpenBSD
#include <err.h>
#endif
#ifdef __FreeBSD_version
#include <err.h>
#endif
#ifdef __NetBSD_Version__
#include <err.h>
#endif

#include <j_lib2.h>
#include <j_lib2m.h>

#include "rtw.h"

/*
 * show_file_heading() -- Show run stats
 */
void show_file_heading(struct s_work *w, char *fname)

{

  fprintf(w->out.fp, "%s\n", LIT_C80);

  if (fname == (char *) NULL)
    fprintf(w->out.fp, "%s\n", LIT_STDIN);
  else
    {
      if (strcmp(fname, FILE_NAME_STDIN) == 0)
	fprintf(w->out.fp, "%s\n", LIT_STDIN);
      else
	fprintf(w->out.fp, "%s\n", fname);
    }
  
  fprintf(w->out.fp, "%s\n", LIT_C80);

} /* show_file_heading() */

/*
 * process_a_file()
 */
void process_a_file(struct s_work *w, char *fname, char **buf, size_t *bsize)

{

  char *fmt_in  = (char *) NULL;
  char *fmt_out = (char *) NULL;
  ssize_t cbytes = (ssize_t) 0;
  ssize_t wbytes = (ssize_t) 0;
  ssize_t tbytes = (ssize_t) 0;
  long int lines_read = 0L;
  long int lines_write = 0L;
  FILE *fp;
  
  if (w->verbose > 0)
    show_file_heading(w, fname);
  
  if ( ! open_in(&fp, fname, w->err.fp) )
    return;

  /*** process data ***/
  while ((cbytes = j2_getline(buf, bsize, fp)) > (ssize_t) -1)
    {
      lines_read++;
      j2_rtw((*buf));
      tbytes += cbytes;
      wbytes += strlen((*buf));
      if (w->justleft == TRUE)
	j2_justleft((*buf));
      fprintf(w->out.fp, "%s\n", (*buf));
      lines_write++;
    }
  
  /*** complete ***/
  if (w->verbose > 1)
    {
      if (fname != (char *) NULL)
	{
	  if (strcmp(fname, FILE_NAME_STDIN) != 0)
	    fmt_in = fname;
	}
      if (w->out.fname != (char *) NULL)
	{
	  if (strcmp(w->out.fname, FILE_NAME_STDIN) != 0)
	    fmt_out = w->out.fname;
	}
      fprintf(w->err.fp, MSG_INFO_I096, (long int) tbytes,
              (fmt_in == (char *) NULL ? LIT_STDIN  : fmt_in));
      fprintf(w->err.fp, MSG_INFO_I097, (long int) wbytes,
              (fmt_out == (char *) NULL ? LIT_STDOUT : fmt_out));
      fprintf(w->err.fp, MSG_INFO_I072, lines_read,
              (fmt_in == (char *) NULL ? LIT_STDIN  : fmt_in));
      fprintf(w->err.fp, MSG_INFO_I080, lines_write, 
              (fmt_out == (char *) NULL ? LIT_STDOUT : fmt_out));
    }
  
  close_in(&fp, fname);

} /* process_a_file() */

/*
 * process_all() -- Process all files
 */
void process_all(int argc, char **argv, struct s_work *w)

{
  int i;
  char *buf = (char *) NULL;
  size_t bsiz = (size_t) 200;
  
  /* allocate initial read buffer memory (optional) */
  buf = (char *) calloc(bsiz, sizeof(char));
  if (buf == (char *) NULL)
    {
      fprintf(w->err.fp, MSG_ERR_E080, strerror(errno));
      return;
    }

  /* process files */
  for (i = optind; i < argc; i++)
    process_a_file(w, argv[i], &buf, &bsiz);

  if (i == optind)
    process_a_file(w, FILE_NAME_STDIN, &buf, &bsiz);

#ifdef OpenBSD
  freezero(buf, bsiz);
#else
  if (buf != (char *) NULL)
    free(buf);
#endif

}  /* process_all() */

/*
 * main()
 */
int main(int argc, char **argv)

{
  struct s_work w;

#ifdef OpenBSD
  if(pledge("stdio rpath wpath cpath",NULL) == -1)
    err(1,"pledge\n");
#endif

  init(argc, argv, &w);
  
  process_all(argc, argv, &w);
  
  close_out(&(w.out));
  close_out(&(w.err));
  exit(EXIT_SUCCESS);

}  /* main() */
