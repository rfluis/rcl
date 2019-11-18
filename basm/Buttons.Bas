$Com                           'Generate code which produces a .COM file
$Source On                     'Don't include source lines from libraries
$Include "Mouse.bas"           'Include some mouse routines
$Include "Graphics.bas"        'Include some graphics routines
$Source On                     'Include source lines in the MAIN program

Check_Hardware                 'Check for presence of a mouse and color video

SCREEN 12                      'Switch to 640x350x16 vga graphics mode
COLOR 0,7                      'set background color to gray
CLS                            'clear the screen

FillBox    0,0,639,13,1        'Draw a blue band across the top of screen

DrawButton 005,020,"Color 1"
DrawButton 095,020,"Color 2"
DrawButton 185,020,"Color 3"
DrawButton 275,020,"Color 4"
DrawButton 365,020,"Color 5"
DrawButton 455,020,"Color 6"
DrawButton 545,020,"  Quit  "

Frame3d   3,50,636,320

Gtext 240,  3,"BASM does Buttons!",15,1
Gtext  70, 90,"Press the left mouse button on any of the buttons in any order.",0,7
Gtext  20,110,"Also try pressing and holding the left button and drag it over the buttons.",0,7
Gtext  60,160,"This program is less than 4k in size.  Can your compiler do that?",4,7
Gtext 200,300,"It could if you were using BASM!",14,7
Gtext  60,330,"This demo copyright (c) 1995 by Kevin T. Diggins, creator of BASM.",0,7

MouseOn

DO
  A = Button 005,020,"Color 1"
  B = Button 095,020,"Color 2"
  C = Button 185,020,"Color 3"
  D = Button 275,020,"Color 4"
  E = Button 365,020,"Color 5"
  F = Button 455,020,"Color 6"
  G = Button 545,020,"  Quit  "

  IF A THEN FillBox (250,210,400,250,1)
  IF B THEN FillBox (250,210,400,250,2)
  IF C THEN FillBox (250,210,400,250,3)
  IF D THEN FillBox (250,210,400,250,4)
  IF E THEN FillBox (250,210,400,250,5)
  IF F THEN FillBox (250,210,400,250,6)
  IF G THEN Terminate

LOOP



SUB Terminate
SCREEN 0
END
END SUB




SUB Check_Hardware
LOCAL a

A = Mouse

IF NOT A THEN
  PRINT "Mouse driver not found."
  END
END IF

A = VIDEOCARD

IF NOT A THEN
  PRINT "This demo requires at least an EGA video display"
  END
END IF

END SUB
