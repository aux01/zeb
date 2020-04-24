#include <stdio.h>
#include <strings.h>

#define LINEMAX (1024 * 4)

int main(void) {
        int width = 80;

        char *colors[] = { "\e[102m", "\e[103m" };
        int colorsz = strlen(colors[0]);
        char *coloroff = "\e[0m";
        int coloroffsz = strlen(coloroff);

        char space[LINEMAX];
        for (int i = 0; i < LINEMAX; i++)
                space[i] = ' ';

        char *line = NULL;
        size_t linecap = 0;
        ssize_t linelen;

        int lineno = 0;
        while ((linelen = getline(&line, &linecap, stdin)) > 0) {
                fwrite(colors[lineno++ % 2], colorsz, 1, stdout);
                fwrite(line, linelen - 1, 1, stdout);
                fwrite(space, width - linelen, 1, stdout);
                fwrite(coloroff, coloroffsz, 1, stdout);
                fwrite("\n", 1, 1, stdout);
        }

        return 0;
}
