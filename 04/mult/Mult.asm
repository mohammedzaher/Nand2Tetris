// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Computing the Mult of two numbers
    @i  // i=1
    M=1
    @result  // result=0
    M=0
(LOOP)
    @i // If (i-R1==0) goto END
    D=M
    @R1
    D=D-M
    @END
    D;JGT
    @R0 // result+=R0
    D=M
    @result
    M=D+M
    @i
    M=M+1
    @LOOP // goto LOOP
    0;JMP
(END)
    @result
    D=M
    @R2
    M=D
    @END
    0;JMP