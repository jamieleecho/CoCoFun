# CoCoFun
Random CoCo 3 code written with CMOC. This repository is intended to be a fun way to write quick and dirty little programs for the CoCo 3 and also revisit old Assembly and Basic programs that I wrote in the late 80s and early 90s before I had any formal software development training.
   
   
The following are required to compile this code on your Mac OS X system.
* [XCode](https://developer.apple.com/xcode/downloads/)
* [CMOC](http://perso.b2b2c.ca/sarrazip/dev/cmoc.html)
* [lwtools](http://lwtools.projects.l-w.ca/)
* [ToolShed](http://sourceforge.net/projects/toolshed/)
* [Mess](http://sourceforge.net/projects/toolshed/)
* CoCo 3 ROMs
   
   
There are several folders which are described below.
* Balloon - You are a ballon on a skateboard that must avoid a pin. This was my first real assembly language game and was written around 1988. This game did not use CoCo 3 graphic modes because I did not know how to access them. Further, it has a really bad flicker because I did not implement double buffering. After writing this game I decided to work on something that actually used CoCo 3 graphic modes and did not need double buffering.
* Splinter - A Breakout clone written in C and assembly language. This was originally written around 1989 in pure assembly language. The purpose was to learn how to use the CoCo 3 graphic modes and to write a game in such a way that does not require double buffering. The newer version adds slightly better graphics, background sounds and replaces joystick support with keyboard support. It still does not implement double buffering - this results in visual "tearing" that can be seen when the ball is in motion and artifacts when bitmaps are close to each other.
* Rick - You are Rick Tracy and ... I never finished this game. The intent was to take advantage of CoCo 3 graphic modes, implement real sprites and double buffering.
* Demo - A cheesy little demo program to test CMOC
* Scratch - Scratch pad for testing ideas quickly
* Misc - Folder containing scripts to help import graphic data
* Ancient - Very old (from the 80s) assembly language programs
* cfgs - Configuration directories for testing programs with mess
