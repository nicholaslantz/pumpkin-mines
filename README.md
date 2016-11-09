# Pumpkin Mines #
## By Nicholas Lantz ##

A Minesweeper implementation written in C using ncurses.

### Installation ##

At the moment, it will only compile on a Linux machine. The program requires
development libraries for ncurses as well as gcc. Once those two are installed,
you just need to type make in the installed directory and the program will
build itself. From there, type ./pumpkin-mines and the program will run.

If you desire to actually install it, there is currently a 'make install'
target which will copy the binary into your '~/bin' directory. From there,
you can add '~/bin' to your PATH environment variables and you can type

```
$ pumpkin-mines
```

into your shell, and the program will launch.

### Controls ###

* wasd or arrow keys to move the cursor.
* enter,e to reveal a cell
* space,f to flag a cell
* q to quit

### To Do ###

* Add better win/loss feedback
* High scores
* Multiple board types (easy, intermediate, hard)
