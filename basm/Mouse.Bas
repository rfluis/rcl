'===========================================================================
'                 This library is designed for the BASM compiler
'===========================================================================



FUNCTION Mouse
'--------------------------------------------------
'Initializes the mouse and returns true if present
'--------------------------------------------------
$ASM
MOV AX,0
INT 33H
$ASM
END FUNCTION




SUB MouseOn
ASM MOV AX,1
ASM INT 33H
END SUB



SUB MouseOff
'---------------------------------------------
' Note:  Each call to this routine must be
' complemented by a call to MouseOn.  This
' means if 3 calls are made to this routine
' then 3 calls must be made to MouseOn before
' the mouse pointer is made visible.
'---------------------------------------------
ASM MOV AX,2
ASM INT 33H
END SUB


FUNCTION MousePressLeft
$ASM
MOV AX,5
MOV BX,0
INT 33H
CMP AX,1
JE MPLTRUE
XOR AX,AX
JMP MPLQUIT
MPLTRUE:
MOV AX,-1
$ASM
MPLQUIT:
END FUNCTION



FUNCTION MousePressRight
ASM MOV AX,5
ASM MOV BX,0
ASM INT 33H
ASM CMP AX,2
ASM JE MPRTRUE
ASM XOR AX,AX
ASM JMP MPRQUIT
MPRTRUE:
ASM MOV AX,-1
MPRQUIT:
END FUNCTION



FUNCTION MouseGetX
ASM MOV AX,3
ASM INT 33H
ASM MOV AX,CX
END FUNCTION


FUNCTION MouseGetY
ASM MOV AX,3
ASM INT 33H
ASM MOV AX,DX
END FUNCTION


SUB MouseSetXY (x,y)
ASM MOV CX,[BP+4]
ASM MOV DX,[BP+6]
ASM MOV AX,4
ASM INT 33H
END SUB



FUNCTION MouseAt (X1,Y1,X2,Y2)
LOCAL XX,YY
'-----------------------------------------------------------------------
' Returns true if mouse is in the rectangle x1,y1,x2,y2 in graphics mode
'-----------------------------------------------------------------------

XX = MouseGetX
YY = MouseGetY

IF XX < X1 THEN
  MouseAt = 0
  Exit Function
END IF

IF XX > X2 THEN
  MouseAt = 0
  Exit Function
END IF

IF YY < Y1 THEN
  MouseAt = 0
  Exit Function
END IF

IF YY > Y2 THEN
  MouseAt = 0
  Exit Function
END IF

MouseAt = 1
END FUNCTION
