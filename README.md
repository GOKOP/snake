# Snake
Snake in ncurses. 
Playable.

Requires ncurses. 
Building on Windows may be a problem, on \*nixes should be fine though.
If you want to compile under Windows, remember that you have to replace `usleep()` with something else.
(`usleep()` is only used in the `millisleep()` procedure in main.cpp)
Only tested on Linux.
