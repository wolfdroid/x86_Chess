;Data Segment
DATASEG

;White Sides

	;Loading File
	whitePawn_graph		db	'wPawn.bmp'				,0
	whiteKnight_graph	db	'wKnight.bmp'			,0
	whiteBishop_graph	db	'wBishop.bmp'			,0
	whiteRook_graph		db	'wRook.bmp'				,0
	whiteQueen_graph		db	'wQueen.bmp'			,0
	whiteKing_graph		db	'wKing.bmp'				,0
	
	;For Drawing in the Table
	whiteTable_graph		dw	0
							dw  whitePawn_graph
							dw  whiteKnight_graph
							dw  whiteBishop_graph
							dw  whiteRook_graph
							dw  whiteQueen_graph
							dw  whiteKing_graph

;Black Sides

	;Loading File
	blackPawn_graph		db	'bPawn.bmp'				,0
	blackKnight_graph	db	'bKnight.bmp'			,0
	blackBishop_graph	db	'bBishop.bmp'			,0
	blackRook_graph		db	'bRook.bmp'				,0
	blackQueen_graph		db	'bQueen.bmp'			,0
	blackKing_graph		db	'bKing.bmp'				,0
	;For Drawing in the Table
	blackTable_graph		dw	0
							dw  blackPawn_graph
							dw  blackKnight_graph
							dw  blackBishop_graph
							dw  blackRook_graph
							dw  blackQueen_graph
							dw  blackKing_graph

; Drawing Cube / Board Mate

	cubeColor_graph	db  28 dup(?), 31d, 2 dup(?), 28d
	cubeSize_graph	dw	75d
	marginX_graph	dw	3d
	marginY_graph	dw	1d
	whiteCube_graph	db	31d
	blackCube_graph	db	28d
	markColor_graph	db	33h

;Code Segment
CODESEG
	;Procedure for Graph Initialization
	proc initGraph_graph
		mov ax, 4F02h
		mov bx, 105h ; Resolution : 1024x768
		int 10h
		ret
	endp

	;Procedure for Drawing Board
	proc drawBoard_graph
		mov al, 31d
		xor di, di
		loopX@printBoard:
			mov bx, offset cubeColor_graph
			add bl, al
			mov al, [byte bx]
			xor si, si
			loopY@printBoard:
				mov bx, offset cubeColor_graph
				add bl, al
				mov al, [byte bx]
				drawCube@printBoard:
					call drawCube_graph
					inc si
					cmp si, 8
					jne loopY@printBoard
			inc di
			cmp di, 8
			jne loopX@printBoard
		ret
	endp
	
	;Drawing Mate / Cube Procedure
	proc drawCube_graph
		push si
		push di
		add di, [marginX_graph]
		add si, [marginY_graph]
		mov ah, 0Ch
		mov cx, ax
		mov dx, [cubeSize_graph]
		mov ax, si
		mul dx
		mov si, ax
		mov dx, [cubeSize_graph]
		mov ax, di
		mul dx
		mov di, ax
		mov ax, cx
		mov cx, [cubeSize_graph]
		;Looping Drawing Function
		loopX@drawCube:
			mov dx, [cubeSize_graph]
			loopY@drawCube:
				add dx, si
				add cx, di
				int 10h
				sub dx, si
				sub cx, di
				cmp dx, 0
				dec dx
				jne loopY@drawCube
		loop loopX@drawCube
		pop di
		pop si
		mov [fileHeight_bmp], 76d
		mov [fileWidth_bmp], 76d
		mov bx, si
		shl bx, 3
		add bx, di
		add bx, offset board_en
		mov ah, [bx]
		xor bh, bh
		cmp ah, 0
		je exit@drawCube
		jl whitePiece@drawCube
		;Black Mate
		blackPiece@drawCube:
			mov bl, ah
			shl bl, 1
			mov dx, [word blackTable_graph + bx]
			call drawFile_bmp
			jmp exit@drawCube
		;White Mate
		whitePiece@drawCube:
			neg ah
			mov bl, ah
			shl bl, 1
			mov dx, [word whiteTable_graph + bx]
			call drawFile_bmp
		;Stopping
		exit@drawCube:
			ret
	endp

	;Procedure for marking Cube (Highlight when user choosing)
	proc markCube_graph
		push si
		push di
		add di, [marginX_graph]
		add si, [marginY_graph]
		mov ah, 0Ch
		xor bx, bx
		push ax
		mov dx, [cubeSize_graph]
		mov ax, si
		mul dx
		mov si, ax
		mov dx, [cubeSize_graph]
		mov ax, di
		mul dx
		mov di, ax
		pop ax
		; Top Line
		mov dx, si
		mov cx, [cubeSize_graph]
		loop1@markCube:
			add cx, di
			int 10h
			inc dx
			int 10h
			inc dx
			int 10h
			sub cx, di
			dec dx
			dec dx
			loop loop1@markCube
		; Bottom Line
		mov dx, si
		add dx, 75d
		mov cx, [cubeSize_graph]
		loop2@markCube:
			add cx, di
			int 10h
			dec dx
			int 10h
			sub cx, di
			inc dx
			loop loop2@markCube
		mov cx, di
		mov dx, [cubeSize_graph]
		loop3@markCube:
			add dx, si
			int 10h
			inc cx
			int 10h
			sub dx, si	
			dec cx
			cmp dx, 0
			dec dx
			jne loop3@markCube
		mov cx, di
		add cx, 75d
		mov dx, [cubeSize_graph]
		loop4@markCube:
			add dx, si
			int 10h
			dec cx
			int 10h
			sub dx, si	
			inc cx
			cmp dx, 0
			dec dx
			jne loop4@markCube
		pop di
		pop si
		ret
	endp

	;Getting Color from BMP Procedure
	proc getColor_graph
		mov ax, si
		shr ax, 1
		jc oddRow@getColor
		;Even Color
		evenRow@getColor:
			mov ax, si
			shl ax, 3
			add ax, di
			shr ax, 1
			jc blackCube@evenRow
			whiteCube@evenRow:
				mov al, [whiteCube_graph]
				jmp exit@getColor
			blackCube@evenRow:
				mov al, [blackCube_graph]
				jmp exit@getColor
		;Odd Color
		oddRow@getColor:
			mov ax, si
			shl ax, 3
			add ax, di
			shr ax, 1
			jnc blackCube@evenRow
			whiteCube@oddRow:
				mov al, [whiteCube_graph]
				jmp exit@getColor
			blackCube@oddRow:
				mov al, [blackCube_graph]
				jmp exit@getColor
		exit@getColor:
			ret
	endp

	;Clear Screen Procedure
	proc cleanScreen_graph
		mov ah, 0Ch
		mov cx, 1024d
		loopX@cleanBoard:
			mov dx, 768d
			loopY@cleanBoard:
				int 10h
				cmp dx, 0
				dec dx
				jne loopY@cleanBoard	
		loop loopX@cleanBoard
		ret
	endp