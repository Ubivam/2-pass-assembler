.text_adr_test
b:
    jmp b
    jmp c
    mul r2, $b
    add r1, *ff
    add r2, &b
    add r2, &c
    jmp $b
c:
    call &b
    call b
.end