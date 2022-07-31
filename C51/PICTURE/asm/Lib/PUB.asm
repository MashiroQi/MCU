PUB  SEGMENT  CODE
RSEG  PUB

PUBLIC DELAY

;A中数据为计数时间,占用R0,R5
DELAY: 
	   WAIT3:
	   MOV R0,#221   
	   WAIT2:
	   MOV R5,#100
	   WAIT1: 
	   DJNZ R5,WAIT1
	   
	   DJNZ R0,WAIT2
       DEC A
       JNZ WAIT3        
       RET              
END