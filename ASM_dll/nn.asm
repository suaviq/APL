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

;; TODO: find a way to make it work :/
mul_vecT_by_vec proc
    mov r9, 0
    vmovupd ymm0, [rcx]             ; ymm0 <- vecT
    vmovupd ymm2, [rdx]             ; ymm2 <- vec
mul_loop:
    vbroadcastsd ymm1, [rcx + r9]   ; ymm1 = 4 * vecT[i]
    vmulpd ymm1, ymm1, ymm2         ; multiply vec elements
    vhaddpd ymm1, ymm1              ; add 2 pairs of vec products
    mov eax, 0001b                  ; create writemask
    kmovd k1, eax                   ; move writemask
    vbroadcastsd ymm1{k1}, xmm1     ; broadcast needed element from low to high part
    vhaddpd ymm1, ymm1              ; add 2 remaining pairs (2 results in both high ymm parts)
    vextractf64x2 xmm1, ymm1, 1     ; move high to low
    vmovupd ymm3{k1}, ymm1          ; ymm3 <- result vec
    vmovupd [rcx], ymm3
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
