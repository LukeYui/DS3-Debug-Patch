EXTERN bDS3DebugMenuPrint:QWORD
EXTERN fDS3DebugMenuPrint:PROC

EXTERN bDS3DebugGUIPrint:QWORD
EXTERN fDS3DebugGUIPrint:PROC

.data

.code

tDS3DebugMenuPrint PROC

	mov edx,[rbx]
	mov rcx,rsi
	mov [rax],edx
	sub rsp,088h
	call _tDS3DebugMenuPrint
	add rsp,088h

	Return:
	jmp [bDS3DebugMenuPrint]

tDS3DebugMenuPrint ENDP

_tDS3DebugMenuPrint PROC

	mov [rsp+08h],rax
	mov [rsp+010h],rbx
	mov [rsp+018h],rcx
	mov [rsp+020h],rdx
	mov [rsp+028h],rbp
	push r8
	push r9
	push rdi
	push rsi
	sub rsp,030h
	mov rbp,rsp
	and rsp,-010h
	mov rcx,rax
	call fDS3DebugMenuPrint
	
	Return:
	mov rsp,rbp
	mov rax,[rsp+058h]
	mov rbx,[rsp+060h]
	mov rcx,[rsp+068h]
	mov rdx,[rsp+070h]
	mov rbp,[rsp+078h]
	add rsp,030h
	pop rsi
	pop rdi
	pop r9
	pop r8
	ret

_tDS3DebugMenuPrint ENDP

tDS3DebugGUIPrint PROC

	sub rsp,088h
	call _tDS3DebugGUIPrint
	add rsp,088h

	Return:
	movss xmm0,DWORD PTR [rax+08h]
	jmp [bDS3DebugGUIPrint]

tDS3DebugGUIPrint ENDP

_tDS3DebugGUIPrint PROC

	mov [rsp+08h],rax
	mov [rsp+010h],rbx
	mov [rsp+018h],rcx
	mov [rsp+020h],rdx
	mov [rsp+028h],rbp
	push r8
	push r9
	push rdi
	push rsi
	sub rsp,060h
	mov rbp,rsp
	and rsp,-010h
	movaps [rsp+010h],xmm0
	movaps [rsp+020h],xmm1
	movaps [rsp+030h],xmm2
	mov r8,rbx
	movss xmm0,DWORD PTR [rax]
	movss xmm1,DWORD PTR [rax+04h]
	call fDS3DebugGUIPrint
	
	Return:
	movaps xmm0,[rsp+010h]
	movaps xmm1,[rsp+020h]
	movaps xmm2,[rsp+030h]
	mov rsp,rbp
	mov rax,[rsp+088h]
	mov rbx,[rsp+090h]
	mov rcx,[rsp+098h]
	mov rdx,[rsp+0A0h]
	mov rbp,[rsp+0A8h]
	add rsp,060h
	pop rsi
	pop rdi
	pop r9
	pop r8
	ret

_tDS3DebugGUIPrint ENDP

END