.vector_table
begin:
.word rst
.word err
.word tim
.word term

.text_reset_handler
inf:
    jmp inf
.text_error_code_handler
error:
    halt
.text_timer_handler
timer:
    halt
.text_terminal_handler
terminal:
    halt
.end