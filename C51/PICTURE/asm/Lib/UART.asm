UART  SEGMENT  CODE
RSEG  UART

PUBLIC UART_INIT

UART_INIT:
	ORL TMOD,#20H �����ü�����������ʽ2
	MOV SCON,#50H ������Ϊ������ʽ1
	MOV PCON,#80H �������ʼӱ�
	MOV TH1,#0FFH ����������ʼֵ����
	MOV TL1,#0FFH
	SETB ES ���򿪽����ж�
	SETB EA �������ж�
	SETB TR1 ���򿪼�����		
	RET

END