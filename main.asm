;Model Declaration
IDEAL
MODEL SMALL                     ;Using Small Model Arc
STACK 100H                      ;Pre order 100H memory STACK
P286                            ;Enable All Command in 80286 with TASM

	;Declaration Library that being used
	include 'graphfun.inc'
	include 'movesfun.inc'
	include 'bmpfun.inc'
	include 'iofun.inc'
	include 'sfx.inc'

;Data Segment
DATASEG
	; Error Messages
	byUser			db	'Program closed by user.$'
	blank 			db	'Error no Output Detected.$'
	;For Acessing Directories
	root@wd			db	'../'			, 0
	pieces@wd		db	'gamefile'		, 0
	;Selection Function
	sourcePos_sel	dd	0
	destPos_sel		dd	0
	step_sel		dw	0
	; Winning Messages
	white@win		db 'White won!$'
	black@win		db 'Black won!$'

;Code Segment
CODESEG
	;Start Function
	START:
		;Intializing
		mov ax, @data
		mov ds, ax
		;Setting Work Directories
		mov ah, 3Bh
		mov dx, offset pieces@wd
		int 21h
		;Call initialization of GUI
		call initBoard_en
		call initGraph_graph
		mov al, 14h
		call cleanScreen_graph
		call initsound
		call drawBoard_graph
		mov di, 7
		mov si, 7
		mov al, [markColor_graph]
		call markCube_graph
		mov [markerCol_io], di
		mov [markerRow_io], si
		;Game Start
		game@start:
			mov [step_sel], 0
			;Getting Source Fucntion
			getSource@game:
				call getData_io
				push si
				push di
				inc [step_sel]
				push offset board_en
				call getOffset_en
				mov [sourceAddr_en], bx
				call validateSource_en
				jc invalid@game
			;Getting Destination Functio
			getDest@game:
				call getData_io
				push si
				push di
				inc [step_sel]
				push offset board_en
				call getOffset_en
				mov [destAddr_en], bx
				call validateDest_en
				jc invalid@game
			mov si, [sourceAddr_en]
			mov di, [destAddr_en]
			call validateMove_en
			jc invalid@game
			cmp [byte di], 6
			je white_won@game
			cmp [byte di], -6
			je black_won@game
			call move_en
			mov cx, [step_sel]
            ;Always Updating Mark
			updateBoard@game:
				pop di
				pop si
				push cx
				call getColor_graph
				call drawCube_graph
				pop cx
				loop updateBoard@game
			;Invalid Game Checking
			updateMark@game:
				mov di, [markerCol_io]
				mov si, [markerRow_io]
				mov al, [markColor_graph]
				call markCube_graph
			neg [turn_en]
			jmp game@start
			;Invalid Game Checking
			invalid@game:
				add sp, [step_sel]
				add sp, [step_sel]
				mov di, [markerCol_io]
				mov si, [markerRow_io]
				mov al, 0Ch
				call markCube_graph
				jmp game@start
			;White Won Function
			white_won@game:
				mov dx, offset white@win
				jmp exit_msg
			;Black Won Function
			black_won@game:
				mov dx, offset black@win
				jmp exit_msg
		;Exit Function
		EXIT:
			mov dx, offset blank 
		exit_msg:
			; Flush io buffer
			mov ah, 0Ch
			int 21h
			; Text Mode
			mov ax, 2h
			int 10h
			; Error Msg
			mov ah, 9h
			int 21h
			; Restore WD
			mov ah, 3Bh
			mov dx, offset root@wd
			int 21h
			; Terminate Program
			mov ax, 4c00h
			int 21h

	END START
