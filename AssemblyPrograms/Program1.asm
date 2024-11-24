@ To calculate (A+B)*C

MOVI R1, 5
MOVI R2, 3
MOVI R3, 4

LD R1
ADD R2
MUL R3
ST 100

@ To do some bitwise operations to ACC and store it at 101 memory address
NOT
LSL 2
LSR 1
ST 101
HLT