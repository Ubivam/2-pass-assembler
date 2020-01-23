.vector_table
begin:
.word rst
.word err
.word tim
.word term

.text_reset_handler
    movb *ff10, 5
inf:
    jmp inf
.text_error_code_handler
error:
    halt
.text_timer_handler
timer:
    add r4, 1
    iret
.text_terminal_handler
terminal:
    mov r5, *ff02
    add r5, 20
    mov *ff00, r5
    iret
.end