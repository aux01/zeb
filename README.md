# zeb

The `zeb` command-line utility writes its input with alternating colors,
sometimes called *"zebra striping"*. This is especially useful when working
with tabular data like CSV, TSV, and sqlite but can be used with any
textual input.

    $ cat ford_escort.csv | column -s, -t | zeb

<img src="./zeb-yes.png" height="293" width="402">

#### Alternating lines and headers

You can give up to three `-c` arguments to specify the ANSI sequence codes
for three distinct rows:

  1. Primary color. Every other line starting at line 1 gets this.
  2. Secondary color. Every other line starting at line 2 gets this.
  3. Header color. The first line gets this in addition to the
     primary color.

Example of alternating red to green background every other line with
bold headings:

    $ cat ford_escort.csv | column -s, -t | zeb -c 41 -c 42 -c 1

Example of above but setting text color to black for each line:

    $ cat ford_escort.csv | column -s, -t | zeb -c "41;30" -c "42;30" -c 1

#### Color width

`zeb` colors a fixed number of columns. You can specify how many with:

    $ cat ford_escort.csv | column -s, -t | zeb -w 100

Fill terminal width with colors:

    $ cat ford_escort.csv | column -s, -t | zeb -w $(tput cols)
