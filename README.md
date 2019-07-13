# Snake
Snake in ncurses. 
Playable.

Requires ncurses. 
I haven't managed to compile it under Windows, on \*nixes should be ok.
I've only tested it under Linux though and I know nothing about BSDs and similar stuff.

To build, set your preferred compiler in the makefile and run `make`.
Of course you need make for that.
Note that it will be compiled simply as *snake*, but `make install` will install it to /usr/local/bin as *coolsnake*, to prevent name conflicts.  
To uninstall simply delete coolsnake from /usr/local/bin.
