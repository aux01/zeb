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
#include <string.h>
#include <getopt.h>

#define LINEMAX 4096

#define ZEB_PRIMARY   0
#define ZEB_SECONDARY 1
#define ZEB_HEADER    2

static char usage[] =
        "Usage: %s [-w <width>] [-c <code>]...\n"
        "Add alternating color to lines read from standard input and write to\n"
        "standard output.\n"
        "\n"
        "Options:\n"
        "    -c, --color=<code>      ANSI color code. Pass up to three -c arguments\n"
        "                            for primary, secondary, and first line styling.\n"
        "    -w, --width=<width>     Number of horizontal columns to colorize.\n";

/*
 * Write to output stream.
 * Exit process non-zero when write fails.
 */
static size_t zeb_write(const char *ptr, size_t size) {
        if (size == 0) return 0;

        size_t w = fwrite(ptr, size, 1, stdout);
        if (w) return w;

        int eof = feof(stdout),
            err = ferror(stdout);
        fprintf(stderr, "error: zeb_write: err=%d, eof=%d\n", err, eof);
        exit(10);
}

/*
 * Write an SGR sequence code to the output stream.
 * Exit process non-zero when write fails.
 */
static void zeb_write_seq(const char *ptr, size_t size) {
        if (size == 0) return;

        zeb_write("\x1b[", 2);
        zeb_write(ptr, size);

        // save and restore sequence codes
        if (ptr[0] == 's' || ptr[0] == 'u')
                return;

        zeb_write("m", 1);
}

int main(int argc, char *argv[]) {
        static struct option longopts[] = {
                { "color", required_argument, NULL, 'c' },
                { "width", required_argument, NULL, 'w' },
                { "help",  no_argument,       NULL, 'h' },
                { NULL,    0,                 NULL,  0  }
        };

        // default width of each table row in character cells
        uint32_t width = 78;

        // 1st color: alternate this on every other line starting at line 1
        // 2nd color: alternate this on every other line starting at line 2
        // 3rd color: alternate this on for line 1 only
        char *colors[3] = { "40", "0", "" };
        size_t colorlen[3] = { strlen(colors[0]), strlen(colors[1]), strlen(colors[2]) };
        int colorpos = 0;

        // parse arguments
        int ch;
        while ((ch = getopt_long(argc, argv, "c:hw:", longopts, NULL)) != -1) {
                switch (ch) {
                case 'c':
                        if (colorpos >= 3) {
                                fprintf(stderr, "error: too many colors: max=3\n");
                                return 1;
                        }
                        colorlen[colorpos] = strlen(optarg);
                        colors[colorpos] = malloc(colorlen[colorpos]);
                        strncpy(colors[colorpos], optarg, colorlen[colorpos]);
                        colorpos++;
                        break;
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

        // buffer of space characters
        char spaces[LINEMAX];
        memset(spaces, ' ', LINEMAX - 1);

        // getline accounting
        char *line = NULL;
        size_t linecap = 0;
        ssize_t linelen;

        // current line number
        int lineno = 0;

        while ((linelen = getline(&line, &linecap, stdin)) > 0) {
                // write altenating color
                int pos = lineno++ % 2;
                zeb_write_seq(colors[pos], colorlen[pos]);

                // write
                if (lineno == 1)
                        zeb_write_seq(colors[ZEB_HEADER], colorlen[ZEB_HEADER]);

                zeb_write_seq("s", 1);                  // save cursor position
                zeb_write(spaces, width);               // write width num of spaces
                zeb_write_seq("u", 1);                  // restore cursor position
                zeb_write(line, (size_t)linelen-1);     // write the line
                zeb_write_seq("0", 1);                  // write color reset
                zeb_write("\n", 1);                     // write the newline
        }

        // reset all colors
        zeb_write_seq("0", 1);

        return 0;
}
