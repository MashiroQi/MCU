/*
	作者：MashiroQi
	配合Python脚本使用
	还不是很完善
	之后会继续更新
*/

REC EQU R1
ROW EQU R2 
COL EQU R3
CSEG AT 0000H
ORG 0000H
JMP START

ORG 0023H  ;串口中断函数

	MOV REC,SBUF
	CLR RI
	MOV A,ROW
	XRL A,#64
	JZ NEXTPICTURE
	BACK1:
	MOV A,COL
	XRL A,#16
	JZ NEXTROW 
	BACK2:
	INC COL
	MOV A,REC
	CALL WDAT
	;测试用
	/*
	MOV SBUF,REC
	JNB TI,$
	CLR TI
	*/
	RETI

NEXTPICTURE:
	MOV ROW,#00H
	MOV A,#36H
	CALL WCMD
	JMP BACK1

NEXTROW:
	MOV COL,#00H
	INC ROW
	MOV A,ROW
	MOV B,#32
	DIV AB
	JZ AREA2
	JMP AREA1
	BACK3:
	JMP BACK2

AREA1:
	MOV A,ROW
	ADD A,#80H
	SUBB A,#32
	CALL WCMD
	MOV A,#88H
	CALL WCMD
	JMP BACK3
AREA2:
	MOV A,ROW
	ADD A,#80H
	CALL WCMD
	MOV A,#80H
	CALL WCMD
	JMP BACK3
	
ORG 0080H

EXTRN CODE (INIT,WDAT,WCMD)
EXTRN CODE (UART_INIT)
EXTRN CODE (DELAY)

START:
	
	MOV REC,#0
	MOV ROW,#0
	MOV COL,#0
	
	CALL INIT
	CALL UART_INIT
	
	MOV A,#80H
	CALL WCMD
	MOV A,#50
	CALL DELAY
	MOV A,#80H
	CALL WCMD
	MOV A,#50
	CALL DELAY
	
LOOP:
	NOP
	JMP LOOP
		
		
END