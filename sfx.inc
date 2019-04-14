DATASEG
	clock equ es:6Ch 	;Clock Declaration
	STOR dw ?		    ;for Storing Temporary Value

CODESEG
	PROC initsound
		CALL MREE
		CALL delay
		CALL delay
		CALL end_sound
		CALL delay

		CALL MREE
		CALL delay
		CALL delay
		CALL end_sound
		CALL delay

		CALL MREE
		CALL delay
		CALL delay
		CALL end_sound
		CALL delay

		CALL MMII
		CALL delay
		CALL delay
		CALL end_sound

		CALL MREE
		CALL delay
		CALL delay
		CALL end_sound

		CALL MLAA
		CALL delay
		CALL delay
		CALL delay
		CALL end_sound

		RET
	ENDP 

;Notes Declaration
PROC MDOO
	MOV     AX, 2415       
	MOV     [STOR], AX
	CALL    SOUNDER         
	RET
ENDP 

;Re note	
PROC MREE	
	MOV     AX, 2280       
	MOV     [STOR], AX
	CALL    SOUNDER         
	RET
ENDP

;MI note
PROC MMII 
	MOV     AX, 2031      
	MOV     [STOR], AX
	CALL    SOUNDER         
	RET
ENDP

;FA note		
PROC MFAA
    MOV     AX, 1809       
    MOV     [STOR], AX
    CALL    SOUNDER         
    RET
ENDP 

;SOL note				
PROC MSOOL 
    MOV     AX, 1715       
    MOV     [STOR], AX
    CALL    SOUNDER         
    RET
ENDP 
				
;LA note				
PROC MLAA
    MOV     AX, 1521       
    MOV     [STOR], AX
    CALL    SOUNDER        
    RET
ENDP 

;SI note		
PROC MSII
    MOV     AX, 1355       
    MOV     [STOR], AX
    CALL    SOUNDER        
    RET
ENDP 

;Delay Function
PROC delay
    push ax 
    mov ax,40h 
    mov es,ax 
    mov ax,[clock] 
    ;first knock
    first:
    cmp ax, [clock]
    mov cx, 2
    je first
    ;looping func
    Loopdelay:
    mov ax, [clock]
    knock:
    cmp ax,[clock]
    je knock
    loop Loopdelay
    pop ax
    RET
ENDP

;Speaker Initialization
PROC sounder
    push ax
    in al, 61h
    or al, 00000011b 
    out 61h, al 
    mov al, 0B6h
    out 43h, al
    mov ax, [STOR] 
    out 42h, al 
    mov al, ah
    out 42h, al 
    pop ax
    RET
ENDP

;Ending Note Function
PROC end_sound
    in al,61h
    and al, 11111100b 
    out 61h, al
    RET
ENDP
