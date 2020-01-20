.global e
.extern p

.text.main
    mov r1, e
    add r1, 22
    sub r1, 3
    mul r1, 10
    cmp r3, $p
.data
    .skip 10
    .word a
    .word p
    .word c
.end