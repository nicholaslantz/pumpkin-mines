# Pumpkin Mines #
## By Nicholas Lantz ##

A Minesweeper implementation written in C using ncurses.

### Installation ###

At the moment, it will only compile on a Linux machine. The program requires
development libraries for ncurses as well as gcc. Once those two are installed,
you just need to type make in the installed directory and the program will
build itself. From there, type ./pumpkin-mines and the program will run.

If you desire to actually install it, just copy the binary to any location
that is on your PATH. (type echo $PATH in a terminal to see directories in your
path). Once this is done, type 

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

For the time being, download and install the program. Then run it with the -h
flag. This will give you a list of all the arguments you can give it.

You can use the following command to create a board as large as your terminal can
handle. To increase or decrease the number of mines, change the fraction
represented in the -m argument. 20% is about right in terms of difficulty.
Please note, you must accomplish the fraction in terms of integer division
because the program will not accept floating point numbers.

```
$ pumpkin-mines -r $((LINES-3)) -c $((COLUMNS-3)) -m $(((2 * LINES * COLUMNS) / 10))
```

### Controls ###

* arrow keys to move the cursor.
* e to reveal a cell
* f to flag a cell
* q to quit
* F3 To toggle debug mode
    * You can also give the program -d or --debug as arguments to
      enable debug mode.

### To Do ###

* Add better win/loss feedback
* High scores
