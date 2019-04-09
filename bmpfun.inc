;Data Segment
DATASEG
	fileHeight_bmp		dw	76d						;BMP File width 76 
	fileWidth_bmp		dw	76d						;BMP File height 76
	cordX_bmp			dw	0					;Temporary Var X
	cordY_bmp			dw	0					;Temporary Var Y
	errorMsg_bmp		db	'Could not open file ERROR!!!! $'		;Error Messages
	fileHandle_bmp		dw	0						;Reserve file handle
	rowBuffer_bmp		db	1024 dup(0)					;Reserving Data for Row
	headerBuffer_bmp	db	54 	dup(0)					;Reserving Data for Header

;Code Segment
CODESEG
	;Drawing BMP File
	proc drawFile_bmp
		pusha
		add di, [marginX_graph]
		add si, [marginY_graph]
		push si ; For Row
		push di ; For Col
		call getHandle_bmp
		call gotoData_bmp
		pop di
		pop si
		call loadBitmap_bmp
		call closeFile_bmp
		popa
		ret
	endp
	;Handling BMP File
	proc getHandle_bmp
		mov ax, 3D00h
		int 21h
		call isFileError_bmp
		mov [fileHandle_bmp], ax
		ret
	endp
	;Closing BMP File
	proc closeFile_bmp
		mov ah, 3Eh
		mov bx, [fileHandle_bmp]
		call isFileError_bmp
		int 21h
		ret
	endp
	;Read the header
	proc gotoHeader_bmp
		mov ax, 4200h
		mov bx, [fileHandle_bmp]
		xor cx, cx
		xor dx, dx
		int 21h
		call isFileError_bmp
		ret
	endp
	;Read the data from datas Location
	proc gotoData_bmp
		call gotoHeader_bmp
		mov ax, 3F00h
		mov cx, 54d
		mov dx, offset headerBuffer_bmp
		int 21h
		call isFileError_bmp
		mov bx, offset headerBuffer_bmp
		mov dx, [bx+10d]
		mov cx, [bx+12d]
		mov ax, 4200h
		mov bx, [fileHandle_bmp]
		int 21h
		ret
	endp
	;Loading Bitmap Function
	proc loadBitmap_bmp	
		mov dx, [cubeSize_graph]
		mov ax, si
		mul dx
		mov [cordY_bmp], ax
		mov dx, [cubeSize_graph]
		mov ax, di
		mul dx
		mov [cordX_bmp], ax
		mov si, [fileHeight_bmp]
		;Loading Bitmap Y axis
		loopY@loadBitmap:
			mov bx, [fileHandle_bmp]
			mov ah, 3Fh
			mov cx, [fileWidth_bmp]
			mov dx, offset rowBuffer_bmp
			int 21h
			mov di, [fileWidth_bmp]
			dec di
			;Loading Bitmap X axis
			loopX@loadBitmap:
				mov bx, offset rowBuffer_bmp
				add bx, di
				mov al, [byte bx]
				cmp al, 0FFh
				je alphaColor@loopX
				mov ah, 0Ch
				xor bl, bl
				mov cx, di
				add cx, [cordX_bmp]
				mov dx, si
				add dx, [cordY_bmp]
				int 10h
				;Getting Alpha Color
				alphaColor@loopX:
					cmp di, 0
					dec di
					jne loopX@loadBitmap
			cmp si, 0
			dec si
			jne loopY@loadBitmap
		ret
	endp
	;File Error Function
	proc isFileError_bmp
		jc fileError@isFileError
		ret
		;File Error Looping Check
		fileError@isFileError:
			mov ah, 9h
			mov dx, offset errorMsg_bmp
			int 21h
			mov ah, 2h
			mov dl, al
			add dl, 48d
			int 21h
			mov ax, 4c00h
			int 21h
	endp
