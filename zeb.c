#include <stdint.h>
#include <stdio.h>
#include <strings.h>

#define LINEMAX (1024 * 4)

int main(void) {
        // width of each table row in character cells
        uint32_t width = 80;

        // alternating color sequences
        char *colors[] = { "\x1b[102m", "\x1b[103m" };
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
                fwrite(colors[lineno++ % 2], colorsz, 1, stdout);
                fwrite(spaces, width, 1, stdout);
                fwrite("\r", 1, 1, stdout);

                // write actual line chars, followed by the color reset
                // sequence, followed by the newline.
                fwrite(line, (size_t)linelen - 1, 1, stdout);
                fwrite(coloroff, coloroffsz, 1, stdout);
                fwrite("\n", 1, 1, stdout);
        }

        return 0;
}
