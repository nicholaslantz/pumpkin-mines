# Pumpkin Mines #
## By Nicholas Lantz ##

A Minesweeper implementation written in C using ncurses.

### Installation ##

At the moment, it will only compile on a Linux machine. The program requires
development libraries for ncurses as well as gcc. Once those two are installed,
you just need to type make in the installed directory and the program will
build itself. From there, type ./pumpkin-mines and the program will run.

If you desire to actually install it, just copy the binary to any location
that is on your PATH. (type echo $PATH in a terminal to see directories in your
path). Once this done, type 

```
$ pumpkin-mines
```

into your shell, and the program will launch.

I recommend making an alias in your .bashrc, .zshrc, etc. like

```
alias pmines=pumpkin-mines
```

As that will make the program much easier to type/run

### Arguments ###

TODO

You can use the following command to create a board as large as your terminal can
handle. To increase or decrease the number of mines, change the fraction
represented in the -m argument. 20% is about right in terms of difficulty.
Please note, you must accomplish the fraction in terms of integer division
because the program will not accept floating point numbers.

```
$ pumpkin-mines -r $((LINES-3)) -c $((COLUMNS-3)) -m $(((2 * LINES * COLUMNS) / 10))
```

### Controls ###

* wasd or arrow keys to move the cursor.
    * shift to move in increments of 5 tiles.
        * Note: This will NOT work with arrow keys, see large comment
          in main.c for explanation.
* enter,e to reveal a cell
* space,f to flag a cell
* q to quit
* F3 To toggle debug mode
    * You can also give the program -d or --debug as arguments to
      enable debug mode.

### To Do ###

* Add better win/loss feedback
* High scores
