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
* Breakout - Breakout game written in C and assembly language. This was written around 1989.
* Rick - You are Rick Tracy. I never finished this game and it currently crashes. The intent was to take advantage of CoCo 3 graphic modes, implement real sprites and double buffering.
* Demo - A cheesy little demo program to test CMOC
* Scratch - Scratch pad for testing ideas quickly
* Misc - Folder containing scripts to help import graphic data
* Ancient - Very old (from the 80s) assembly language programs
* cfgs - Configuration directories for testing programs with mess
