# CoCoFun

Random CoCo 3 code written with CMOC. This repository is intended to
be a fun way to write quick and dirty little programs for the CoCo 3 and also
revisit old Assembly and Basic programs that I wrote in the late 80s and early
90s before I had any formal software development training. More recently,  I
used it to enhance CMOC's optimizer.


There are several folders which are described below.
* Ancient - Very old (from the 80s) assembly language programs
* Balloon - You are a ballon on a skateboard that must avoid a pin. This was my
  first real assembly language game and was written around 1988. This game did
not use CoCo 3 graphic modes because I did not know how to access them. After
writing this game I decided to work on something that actually used CoCo 3
graphic modes and did not need double buffering.
* Bouncy - Use C code posted by Lee Patterson to enhance CMOC compiler optimizer.
* cfgs - Configuration directories for testing programs with mess
* Demo - A cheesy little demo program to test CMOC
* Misc - Folder containing scripts to help import graphic data
* Primes - Use CMOC to calculate prime numbers.
* Rick - You are Rick Tracy and ... I never finished this game. The intent was
  to take advantage of CoCo 3 graphic modes, implement real sprites and double
buffering.
* Splinter - A Breakout clone written in C and assembly language. This was
  originally written around 1989 in pure assembly language. The purpose was to
learn how to use the CoCo 3 graphic modes and to write a game in such a way that
does not require double buffering. The newer version adds slightly better
graphics, background sounds and replaces joystick support with keyboard support.
It still does not implement double buffering - this results in visual "tearing"
that can be seen when the ball is in motion and artifacts when bitmaps are close
to each other.


## Requirements   
The following are required to compile this code on your Mac OS X system.
* [XCode](https://developer.apple.com/xcode/)
* [Docker](http://www.docker.com)
* [MAME](https://github.com/mamedev/mame/) installed in ~/Applications/mame
* CoCo 3 ROMs installed in MAME
   

## Building and running
To build DSK files for each folder, type the following from the root folder:
```
./build
```

To clean everything:
```
./build clean
```

To build and run an individual demo:
```
cd <folder>
./build
make run
```

