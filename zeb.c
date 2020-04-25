/*
 *
 * zeb.c
 *
 * Copyright (c) 2020, Auxrelius I <aux01@aux.life>
 * All rights reserved.
 *
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <getopt.h>

#define LINEMAX (1024 * 4)

static char usage[] =
        "Usage: %s [-w <width>]\n"
        "Add alternating color to lines read from standard input and write to\n"
        "standard output.\n"
        "\n"
        "Options:\n"
        "    -w <width>         Number of horizontal columns to colorize.\n";

/*
 * Write to output stream.
 * Exit process non-zero when write fails.
 */
static size_t zeb_write(const char *ptr, size_t size) {
        size_t w = fwrite(ptr, size, 1, stdout);
        if (w) return w;

        int eof = feof(stdout),
            err = ferror(stdout);
        fprintf(stderr, "error: write failed (err=%d, eof=%d)\n", err, eof);
        exit(10);
}

int main(int argc, char *argv[]) {
        static struct option longopts[] = {
                { "width", required_argument, NULL, 'w' },
                { "help",  no_argument,       NULL, 'h' },
                { NULL,    0,                 NULL,  0  }
        };

        // width of each table row in character cells
        uint32_t width = 78;

        // parse arguments
        int ch;
        while ((ch = getopt_long(argc, argv, "hw:", longopts, NULL)) != -1) {
                switch (ch) {
                case 'h':
                        printf(usage, argv[0]);
                        exit(0);
                        break;
                case 'w':
                        width = (uint32_t)atoi(optarg);
                        break;
                default:
                        printf(usage, argv[0]);
                        exit(1);
                }
        }

        // alternating color sequences
        char *colors[] = { "\x1b[40m", "\x1b[0m" };
        size_t colorsz = strlen(colors[0]);

        // reset
        char *coloroff = "\x1b[0m";
        size_t coloroffsz = strlen(coloroff);

        // buffer of space characters
        char spaces[LINEMAX];
        memset(spaces, ' ', LINEMAX - 1);

        // current line number
        int lineno = 0;

        // getline accounting
        char *line = NULL;
        size_t linecap = 0;
        ssize_t linelen;

        while ((linelen = getline(&line, &linecap, stdin)) > 0) {
                // write color sequence followed by N space chars, followed
                // by \r (move to beginning of line)...
                zeb_write(colors[lineno++ % 2], colorsz);
                zeb_write(spaces, width);
                zeb_write("\r", 1);

                // write actual line chars, followed by the color reset
                // sequence, followed by the newline.
                zeb_write(line, (size_t)linelen - 1);
                zeb_write(coloroff, coloroffsz);
                zeb_write("\n", 1);
        }

        return 0;
}
