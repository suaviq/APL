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
;; ymm, jezeli 64 bity, ymm, jezeli 128, zmm, jezeli 256
;; return jest w eax
;; ================ ;;

;; === TU IDA NASZE PROCEDURY === ;

MyProc1 proc
mov eax, ecx
ret
MyProc1 endp

add_vectors proc
vmovupd ymm1, [rcx]          ; ymm1 <- A
vaddpd ymm1, ymm1, [rdx]     ; ymm1 = ymm1 + B
vmovupd [rcx], ymm1          ; A <- ymm1 (A + B)
ret
add_vectors endp

subtract_vectors proc
vmovupd ymm1, [rcx]          ; ymm1 <- A
vsubpd ymm1, ymm1, [rdx]     ; ymm1 = ymm1 - B
vmovupd [rcx], ymm1          ; A <- ymm1 (A - B)
ret
subtract_vectors endp


add_scalar_to_vector proc
vmovupd ymm1, [rcx]             ; ymm1 <- array A at addr rcx
vmovapd xmm2, [rdx]				; xmm2 <- scalar
vbroadcastsd ymm2, xmm2			; ymm2 <- low part of xmm2 * 4
vaddpd ymm1, ymm1, ymm2         ; ymm1 = ymm1 + ymm2
vmovupd [rcx], ymm1             ; A <- ymm1 (A + scalar)
ret
add_scalar_to_vector endp

subtract_scalar_from_vector proc
vmovupd ymm1, [rcx]             ; ymm1 <- array A at addr rcx
vmovapd xmm2, [rdx]				; xmm2 <- scalar
vbroadcastsd ymm2, xmm2			; ymm2 <- low part of xmm2 * 4
vsubpd ymm1, ymm1, ymm2			; ymm1 = ymm1 - ymm2
vmovupd [rcx], ymm1				; A <- ymm1 (A - scalar)
ret
subtract_scalar_from_vector endp

END 
;; === KONIEC nn.asm === ;;