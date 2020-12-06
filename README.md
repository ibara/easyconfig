Easy Config
===========
Configuration for the rest of us.

Why?
----
Do you struggle with autotools? Is CMake more complex than you need?

Easy Config is an ncurses-driven, flat text file configuration utility to create GNU style `configure` scripts that then create `Makefile`s. By default, the generated `Makefile`s are ready for BSD make and GNU make.

Documentation
-------------
Forthcoming. Easy Config is still being finalized. Easy Config uses itself to generate its `configure` script. Look at `easyconfig.conf` for now to see how easy it is to write your own Easy Config files!

Dependencies
------------
Uses ncurses and the
[dialog](https://invisible-island.net/dialog/manpage/dialog_lib.html)
library. Both should be readily available from your package manager.

License
-------
ISC License. See `LICENSE` for more information.
