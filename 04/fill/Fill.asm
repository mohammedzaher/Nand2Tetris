// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.


(LOOP)
    @KBD  // If KBD==0 goto WHITE
    D=M
    @WHITE
    D;JEQ
    @color  //Black Color
    M=-1
    @START
    0;JMP
    (WHITE) // White Color
    @color
    M=0
(START)
    @SCREEN
    D=A
    @address
    M=D  // address = 16384 (base address og the Hack screen)
(INNERLOOP)
    @address
    D=M
    @24576
    D=D-A
    @LOOP
    D;JEQ // if address == 24576 goto LOOP

    @color
    D=M
    @address
    A=M
    M=D  // RAM[address] = color (16 pixels)

    @address
    M=M+1
    @INNERLOOP
    0;JMP //goto INNERLOOP
