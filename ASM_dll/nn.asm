;; === nn.asm === ;;
OPTION CASEMAP:NONE
.data
    ret_val qword 1.0
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


    ;; === OPERATIONS WITH VECTORS ===
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

vector_wise_multiply proc
    vmovupd ymm1, [rcx]          ; ymm1 <- A
    vmulpd ymm1, ymm1, [rdx]     ; ymm1 = ymm1 * B
    vmovupd [rcx], ymm1          ; A <- ymm1 (A * B)
    ret
vector_wise_multiply endp

mul_vecT_by_vec proc
    mov r9, 0                       ; counter for loop / result index
    mov r10, 0                      ; counter for vecT index
mul_loop:
    vmovupd ymm3, [rcx][r10]        ; ymm3 <- vecT[i]
    vbroadcastsd ymm0, xmm3         ; ymm0 = 4 * vecT[i]
    vmulpd ymm2, ymm0, [rdx]        ; multiply vec row with vecT[i]
    vmovupd [r8][r9], ymm2          ; store result in result matrix
    add r9, 20h                     ; increment r9 by 64 (32 bytes = 256b = 4 doubles)
    add r10, 8                      ; increment vecT index
    cmp r9, 80h                     ; compare loop counter
    jne mul_loop                    ; loop
    ret
mul_vecT_by_vec endp


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

mul_vector_by_scalar proc
    vmovupd ymm1, [rcx]             ; ymm1 <- array A at addr rcx
    vmovapd xmm2, [rdx]             ; xmm2 <- scalar
    vbroadcastsd ymm2, xmm2         ; ymm2 <- low part of xmm2 * 4
    vmulpd ymm1, ymm1, ymm2         ; ymm1 = ymm1 * ymm2
    vmovupd [rcx], ymm1             ; A <- ymm1 (A * scalar)
    ret
mul_vector_by_scalar endp

;; TODO: optimise this by replacing multiplication with addition
mul_matrix_by_scalar proc
    mov ebx, 20h                ; ebx = 32 (32 bytes is 4 doubles = our offset)
    mov r8, 0                   ; r8 <- 0 (counter)
    mov r9, rdx                 ; move rdx to r9 because `mul` clears rdx
    vmovapd xmm2, [rdx]         ; xmm2 <- scalar
    vbroadcastsd ymm2, xmm2     ; ymm2 <- low part of xmm2 * 4
loop_label:
    mov rax, r8                 ; rax = r8
    mul ebx                     ; rax = r8 * 32
    mov rdx, r9                 ; get saved address from r9
    vmovupd ymm1, [rcx][rax]    ; ymm1 <- array A (a row in matrix) at addr rcx + offset
    vmulpd ymm1, ymm1, ymm2     ; ymm1 = ymm1 * ymm2
    vmovupd [rcx][rax], ymm1    ; A <- ymm1 (A * scalar)
    inc r8                      ; increment counter
    cmp r8, 4                   ; check if 4 passes have been made
    jne loop_label              ; jump to loop_label if not
    ret
mul_matrix_by_scalar endp


    ;; === OPERATIONS WITH MATRICES ===
;; TODO: optimise this by replacing multiplication with addition
add_matrices proc
    mov ebx, 20h                    ; ebx = offset
    mov r8, 0                       ; r8 <- 0 (counter)
    mov r9, rdx                     ; move rdx to r9 because `mul` clears rdx
loop_label:
    mov rax, r8                     ; rax = r8
    mul ebx                         ; rax = r8 * 32
    mov rdx, r9                     ; get saved address from r9
    vmovupd ymm1, [rax][rcx]        ; ymm1 <- array A (a row in matrix) at addr rcx + offset
    vaddpd ymm1, ymm1, [rax][rdx]   ; ymm1 = ymm1 + ymm2
    vmovupd [rax][rcx], ymm1        ; A <- ymm1 (A + B)
    inc r8                          ; increment counter
    cmp r8, 4                       ; check if 4 passes have been made
    jne loop_label                  ; jump to loop_label if not
    ret
add_matrices endp

;; TODO: optimise this by replacing multiplication with addition
subtract_matrices proc
    mov ebx, 20h                    ; ebx = offset
    mov r8, 0                       ; r8 <- 0 (counter)
    mov r9, rdx                     ; move rdx to r9 because `mul` clears rdx (???)
loop_label:
    mov rax, r8                     ; rax = r8
    mul ebx                         ; rax = r8 * 32
    mov rdx, r9                     ; get saved address from r9
    vmovupd ymm1, [rax][rcx]        ; ymm1 <- array A (a row in matrix) at addr rcx + offset
    vsubpd ymm1, ymm1, [rax][rdx]   ; ymm1 = ymm1 + ymm2
    vmovupd [rax][rcx], ymm1        ; A <- ymm1 (A + B)
    inc r8                          ; increment counter
    cmp r8, 4                       ; check if 4 passes have been made
    jne loop_label                  ; jump to loop_label if not
    ret
subtract_matrices endp


END ;; === KONIEC nn.asm === ;;
