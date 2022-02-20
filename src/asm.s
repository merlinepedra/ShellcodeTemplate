
extern Entry

global Start
global GetRIP

section .text$A
	Start:
        push    rsi
        mov		rsi, rsp
        and		rsp, 0FFFFFFFFFFFFFFF0h

        sub		rsp, 020h
        call    Entry

        mov		rsp, rsi
        pop		rsi
    ret

section .text$F
    GetRIP:
        call    retptr

    retptr:
        pop	rax
        sub	rax, 5
    ret
