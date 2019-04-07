;Data Segment
DATASEG
	board_en		db	64 dup(0)		;Total cube / mate = 8 x 8
	turn_en			db	-1d				;Turn Function
	sourceAddr_en	dw	0				;Source Memory
	destAddr_en		dw	0				;Destination Address

;Code Segment
CODESEG
;Board Intialization Engine
	proc initBoard_en
		mov cx, 64
		;Make Sure the Tiles / Mate / Cubes is Clear
		cleanBoard:
			mov ax, cx
			dec ax
			mov bx, offset board_en
			add bx, ax
			mov [byte bx], 0
			loop cleanBoard
		mov cx, 8
		;Pawn Initialization
		initPawns:
			mov ax, cx
			dec ax
			mov bx, offset board_en
			add bx, ax
			add bx, 8
			mov [byte bx], 1
			add bx, 40
			mov [byte bx], -1
			loop initPawns
		;Rooks Initialization
		initRooks:
			mov bx, offset board_en
			mov [byte bx], 4
			mov [byte bx+7], 4
			add bx, 56
			mov [byte bx], -4
			mov [byte bx+7], -4
		;Knight Intialization
		initKnights:
			mov bx, offset board_en
			mov [byte bx+1], 2
			mov [byte bx+6], 2
			add bx, 56
			mov [byte bx+1], -2
			mov [byte bx+6], -2
		;Bishop Intialization
		initBishops:
			mov bx, offset board_en
			mov [byte bx+2], 3
			mov [byte bx+5], 3
			add bx, 56
			mov [byte bx+2], -3
			mov [byte bx+5], -3
		;Queen Intialization
		initQueens:
			mov bx, offset board_en
			mov [byte bx+3], 5
			add bx, 56
			mov [byte bx+3], -5
		;King Initialization
		initKings:
			mov bx, offset board_en
			mov [byte bx+4], 6
			add bx, 56
			mov [byte bx+4], -6
		ret
	endp
	
	;Getting Offest for Every Chess Component
	proc getOffset_en
		pop dx
		mov ax, si
		shl ax, 3
		add ax, di
		pop bx
		add bx, ax
		push dx
		ret
	endp

	;Validate Function Engine Procedure : For Piece that want to move
	proc validateSource_en
		cmp [turn_en], 1d
		je blackMove_validateSource
		whiteMove_validateSource:
			cmp [byte bx], 0
			jge invalidMove_validateSource
			jmp validMove_validateSource
		blackMove_validateSource:
			cmp [byte bx], 0
			jle invalidMove_validateSource
			jmp validMove_validateSource
		invalidMove_validateSource:
			stc
			ret
		validMove_validateSource:
			cli
			ret
	endp

	;Validate Destination whether the move is legal or not
	proc validateDest_en
		cmp [turn_en], 1d
		je blackMove_validateDest

		whiteMove_validateDest:
			cmp [byte bx], 0
			jl invalidMove_validateDest
			jmp validMove_validateDest

		blackMove_validateDest:
			cmp [byte bx], 0
			jg invalidMove_validateDest
			jmp validMove_validateDest

		invalidMove_validateDest:
			stc
			ret

		validMove_validateDest:
			cli
			ret
	endp

	;Validate Standard Chess Move
	proc validateMove_en
		pusha
		xor ah, ah
		mov al, [si]
		cmp al, 0
		jg switchMoves_validateMove
		neg al
		switchMoves_validateMove:
			shl ax, 1
			mov bx, ax
			jmp [word jt@switchMoves + bx]
			jt@switchMoves:
				dw ?
				dw pawn@moves
				dw knight@moves
				dw bishop@moves
				dw rook@moves
				dw queen@moves
				dw king@moves
		moves_validateMove:
			;Pawn Moves Function
			pawn@moves:
				xor cx, cx
				cmp [byte si], 0
				jg black@pawn
				;White Pawn Funcion
				white@pawn:
					;Standard Move
					pawn1@white:
						mov bx, si
						sub bx, 8d
						cmp [byte bx], 0
						jne pawn2@white
						push bx
						inc cx
					;First Move
					pawn2@white:
						mov bx, offset board_en
						add bx, 48d
						cmp si, bx
						jb pawn3@white
						mov bx, offset board_en
						add bx, 55d
						cmp si, bx
						ja pawn3@white
						mov bx, si
						sub bx, 16d
						push bx
						inc cx
					;Parsing Eating Move Left
					pawn3@white:
						mov bx, si
						sub bx, 7d
						cmp [byte bx], 0
						je pawn4@white
						push bx
						inc cx
					;Parsing Eating Move Right
					pawn4@white:
						mov bx, si
						sub bx, 9d
						cmp [byte bx], 0
						je pawn5@white
						push bx
						inc cx
					;Move Verification
					pawn5@white:
						jmp isValid_validateMove
				;Black Pawn Function
				black@pawn:
					;Standard Move
					pawn1@black:
						mov bx, si
						add bx, 8d
						cmp [byte bx], 0
						jne pawn2@black
						push bx
						inc cx
					;First Move
					pawn2@black:
						mov bx, offset board_en
						add bx, 8d
						cmp si, bx
						jb pawn3@black
						mov bx, offset board_en
						add bx, 15d
						cmp si, bx
						ja pawn3@black
						mov bx, si
						add bx, 16d
						push bx
						inc cx
					;Standard Eating Move West
					pawn3@black:
						mov bx, si
						add bx, 7d
						cmp [byte bx], 0
						je pawn4@black
						push bx
						inc cx
					;Standard Eating Move East
					pawn4@black:
						mov bx, si
						add bx, 9d
						cmp [byte bx], 0
						je pawn5@black
						push bx
						inc cx
					;Move Verification
					pawn5@black:
						jmp isValid_validateMove
			;Knight Moves Function
			knight@moves:
				xor cx, cx
                    ;Move to north Knight function 1
                    knight1@knight:
                        mov bx, si
                        sub bx, 17d
                        push bx
                        inc cx
                    ;Move to north Knight function 2
                    knight2@knight:
                        mov bx, si
                        sub bx, 15d
                        push bx
                        inc cx
                    ;Move to South Knight Function 1
                    knight3@knight:
                        mov bx, si
                        add bx, 15d
                        push bx
                        inc cx
                    ;Move to South Knight Function 2
                    knight4@knight:
                        mov bx, si
                        add bx, 17d
                        push bx
                        inc cx
                    ;Move to West Knight Function 1
                    knight5@knight:
                        mov bx, si
                        add bx, 6d
                        push bx
                        inc cx
                    ;Move to East Knight Function 1
                    knight6@knight:
                        mov bx, si
                        sub bx, 10d
                        push bx
                        inc cx
                    ;Move to West Knight Function 2
                    knight7@knight:
                        mov bx, si
                        add bx, 10d
                        push bx
                        inc cx
                    ;Move to East Knight Function 2
                    knight8@knight:
                        mov bx, si
                        sub bx, 6d
                        push bx
                        inc cx
                    ;Verification
                    knight9@knight:
                        jmp isValid_validateMove
			;Bishop Move Function
			bishop@moves:
				xor cx, cx
				;Bishop mmove North West
				bishop1@bishop:
					mov bx, si
					mov dx, 7d
					;Bishop can't move North
					loopMoves@bishop1:
						sub bx, 8d
						dec bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne bishop2@bishop
						cmp dx, 0
						dec dx
						jne loopMoves@bishop1
				;Bishop move North East
				bishop2@bishop:
					mov bx, si
					mov dx, 7d
					;Bishop can't move North
					loopMoves@bishop2:
						sub bx, 8d
						inc bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne bishop3@bishop
						cmp dx, 0
						dec dx
						jne loopMoves@bishop2
				;Bishop move South East
				bishop3@bishop:
					mov bx, si
					mov dx, 7d
					;bishop can't move south
					loopMoves@bishop3:
						add bx, 8d
						dec bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne bishop4@bishop
						cmp dx, 0
						dec dx
						jne loopMoves@bishop3
				;Bishop move South East
				bishop4@bishop:
					mov bx, si
					mov dx, 7d
					loopMoves@bishop4:
						add bx, 8d
						inc bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne bishop5@bishop
						cmp dx, 0
						dec dx
						jne loopMoves@bishop4
				;Bishop Move Validation
				bishop5@bishop:
					jmp isValid_validateMove
			;Rook Moves Function
			rook@moves:
				xor cx, cx
				;Rook move North
				rook1@rook:
					mov bx, si
					mov dx, 7d
					;Rook can only move North
					loopMoves@rook1:
						sub bx, 8d
						push bx
						inc cx
						cmp [byte bx], 0
						jne rook2@rook
						cmp dx, 0
						dec dx
						jne loopMoves@rook1
				;Rook move South
				rook2@rook:
					mov bx, si
					mov dx, 7d
					;Rook can only move South
					loopMoves@rook2:
						add bx, 8d
						push bx
						inc cx
						cmp [byte bx], 0
						jne rook3@rook
						cmp dx, 0
						dec dx
						jne loopMoves@rook2
				;Rook move West
				rook3@rook:
					mov bx, si
					mov dx, 7d
					;Rook can only move West
					loopMoves@rook3:
						dec bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne rook4@rook
						cmp dx, 0
						dec dx
						jne loopMoves@rook3
				;Rook move East
				rook4@rook:
					mov bx, si
					mov dx, 7d
					;Rook can only move East
					loopMoves@rook4:
						inc bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne rook5@rook
						cmp dx, 0
						dec dx
						jne loopMoves@rook4
				;Verification
				rook5@rook:
					jmp isValid_validateMove
			;Queen Moves Function
			queen@moves:
				xor cx, cx
				;Queen Move North to left
				queen1@queen:
					mov bx, si
					mov dx, 7d
					;Queen can only move North
					loopMoves@queen1:
						sub bx, 8d
						dec bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne queen2@queen
						cmp dx, 0
						dec dx
						jne loopMoves@queen1
				;Queen Move North to Right
				queen2@queen:
					mov bx, si
					mov dx, 7d
					;Queen can only move North
					loopMoves@queen2:
						sub bx, 8d
						inc bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne queen3@queen
						cmp dx, 0
						dec dx
						jne loopMoves@queen2
				;Queen Move South to left
				queen3@queen:
					mov bx, si
					mov dx, 7d
					;Queen can only move South
					loopMoves@queen3:
						add bx, 8d
						dec bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne queen4@queen
						cmp dx, 0
						dec dx
						jne loopMoves@queen3
				;Queen Move South to Right
				queen4@queen:
					mov bx, si
					mov dx, 7d
					;Queen can only move South
					loopMoves@queen4:
						add bx, 8d
						inc bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne queen5@queen
						cmp dx, 0
						dec dx
						jne loopMoves@queen4
				;Queen Move South to Left
				queen5@queen:
					mov bx, si
					mov dx, 7d
					;Queen can only move South
					loopMoves@queen5:
						sub bx, 8d
						push bx
						inc cx
						cmp [byte bx], 0
						jne queen6@queen
						cmp dx, 0
						dec dx
						jne loopMoves@queen5
				;Queen Move North East Line
				queen6@queen:
					mov bx, si
					mov dx, 7d
					;Queen can only move to North East
					loopMoves@queen6:
						add bx, 8d
						push bx
						inc cx
						cmp [byte bx], 0
						jne queen7@queen
						cmp dx, 0
						dec dx
						jne loopMoves@queen6
				;Queen Move North East Line
				queen7@queen:
					mov bx, si
					mov dx, 7d
					;Looping for Every Line
					loopMoves@queen7:
						dec bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne queen8@queen
						cmp dx, 0
						dec dx
						jne loopMoves@queen7
				;Queen Move North West Line
				queen8@queen:
					mov bx, si
					mov dx, 7d
					;Looping for Every Line
					loopMoves@queen8:
						inc bx
						push bx
						inc cx
						cmp [byte bx], 0
						jne queen9@queen
						cmp dx, 0
						dec dx
						jne loopMoves@queen8
				;Queen Move Validation
				queen9@queen:
					jmp isValid_validateMove

			;King move Function
			king@moves:
				xor cx, cx
				;king move south to Right
				king1@king:
					mov bx, si
					sub bx, 8d
					push bx
					inc cx
				;King move north to Right
				king2@king:
					mov bx, si
					add bx, 8d
					push bx
					inc cx
				;Scanning for every Column to Left
				king3@king:
					mov bx, si
					dec bx
					push bx
					inc cx
				;Scanning for every Column to Right
				king4@king:
					mov bx, si
					inc bx
					push bx
					inc cx
				;King move to North East
				king5@king:
					mov bx, si
					sub bx, 9d
					push bx
					inc cx
				;King Move to North West
				king6@king:
					mov bx, si
					sub bx, 7d
					push bx
					inc cx
				;King move to South East
				king7@king:
					mov bx, si
					add bx, 9d
					push bx
					inc cx
				;King move to South West
				king8@king:
					mov bx, si
					add bx, 7d
					push bx
					inc cx
				;King Move Validation 
				king9@king:
					jmp isValid_validateMove

		;Move Validation Function
		isValid_validateMove:
			cmp cx, 0
			je invalidMove_validateMove
			;Always Checking for the Validation
			loopMoves_isValid:
				dec cx
				pop bx
				cmp bx, di
				je validMove_isValid
				cmp cx, 0
				jne loopMoves_isValid
				jmp invalidMove_validateMove
			validMove_isValid:
				add sp, cx
				add sp, cx
				jmp validMove_validateMove
		;Invalid Move Function
		invalidMove_validateMove:
			popa
			stc
			ret
		validMove_validateMove:
			popa
			cli
			ret
	endp

	;Move Engine Function
	proc move_en
		mov ah, [byte si]
		mov [byte di], ah
		mov [byte si], 0
		ret
	endp