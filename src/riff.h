/* Copyright (C) 1999--2005 Chris Vaill
   This file is part of normalize.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA*/

#ifndef _RIFF_H_
#define _RIFF_H_

#include <stdio.h>
#include <stdint.h>
#if HAVE_INTTYPES_H
# include <inttypes.h>
#endif
#if HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

/* RIFF data types */
typedef uint32_t fourcc_t;

struct _riff_t /* pulled out of riff.c file, should not have been there */
{
    FILE *fp; /* needed for stream writing */
    int mode;
    FILE *msg_stream; /* stream to which messages are written */
};

typedef struct _riff_t *riff_t;

typedef struct _riff_chunk_t
{
  riff_t riff;   /* riff the chunk is part of */
  long offset;   /* file offset (to beginning of data part) */
  fourcc_t id;   /* chunk id */
  uint32_t size; /* chunk size, not including id and size fields */
  fourcc_t type; /* form type; just for RIFF and LIST chunks */
  int write;     /* are we writing this chunk? */
} riff_chunk_t;

/* definitions for common id fields of riff_chunk_info_t */
#ifdef WORDS_BIGENDIAN
# define RIFFID_RIFF ((fourcc_t)0x52494646) /* "RIFF" in big-endian */
# define RIFFID_LIST ((fourcc_t)0x4C495354) /* "LIST" in big-endian */
# define RIFFID_JUNK ((fourcc_t)0x4A554E4B) /* "JUNK" in big-endian */
#else
# define RIFFID_RIFF ((fourcc_t)0x46464952) /* "RIFF" in little-endian */
# define RIFFID_LIST ((fourcc_t)0x5453494C) /* "LIST" in little-endian */
# define RIFFID_JUNK ((fourcc_t)0x4B4E554A) /* "JUNK" in little-endian */
#endif

/* definitions for riff access permissions */
#define RIFF_RDONLY 1
#define RIFF_WRONLY 2
#define RIFF_RDWR   (RIFF_RDONLY | RIFF_WRONLY)

/* definitions for search flags */
#define RIFF_SRCH_OFF      0
#define RIFF_SRCH_FLAT     1
#define RIFF_SRCH_RECURSE  2

/* new interface */
riff_t riff_open(const char *fname, int mode);
riff_t riff_fdopen(int fd, int mode);
int riff_close(riff_t riff);
int riff_descend(riff_t riff, riff_chunk_t *chnk, riff_chunk_t *par_chnk, int search);
int riff_create_chunk(riff_t riff, riff_chunk_t *chnk);
int riff_ascend(riff_t riff, riff_chunk_t *chnk);
FILE *riff_stream(riff_t riff);
FILE *riff_chunk_stream(riff_t riff, riff_chunk_t *chnk);

/* somewhat overly complicated op here: */
#define riff_fourcc_to_string(s,f) do{s[4]='\0';*((fourcc_t *)(s))=(f);}while(0)
#define riff_string_to_fourcc(s) (*((fourcc_t *)s))

#endif /* _RIFF_H_ */
