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


END 
;; === KONIEC nn.asm === ;;
