;; === nn.asm === ;;
OPTION CASEMAP:NONE
.CODE
DllEntry PROC hInstDLL:DWORD, reason:DWORD, reserved1:DWORD
mov	eax, 1
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


    ;; === BASIC VECTOR ARITHMETICS ===
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

;; TODO: check if this works
vector_wise_multiply proc
    vmovupd ymm1, [rcx]          ; ymm1 <- A
    vmulpd ymm1, ymm1, [rdx]     ; ymm1 = ymm1 * B
    vmovupd [rcx], ymm1          ; A <- ymm1 (A * B)
    ret
vector_wise_multiply endp


    ;; === OPERATIONS WITH SCALARS ===
add_scalar_to_vector proc
    vmovupd ymm1, [rcx]             ; ymm1 <- array A at addr rcx
    vmovapd xmm2, [rdx]             ; xmm2 <- scalar
    vbroadcastsd ymm2, xmm2         ; ymm2 <- low part of xmm2 * 4
    vaddpd ymm1, ymm1, ymm2         ; ymm1 = ymm1 + ymm2
    vmovupd [rcx], ymm1             ; A <- ymm1 (A + scalar)
    ret
add_scalar_to_vector endp

subtract_scalar_from_vector proc
    vmovupd ymm1, [rcx]             ; ymm1 <- array A at addr rcx
    vmovapd xmm2, [rdx]             ; xmm2 <- scalar
    vbroadcastsd ymm2, xmm2         ; ymm2 <- low part of xmm2 * 4
    vsubpd ymm1, ymm1, ymm2         ; ymm1 = ymm1 - ymm2
    vmovupd [rcx], ymm1             ; A <- ymm1 (A - scalar)
    ret
subtract_scalar_from_vector endp

;; TODO: check if this works
mul_vector_by_scalar proc
    vmovupd ymm1, [rcx]             ; ymm1 <- array A at addr rcx
    vmovapd xmm2, [rdx]             ; xmm2 <- scalar
    vbroadcastsd ymm2, xmm2         ; ymm2 <- low part of xmm2 * 4
    vmulpd ymm1, ymm1, ymm2         ; ymm1 = ymm1 * ymm2
    vmovupd [rcx], ymm1             ; A <- ymm1 (A * scalar)
    ret
mul_vector_by_scalar endp

;; TODO: check if this works
mul_matrix_by_scalar proc
    xor eax, eax                ; eax = 0
    mov ebx, 20h                ; ebx = 32 (32 bytes is 4 doubles = our offset)
    vmovapd xmm2, [rdx]         ; xmm2 <- scalar
    vbroadcastsd ymm2, xmm2     ; ymm2 <- low part of xmm2 * 4
    mov ecx, 0                  ; ecx <- 0 (counter)
loop_label:
    mov eax, ecx                ; eax = ecx (loop counter)
    mul ebx                     ; eax = ecx * 32
    vmovupd ymm1, [rcx][eax]    ; ymm1 <- array A (a row in matrix) at addr rcx + offset
    vmulpd ymm1, ymm1, ymm2     ; ymm1 = ymm1 * ymm2
    vmovupd [rcx][eax], ymm1    ; A <- ymm1 (A * scalar)
    inc ecx                     ; increment counter
    cmp ecx, 3                  ; check if 4 passes have been made
    jne loop_label              ; jump to loop_label if not
    ret
mul_matrix_by_scalar endp


    ;; === OPERATIONS WITH MATRICES ===
;; TODO: check if this works
add_matrices proc
    xor eax, eax                    ; eax = 0
    mov ebx, 20h                    ; ebx = offset
    mov ecx, 0                      ; ecx <- 0 (counter)
loop_label:
    mov eax, ecx                    ; eax = ecx
    mul ebx                         ; eax = ecx * 32
    vmovupd ymm1, [rcx][eax]        ; ymm1 <- array A (a row in matrix) at addr rcx + offset
    vaddpd ymm1, ymm1, [rdx][eax]   ; ymm1 = ymm1 + ymm2
    vmovupd [rcx][eax], ymm1        ; A <- ymm1 (A + B)
    inc ecx                         ; increment counter
    cmp ecx, 3                      ; check if 4 passes have been made
    jne loop_label                  ; jump to loop_label if not
    ret
add_matrices endp

;; TODO: check if this works
subtract_matrices proc
    xor eax, eax                    ; eax = 0
    mov ebx, 20h                    ; ebx = offset
    mov ecx, 0                      ; ecx <- 0 (counter)
loop_label:
    mov eax, ecx                    ; eax = ecx
    mul ebx                         ; eax = ecx * 32
    vmovupd ymm1, [rcx][eax]        ; ymm1 <- array A (a row in matrix) at addr rcx + offset
    vsubpd ymm1, ymm1, [rdx][eax]   ; ymm1 = ymm1 - ymm2
    vmovupd [rcx][eax], ymm1        ; A <- ymm1 (A - B)
    inc ecx                         ; increment counter
    cmp ecx, 3                      ; check if 4 passes have been made
    jne loop_label                  ; jump to loop_label if not
    ret
add_matrices endp


END ;; === KONIEC nn.asm === ;;
