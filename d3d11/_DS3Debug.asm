EXTERN bDS3DebugMenuPrint:QWORD
EXTERN fDS3DebugMenuPrint:PROC

EXTERN bDS3DebugGUIPrint:QWORD
EXTERN fDS3DebugGUIPrint:PROC

EXTERN bDS3DebugMenuNEXT:QWORD
EXTERN fDS3DebugMenuNEXT:PROC

EXTERN bIPV4Hook:PROC

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
	sub rsp,060h
	mov rbp,rsp
	and rsp,-010h
	movaps [rsp+010h],xmm0
	movaps [rsp+020h],xmm1
	movaps [rsp+030h],xmm2
	mov rcx,rax
	call fDS3DebugMenuPrint
	
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

_tDS3DebugMenuPrint ENDP

tDS3DebugGUIPrint PROC

	sub rsp,088h
	call _tDS3DebugGUIPrint
	add rsp,088h

	Return:
	movss xmm0,DWORD PTR [rax+08h]
	jmp [bDS3DebugGUIPrint]

tDS3DebugGUIPrint ENDP

.data

fFloatSubY REAL4 7.00f

.code


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
	subss xmm1,DWORD PTR [fFloatSubY]
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

tDS3DebugMenuNEXT PROC

	sub rsp,088h
	call _tDS3DebugMenuNEXT
	add rsp,088h

	Return:
	jmp [bDS3DebugMenuNEXT]
	
tDS3DebugMenuNEXT ENDP

_tDS3DebugMenuNEXT PROC

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
	movaps [rsp+010h],xmm8
	movaps [rsp+020h],xmm7
	movaps [rsp+030h],xmm0
	movaps xmm1, [rsp+10h]
	movaps xmm2, [rsp+20h]
	mov rcx,rax
	call fDS3DebugMenuNEXT
	
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

_tDS3DebugMenuNEXT ENDP

tIPV4Hook PROC

	ret
	jmp [bIPV4Hook]

tIPV4Hook ENDP

END