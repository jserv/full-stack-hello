; FIXME: all instructions should be used for coverage test.
add $40 $2 #1
add #1 $8 #2
print #1
print #2
jmp #6
sub #2 $16 #3		; skipped because previous jump
add #2 $100 #3		; to be executed
print #3
mul $-2 $2 #4           ; multiply: (-2)*(2)
mul $-2 $-40 #5         ; multiply: (-2)*(-40)
print #4
print #5
div $4 $2 #6            ; divide: 4 / 2 = 2
mod $5 $2 #7            ; modulo: 5 % 2 = 1
print #6
print #7
