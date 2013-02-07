/*-
 * Copyright (c) 2013 Romain Tartière <romain@blogreen.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum { LP_UNKNOWN, LP_EDID, LP_EDID_BYTES } log_position = LP_UNKNOWN;

void
usage (void)
{
    fprintf (stderr, "usage: edid-dump [-d driver-name] [-u driver-unit] /var/log/Xorg.0.log\n");
    exit (EXIT_FAILURE);
}

int
main (int argc, char *argv[])
{
    const char *driver_name = "NVIDIA";
    int driver_unit = 0;

    char *driver_needle = NULL;
    int driver_needle_length = 0;

    char screen_name[BUFSIZ], screen_manufacturer[BUFSIZ], screen_model[BUFSIZ];
    char *edid_end_sequence = NULL;

    char buffer[BUFSIZ];
    int byte;

    FILE *log, *dump;

    int ch;
    while ((ch = getopt(argc, argv, "d:u:")) != -1) {
	switch (ch) {
	case 'd':
	    driver_name = optarg;
	    break;
	case 'u':
	    driver_unit = strtol (optarg, NULL, 10);
	    break;
	case '?':
	    usage ();
	    break;
	}
    }

    argc -= optind;
    argv += optind;

    if (argc != 1)
	usage ();

    asprintf(&driver_needle, "%s(%d): ", driver_name, driver_unit);
    driver_needle_length = strlen (driver_needle);

    log = fopen (argv[0], "r");
    while (fgets(buffer, sizeof(buffer), log)) {
	char *p = strstr(buffer, driver_needle);
	if (!p)
	    continue;
	p += driver_needle_length;

	p[strlen (p) - 1] = '\0';

	switch (log_position) {
	case LP_UNKNOWN:
	    if (3 == sscanf (p, "--- EDID for %s %s (%[^)]) ---", screen_manufacturer, screen_model, screen_name)) {
		log_position = LP_EDID;
		asprintf(&edid_end_sequence, "--- End of EDID for %s %s (%s) ---", screen_manufacturer, screen_model, screen_name);
		fprintf (stderr, "Found EDID for %s %s\n", screen_manufacturer, screen_model);
		dump = fopen(screen_name, "w");
	    }
	    break;
	case LP_EDID:
	    if (0 == strcmp(p, "Raw EDID bytes:")) {
		log_position = LP_EDID_BYTES;
	    }
	    break;
	case LP_EDID_BYTES:
	    if (0 == strcmp(p, edid_end_sequence)) {
		log_position = LP_UNKNOWN;
		free (edid_end_sequence);
		fclose (dump);
		fprintf (stderr, "EDID dump written to %s\n", screen_name);
	    } else {
		while (*p) {
		    byte = strtol (p, &p, 16);
		    fprintf(dump, "%c", byte);
		}
	    }
	    break;
	}
    }

    fclose (log);

    free (driver_needle);

    exit (EXIT_SUCCESS);
}
