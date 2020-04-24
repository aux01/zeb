#include <stdio.h>
#include <strings.h>

#define LINEMAX (1024 * 4)

int main(void) {
        // width of each table row in character cells
        int width = 80;

        // alternating color sequences
        char *colors[] = { "\e[102m", "\e[103m" };
        int colorsz = strlen(colors[0]);

        // reset
        char *coloroff = "\e[0m";
        int coloroffsz = strlen(coloroff);

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
                fwrite(line, linelen - 1, 1, stdout);
                fwrite(coloroff, coloroffsz, 1, stdout);
                fwrite("\n", 1, 1, stdout);
        }

        return 0;
}
