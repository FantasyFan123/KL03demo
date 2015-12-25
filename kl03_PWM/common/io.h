/*
 * File:		io.h
 * Purpose:		Serial Input/Output routines
 *
 */

#ifndef _IO_H
#define _IO_H


/********************************************************************/

char	
in_char(void);

void
out_char(char);

int
char_present(void);
#ifdef CW
int		
printf_k(const char *, ... );
#else
int		
printf(const char *, ... );
#endif

int
sprintf(char *, const char *, ... );

/********************************************************************/

#endif
