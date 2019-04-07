;Data Segment
DATASEG
	;For Buffer
	buffer_io		db	0
	;For Marker
	markerCol_io	dw	0
	markerRow_io	dw	0

CODESEG
	;Getting IO function
	proc getData_io
		mov ah, 0h
		int 16h
		cmp ah, 20h
		ja undefinedKey@getData
		xor bh, bh
		mov bl, ah
		shl bx, 1
		jmp [word binds_io + bx]
		;User Inputing Undefined Key
		undefinedKey@getData:
			jmp getData_io
		;Key Bindings
		binds_io:
			dw undefinedKey@getData 		 ; UNDEFINED
			dw quit@userAction				 ; Esc[1d]
			dw 15d dup(undefinedKey@getData) ; UNDEFINED
			dw markerUp@userAction 			 ; W[17d]
			dw 10d dup(undefinedKey@getData) ; UNDEFINED
			dw submit@userAction 			 ; Enter[28d]
			dw undefinedKey@getData 		 ; UNDEFINED
			dw markerLeft@userAction 		 ; A[30d]
			dw markerDown@userAction 		 ; S[31d]
			dw markerRight@userAction 		 ; D[32d]

	endp
	;Reading User Input
	proc userAction
		;User Finished Input
		quit@userAction:
			mov dx, offset byUser 
			jmp exit_msg
		;Submiting User Input
		submit@userAction:
			ret
		;Markering Up User Action
		markerUp@userAction:
			mov si, [markerRow_io]
			mov di, [markerCol_io]
			cmp si, 0d
			je exit@markerUp
			;Markering by Blue Square
			decRow@markerUp:
				call getColor_graph
				call markCube_graph
				dec si
				mov al, [markColor_graph]
				call markCube_graph
				mov [markerCol_io], di
				mov [markerRow_io], si
			;Finished Markering
			exit@markerUp:
				jmp getData_io
		;Markering Down User Action
		markerDown@userAction:
			mov si, [markerRow_io]
			mov di, [markerCol_io]
			cmp si, 7d
			je exit@markerDown
			;Markering by Red Square
			incRow@markerDown:
				call getColor_graph
				call markCube_graph
				inc si
				mov al, [markColor_graph]
				call markCube_graph
				mov [markerCol_io], di
				mov [markerRow_io], si
			;Finished Markering
			exit@markerDown:
				jmp getData_io
		;Markering user action that to the right
		markerRight@userAction:
			mov si, [markerRow_io]
			mov di, [markerCol_io]
			cmp di, 7d
			je incRow@markerRight
			;For Every Columns
			incCol@markerRight:
				call getColor_graph
				call markCube_graph
				inc di
				mov al, [markColor_graph]
				call markCube_graph
				mov [markerCol_io], di
				jmp exit@markerRight	
			;For Every Rows
			incRow@markerRight:
				cmp si, 7d
				je exit@markerRight
				call getColor_graph
				call markCube_graph
				inc si
				xor di, di
				mov al, [markColor_graph]
				call markCube_graph
				mov [markerCol_io], di
				mov [markerRow_io], si
				jmp exit@markerRight
			;Exiting
			exit@markerRight:
				jmp getData_io
		;Markering Action to the Left
		markerLeft@userAction:
			mov si, [markerRow_io]
			mov di, [markerCol_io]
			cmp di, 0d
			je decRow@markerLeft
			;For Every Columns
			decCol@markerLeft:
				call getColor_graph
				call markCube_graph
				dec di
				mov al, [markColor_graph]
				call markCube_graph
				mov [markerCol_io], di
				jmp exit@markerLeft
			;For Every Rows
			decRow@markerLeft:
				cmp si, 0d
				je exit@markerLeft
				call getColor_graph
				call markCube_graph
				dec si
				mov di, 7d
				mov al, [markColor_graph]
				call markCube_graph
				mov [markerCol_io], di
				mov [markerRow_io], si
				jmp exit@markerLeft
			;Exiting Function
			exit@markerLeft:
				jmp getData_io
	endp