@256
D=A
@SP
M=D
@RETURN1
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
D=M-D
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.init
0;JMP
(RETURN1)
(Sys.init)
@4000
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@THIS
M=D
@5000
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@THAT
M=D
@RETURN2
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
D=M-D
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.main
0;JMP
(RETURN2)
@5
D=A
@1
D=D+A
@addr
M=D
@SP
M=M-1
@SP
A=M
D=M
@addr
A=M
M=D
(Sys.init&LOOP)
@Sys.init&LOOP
0;JMP
(Sys.main)
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@4001
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@THIS
M=D
@5001
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@THAT
M=D
@200
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@1
D=D+A
@addr
M=D
@SP
M=M-1
@SP
A=M
D=M
@addr
A=M
M=D
@40
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@2
D=D+A
@addr
M=D
@SP
M=M-1
@SP
A=M
D=M
@addr
A=M
M=D
@6
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@3
D=D+A
@addr
M=D
@SP
M=M-1
@SP
A=M
D=M
@addr
A=M
M=D
@123
D=A
@SP
A=M
M=D
@SP
M=M+1
@RETURN3
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@1
D=A
@SP
D=M-D
@5
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.add12
0;JMP
(RETURN3)
@5
D=A
@0
D=D+A
@addr
M=D
@SP
M=M-1
@SP
A=M
D=M
@addr
A=M
M=D
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@3
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@4
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
M=D+M
@SP
M=M-1
@SP
A=M
A=A-1
D=M
A=A-1
M=D+M
@SP
M=M-1
@SP
A=M
A=A-1
D=M
A=A-1
M=D+M
@SP
M=M-1
@SP
A=M
A=A-1
D=M
A=A-1
M=D+M
@SP
M=M-1
@LCL
D=M
@endframe
M=D
@5
D=A
@endframe
A=M-D
D=M
@ret
M=D
@SP
M=M-1
@SP
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M
D=D+1
@SP
M=D
@1
D=A
@endframe
A=M-D
D=M
@THAT
M=D
@2
D=A
@endframe
A=M-D
D=M
@THIS
M=D
@3
D=A
@endframe
A=M-D
D=M
@ARG
M=D
@4
D=A
@endframe
A=M-D
D=M
@LCL
M=D
@ret
A=M
0;JMP
(Sys.add12)
@4002
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@THIS
M=D
@5002
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@THAT
M=D
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@12
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
A=M
A=A-1
D=M
A=A-1
M=D+M
@SP
M=M-1
@LCL
D=M
@endframe
M=D
@5
D=A
@endframe
A=M-D
D=M
@ret
M=D
@SP
M=M-1
@SP
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M
D=D+1
@SP
M=D
@1
D=A
@endframe
A=M-D
D=M
@THAT
M=D
@2
D=A
@endframe
A=M-D
D=M
@THIS
M=D
@3
D=A
@endframe
A=M-D
D=M
@ARG
M=D
@4
D=A
@endframe
A=M-D
D=M
@LCL
M=D
@ret
A=M
0;JMP
