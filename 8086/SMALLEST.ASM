ASSUME CS:CODE, DS:DATA
DATA SEGMENT
        N1 DW 100H, 59H, 29H, 66H
        COUNT DW 04H
        N2 DW 1 DUP(?)
        N3 DW 1 DUP(?)
DATA ENDS
CODE SEGMENT
START:  MOV AX, DATA
        MOV DS, AX
        MOV SI, OFFSET N1
        MOV DX, [SI]
        MOV CX, COUNT
        DEC CX
NEXT:   ADD SI, 2
        CMP [SI], DX
        JA NOSWAP
        MOV DX, [SI]
NOSWAP: LOOP NEXT
        MOV N2, DX
        MOV AX, N2
        MOV BX, 02H
        DIV BX
		MOV N3, AH
        XOR AX, AX
        MOV AH, 4CH
        INT 21H

CODE ENDS
END START
