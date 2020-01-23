### What is this?

Ever wanted to build fancy "graphical", interactive user interfaces in your C++ terminal Linux application? If so you probably tried to work yourself into [ncurses](https://en.wikipedia.org/wiki/Ncurses) and probably gave up at one point into development. However it might be, this project tries to be a more convenient way of using ncurses in C++ terminal applications.

### What do i need?

1.  a linux distribution installed on your favourite hardware (virtual, pc, laptop, raspberry, ...)
2.  a C++ compiler (probably g++). Minimum C++ version necessary: 11
3.  ncurses development sources. Install from your favourite paket sources (e.g. debian-based: `sudo apt install libncurses5-dev`) or here: https://ftp.gnu.org/gnu/ncurses/

### How do i start?

1.  `git clone`
2.  `make all`
3.  `bin/demo` or `bin/snake`