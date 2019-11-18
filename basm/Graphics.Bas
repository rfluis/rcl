'--------------------------------------------------------------------------
'         This Library designed for the BASM Compiler Version 2.0
'--------------------------------------------------------------------------
' SUB Gtext       (X,Y,A$,FG,BG)
' SUB DrawEGAChar  (X,Y,A$,Color)
' SUB RecessFrame  (X1,Y1,X2,Y2)
' SUB RaisedFrame  (X1,Y1,X2,Y2)
' SUB SolidFrame   (X1,Y1,X2,Y2,Color)
' SUB Frame3D      (X1,Y1,X2,Y2)
' SUB FillBox      (X1,Y1,X2,Y2,Color)
' SUB DrawButton   (X,Y,A$)
' FUNCTION Button  (X,Y,Button$)
'-------------------------------------------------------------------------


SUB Gtext (XX,YY,A$,FG,BG)
'---------------------------------------------
'Draws a string of characters in graphics mode
'** Note ** XX,YY are pixel coordinates
'---------------------------------------------
LOCAL A,X,Y,LenA,B$
X = XX
Y = YY
LenA = LEN(A$)

REPEAT LenA
  Incr A
  B$ = MID$(A$,A,1)
  DrawEgaChar(X,Y,B$,FG,BG)
  INCR X,8
END REPEAT
END SUB




SUB DrawEGAChar (XX,YY,A$,FG,BG)
'--------------------------------------
'This routine draws a single character
'** Note ** XX,YY are pixel coordinates
'--------------------------------------
LOCAL A,B,C,D,E,X,Y,OFS1,OFS2,B$

X  =  XX
Y  =  YY
B$ =  A$
A  =  ASC(B$)

DEF SEG  = &HF000       'point to rom character set segment
OFS1 = A * 8 + &hFA6D

FOR B = 1 TO 8
  OFS2 = OFS1 + B
  C = PEEK(OFS2)
  IF C = 0 THEN
    X = XX
    INCR Y
  ELSE
    FOR D = 7 TO 0 STEP -1
      E = Pow (2,D)
      IF C < E THEN
        IF FG = 1 THEN
    PSET (X,Y),FG
        ELSE
    PSET (X,Y),BG
        END IF
        INCR X
      ELSE
        IF FG = 1 THEN
    PSET (X,Y),BG
        ELSE
    PSET (X,Y),FG
        END IF
        INCR X
        E = Pow (2,D)
        C = C - E
      END IF
    NEXT
    INCR Y
    X = XX
  END IF
NEXT
END SUB




SUB RecessFrame (X1,Y1,X2,Y2)
'---------------------------------------
'This routine draws a recessed rectangle
'in the selected graphics mode.  It uses
'black and white colors for the frame
'---------------------------------------
LOCAL XX
LINE (X1,Y1,X2,Y1),0
XX = X1 + 1
LINE (XX,Y1,XX,Y2),0
LINE (X1,Y1,X1,Y2),0
LINE (X2,Y1,X2,Y2),8
XX = X2 - 1
LINE (XX,Y1,XX,Y2),15
LINE (X1,Y2,X2,Y2),15
END SUB




SUB SolidFrame (X1,Y1,X2,Y2,Kol)
'----------------------------------------
'This routine draws a hollow rectangle in
'the selected graphics mode.  It uses the
'parameter KOL for the color of the frame
'----------------------------------------
Line X1,Y1,X2,Y1,Kol
Line X1,Y1,X1,Y2,Kol
Line X2,Y1,X2,Y2,Kol
Line X1,Y2,X2,Y2,Kol
END SUB



SUB RaisedFrame (X1,Y1,X2,Y2)
'---------------------------------------
'This routine draws an embossed rectangle
'in the selected graphics mode.  It uses
'black and white colors for the frame
'---------------------------------------
LOCAL XX
LINE (X1,Y1,X2,Y1),15
XX = X1 + 1
LINE (XX,Y1,XX,Y2),15
LINE (X1,Y1,X1,Y2),15
LINE (X2,Y1,X2,Y2),0
XX = X2 - 1
LINE (XX,Y1,XX,Y2),0
LINE (X1,Y2,X2,Y2),0
END SUB


SUB Frame3D (X1,Y1,X2,Y2)
LOCAL XX1,XX2,YY1,YY2
'-----------------------------------------------------------------------
'Routine draws a 3-D rectangle in the current graphics mode. It's setup
'to use black & white for the frame and looks best on a gray background
'-----------------------------------------------------------------------
'First the outside bevel
'------------------------
Line X1,Y1,X2,Y1,15
Line X1,Y1,X1,Y2,15
Line X2,Y1,X2,Y2,0
Line X1,Y2,X2,Y2,0
'-------------------------
'Now draw the inside bevel
'--------------------------
XX1 = X1 + 5
XX2 = X2 - 5
YY1 = Y1 + 5
YY2 = Y2 - 5
Line XX1,YY1,XX2,YY1,0
Line XX1,YY1,XX1,YY2,0
Line XX2,YY1,XX2,YY2,15
Line XX1,YY2,XX2,YY2,15
END SUB



SUB FillBox(X1,Y1,X2,Y2,Kol)
'----------------------------------------
'This routine draws a solid rectangle in
'the selected graphics mode.  It uses the
'parameter KOL for the color of the frame
'----------------------------------------
LOCAL A
FOR A = Y1 to Y2
  LINE (X1,A,X2,A,Kol)
NEXT
END SUB





FUNCTION Button(X,Y,But$)
'------------------------------------------------------------
'This routine controls the graphical behavior of a previously
'drawn button.  It returns either TRUE or FALSE depending if
'the left mouse button was released on the face of the button
'-------------------------------------------------------------

LOCAL M1,M2,State
LOCAL X1,Y1,X2,Y2
LOCAL Facex,Facey

FaceX = X + 9          'Top Left Corner of Button Face When depressed
FaceY = Y + 8          '                ditto

X1 = LEN(But$) * 8 + X + 16
X2 = LEN(But$) * 8 + X + 10

Y1 = Y + 18
Y2 = Y + 16

M1 = MouseAt(X,Y,X2,Y2)
M2 = MousePressLeft

IF NOT M1 THEN                     'If the cursor is not on the face
  Button = 0                       'of the Button then do a quick exit
  EXIT FUNCTION                    'Remember to clear the function or
END IF                             'AX may have garbage in it!

IF NOT M2 THEN                     'If the left mouse Button is not pressed
  Button = 0                       'then do a quick exit.
  EXIT FUNCTION
END IF

IF M1 THEN                         'Is the cursor in the Button face?
  IF M2 THEN                       'And is the left mouse Button pressed?
    State = -1                     'we have pressed the left button

    MouseOff
    FillBox x,y,x1,y1,7            'clear the button face
    Gtext(FaceX,Facey,But$,0,7)   'and scoot the button text down & right
    RecessFrame (X,Y,X1,Y1)        'so we alter the appearance of the button
    MouseOn

    WHILE M2                       'while the left Button is down
      M2 = MousePressLeft
      M1 = Mouseat(X,Y,X2,Y2)
      IF NOT M1 THEN               'If we're not on the Button face...

  MouseOff
        Decr Facex,2               'adjust Button Text Coordinates to normal
        Decr Facey,2
        FillBox x,y,x1,y1,7        'and repaint the button face
  Gtext(FaceX,Facey,But$,0,7)
        RaisedFrame (X,Y,X1,Y1)    'restore the un-pushed appearance
        MouseOn

  Button = 0                 'We've moved off the button
        EXIT Function              'so break out of the loop!
      END IF
    WEND

'--------------------------------------------------------------------
'otherwise we've released the button.  See if we're still on the face
'--------------------------------------------------------------------

    M1 = MouseAt(X,Y,X2,Y2)

    IF M1  THEN
      State = -1            'yes, we're still on it
    ELSE
      State = 0             'no, we've moved off
    END IF

    MouseOff
    Decr Facex,2               'adjust Button Text Coordinates to normal
    Decr Facey,2
    FillBox x,y,x1,y1,7        'and repaint the button face
    Gtext(FaceX,Facey,But$,0,7)
    RaisedFrame (X,Y,X1,Y1)    'restore the un-pushed appearance
    MouseOn

  END IF
END IF

Button = State
END FUNCTION




SUB DrawButton(X,Y,A$)
'------------------------------------------------------------
'This routine draws the initial face of a button.  Use the
'Button Function to control the behavior of the button.
'-------------------------------------------------------------
LOCAL Pixel,XX,YY,X1,Y1,X2,Y2

Pixel = LEN(A$) * 8
XX = X + Pixel + 16
YY = Y + 18

X1 = X  - 2
Y1 = Y  - 1
X2 = XX + 1
Y2 = YY + 1

Fillbox     (X,Y,XX,YY,7)
RaisedFrame (X,Y,XX,YY)
SolidFrame  (X1,Y1,X2,Y2,0)

XX = X + 8
YY = Y + 6
Gtext(XX,YY,A$,0,7)
END SUB
