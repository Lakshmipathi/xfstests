/*
 * Copyright (c) 2000-2001 Silicon Graphics, Inc.  All Rights Reserved.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 * 
 * Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 * Mountain View, CA  94043, or:
 * 
 * http://www.sgi.com 
 * 
 * For further information regarding this notice, see: 
 * 
 * http://oss.sgi.com/projects/GenInfo/SGIGPLNoticeExplan/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
 * fill pathname key nbyte
 *
 * use key to seed random number generator so data is deterministic
 *
 * lines are at most 72 bytes long so diff(1) does not choke
 */

int
main(int argc, char **argv)
{
    FILE		*f;
    unsigned int	seed;
    long		i;
    int			nbyte;
    char		*hdr;
    char		*hp;
    char		c;

    /* quick and dirty, no args checking */
    if ((f = fopen(argv[1], "w")) == NULL) {
	fprintf(stderr, "fill: cannot create \"%s\": %s\n", argv[1], strerror(errno));
	exit(1);
    }
    seed = 0;
    i = 0;
    while (argv[2][i]) {
	seed <<= 8;
	seed |= argv[2][i];
	i++;
    }
    srand(seed);

    nbyte = atoi(argv[3]);

    /*
     * line format
     *
     * byte offset @ start of this line XXXXXXXXXXXX
     * test iteration number XXXX
     * key (usually file name) argv[2]
     * random bytes to fill the line
     */

    hdr = (char *)malloc(12+1+4+1+strlen(argv[2])+1+1);
    sprintf(hdr, "%012ld %04d %s ", (long int)0, 0, argv[2]);
    hp = hdr;

    for (i = 0; i < nbyte-1; i++) {
	if (*hp) {
	    c = *hp;
	    hp++;
	}
	else if ((i % 72) == 71)
	    c = '\n';
	else
	    c = 32+(rand() % (128-32));
	fputc(c, f);
	if (c == '\n') {
	    hp = hdr;
	    sprintf(hdr, "%012ld %04d %s ", i+1, 0, argv[2]);
	}
    }
    fputc('\n', f);

    exit(0);
}
