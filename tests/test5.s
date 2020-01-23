.global a, b, c
.extern t, u
.skip 4
.text_main
main:
    mov r1 , 100
b:
    mov r2 , $b
    call fa
    push r3
    mov r1, 25
    mov r2, $b
    call fs
.align 4
.text_add
fa:
    add r3, r1
    add r3, r2
    ret
.align 4
.text_sub
fs:
    sub r3, r1
    sub r3, rs
    ret
.align 4
.end