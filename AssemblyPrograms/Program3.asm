@Calculates factorial using stack

MOVI R1, 5
LD R1
CALL factorial

. factorial
CMP
JEQ BASE
JGT RECURSE
JMP BASE

. RECURSE
PUSH
LD RA
PUSH
LD R1
MOVI R2, 1
SUB R2
CALL factorial
POP
MOV RA, ACC
POP
MUL R3
MOV R3, ACC
RET

. BASE
MOVI R3, 1
RET


