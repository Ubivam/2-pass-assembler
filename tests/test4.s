.global b,k
.extern l
.text_main
.word ana pet zeh iht
.byte ruk
.align 4
.equ g 33
.skip 10
a:
movw r6[0], 1234
mul r3, 1442
push g
l:
div r5, r2[a]
add r2, r1[l]
addb r4, 10
.align 16
s:
sub r1, r2
   jmp s
   jmp l
.text_zma
mul r1, 33
e:
xor r1, e
.end