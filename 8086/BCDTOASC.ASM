ASSUME DS:DATA, CS:CODE

DATA SEGMENT
        N1 DW 9032H
        N2 DW 9090H
        MSG DB 'INPUT: $'
        N3 DW 2 DUP(?)
DATA ENDS

CODE SEGMENT
START:
        MOV AX, DATA
        MOV DS, AX
        MOV AH, 09H
        LEA DX, MSG
        INT 21H
        MOV AH, 01H
        INT 21H
        AND AL, 0FH
        MOV DL, N1
        MOV AL, 02H
        INT 21H

        
        
        JMP STOP
        XOR BX, BX
        MOV AX, N1
        ADD AX, N2
        MOV N3, AX
        JNC STOP
        INC BX
STOP:   MOV N3+2, BX
        MOV AH, 4CH
        INT 21H
CODE ENDS
END START

