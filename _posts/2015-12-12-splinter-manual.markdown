---
layout: post
title:  "Splinter Manual"
date:   2015-12-12 08:29:56 -0500
categories: splinter manual breakout coco3
---
## Splinter   [![Download]({{site.url}}/{{site.baseurl}}/assets/Images/Download.png)]({{site.url}}/{{site.baseurl}}/assets/2015-12-12/Splinter1.dsk)
![Game Screen]({{site.url}}/{{site.baseurl}}/assets/2015-12-12/Images/GameAction.png)
Splinter is a [CoCo 3](http://tandycoco.com/coco3.shtml) [break-the-bricks](https://en.wikipedia.org/wiki/Breakout_(video_game)) video game. Splinter features colorful 320x192 graphics and smooth animation.


## Requirements
Splinter requires the following:


* A Tandy Color Computer 3 or emulator with at least 128K or more RAM
* A Floppy Disk Drive such as the FD-501 or emulator such as the CoCo SDC


## Getting Started
Splinter is supplied on a disk image that is compatible with most CoCo 3 emulators. To play the game, you must transfer the disk image to a physical floppy disk or transfer it to a system such as [DriveWire](https://sites.google.com/site/drivewire4/) or the [CoCo SDC](http://cocosdc.blogspot.com). Please refer to the appropriate system manual for inserting the game disk or image into your system.

Turn on your computer or emulator and insert the disk or image into the system. Type the following at the OK prompt and press the **\[Enter\]** key.

~~~~~~
RUN "*"
~~~~~~

Splinter will load and display an image of a rainbow. Press **\[Spacebar\]** until the rainbow colors look correct and then press the **\[Enter\]** key.
![Rainbow Screen]({{site.url}}/{{site.baseurl}}/assets/2015-12-12/Images/GameRainbow.png)

Splinter will now show its title screen.
![Title Screen]({{site.url}}/{{site.baseurl}}/assets/2015-12-12/Images/GameTitle.png)


## Objective
The objective of Splinter is to break through bricks by hitting a ball towards them with your paddle. You start with 5 balls but will lose one each time you miss it. Beware! The ball speed increases as you break through each screen of bricks.

## Game Play
At the title screen, press any key to start playing Splinter. It will switch to the game screen. You will notice a pink and blue paddle on the left side of the screen. Aim the paddle by pressing the **\[Up\]** and **\[Down\]** arrow keys. The grey ball starts to the right of the paddle followed by rainbow colored bricks. The right side of the screen has the current score and number of remaining balls.
![Game Start Screen]({{site.url}}/{{site.baseurl}}/assets/2015-12-12/Images/GameStart.png)

The score will increase each time a brick is hit. When all bricks on the screen are destroyed, a screen with a new set of bricks will appear. The number of remaining balls will decrease each time you miss the ball. The game ends when no balls remain and a Game Over message will appear. Pressing any key will return to the title screen.
![Game Over]({{site.url}}/{{site.baseurl}}/assets/2015-12-12/Images/GameOver.png)

You may pause an in-progress game at any time by pressing the **\[P\]** key. Press any key to resume.You may also quit an in-progress game by pressing the **\[Break\]** key. Press the **\[Y\]** key to quit the game and the **\[N\]** key to resume game play.
![Game Pause]({{site.url}}/{{site.baseurl}}/assets/2015-12-12/Images/GamePaused.png)
![Game Quit]({{site.url}}/{{site.baseurl}}/assets/2015-12-12/Images/GameQuit.png)


## Background
I developed the original version of Splinter in pure assembly language back in 1988. I revisited this code in 2015 not only to explore the CoCo 3 hardware in greater depth, but also explore how to implement various low-level math routines. The current version of Splinter is the result of these efforts. This source code is available at:
[https://github.com/jamieleecho/CoCoFun](https://github.com/jamieleecho/CoCoFun)


## Acknowledgements
I thank Pierre Sarrazin for his insightful comments and for developing the [CMOC](http://perso.b2b2c.ca/sarrazip/dev/cmoc.html) compiler that was used to develop Splinter.
