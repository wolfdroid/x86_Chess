DATASEG

 clock equ es:6Ch ;clock pada es
 STOR dw ? ;deklarasi variabel tone yang kosong

CODESEG

proc initendingsong
	   
		call MREE
        call delay
        call delay
        call matisuara 
        
        call MLAA
        call delay
        call delay
        call matisuara  
        
        call MFAA
        call delay
        call delay
        call delay
        call delay
        call matisuara
        
        call MREE
        call delay
        call delay
        call matisuara 
        
        call MLAA
        call delay
        call delay
        call matisuara  
        
        call MFAA
        call delay
        call delay
        call delay
        call delay
        call matisuara
    
        call MSII
        call delay
        call delay
        call delay
        call matisuara           
        
        call MLAA
        call delay
        call matisuara 
        
        call MSOOL
        call delay
        call delay
        call matisuara
        
        call MFAA
        call delay
        call delay
        call matisuara
        
        call MMII
        call delay
        call delay
        call matisuara
        
        call MFAA
        call delay
        call delay
        call matisuara
        
        call MSOOL
        call delay
        call delay
        call delay
        call delay
        call matisuara
		
	ret
endp initendingsong

proc initsound
	call MREE
	call delay
	call delay
	call matisuara
	call delay
	
	CALL MREE
	call delay
	call delay
	call matisuara
	call delay
	
	CALL MREE
	call delay
	call delay
	call matisuara
	call delay
	
	CALL MMII
	call delay
	call delay
	call matisuara
	
	CALL MREE
	call delay
	call delay
	call matisuara
	
	CALL MLAA
	call delay
	call delay
	call delay
	call matisuara
	
	ret
endp 


		
		proc MDOO
					MOV     AX, 2415       
					MOV     [STOR], AX
					CALL    SOUNDER         ;PUTAR NADA
					ret
				endp 
	

		proc MREE	
					MOV     AX, 2280       
					MOV     [STOR], AX
					CALL    SOUNDER         ;PUTAR NADA
					ret
				endp 

		proc MMII 
					MOV     AX, 2031      
					MOV     [STOR], AX
					CALL    SOUNDER         ;PUTAR NADA
					ret
				endp 
		
		proc MFAA
					MOV     AX, 1809       
					MOV     [STOR], AX
					CALL    SOUNDER         ;PUTAR NADA
					ret
				endp 
				
		proc MSOOL 
					MOV     AX, 1715       
					MOV     [STOR], AX
					CALL    SOUNDER         ;PUTAR NADA
					ret
				endp 
				
				
		proc MLAA
					MOV     AX, 1521       
					MOV     [STOR], AX
					CALL    SOUNDER         ;PUTAR NADA
					ret
				endp 
		
		proc MSII
					MOV     AX, 1355       
					MOV     [STOR], AX
					CALL    SOUNDER         ;PUTAR NADA
					ret
				endp 

proc delay
 push ax 
 mov ax,40h 
 mov es,ax 
 mov ax,[clock] 
 
 Ketukawal:
 cmp ax, [clock]
 mov cx, 2
 je Ketukawal

 Loopdelay:
 mov ax, [clock]
 ketuk:
 cmp ax,[clock]
 je ketuk
 loop Loopdelay
 pop ax
ret
endp

proc sounder
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
 ret
endp

proc matisuara
 in al,61h
 and al, 11111100b 
 out 61h, al
 ret
endp
	
		


				

	
	
