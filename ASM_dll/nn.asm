;; === nn.asm === ;;
OPTION CASEMAP:NONE
.CODE
DllEntry PROC hInstDLL:DWORD, reason:DWORD, reserved1:DWORD
mov	eax, 1 	;TRUE
ret
DllEntry ENDP

;; === TUTORIAL === ;;
;; kolejnosc argumentow:
;;
;; 1 -- rcx -- *mm0
;; 2 -- rdx -- *mm1
;; 3 -- r8  -- *mm2
;; 4 -- r9  -- *mm3
;;
;; lewe, jezeli int, prawe, jezeli float/double
;; xmm, jezeli 64 bity, ymm, jezeli 128, zmm, jezeli 256
;; return jest w eax
;; ================ ;;

;; === TU IDA NASZE PROCEDURY === ;

;; na razie to tylko zwraca to, co sie wysyle jako 1. argument
MyProc1 proc
mov eax, ecx
ret

MyProc1 endp

add_vectors proc ; a: QWORD, b: QWORD
movupd xmm1, [rcx]          ; xmm1 <- A
movupd xmm2, [rdx]          ; xmm2 <- B
addpd xmm1, xmm2            ; xmm1 = xmm1 + xmm2
movupd [rcx], xmm1          ; A <- xmm0 (A + B)
ret
add_vectors endp

;vaddpd xmm2, xmm0, xmm1         ; xmm1 = xmm1 + xmm2		;alternative to store result in third vector

substract_vectors proc ; a: QWORD, b: QWORD
movupd xmm1, [rcx]          ; xmm1 <- A
movupd xmm2, [rdx]          ; xmm2 <- B
subpd xmm1, xmm2            ; xmm1 = xmm1 + xmm2
movupd [rcx], xmm1          ; A <- xmm0 (A + B)
ret
substract_vectors endp

END 

;; === KONIEC nn.asm === ;;
